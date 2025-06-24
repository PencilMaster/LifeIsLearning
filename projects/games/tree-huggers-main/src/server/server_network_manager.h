#pragma once

#include "sockpp/tcp_socket.h"
#include "sockpp/tcp_connector.h"
#include "sockpp/tcp_acceptor.h"

#include "../../lib/common/network/requests/client_request.h"
#include "../../lib/common/network/responses/server_response.h"
#include "../../lib/common/player.h"
#include "../../lib/common/game_state.h"

#include <thread>
#include <functional>
#include <unordered_map>
#include <shared_mutex>

class ServerNetworkManager {
private:

    inline static ServerNetworkManager* _instance;
    inline static std::shared_mutex _rw_lock;
    inline static sockpp::tcp_acceptor _acc;

    inline static std::unordered_map<std::string, std::string> _player_id_to_address;
    inline static std::unordered_map<std::string, sockpp::tcp_socket> _address_to_socket;

    /** @brief builds up the connection
     * @param port the port where the server will be listening
   */
    void connect(const std::string& url, const uint16_t  port);

    /** @brief loop listening for incoming responses
   */
    static void listener_loop();

    /** @brief read an incoming message via sockpp
   */
    static void read_message(sockpp::tcp_socket socket,
                             const std::function<void(const std::string&, const sockpp::tcp_socket::addr_t&)>& message_handler);
    
    /** @brief handle incoming messages
   */
    static void handle_incoming_message(const std::string& msg, const sockpp::tcp_socket::addr_t& peer_address);
    
    /** @brief send back a message via sockpp
   */
    static ssize_t send_message(const std::string& msg, const std::string& address);
public:
    ServerNetworkManager();
    ~ServerNetworkManager();

    /** @brief send fullstatemessages to all other players
   *  @param  msg message to send
   *  @param  players vector of players to send the message to
   *  @param  exclude player not to send message to (usually the requester as they already get a request response)
   */
    static void broadcast_message(ServerResponse& msg, const std::vector<Player*>& players, const Player* exclude);
    
    /** @brief send a message to a player
   *  @param  msg message to send
   *  @param  player player to send the message to
   */
    static void send_message_to_player(ServerResponse& msg, const Player* player);

    /** @brief remove player if they left
   *  @param  player_id player_id of the player that left
    */
    static void on_player_left(std::string player_id);
};

