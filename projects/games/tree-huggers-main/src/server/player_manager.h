#ifndef ECOLOGGICAL_PLAYER_MANAGER_H
#define ECOLOGGICAL_PLAYER_MANAGER_H

#include <string>
#include <shared_mutex>
#include <unordered_map>

#include "../../lib/common/player.h"

class PlayerManager {

private:

    inline static std::shared_mutex _rw_lock;
    static std::unordered_map<std::string, std::shared_ptr<Player>> _players_lut;

public:
    /** @brief tries to get the player ptr to the player with id player_id
   *  @param  player_id id of player to get
   *  @param  player_ptr pointer to player, used for return by reference
   *  @return true if player was found, false if not
   */
    static bool try_get_player(const std::string& player_id, std::shared_ptr<Player> &player_ptr);
    
    /** @brief tries to get the player ptr to the player with id player_id if unsuccessful, adds the player to players_lut
   *  @param  player_id id of player to add or get
   *  @param  player_ptr pointer to player, used for return by reference
   *  @return true if player was found or added to the list, false if not
   */
    static bool add_or_get_player(std::string name, const std::string& player_id, std::shared_ptr<Player>& player_ptr);
    
    /** @brief removes the player from the list
   *  @param  player_id id of player to end phase
   *  @param  player_ptr pointer to player, used for return by reference, if player was found
   *  @return true if player was removed, false if not
   */
    static bool remove_player(const std::string& player_id, std::shared_ptr<Player>& player);  // not implemented
};


#endif //LAMA_PLAYER_MANAGER_H

