#ifndef GAME_INSTANCE_MANAGER_H
#define GAME_INSTANCE_MANAGER_H

#include <memory>
#include <mutex>
#include <map>

#include "game_instance.h"
#include "player.h"
#include "player_manager.h"

class GameInstanceManager{
    using game_id_t = std::string;
    using player_idx_t = int;
    using player_id_t = std::string;
    size_t game_counter;
    static std::mutex game_instance_manager_mutex;
    static std::map<game_id_t, std::shared_ptr<GameInstance>> game_instances;

public:
    GameInstanceManager();

    /** @brief  finds a joinable game and adds player, if none is found it starts a new game and adds player, 
     *          player_idx of player is changed
     *  @param  player_ptr shared pointer to player
     */
    void join_game(std::shared_ptr<Player>);

    /** @brief removes player from specified game, if the last player leaves the game, the game is deleted and
     *  removed from the game instances
     *  @param player_idx_t id of player
     */
    void leave_game(player_idx_t const&, game_id_t const&);

    /** @brief  gets game instance that matches id
     *  @param  game_id_t id of game instance
     *  @return shared pointer to game instance, nullptr if not found
     */
    static std::shared_ptr<GameInstance> get_game_instance(game_id_t const&);

    /** @brief  tries to add player to any game 
     *  @param  player_ptr shared pointer to player
     *  @param  game_instance_ptr shared pointer to game instance
     *  @return true if player was added to a game, false if not
    */
    static bool try_add_player_to_any_game(std::shared_ptr<Player>, std::shared_ptr<GameInstance> &);

    /** @brief  tries to add player to given game instance 
     *  @param  player_ptr shared pointer to player
     *  @param  game_instance_ptr shared pointer to game instance
     *  @return true if player was added to a game, false if not
    */
    static bool try_add_player(std::shared_ptr<Player>, std::shared_ptr<GameInstance> &);

    /** @brief  tries to find the game_instance with given game_id
     *  @param  game_id searched game_id
     *  @param  game_instance_ptr shared pointer to game instance
     *  @return true if game_instance was found, false if not
    */
    static bool try_get_game_instance(std::string, std::shared_ptr<GameInstance> &);

    /** @brief  tries to find player with player_id and returns the pointer and game_instance by reference 
     *  @param  player_id player_id of searched player
     *  @param  player_ptr shared pointer to player
     *  @param  game_instance_ptr shared pointer to game instance
     *  @return true if player and game_instance was found, false if not
    */
    static bool try_get_player_and_game_instance(player_id_t, std::shared_ptr<Player> &, std::shared_ptr<GameInstance> &, std::string &);

private:
    /** @brief  creates a new game instance, adds it to game_instances and returns shared pointer to it
     *  @return shared pointer to game instance
     */
    static std::shared_ptr<GameInstance> create_new_game();

    /** @brief  finds a joinable game instance
     *  @return shared pointer to game instance, nullptr if not found
     */
    static std::shared_ptr<GameInstance> find_joinable_game();
    
};

#endif // GAME_INSTANCE_MANAGER_H