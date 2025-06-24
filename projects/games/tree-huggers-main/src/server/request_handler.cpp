#include "request_handler.h"
#include "player_manager.h"
#include "game_instance_manager.h"
#include "game_instance.h"

// Include all request types
#include "../../lib/common/network/requests/server_connect_request.h"
#include "../../lib/common/network/requests/start_game_request.h"
#include "../../lib/common/network/requests/place_road_request.h"
#include "../../lib/common/network/requests/buy_truck_request.h"
#include "../../lib/common/network/requests/place_truck_request.h"
#include "../../lib/common/network/requests/remove_truck_request.h"
#include "../../lib/common/network/requests/end_phase_request.h"

// Handles incoming requests and returns a response
RequestResponse* RequestHandler::handle_request(const ClientRequest& req) {
    
    // Prepare variables used by every request type
    std::shared_ptr<Player> player = nullptr;
    std::string err;
    std::shared_ptr<GameInstance> game_instance_ptr = nullptr;

    // Get common properties of the request
    RequestType type = req.get_type();
    std::string req_id = req.get_req_id();
    std::string game_id = req.get_game_id();
    std::string player_id = req.get_client_id();


    // Switch behavior according to the request type
    switch (type) {

        // ##################### SERVER CONNECT ##################### //
        case RequestType::server_connect: {
            auto connect_req = static_cast<const ServerConnectRequest&>(req);
            std::string client_name = connect_req.get_client_name();


            // Add or retrieve player using the client name
            PlayerManager::add_or_get_player(client_name, player_id, player);

            if (game_id.empty()) {
                // Attempt to connect to any available game
                if (GameInstanceManager::try_add_player_to_any_game(player,game_instance_ptr)) {
                    //Segfault is probably in to_json
                    return new RequestResponse(game_instance_ptr->get_game_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                } else {
                    return new RequestResponse("", req_id, false, nullptr, err);
                }
            } else {
                // Attempt to connect to a specific game
                game_instance_ptr = GameInstanceManager::get_game_instance(game_id);
                if (game_instance_ptr != nullptr) {
                    if (GameInstanceManager::try_add_player(player, game_instance_ptr)) {
                        return new RequestResponse(game_id, req_id, true,
                                                    game_instance_ptr->get_game_state()->to_json(), err);
                    } else {
                        return new RequestResponse("", req_id, false, nullptr, err);
                    }
                } else {
                    return new RequestResponse("", req_id, false, nullptr, "Requested game not found.");
                }
            }
        }

        // ##################### START GAME ##################### //
        case RequestType::start_game: {
            if (GameInstanceManager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                if (game_instance_ptr->start_game(player)) {
                    return new RequestResponse(game_instance_ptr->get_game_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                }
            }
            return new RequestResponse("", req_id, false, nullptr, err);
        }

        // ##################### PLACE ROAD ##################### //
        case RequestType::place_road: {
            auto road_req = static_cast<const PlaceRoadRequest&>(req);
            int cell_id = road_req.get_cell_id();
            //attempt to find player and their game
            if (GameInstanceManager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                //attempt to place the desired road
                if (game_instance_ptr->place_road(cell_id, player->get_player_idx(), err)) {
                    return new RequestResponse(game_instance_ptr->get_game_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                }
            }
            return new RequestResponse("", req_id, false, nullptr, err);
        }

        // ##################### BUY TRUCK ##################### //
        case RequestType::buy_truck: {
            auto truck_req = static_cast<const BuyTruckRequest&>(req);
            int big_trucks = truck_req.get_big_trucks();
            int small_trucks = truck_req.get_small_trucks();
            //attempt to find player and their game
            if (GameInstanceManager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                //attempt to buy the desired amount of trucks
                if (game_instance_ptr->buy_trucks(player->get_player_idx(), big_trucks, small_trucks, err)) {
                    return new RequestResponse(game_instance_ptr->get_game_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                }
            }
            return new RequestResponse("", req_id, false, nullptr, err);
        }

        // ##################### PLACE TRUCK ##################### //
        case RequestType::place_truck: {
            auto truck_req = static_cast<const PlaceTruckRequest&>(req);
            int cell_id = truck_req.get_cell_id();
            TruckType truck_type = truck_req.get_truck_type();
            //attempt to find player and their game
            if (GameInstanceManager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                //attempt to place the desired type of truck on the given cell
                if (game_instance_ptr->place_truck(cell_id,player->get_player_idx(), truck_type, err)) {
                    return new RequestResponse(game_instance_ptr->get_game_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                }
            }
            return new RequestResponse("", req_id, false, nullptr, err);
        }

        // ##################### REMOVE TRUCK ##################### //
        case RequestType::remove_truck: {
            auto truck_req = static_cast<const RemoveTruckRequest&>(req);
            int cell_id = truck_req.get_cell_id();
            int big_trucks = truck_req.get_big_trucks();
            int small_trucks = truck_req.get_small_trucks();
            //attempt to find player and their game
            if (GameInstanceManager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                //attempt to remove the given amount of trucks type of truck on the given cell
                if (game_instance_ptr->remove_trucks(cell_id,player->get_player_idx(), big_trucks, small_trucks, err)) {
                    return new RequestResponse(game_instance_ptr->get_game_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                }
            }
            return new RequestResponse("", req_id, false, nullptr, err);
        }

        // ##################### END PHASE ##################### //
        case RequestType::end_phase: {
            //attempt to find player and their game
            if (GameInstanceManager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                //attempt to end the given player's phase
                if (game_instance_ptr->end_phase(player->get_player_idx(), err)) {
                    return new RequestResponse(game_instance_ptr->get_game_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                }
            }
            return new RequestResponse("", req_id, false, nullptr, err);
        }

        // ##################### UNKNOWN REQUEST ##################### //
        default:
            return new RequestResponse("", req_id, false, nullptr, "Unknown RequestType " + std::to_string(type));
    }
}

