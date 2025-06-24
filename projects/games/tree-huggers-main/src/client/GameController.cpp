#include "GameController.h"

#include "truck_shop/TruckShop.h"
#include "network/requests/server_connect_request.h"
#include "network/requests/start_game_request.h"
#include "network/requests/buy_truck_request.h"
#include "network/requests/place_truck_request.h"
#include "network/requests/remove_truck_request.h"
#include "network/requests/place_road_request.h"
#include "network/requests/end_phase_request.h"
#include "network/ClientNetworkManager.h"

// static member initializations
GameWindow* GameController::_gameWindow = nullptr;
ConnectionPanel* GameController::_connectionPanel = nullptr;
LobbyScreenPanel* GameController::_lobbyPanel = nullptr;
MainGamePanel* GameController::_mainGamePanel = nullptr;
EndScreenPanel* GameController::_endScreenPanel = nullptr;
TruckShop* GameController::shop = nullptr;

wxString GameController::inputServerAddress;
std::shared_ptr<const GameState> GameController::_currentGameState = nullptr;
unique_serializable GameController::_my_id;

void GameController::init(GameWindow* gameWindow) {    
    _gameWindow = gameWindow;

    // Set up main panels
    _connectionPanel = new ConnectionPanel(gameWindow);
    _lobbyPanel = new LobbyScreenPanel(gameWindow);
    _mainGamePanel = new MainGamePanel(gameWindow);

    // Hide all panels at the start
    _connectionPanel->Hide();
    _lobbyPanel->Hide();
    _mainGamePanel->Hide();

    // Only show connection panel at the start of the game
    _gameWindow->showPanel(_connectionPanel);

    // Set status bar
    showStatus("Not connected");

}

void GameController::connectToServer() {
    // get the values from UI input fields of the connection panel
    inputServerAddress = _connectionPanel->getServerAddress().Trim();
    wxString inputServerPort = _connectionPanel->getServerPort().Trim();
    wxString inputPlayerName = _connectionPanel->getPlayerName().Trim();

    // check that all values were provided
    if(inputServerAddress.IsEmpty()) {
        GameController::showError("Input Error", "Please provide the server's address");
        return;
    }
    if(inputServerPort.IsEmpty()) {
        GameController::showError("Input Error", "Please provide the server's port number");
        return;
    }
    if(inputPlayerName.IsEmpty()) {
        GameController::showError("Input Error", "Please enter your desired player name");
        return;
    }

    // convert host from wxString to std::string
    std::string host = inputServerAddress.ToStdString();

    // convert port from wxString to uint16_t
    unsigned long portAsLong;
    if(!inputServerPort.ToULong(&portAsLong) || portAsLong > 65535) {
        GameController::showError("Connection Error", "Invalid Port");
        return;
    }
    uint16_t port = (uint16_t) portAsLong;

    // convert player name from wxString to std::string
    std::string playerName = inputPlayerName.ToStdString();

    // connect to network
    ClientNetworkManager::init(host, port);

    // send request to join game
    ServerConnectRequest request = ServerConnectRequest(GameController::_my_id.get_id(), playerName); //Do we need serializable players?
    ClientNetworkManager::sendRequest(request);

}

void GameController::updateGameState(std::shared_ptr<const GameState> newGameState) {
    // the existing game state is now old
    std::shared_ptr<const GameState> oldGameState = GameController::_currentGameState;

    // save the new game state as our current game state
    GameController::_currentGameState = newGameState;

    if(oldGameState == nullptr) {
        // pass the first game state we receive to the main game panel
        _mainGamePanel->initial_game_state(_currentGameState);
    }
    else {
        // check if a new round started, and display message accordingly
        if(oldGameState->get_current_round() > 0 && oldGameState->get_current_round() < newGameState->get_current_round()) {
            GameController::showNewRoundMessage(oldGameState, newGameState);
        }
    }

    std::vector<Player *> players_vec = _currentGameState->get_players();
    
    phases curr_phase = _currentGameState->get_current_phase();
   
    if(curr_phase == lobby) {
        // show the lobby panel (if we are already showing it, nothing will happen)
        _gameWindow->showPanel(_lobbyPanel);

        // update the lobby panel's player list
        for(int i = 0; i < _currentGameState->max_num_players; i++) {
            std::shared_ptr<Player> new_player = newGameState->get_player_ptr(i);
            if(new_player != nullptr){
                // if a player is occupying the slot in the new game state, try to add them to the lobby
                _lobbyPanel->AttemptAddPlayer(new_player->get_name(), new_player->get_player_id());
            }
            // removing is not yet implemented
            // else if (oldGameState != nullptr){
            //     std::shared_ptr<Player> old_player = oldGameState->get_player_ptr(i);
            //     // if a player is occupying the slot in the old game state, try to remove them from the lobby
            //     else if(old_player != nullptr) {
            //         _lobbyPanel->AttemptRemovePlayer(old_player->get_name(), old_player->get_player_id());
            //     }
            // }
        }
    }

    if(_currentGameState->is_started()) {
        _mainGamePanel->update_game_state(GameController::_currentGameState);
        // make sure we are showing the main game panel in the window (if we are already showing it, nothing will happen)
        _gameWindow->showPanel(GameController::_mainGamePanel);
    }

    bool my_turn = _currentGameState->get_current_player() == _currentGameState->get_player_idx(get_my_id());
    if(curr_phase == truck_buy && my_turn) { 
        // if it's this client's turn and we're in the truck buying phase, show a truck shop
        makeTruckShop();
    }

    if(_currentGameState->is_finished()) {
        GameController::showEndScreen();
    }
}

