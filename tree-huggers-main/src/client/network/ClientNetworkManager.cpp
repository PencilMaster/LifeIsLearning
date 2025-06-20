#include "ClientNetworkManager.h"


#include "../GameController.h"
#include "../../../lib/common/network/responses/server_response.h"
#include "../../../lib/common/network/responses/request_response.h"
#include "../../../lib/common/network/responses/full_state_response.h"

#include <sockpp/exception.h>
#include <sstream>
#include <typeinfo>


// initialize static members
sockpp::tcp_connector* ClientNetworkManager::_connection = nullptr;

bool ClientNetworkManager::_connectionSuccess = false;
bool ClientNetworkManager::_failedToConnect = false;



void::ClientNetworkManager::init(const std::string& host, const uint16_t port) {

    // initialize sockpp framework
    sockpp::socket_initializer sockInit;

    // reset connection status
    ClientNetworkManager::_connectionSuccess = false;
    ClientNetworkManager::_failedToConnect = false;

    // delete exiting connection and create new one
    if(ClientNetworkManager::_connection != nullptr) {
        ClientNetworkManager::_connection->shutdown();
        delete ClientNetworkManager::_connection;
    }
    ClientNetworkManager::_connection = new sockpp::tcp_connector();

    // try to connect to server
    if(ClientNetworkManager::connect(host, port)) {
        GameController::showStatus("Connected to " + host + ":" + std::to_string(port));
        ClientNetworkManager::_connectionSuccess = true;

        // start network thread
        ResponseListenerThread* responseListenerThread = new ResponseListenerThread(ClientNetworkManager::_connection);
        if(responseListenerThread->Run() != wxTHREAD_NO_ERROR) {
            GameController::showError("Connection error", "Could not create client network thread");
        }

    } else {
        ClientNetworkManager::_failedToConnect = true;
        GameController::showStatus("Not connected");
    }
}


bool ClientNetworkManager::connect(const std::string& host, const uint16_t port) {

    // create sockpp address and catch any errors
    sockpp::inet_address address;
    try {
        address = sockpp::inet_address(host, port);
    } catch (const sockpp::getaddrinfo_error& e) {
        GameController::showError("Connection error", "Failed to resolve address " + e.hostname());
        return false;
    }

    // establish connection to given address
    if (!ClientNetworkManager::_connection->connect(address)) {
        GameController::showError("Connection error", "Failed to connect to server " + address.to_string());
        return false;
    }

    return true;
}


void ClientNetworkManager::sendRequest(const ClientRequest &request) {

    // wait until network is connected (max. 5 seconds)
    int connectionCheckCounter = 0;
    while(!ClientNetworkManager::_connectionSuccess
          && !ClientNetworkManager::_failedToConnect
          && connectionCheckCounter < 200) {
        wxMilliSleep(25);
        connectionCheckCounter++;
    }

    // do not continue if failed to connect to server
    if(ClientNetworkManager::_failedToConnect) {
        return;
    }

    if(ClientNetworkManager::_connectionSuccess && ClientNetworkManager::_connection->is_connected()) {

        // serialize request into JSON string
        rapidjson::Document* jsonDocument = request.to_json();
        std::string message = json_utils::to_string(jsonDocument);
        delete jsonDocument;

        // turn message into stream and prepend message length
        std::stringstream messageStream;
        messageStream << std::to_string(message.size()) << ':' << message;
        message = messageStream.str();

        // output message for debugging purposes
#ifdef PRINT_NETWORK_MESSAGES
        std::cout << "Sending request : " << message << std::endl;
#endif

        // send message to server
        ssize_t bytesSent = ClientNetworkManager::_connection->write(message);

        // if the number of bytes sent does not match the length of the message, probably something went wrong
        if (bytesSent != ssize_t(message.length())) {
            GameController::showError("Network error", "Error writing to the TCP stream: " + ClientNetworkManager::_connection->last_error_str());
        }

    } else {
        GameController::showError("Network error", "Lost connection to server");
    }
}

void ClientNetworkManager::parseResponse(const std::string& message) {

    // output message for debugging purposes
#ifdef PRINT_NETWORK_MESSAGES
    std::cout << "Received response : " << message << std::endl;
#endif

    rapidjson::Document json = rapidjson::Document(rapidjson::kObjectType);
    json.Parse(message.c_str());

    try {
        ServerResponse* res = ServerResponse::from_json(json);
        ResponseType res_type = static_cast<ResponseType>(res->get_type());
        if(res_type == req_response){
            RequestResponse* req_res = dynamic_cast<RequestResponse*>(res);
            if (req_res->get_success()) {
                if (req_res->has_state()) {
                    auto state = GameState::from_json(*(req_res->get_state())); // Deserialize the game state
                    int player_idx = state->get_player_idx(GameController::get_my_id());
                    GameController::updateGameState(state);          // Update the game state in the client
                } else {
                    GameController::showError("Network Error", "Expected a state as JSON inside the RequestResponse. But there was none.");
                }
            } else {
                GameController::showError("Request Failed", req_res->get_err());
            }
        }
        else if(res_type == full_state_msg){
            FullStateResponse* full_state_res = dynamic_cast<FullStateResponse*>(res);
            try {
                if (full_state_res->has_state()) {
                    auto state = GameState::from_json(*(full_state_res->get_state())); // Deserialize the game state
                    GameController::updateGameState(state);          // Update the game state in the client
                    int player_idx = state->get_player_idx(GameController::get_my_id());
                } else {
                    throw std::runtime_error("State JSON is null in FullStateResponse.");
                }
            } catch (const std::exception& e) {
            }
        }

    } catch (std::exception e) {
        GameController::showError("JSON parsing error", "Failed to parse message from server:\n" /*+ message + "\n"*/ + (std::string) e.what());
    }
}