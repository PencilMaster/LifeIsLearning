#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <memory>

#include "../third-party/rapidjson/include/rapidjson/document.h"

class Player {
public:
    using Player_id_type = std::string;
    using Player_idx_type = int;

private:
    // Name of the player
    std::string name;

    // Unique player ID
    Player_id_type player_id;

    //game-specific player Index
    Player_idx_type player_idx;

    // Current balance of the player
    int balance = 10;

    // Pair representing the number of big and small trucks
    std::pair<int, int> garage;

    // Total amount of logs the player has logged
    int total_logged;

    // ID of the game the player is participating in
    std::string game_id;

public:
//methods
    /* @brief Constructor to initialize a player with player ID and name */
    Player(Player_id_type player_id, std::string name);

    /* @brief Deserialization Constructor for comfort */
    Player(Player_id_type player_id, Player_idx_type player_idx, std::string player_name, int balance,
            int big_trucks, int small_trucks, int total_logged);

    /* @brief Add a specified number of big and small trucks to the player's garage 
       @param big_trucks Number of big trucks to add 
       @param small_trucks Number of small trucks to add */
    void add_num_trucks(int big_trucks, int small_trucks);

    /* @brief Check if the player has enough trucks to reduce 
       @param big_trucks Number of big trucks to reduce 
       @param small_trucks Number of small trucks to reduce 
       @return True if reducible, false otherwise */
    bool are_trucks_reduceable(int big_trucks, int small_trucks);

    /* @brief Reduce the number of big and small trucks the player has 
       @param big_trucks Number of big trucks to reduce 
       @param small_trucks Number of small trucks to reduce 
       @return True if successful, false otherwise */
    bool reduce_num_trucks(int big_trucks, int small_trucks);

    /* @brief Get the total amount of logs the player has logged 
       @return Total logs */
    int get_total_logged() const;

    /* @brief Add to the total amount of logs the player has logged 
       @param logs Amount to add */
    void add_to_total_logged(int);

//=====Getters and Setters=====
    /* @brief Get the current balance of the player 
       @return Current balance */
    int get_balance() const;

    /* @brief Set the balance of the player 
       @param balance New balance */
    void set_balance(int);

    /* @brief Add a specified amount to the player's balance 
       @param amount Amount to add */
    void add_to_balance(unsigned int);

    /* @brief Reduce the player's balance by a specified amount 
       @param amount Amount to reduce */
    void reduce_balance(unsigned int);

    /* @brief Get the name of the player 
       @return Player's name */
    std::string get_name() const;

    /* @brief Get the number of big and small trucks the player has 
       @return Pair of big and small trucks */
    std::pair<int, int> get_num_trucks() const;

    /* @brief Set the number of big and small trucks the player has 
       @param big_trucks Number of big trucks 
       @param small_trucks Number of small trucks */
    void set_num_trucks(int big_trucks, int small_trucks);

    /* @brief Get the player ID 
       @return Player ID */
    Player_id_type get_player_id() const;

    /* @brief Get the player Index 
       @return Player Index */
    Player_idx_type get_player_idx() const;

    /* @brief Set the player ID 
       @param player_id New player ID */
    void set_player_id(Player_id_type);

    /* @brief Set the player Index 
       @param player_idx New player Index */
    void set_player_idx(Player_idx_type);

    /* @brief Get the game ID 
       @return Game ID */
    std::string get_game_id() const;

    /* @brief Set the game ID 
       @param game_id New game ID */
    void set_game_id(std::string);

//=====Serialization=====
    /* @brief Serialize the player data 
       @param jsonObject JSON object to serialize into 
       @param allocator Allocator for the JSON document */
    void serialize(rapidjson::Value& jsonObject, rapidjson::Document::AllocatorType& allocator) const;

    /* @brief Deserialize the player data 
       @param jsonObject JSON object to deserialize from 
       @return Shared pointer to the deserialized player */
    static std::shared_ptr<Player> deserialize(const rapidjson::Value& jsonObject);

};

#endif