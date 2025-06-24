#include "game_instance_manager.h"

std::mutex GameInstanceManager::game_instance_manager_mutex;
std::map<std::string, std::shared_ptr<GameInstance>> GameInstanceManager::game_instances;

GameInstanceManager::GameInstanceManager(): game_counter(0){}

void GameInstanceManager::join_game(std::shared_ptr<Player> player_ptr){
    std::lock_guard<std::mutex> lock(game_instance_manager_mutex);
    std::shared_ptr<GameInstance> game_instance(nullptr);
    if(!try_add_player_to_any_game(player_ptr, game_instance)){
        //if no joinable game was found, create a new game
        std::shared_ptr<GameInstance> game_instance = create_new_game();
        game_instance->add_player(player_ptr);
    }
}

void GameInstanceManager::leave_game(player_idx_t const& player_id, game_id_t const& game_id){
    std::lock_guard<std::mutex> lock(game_instance_manager_mutex);
    std::shared_ptr<GameInstance> game_instance = get_game_instance(game_id);

    if(game_instance == nullptr){
        return;
    }

    game_instance->remove_player(player_id);
    if(game_instance->get_game_state()->get_num_players() == 0){
        game_instances.erase(game_id);
    }
    return;
}

std::shared_ptr<GameInstance> GameInstanceManager::get_game_instance(game_id_t const& game_id){
    std::lock_guard<std::mutex> lock(game_instance_manager_mutex);
    auto it = game_instances.find(game_id);
    if(it != game_instances.end()){
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<GameInstance> GameInstanceManager::create_new_game(){
    std::shared_ptr<GameInstance> game_instance = std::make_shared<GameInstance>();
    game_instances.insert({game_instance->get_game_id(),game_instance});
    return game_instance;
}

std::shared_ptr<GameInstance> GameInstanceManager::find_joinable_game(){
    for(auto it = game_instances.begin(); it != game_instances.end(); ++it){
        if(it->second->is_joinable()){
            return it->second;
        }
    }
    std::shared_ptr<GameInstance> game_instance = create_new_game();
    return game_instance;
}

bool GameInstanceManager::try_add_player_to_any_game(std::shared_ptr<Player> player_ptr, std::shared_ptr<GameInstance> &game_instance_ptr){
    if(game_instance_ptr == nullptr){
        for (int i = 0; i < 10; i++) {
            // make at most 10 attempts of joining a src (due to concurrency, the game could already be full or started by the time
            // try_add_player_to_any_game() is invoked) But with only few concurrent requests it should succeed in the first iteration.
            game_instance_ptr = find_joinable_game();
            if (try_add_player(player_ptr, game_instance_ptr)) {
                return true;
            }
        }
        return false;
    } 
    else{
        return try_add_player(player_ptr, game_instance_ptr);
    }
}

bool GameInstanceManager::try_add_player(std::shared_ptr<Player> player_ptr, std::shared_ptr<GameInstance> &game_instance_ptr){
    if (game_instance_ptr->try_add_player(player_ptr)) {
        //mark the player as part of the found game
        player_ptr->set_game_id(game_instance_ptr->get_game_id());
        return true;
    } else {
        return false;
    }

}

bool GameInstanceManager::try_get_game_instance(std::string game_id, std::shared_ptr<GameInstance> &game_instance_ptr){
    std::lock_guard<std::mutex> lock(game_instance_manager_mutex);
    //set to nullptr in case there is some garbage value already stored in game_instance_ptr
    game_instance_ptr = nullptr;
    auto it = GameInstanceManager::game_instances.find(game_id);
    if (it != game_instances.end()) {
        game_instance_ptr = it->second;
    }
    return game_instance_ptr != nullptr;

}

bool GameInstanceManager::try_get_player_and_game_instance(player_id_t player_id, std::shared_ptr<Player> &player_ptr, std::shared_ptr<GameInstance> &game_instance_ptr, std::string &err){
    if (PlayerManager::try_get_player(player_id, player_ptr)) {
        if (GameInstanceManager::try_get_game_instance(player_ptr->get_game_id(), game_instance_ptr)) {
            return true;
        } else {
            err = "Could not find game_id" + player_ptr->get_game_id() + " associated with this player";
        }
    } else {
        err = "Could not find requested player " + player_id + " in database.";
    }
    return false;

}