void GameController::makeTruckShop() {
    shop = new TruckShop("Truck Shop");
    shop->set_game_state(_currentGameState);
    shop->ShowModal();
}

void GameController::startGame() {
    StartGameRequest request = StartGameRequest(GameController::_my_id.get_id());
    ClientNetworkManager::sendRequest(request);
}

void GameController::placeRoad(int cell_id) {
    PlaceRoadRequest request = PlaceRoadRequest(GameController::_my_id.get_id(), GameController::_currentGameState->get_game_id(), cell_id);
    ClientNetworkManager::sendRequest(request);
}

void GameController::buyTruck() {
    if(shop == nullptr) {
    }

    // get the number of big and small trucks the player wants to buy from the truck shop
    int inputBigTrucks = shop->get_big();
    int inputSmallTrucks = shop->get_small();

    BuyTruckRequest request = BuyTruckRequest(GameController::_my_id.get_id(),inputBigTrucks,inputSmallTrucks);
    ClientNetworkManager::sendRequest(request);
}

void GameController::placeTruck(int cell_id, TruckType truck_type) {
    PlaceTruckRequest request = PlaceTruckRequest(GameController::_my_id.get_id(), GameController::_currentGameState->get_game_id(), cell_id,truck_type);
    ClientNetworkManager::sendRequest(request);
}

void GameController::removeTruck(int cell_id, int big_trucks, int small_trucks) {
    RemoveTruckRequest request = RemoveTruckRequest(GameController::_my_id.get_id(), GameController::_currentGameState->get_game_id(), cell_id, big_trucks, small_trucks);
    ClientNetworkManager::sendRequest(request);
}

void GameController::end_phase() {
    EndPhaseRequest request = EndPhaseRequest(GameController::_my_id.get_id(), GameController::_currentGameState->get_game_id());
    ClientNetworkManager::sendRequest(request);
}

void GameController::showNewRoundMessage(std::shared_ptr<const GameState> oldGameState, std::shared_ptr<const GameState> newGameState) {    
    std::string title = "Round Completed";
    std::string message = "Your balance changed by: ";
    std::string buttonLabel = "Start next round";

    // add the point differences of all players to the messages
    std::shared_ptr<Player> oldPlayerState = oldGameState->get_player_ptr(GameController::_my_id.get_id());
    std::shared_ptr<Player> newPlayerState = newGameState->get_player_ptr(GameController::_my_id.get_id());
    int nextround = newGameState->get_current_round();

    int balanceDelta = newPlayerState->get_balance() - oldPlayerState->get_balance();
    std::string scoreText = std::to_string(balanceDelta);
    if(balanceDelta > 0) {
        scoreText = "+" + scoreText;
    }

    message += scoreText;

    message += "\n Going into round: ";
    message += std::to_string(nextround);

    wxMessageDialog dialogBox = wxMessageDialog(nullptr, message, title, wxICON_NONE);
    dialogBox.SetBackgroundColour(wxColour(230, 252, 252)); // light blue
    dialogBox.SetOKLabel(wxMessageDialog::ButtonLabel(buttonLabel));
    dialogBox.ShowModal();

}

wxEvtHandler* GameController::getMainThreadEventHandler() {
    return GameController::_gameWindow->GetEventHandler();
}

void GameController::showError(const std::string& title, const std::string& message) {
    wxMessageBox(message, title, wxICON_ERROR);
}

void GameController::showStatus(const std::string& message) {
    GameController::_gameWindow->setStatus(message);
}

void GameController::showEndScreen() {
    _endScreenPanel = new EndScreenPanel(_gameWindow,_currentGameState);
    GameController::_gameWindow->showPanel(GameController::_endScreenPanel);
}

std::string GameController::get_my_id() {
    return GameController::_my_id.get_id();
}
