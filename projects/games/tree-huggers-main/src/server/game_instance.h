#pragma once

#include <memory>
#include <mutex>
#include "game_state.h"


class GameInstance{

  std::shared_ptr<GameState> game_state;
  std::mutex game_instance_mutex;

public:
  GameInstance();

  //Deconstructor
  ~GameInstance(){}

  /** @brief  adds a player to the game
   *  @param  player_ptr shared pointer to player
   */
  void add_player(std::shared_ptr<Player>);

  /** @brief  tries to add a player to the game
   *  @param  player_ptr shared pointer to player
   *  @return returns if adding the player was successful
   */
  bool try_add_player(std::shared_ptr<Player> &);

  /** @brief  removes a player from the game
   *  @param  size_t id of player
   */
  void remove_player(size_t const&);

  /** @brief  gets game state
   *  @return shared pointer to game state
   */
  std::shared_ptr<GameState> get_game_state();

  /** @brief  gets game id
   *  @return std::string game id
   */
  std::string get_game_id();

  /** @brief checks if less than four players, not started 
   *  @return true if joinable, false otherwise
  */
  bool is_joinable();

  /** @brief checks if game is empty
   * @return true if empty, false otherwise
   */
  bool is_empty();

  /** @brief attempts start the game, checks if 4 players have joined
   *  @return true if game was started
   */
  bool start_game(std::shared_ptr<Player> &player_ptr);

  /** @brief attempts to place road, checks if connection to city exits and if player has enough balance
   *  @param  cell_id id of cell to place road on
   *  @param  player_idx index of player
   *  @param  err string to store error messages
   *  @return true if road is placed, false otherwise
   */
  bool place_road(int cell_id, int player_idx, std::string &err);

  /** @brief attempts to place truck, checks if player has enough trucks and if cell is empty
   *  @param  cell_id index of cell to place truck on
   *  @param  player_idx index of player placing
   *  @param  truck_type type of truck to place
   *  @param  err string to store error messages
   *  @return true if truck is placed, false otherwise
   */
  bool place_truck(int cell_id, int player_idx, TruckType truck_type, std::string &err);

  /** @brief attempts to buy truck, checks if player has enough balance
   *  @param  player_idx index of player buying trucks
   *  @param  big_truck number of big trucks to buy
   *  @param  small_truck number of small trucks to buy
   *  @param  err string to store error messages
   *  @return true if truck is bought, false otherwise
   */
  bool buy_trucks(int player_idx, int big_truck, int small_truck, std::string &err);

  /** @brief attempts to remove trucks on a cell
   *  @param  cell_id index of cell to remove truck from
   *  @param  player_idx index of player
   *  @param  big_truck number of big trucks to remove
   *  @param  small_truck number of small trucks to remove
   *  @param  err string to store error messages
   *  @return true if truck is bought, false otherwise
   */
  bool remove_trucks(int cell_id, int player_idx, int big_truck, int small_truck, std::string &);

  /** @brief end the current phase for the player specified by the id, if all players ended go to next phase
   *  @param  player_idx idx of player to end phase
   *  @param  err string to store error messages
   *  @return true if phase was ended, false otherwise
   */
  bool end_phase(int player_idx, std::string &err);

  /** @brief set current player to the next person
   */
  void next_player();

  private:
  /** @brief end the current phase for the player specified by the id, if all players ended go to next phase
   *  @param  player_idx idx of player to end phase
   *  @param  err string to store error messages
   *  @return true if phase was ended, false otherwise
   */
  bool end_phase_no_lock(int player_idx, std::string &err);

  /** @brief starts a new road placing phase
   */
  void start_road_placing_phase();

  /** @brief starts a new trick buying phase
   */
  void start_truck_buying_phase();

  /** @brief starts a new truck placing phase
   */
  void start_truck_placing_phase();

  /** @brief ends the current round
   */
  void end_of_round();







};
