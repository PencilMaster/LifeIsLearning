#include "server_network_manager.h"
#include "game_instance.h"
#include "network/responses/full_state_response.h"
#include <iostream>
#include "network/responses/request_response.h"


GameInstance::GameInstance(){
    game_state = std::make_shared<GameState>();
}

void GameInstance::add_player(std::shared_ptr<Player> player_ptr){
    std::lock_guard<std::mutex> lock(game_instance_mutex);
    game_state->add_player(std::shared_ptr<Player>(player_ptr));
    //notify all other players that a new player joined
    FullStateResponse state_update_msg = FullStateResponse(this->get_game_id(), *game_state);
    ServerNetworkManager::broadcast_message(state_update_msg, game_state->get_players(), player_ptr.get());
}

bool GameInstance::try_add_player(std::shared_ptr<Player> &player_ptr){
    if(GameInstance::is_joinable()){
        GameInstance::add_player(player_ptr);
        player_ptr->set_game_id(this->get_game_id());
        return true;
    }
    else return false;
}

void GameInstance::remove_player(size_t const& player_id){
    std::lock_guard<std::mutex> lock(game_instance_mutex);
    game_state->remove_player(player_id);
    //notify all other players that this player left
    FullStateResponse state_update_msg = FullStateResponse(this->get_game_id(), *game_state);
    ServerNetworkManager::broadcast_message(state_update_msg, game_state->get_players(), game_state->get_player_ptr(player_id).get());
}

std::shared_ptr<GameState> GameInstance::get_game_state(){
    std::lock_guard<std::mutex> lock(game_instance_mutex);
    return game_state;
}

std::string GameInstance::get_game_id(){
    return game_state->get_game_id();
}

bool GameInstance::is_joinable(){
    std::lock_guard<std::mutex> lock(game_instance_mutex);
    //a game can only be joined in the lobby
    return (game_state->get_current_phase() == lobby && game_state->get_num_players() < 4);
}

bool GameInstance::is_empty(){
    std::lock_guard<std::mutex> lock(game_instance_mutex);
    return !(game_state->get_num_players());
}

bool GameInstance::start_game(std::shared_ptr<Player> &player_ptr){
    std::lock_guard<std::mutex> lock(game_instance_mutex);
    bool startable = game_state->start_game();
    if(startable) {
        FullStateResponse state_update_msg = FullStateResponse(this->get_game_id(), *game_state);
        ServerNetworkManager::broadcast_message(state_update_msg, game_state->get_players(), player_ptr.get());
    }
    return startable;
}

bool GameInstance::place_road(int cell_id, int player_idx, std::string &err){
    std::lock_guard<std::mutex> lock(game_instance_mutex);
    if(game_state->place_road(cell_id, player_idx)){
        next_player();
        //notify all players about the new game state
        FullStateResponse state_update_msg = FullStateResponse(this->get_game_id(), *game_state);
        ServerNetworkManager::broadcast_message(state_update_msg, game_state->get_players(), game_state->get_player_ptr(player_idx).get());
        auto game_state_json = game_state->to_json();
        RequestResponse response = RequestResponse(this->get_game_id(), "place_road",true, game_state_json, "");
        return true;
    }
    else{
        err = "Failed to place road at cell: " + cell_id;
        return false;
    }
}

bool GameInstance::place_truck(int cell_id, int player_idx, TruckType truck_type, std::string &err){
    std::lock_guard<std::mutex> lock(game_instance_mutex);
    if(game_state->place_truck(cell_id, player_idx, truck_type)){
        next_player();
        //notify all players about the new game state
        FullStateResponse state_update_msg = FullStateResponse(this->get_game_id(), *game_state);
        ServerNetworkManager::broadcast_message(state_update_msg, game_state->get_players(), game_state->get_player_ptr(player_idx).get());
        auto game_state_json = game_state->to_json();
        RequestResponse response = RequestResponse(this->get_game_id(), "place_truck",true, game_state_json, "");
        return true;
    }
    else{
        err = "Failed to place truck on cell " + std::to_string(cell_id);
        return false;
    }
}

bool GameInstance::buy_trucks(int player_idx, int big_truck, int small_truck, std::string &err){
    std::lock_guard<std::mutex> lock(game_instance_mutex);
    if(game_state->buy_truck(player_idx, big_truck, small_truck)){
        //trucks can only be bought once a round, therefore end the player's phase
        end_phase_no_lock(player_idx, err);
        return true;
    }
    else{
        err = "Failed to buy " + std::to_string(big_truck) + " big trucks and " + std::to_string(small_truck) + " small trucks.";
        return false;
    }
}

bool GameInstance::remove_trucks(int cell_id, int player_idx, int big_truck, int small_truck, std::string &err){
    std::lock_guard<std::mutex> lock(game_instance_mutex);
    if(game_state->remove_trucks(cell_id, player_idx, big_truck, small_truck)){
        //notify the players about the new game state
        FullStateResponse state_update_msg = FullStateResponse(this->get_game_id(), *game_state);
        ServerNetworkManager::broadcast_message(state_update_msg, game_state->get_players(), game_state->get_player_ptr(player_idx).get());
        return true;
    }
    else{
        err = "Failed to remove " + std::to_string(big_truck) + " big trucks and " + std::to_string(small_truck) + " small trucks at cell" + std::to_string(cell_id);
        return false;
    }
}

bool GameInstance::end_phase(int player_idx, std::string &err){
    std::lock_guard<std::mutex> lock(game_instance_mutex);
    return end_phase_no_lock(player_idx, err);
}

bool GameInstance::end_phase_no_lock(int player_idx, std::string &err){
    const phases phase = game_state->get_current_phase();
    if(!game_state->get_end_phase(player_idx)){
        game_state->set_end_phase(player_idx);
        if(game_state->should_end_phase()){
            //decide which phase comes next
            switch(phase){
                case road_place:
                    start_truck_buying_phase();
                    break;
                case truck_buy:
                    start_truck_placing_phase();
                    break;
                case truck_place:
                    end_of_round();
                    break;
                default:
                    break;
            }
        }
        else{
            next_player();
        }
        //notify all other players about the change in game_state
        FullStateResponse state_update_msg = FullStateResponse(this->get_game_id(), *game_state);
        ServerNetworkManager::broadcast_message(state_update_msg, game_state->get_players(), game_state->get_player_ptr(player_idx).get());
        return true;
    }
    else{
        err = "Phase was already ended!";
        return false;
    }
}

void GameInstance::start_road_placing_phase(){
    game_state->set_current_phase(road_place);
    game_state->set_current_player(0);
    game_state->reset_phase_end_list();
}

void GameInstance::start_truck_buying_phase(){
    game_state->set_current_phase(truck_buy);
    game_state->set_current_player(0);
    game_state->reset_phase_end_list();
}

void GameInstance::start_truck_placing_phase(){
    game_state->set_current_phase(truck_place);
    game_state->set_current_player(0);
    game_state->reset_phase_end_list();
}

void GameInstance::end_of_round(){
    game_state->next_round();
    game_state->set_current_phase(road_place);
    game_state->set_current_player(0);
    game_state->reset_phase_end_list();
}

void GameInstance::next_player(){
    //ensure that there is no infinite loop
    if(!game_state->should_end_phase()){
        //choose the next player that hasn't ended their phase yet by cycling over indices
        int i = game_state->get_current_player();
        do {
            i++;
            i %= 4;
        } while(game_state->get_end_phase(i));
        game_state->set_current_player(i);
    }
}
