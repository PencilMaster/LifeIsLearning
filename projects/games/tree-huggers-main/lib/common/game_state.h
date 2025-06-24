#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <string>
#include <vector>
#include <array>
#include <memory>
#include "cell.h"
#include "player.h"

#include "serialization/serializable.h"
#include "serialization/serializable_value.h"
#include "serialization/unique_serializable.h"

enum phases
{
    lobby,
    road_place,
    truck_buy,
    truck_place,
    game_end,
};

class GameState : public unique_serializable
{

public:

//typedefs
    using cell_ptr_t = std::shared_ptr<Cell>;
    using board_t = std::vector<std::vector<cell_ptr_t>>;
    using cell_list_t = std::vector<cell_ptr_t>;

//consts
    static const int max_num_players = 4;

private:
//class variables
    std::string game_id;//unique game id
    board_t cells; //2D vector of cells
    cell_list_t cells_list; //list of cells used for iterating 
    cell_list_t cities;
    std::vector<std::pair<int, int>> cell_indices; //list of indices of cells
    std::array<std::shared_ptr<Player>, max_num_players> players; //array of player pointers - index is player id
    int current_player_idx = 0; //player id of player whose turn it is
    double initial_biodiversity_number;
    double current_biodiversity_number;
    int wood_price = 2;
    int fuel_price = 1;
    phases phase = lobby; //current phase of the game either lobby, road_place, truck_buy, truck_place, game_end
    std::array<bool, max_num_players> phase_end_list; //list of players who have ended their phase
    int current_round = 0; //current round of the game
    bool started = false;
    bool is_finished_ = false;
    int max_round = 10; //maximum number of rounds in the game

 
 public:

    /** @brief Constructor
     * @param string UUID of the game 
     **/
    GameState(std::string game_id);

    /** @brief default Constructor
     **/
    GameState();

    /** @brief Constructor for deserialization
     * @param string UUID of the game 
     * @param int player_idx index of the current player
     * @param bool has_started true if the game has started
     * @param array<bool, max_num_players> phase_end_lst list of players who have ended their phase
     * @param double biodiversity the biodiversity of the game
     * @param array<std::shared_ptr<Player>, max_num_players> &players array of player pointers
     * @param cell_list_t cells list of cells
     * @param int wood_price the price of wood
     * @param phases phase the current phase of the game
     * @param int round the current round of the game
     **/
    GameState(std::string id, int player_idx, bool has_started, std::array<bool, max_num_players> phase_end_lst,
                        double biodiversity, std::array<std::shared_ptr<Player>, max_num_players> &players,
                        cell_list_t cells, int wood_price, phases phase, int round);

    
    /** @brief attempts to start a game, checks if max_num_players players have joined
        @return true if game was started
      **/
    bool start_game();

    /** @brief checks if the game is startable
     *  @return true if the game is startable, false otherwise
     **/
    bool is_startable() const;

    /** @brief checks if the game was started
     *  @return bool true if game was started
     */
    bool is_started() const;

    /** @brief checks if the game is joinable
     *  @return true if the game is joinable, false otherwise
     **/
    bool is_joinable() const;

    /** @brief checks if the game is empty
     *  @return true if the game is empty, false otherwise
     **/
    bool is_empty() const;



    /** @brief checks if a road can be placed on a cell
     *  @param cell_id the id of the cell to place the road on
     *  @param player_idx the player placing the road
     *  @return true if road can be placed, false otherwise
     **/
    bool is_road_placeable(int cell_id, int player_idx) const;

    /** @brief attempts to place road, checks if connection to city exits and if player has enough balance
     *  @param cell_id the cell to place the road on
     *  @param player_idx the player placing the road
     *  @return true if road is placed, false otherwise
    **/
    bool place_road(int cell_id, int player_idx);

    /** @brief attempts to place truck, checks if player has enough trucks and if cell is empty
     *  @param cell_id the cell to place the truck on
     *  @param player_idx the player placing the truck
     *  @param Truck the type of truck to place
     *  @return true if truck is placed, false otherwise
    **/
    bool place_truck(int cell, int player_idx, TruckType Truck);

    /** @brief checks if a truck can be placed on a cell
     *  @param cell_id the id of the cell to place the truck on
     *  @param player_idx the player placing the truck
     *  @param Truck the type of truck to place
     *  @return true if truck can be placed, false otherwise
     **/
    bool is_truck_placeable(int cell_id, int player_idx, TruckType Truck) const;

    /** @brief attempts to buy truck, checks if player has enough balance
     *  @param player_idx the player buying the truck
     *  @param n_big_trucks the number of big trucks to buy
     *  @param n_small_trucks the number of small trucks to buy
     *  @return true if truck is bought, false otherwise
    **/
    bool buy_truck(int player_idx, int n_big_trucks, int n_small_trucks);

