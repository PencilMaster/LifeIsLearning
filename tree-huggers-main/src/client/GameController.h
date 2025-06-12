#pragma once
#include "game_window/GameWindow.h"
#include "connection_panel/ConnectionPanel.h"
#include "truck_shop/TruckShop.h"
#include "lobby_screen_panel/LobbyScreenPanel.h"
#include "main_game_panel/MainGamePanel.h"
#include "end_screen_panel/EndScreenPanel.h"
#include "network/ResponseListenerThread.h"
#include "../../lib/common/game_state.h"
#include "../../lib/common/serialization/uuid_generator.h"
#include "../../lib/common/serialization/serializable_value.h"
#include "../../lib/common/serialization/serializable.h"


// collection of static methods to control the game
class GameController {

public:
    /**
        @brief initializes the game controller
        @param gameWindow the game window to use for displaying the panels
    **/
    static void init(GameWindow* gameWindow);

    /**
        @brief takes inputs from the connection panel and attempts to connect to 
            the server via the ClientNetworkManager
    **/
    static void connectToServer();

    /**
        @brief updates _currentGameState, shows the correct panel and provides the panels with the new game state 
        @param newGameState the new game state to update the game controller with
    **/
    static void updateGameState(std::shared_ptr<const GameState> newGameState);

    /**
        @brief creates a new truck shop panel and sets its game state
    **/
    static void makeTruckShop();

    /**
        @brief starts the game by sending a start game request to the server
    **/
    static void startGame();

    /**
        @brief sends a request to place a road at the given cell id
        @param cell_id the id of the cell to place the road on
    **/
    static void placeRoad(int cell_id);

    /**
        @brief gets the number of trucks the player wants to buy from the truck shop
            and sends a request to buy the trucks to the server
    **/
    static void buyTruck();

    /**
        @brief sends a request to place a truck at the given cell id
        @param cell_id the id of the cell to place the truck on
        @param truck_type the type of truck to place
    **/
    static void placeTruck(int cell_id, TruckType truck_type);

    /**
        @brief sends a request to remove a truck from the given cell id
        @param cell_id the id of the cell to remove the truck from
        @param big_trucks the number of big trucks to remove
        @param small_trucks the number of small trucks to remove
    **/
    static void removeTruck(int cell_id, int big_trucks, int small_trucks);

    /**
        @brief sends a request to end the current phase
    **/
    static void end_phase();

    /**
        @brief getter for the main thread event handler
        @return the event handler for the main thread
    **/
    static wxEvtHandler* getMainThreadEventHandler();

    /**
        @brief shows an error message with the given title and message
        @param title the title of the error message
        @param message the message to display
    **/
    static void showError(const std::string& title, const std::string& message);

    /**
        @brief shows a status message in the status bar via the game window
        @param message the message to display
    **/
    static void showStatus(const std::string& message);

    /**
        @brief shows a dialog box with information on how the player's balance changed and the next round
        @param oldGameState the previous game state
        @param newGameState the new game state
    **/
    static void showNewRoundMessage(std::shared_ptr<const GameState> oldGameState, std::shared_ptr<const GameState> newGameState);
    
    /**
        @brief shows the end screen panel using the game window
    **/
    static void showEndScreen();

    /**
        @brief getter for unique id of the client (_my_id)
        @return client id as a string
    **/
    static std::string get_my_id();

private:
    // panels
    static GameWindow* _gameWindow;
    static ConnectionPanel* _connectionPanel;
    static LobbyScreenPanel* _lobbyPanel;
    static MainGamePanel* _mainGamePanel;
    static EndScreenPanel* _endScreenPanel;
    static TruckShop* shop;

    // server address from connection panel
    static wxString inputServerAddress;

    // unique id of the client
    static unique_serializable _my_id;

    // pointer to the current game state
    static std::shared_ptr<const GameState> _currentGameState;

};

