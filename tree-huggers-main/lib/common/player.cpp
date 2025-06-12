#include "player.h"
#include "exceptions/EcologgicalException.h"
#include <iostream>

//Constructor to initialize Player with Id and name
Player::Player(Player_id_type player_id, std::string player_name) 
    : player_id(player_id), name(player_name), total_logged(0), balance(10), garage(0, 0) {}

Player::Player(Player_id_type player_id, Player_idx_type player_idx, std::string player_name, int balance,
            int big_trucks, int small_trucks, int total_logged) : player_id(player_id), player_idx(player_idx),
            name(player_name), balance(balance), total_logged(total_logged) {
                garage = std::pair(big_trucks, small_trucks);
            }

// Get the current balance of the player
int Player::get_balance() const {
    return balance;
}

// Set a new balance for the player
void Player::set_balance(int new_balance) {
    balance = new_balance;
}

// Add a specified amount to the player's balance
void Player::add_to_balance(unsigned int add) {
    balance += add;
}

// Subtract a specified amount from the player's balance
void Player::reduce_balance(unsigned int sub) {
    balance -= sub;
}

// Get the player's name
std::string Player::get_name() const {
    return name;
}

// Get the number of big and small trucks in the player's garage
std::pair<int, int> Player::get_num_trucks() const {
    return garage;
}

// Set the number of big and small trucks in the player's garage
void Player::set_num_trucks(int big_trucks, int small_trucks) {
    garage = std::make_pair(big_trucks, small_trucks);
}

// Add a specified number of big and small trucks to the player's garage
void Player::add_num_trucks(int big_trucks, int small_trucks) {
    garage.first += big_trucks;
    garage.second += small_trucks;
}

bool Player::are_trucks_reduceable(int big_trucks, int small_trucks) {
    return garage.first >= big_trucks && garage.second >= small_trucks;
}
// Reduce the number of big and small trucks in the player's garage
// Returns false if there are not enough trucks to reduce, true otherwise
bool Player::reduce_num_trucks(int big_trucks, int small_trucks) {
    if (!are_trucks_reduceable(big_trucks, small_trucks)) {
        return false;
    }
    garage.first -= big_trucks;
    garage.second -= small_trucks;
    return true;
}

// Get the total amount logged by the player
int Player::get_total_logged() const {
    return total_logged;
}

// Get the player's ID
Player::Player_id_type Player::get_player_id() const {
    return player_id;
}

// Get the game ID
std::string Player::get_game_id() const {
    return game_id;
}

// Set the game ID
void Player::set_game_id(std::string new_game_id) {
    game_id = new_game_id;
}

//add to total logged
void Player::add_to_total_logged(int add){
    total_logged += add;
}

void Player::set_player_id(Player_id_type id){
    player_id = id;
}

Player::Player_idx_type Player::get_player_idx() const {
    return player_idx;
}

void Player::set_player_idx(Player_idx_type idx){
    player_idx = idx;
}

// Serialization
std::shared_ptr<Player> Player::deserialize(const rapidjson::Value& json){

    if(json.HasMember("player_id") && 
        json.HasMember("player_idx") && 
        json.HasMember("player_name") && 
        json.HasMember("balance") && 
        json.HasMember("big_trucks") && 
        json.HasMember("small_trucks") && 
        json.HasMember("total_logged")){
        return std::make_shared<Player>(
                json["player_id"].GetString(),
                json["player_idx"].GetInt(),
                json["player_name"].GetString(),
                json["balance"].GetInt(),
                json["big_trucks"].GetInt(),
                json["small_trucks"].GetInt(),
                json["total_logged"].GetInt()
            );
        }
    else{
        return nullptr;
        throw EcologgicalException("Failed to deserialize Cell. Required entries were missing.");
    }
    
}

void Player::serialize(rapidjson::Value& jsonObject, rapidjson::Document::AllocatorType& allocator) const {
        jsonObject.SetObject();
        jsonObject.AddMember("player_id", rapidjson::Value(player_id.c_str(), allocator), allocator);
        jsonObject.AddMember("player_idx", player_idx, allocator);
        jsonObject.AddMember("player_name", rapidjson::Value(name.c_str(), allocator), allocator);
        jsonObject.AddMember("balance", balance, allocator);
        jsonObject.AddMember("big_trucks", garage.first, allocator);
        jsonObject.AddMember("small_trucks", garage.second, allocator);
        jsonObject.AddMember("total_logged", total_logged, allocator);
    }