    /** @brief checks if a truck can be bought by a player
     *  @param player_idx the player buying the truck
     *  @param n_big_trucks the number of big trucks to buy
     *  @param n_small_trucks the number of small trucks to buy
     *  @return true if truck can be bought, false otherwise
     **/
    bool are_trucks_buyable(int player_idx, int n_big_trucks, int n_small_trucks) const;

    /** @brief  attempts to remove trucks from a cell, 
     *          checks if there are enough trucks for the player to remove
     *  @param cell_id the cell to remove the trucks from
     *  @param player_idx the player removing the trucks
     *  @param n_big_trucks the number of big trucks to remove
     *  @param n_small_trucks the number of small trucks to remove
     *  @return true if trucks are removed, false otherwise
     * */
    bool remove_trucks(int cell_id, int player_idx, int n_big_trucks, int n_small_trucks);

    /** @brief checks if trucks can be removed from a cell
     *  @param cell_id the cell to remove the trucks from
     *  @param player_idx the player removing the trucks
     *  @param n_big_trucks the number of big trucks to remove
     *  @param n_small_trucks the number of small trucks to remove
     *  @return true if trucks can be removed, false otherwise
     **/
    bool are_trucks_removable(int cell_id, int player_idx, int n_big_trucks, int n_small_trucks) const;

    /** @brief calculates the biodiversity of the game
     *  @return the biodiversity of the game as specified in the rules
    **/
    double calculate_biodiversity() const;

    /** @brief tries to add a player to the game
     *  @param player_ptr the player to add
     *  @return true if player is added, false otherwise
     **/
    bool add_player(std::shared_ptr<Player> player_ptr);

    /** @brief removes a player from the game
     *  @param player_idx the idx of the player to remove
     *  @return true if player is removed, false otherwise
     * 
     **/
    bool remove_player(int player_idx);

    /** @brief  increments current round, sets phase road_place
     *  @return void
    **/
    void next_round();

    /** @brief  checks if all entries of phase end list are true 
     *  @return true if all entries are true, false otherwise
    */
    bool should_end_phase();

    /** @brief  resets the phase end list to all false
     *  @return void
    */
    void reset_phase_end_list();

    /** @brief  checks if 10 rounds have been played
     *  @return bool true if game is finished
    */
    bool is_finished() const;
 
    /** @brief updates the shortest distances from cells to the closest city for 1,2,3 trucks
     **/
    void update_shortest_distances();

// Getters and setters
    std::string get_game_id()const;

    int get_wood_price() const;
    void set_wood_price(int);

    int get_fuel_price() const;
    void set_fuel_price(int);

    Cell get_cell(unsigned int cell_id) const;
    Cell get_cell(int row, int col) const;
    std::shared_ptr<Cell> get_cell_ptr(int cell_id);
    std::vector<std::pair<int,int>> get_cell_indices() const;

    int get_current_player() const;
    void set_current_player(int);
    unsigned int get_num_players()const;
    int get_player_idx(std::string player_id) const;
    std::shared_ptr<Player> get_player_ptr(int player_idx) const;
    std::shared_ptr<Player> get_player_ptr(std::string player_id) const;
    std::vector<Player *> get_players() const;

    int get_current_round() const;

    bool get_end_phase(int player_idx) const;
    void set_end_phase(int player_idx);
    
    void set_current_phase(phases);
    phases get_current_phase() const;

// Serialization
    /** @brief constructs a game state from a JSON object
     *  @param json the JSON object to construct the game state from
     *  @return a shared pointer to the constructed game state
     **/
    static std::shared_ptr<const GameState> from_json(const rapidjson::Value& json);

    /** @brief writes the game state into a JSON object
     *  @param json the JSON object to write the game state into
     *  @param allocator the allocator to use for the JSON object
     *  @return void
     **/
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;    

private:
   //helper functions

    /** @brief constructs the matrix of cells
     *  @param file_path the path to the JSON file to read the matrix from
     *  TODO: Make this function read a json file to construct the matrix
     **/
    void build_game_board();

    /** @brief gets the neighbours of a cell
     *  @param cell the cell to get the neighbours of
     *  @return a list of the neighbours of the cell
     **/
    cell_list_t get_neighbours(cell_ptr_t cell) const;


    /**  @brief gets largest continuous forest, helper for biodiversity calculation 
     **/
    void get_full_forest(std::vector<std::vector<unsigned int>>& cell_sets,unsigned int) const;

    /** @brief performs end of round calculations according to rules 
     * @return void
    */
    void end_of_round_calculation();

};

#endif