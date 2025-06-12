#include "game_state.h"
#include <iostream>

#include <set>

#include "serialization/vector_utils.h"
#include "exceptions/EcologgicalException.h"

GameState::GameState() : unique_serializable()
{
    this->game_id = this->get_id();
    this->build_game_board();
    this->players = {nullptr, nullptr, nullptr, nullptr};
    this->current_player_idx = 0;
    this->initial_biodiversity_number = calculate_biodiversity();
    this->current_biodiversity_number = initial_biodiversity_number;
    update_shortest_distances();
}

// Deserialization Constructor
GameState::GameState(std::string id, int player_idx, bool has_started, std::array<bool, max_num_players> phase_end_lst,
                     double biodiversity, std::array<std::shared_ptr<Player>, max_num_players> &players_arr,
                     cell_list_t cells, int _wood_price, phases current_phase, int round) : unique_serializable(id), game_id(id), current_player_idx(player_idx), started(has_started),
                                                                                            current_biodiversity_number(biodiversity), wood_price(_wood_price), current_round(round)
{
    build_game_board();
    phase_end_list = phase_end_lst;
    cells_list = cells;
    players = players_arr;
    phase = current_phase;
    update_shortest_distances();
}

GameState::GameState(std::string id) : unique_serializable(id)
{
    this->build_game_board();
    this->players = {nullptr, nullptr, nullptr, nullptr};
    this->initial_biodiversity_number = calculate_biodiversity();
    this->update_shortest_distances();
}

bool GameState::add_player(std::shared_ptr<Player> player_ptr)
{
    for (unsigned i = 0; i < max_num_players; ++i)
    { // adds a player and assigns an id if there is space
        if (players[i] == nullptr)
        {
            players[i] = player_ptr;
            player_ptr->set_player_idx(i);
            return true;
        }
    }

    return false;
}

bool GameState::remove_player(int player_idx)
{
    if (players[player_idx] != nullptr)
    {
        players[player_idx] = nullptr;
        return true;
    }

    else
        return false;
}

bool GameState::start_game()
{
    started = true;
    for (unsigned i = 0; i < max_num_players; ++i)
    {
        if (players[i] == nullptr)
        {
            return false;
        }
    }
    if (phase == lobby)
    {
        phase = road_place;
        current_round = 1;
        return true;
    }
    return false;
}

bool GameState::is_road_placeable(int cell_id, int player_idx) const
{
    std::shared_ptr<Player> player = players[player_idx];
    auto cell = cells_list[cell_id];
    cell_list_t neighbours = get_neighbours(cell);

    // check if the cell is connected to a city or a road
    bool connected = false;
    for (auto neighbour : neighbours)
    {
        if (neighbour->get_cell_type() == City || neighbour->get_num_roads() > 0)
        {
            connected = true;
            break;
        }
    }

    // road is placeable if connected and player has enough balance and cell is road placeable
    if (connected && player->get_balance() > 0 && cell->is_road_placeable())
    {
        return true;
    }

    return false;
}

bool GameState::place_road(int cell_id, int player_idx)
{
    std::shared_ptr<Player> player = players[player_idx];
    if (is_road_placeable(cell_id, player_idx))
    {
        cells_list[cell_id]->place_road();
        player->reduce_balance(1);
        update_shortest_distances();
        return true;
    }

    return false;
}

bool GameState::are_trucks_buyable(int player_idx, int big_truck, int small_truck) const
{
    if (get_player_ptr(player_idx) == nullptr)
    {
        return false;
    }
    std::shared_ptr<Player> player = players[player_idx];

    // either the player has enough balance or the player is not buying any trucks
    return player->get_balance() >= (big_truck + small_truck) || (big_truck + small_truck) == 0;
}

bool GameState::buy_truck(int player_idx, int big_truck, int small_truck)
{
    std::shared_ptr<Player> player = players[player_idx];
    if (are_trucks_buyable(player_idx, big_truck, small_truck))
    {
        // trucks cost one money unit each
        player->reduce_balance(big_truck + small_truck);
        // the trucks are added to the garage
        player->add_num_trucks(big_truck, small_truck);
        return true;
    }

    else
        return false;
}
bool GameState::is_truck_placeable(int cell_id, int idx, TruckType Truck) const
{
    std::shared_ptr<Player> player = players[idx];

    // check if the player has enough trucks and the truck can be placed on the cell
    if (Truck == TruckType::Big && (player->get_num_trucks().first > 0) && cells_list[cell_id]->is_truck_placeable(std::make_pair(player->get_player_idx(), Truck)))
    { // checking in case the player wants to place a big truck
        return true;
    }
    else if (Truck == TruckType::Small && (player->get_num_trucks().second > 0) && cells_list[cell_id]->is_truck_placeable(std::make_pair(player->get_player_idx(), Truck)))
    { // cheking in case the player wants to place a small truck
        return true;
    }
    else
        return false;
}

bool GameState::place_truck(int cell_id, int player_idx, TruckType Truck)
{
    std::shared_ptr<Player> player = players[player_idx];
    if (is_truck_placeable(cell_id, player_idx, Truck))
    {
        cells_list[cell_id]->place_truck(std::make_pair(player_idx, Truck));
        if (Truck == TruckType::Big)
        {
            player->reduce_num_trucks(1, 0);
        }
        else
        {
            player->reduce_num_trucks(0, 1);
        }
        return true;
    }
    return false;
}

bool GameState::are_trucks_removable(int cell_id, int idx, int big_truck, int small_truck) const
{
    //checks if trucks can be removed from the cell
    return cells_list[cell_id]->are_trucks_removable(big_truck, small_truck, idx);
}

bool GameState::remove_trucks(int cell_id, int idx, int big_truck, int small_truck)
{
    std::shared_ptr<Player> player = players[idx];

    //check if trucks can be removed from the cell
    if (cells_list[cell_id]->remove_trucks(big_truck, small_truck, idx))
    {
        //trucks are added back to the garage
        player->add_num_trucks(big_truck, small_truck);
        return true;
    }
    else
        return false;
}


void GameState::build_game_board()
{

    cells = board_t(10, std::vector<std::shared_ptr<Cell>>(10, nullptr));
    cells_list = cell_list_t(45, nullptr);

//cell indices that describe the setup of the gameboard
    cell_indices = {
        {3, 0},
        {1, 1},
        {2, 1},
        {3, 1},
        {4, 1},
        {5, 1},
        {6, 1}, // 6
        {1, 2},
        {2, 2},
        {3, 2},
        {4, 2},
        {5, 2},
        {6, 2},
        {7, 2}, // 13
        {2, 3},
        {3, 3},
        {4, 3},
        {5, 3},
        {6, 3},
        {7, 3}, // 19
        {3, 4},
        {4, 4},
        {5, 4},
        {6, 4},
        {7, 4}, // 24
        {3, 5},
        {4, 5},
        {5, 5},
        {6, 5},
        {7, 5},
        {8, 5}, // 30
        {3, 6},
        {4, 6},
        {5, 6},
        {6, 6},
        {7, 6},
        {8, 6},
        {9, 6}, // 37
        {4, 7},
        {5, 7},
        {6, 7},
        {7, 7},
        {8, 7},
        {9, 7}, // 43
        {7, 8}  // 44

    };

    int idx = 0;

    //build the game board
    for (auto pair : cell_indices)
    {
        std::shared_ptr<Cell> curr = std::make_shared<Cell>(idx);
        cells_list[idx] = curr;
        cells[pair.first][pair.second] = curr;
        ++idx;
    }

    // initiate all the special cells

    cells_list[0]->set_cell_type(City);
    cells_list[44]->set_cell_type(City);

    cities.push_back(cells_list[0]);
    cities.push_back(cells_list[44]);

    std::vector<int> start_road_indäxes{3, 10, 17, 22, 28, 34, 40};

    for (auto i : start_road_indäxes)
    {
        cells_list[i]->place_road();
        cells_list[i]->place_road();
        cells_list[i]->set_forest_cover_max(0);
    }

    std::vector<int> concession_center_indices = {8, 32, 12, 36};

    for (size_t i = 0; i < max_num_players; i++)
    {
        for (auto n : get_neighbours(cells_list[concession_center_indices[i]]))
        {
            n->set_concessionaire(i);
        }
        cells_list[concession_center_indices[i]]->set_concessionaire(i);
    }
}

GameState::cell_list_t GameState::get_neighbours(cell_ptr_t cell) const
{
    cell_list_t neighbours = {};
    if (cell == nullptr)
    {
        return neighbours;
    }

    int idx = cell->get_cell_id();
    int row = cell_indices.at(idx).first;
    int col = cell_indices.at(idx).second;

    //add cell with indices r and c if they are not a nullpointr, this is needed since there are edge cells
    auto conditional_add = [&](int r, int c)
    {
        if (r >= 0 && r < cells.size() && c >= 0 && c < cells.at(0).size())
        {
            if (cells.at(r).at(c) != nullptr)
            {
                neighbours.push_back(cells_list[cells.at(r).at(c)->get_cell_id()]);
            }
        }
    };
    //add the neighbour cells if they are not nullptr
    conditional_add(row - 1, col);
    conditional_add(row, col + 1);
    conditional_add(row + 1, col + 1);
    conditional_add(row + 1, col);
    conditional_add(row, col - 1);
    conditional_add(row - 1, col - 1);

    return neighbours;
}

void GameState::get_full_forest(std::vector<std::vector<unsigned int>> &cell_sets, unsigned int idx) const
{
    GameState::cell_list_t neighbours = get_neighbours(GameState::cells_list[idx]);

    //this function return the set the cell is in
    auto find_set = [&](unsigned int idx) -> unsigned int
    {
        for (unsigned int set = 0; set < cell_sets.size(); ++set)
        {
            for (unsigned int i = 0; i < cell_sets[set].size(); ++i)
            {
                if (cell_sets[set][i] == idx)
                {
                    return set;
                }
            }
        }
        return cell_sets.size();
    };

    //returns whether a cell is contained in any set
    auto contains = [&](unsigned int idx) -> bool
    {
        for (unsigned int set = 0; set < cell_sets.size(); ++set)
        {
            for (unsigned int i = 0; i < cell_sets[set].size(); ++i)
            {
                if (cell_sets[set][i] == idx)
                {
                    return true;
                }
            }
        }
        return false;
    };

    //if its a city or not untouched forest, it does not count towards biodiversity otherwise its a forest
    if (cells_list[idx]->get_cell_type() == City || cells_list[idx]->get_forest_cover() != 10)
    {
        return;
    }
    else
    {
        cell_sets.push_back({idx});
    }
    //if the neighbours are suitable we add them to the forest
    for (unsigned i = 0; i < neighbours.size(); ++i)
    {
        if (!contains(neighbours[i]->get_cell_id()) && cells_list[neighbours[i]->get_cell_id()]->get_cell_type() == Forest && cells_list[neighbours[i]->get_cell_id()]->get_forest_cover() == 10)
        {
            cell_sets[find_set(idx)].push_back(neighbours[i]->get_cell_id());
            get_full_forest(cell_sets, neighbours[i]->get_cell_id());
        }
    }
    return;
}

double GameState::calculate_biodiversity() const
{
    //initialize the counter
    double biodiversity = 0;
    //sets of connected forests
    std::vector<std::vector<unsigned int>> cell_sets = {};
    //we call get full forest on every cell
    for (unsigned int i = 0; i < cells_list.size(); ++i)
    {
        get_full_forest(cell_sets, i);
    }
    if (cell_sets.size() == 0)
    {
        return 0;
    }
    //we return the size of the biggest set
    for (unsigned int i = 0; i < cell_sets.size(); ++i)
    {
        double size = cell_sets[i].size();
        biodiversity = std::max(biodiversity, size);
    }

    return biodiversity;
}

void GameState::next_round()
{
    current_round++;
    end_of_round_calculation();
    phase = road_place;
    return;
}

// Getters and setters
int GameState::get_current_round() const
{
    return current_round;
}

int GameState::get_current_player() const
{
    return current_player_idx;
}

void GameState::set_current_player(int player_idx)
{
    current_player_idx = player_idx;
    return;
}

std::vector<std::pair<int, int>> GameState::get_cell_indices() const
{
    return cell_indices;
}

void GameState::set_fuel_price(int price)
{
    fuel_price = price;
    return;
}

Cell GameState::get_cell(int row, int col) const
{
    return *(cells[row][col]);
}

bool GameState::is_finished() const
{
    return current_round > max_round;
}

bool GameState::get_end_phase(int player_idx) const
{
    return phase_end_list[player_idx];
}

void GameState::set_end_phase(int player_idx)
{
    phase_end_list[player_idx] = true;
}

bool GameState::should_end_phase()
{
    for (unsigned i = 0; i < max_num_players; ++i)
    {
        if (!phase_end_list[i])
        {
            return false;
        }
    }
    return true;
}


bool GameState::is_started() const
{
    return started;
}

std::vector<Player *> GameState::get_players() const
{
    std::vector<Player *> players;
    for (auto &player : this->players)
    {
        if (player)
        {
            players.push_back(player.get());
        }
    }
    return players;
}

bool GameState::is_startable() const
{
    return get_num_players() == max_num_players;
}

bool GameState::is_joinable() const
{
    return get_num_players() < max_num_players;
}

bool GameState::is_empty() const
{
    return get_num_players() == 0;
}

std::string GameState::get_game_id() const
{
    return this->get_id();
}
unsigned int GameState::get_num_players() const
{
    unsigned int counter = 0;
    for (unsigned int i = 0; i < max_num_players; ++i)
    {
        if (players[i] != nullptr)
        {
            ++counter;
        }
    }
    return counter;
}

int GameState::get_wood_price() const
{
    return wood_price;
}

void GameState::set_wood_price(int price)
{
    wood_price = price;
    return;
}

int GameState::get_fuel_price() const
{
    return fuel_price;
}

std::shared_ptr<Player> GameState::get_player_ptr(int player_idx) const
{
    return players[player_idx];
}

std::shared_ptr<Player> GameState::get_player_ptr(std::string player_id) const
{
    for (std::shared_ptr<Player> player : players)
    {
        if (player->get_player_id() == player_id)
        {
            return player;
        }
    }
    return nullptr;
}

int GameState::get_player_idx(std::string player_id) const
{
    for (unsigned i = 0; i < max_num_players; ++i)
    {
        if (players[i] != nullptr && players[i]->get_player_id() == player_id)
        {
            return i;
        }
    }
    return -1;
}

std::shared_ptr<Cell> GameState::get_cell_ptr(int cell_id)
{
    return cells_list[cell_id];
}

Cell GameState::get_cell(unsigned int idx) const
{
    return *(cells_list[idx]);
}

phases GameState::get_current_phase() const
{
    return phase;
}

void GameState::set_current_phase(phases new_phase)
{
    phase = new_phase;
    return;
}

// helper functions
void GameState::reset_phase_end_list()
{
    for (unsigned i = 0; i < max_num_players; ++i)
    {
        phase_end_list[i] = false;
    }
    return;
}

void GameState::update_shortest_distances()
{
    for (size_t i = 0; i < 3; i++)
    //do once for each road capacity
    {
        for (auto city : cities)
        //use the minimum distance to either of the cities
        {
            //algortithm very similar to dykstra
            cell_list_t stack;
            stack.push_back(city);
            while (stack.size() > 0)
            {
                cell_ptr_t curr = stack.back();
                stack.pop_back();
                cell_list_t neighbours = get_neighbours(curr);
                std::set<cell_ptr_t> visited;
                for (auto neighbour : neighbours)
                {
                    if (visited.find(neighbour) != visited.end())
                        continue;
                    if (neighbour->get_num_roads() < i + 1)
                        continue;
                    if (neighbour->get_shortest_distance(i) >= curr->get_shortest_distance(i) + 1 || neighbour->get_shortest_distance(i) == 0)
                    {
                        neighbour->set_shortest_distance(i, curr->get_shortest_distance(i) + 1);
                        stack.push_back(neighbour);
                    }
                    visited.insert(neighbour);
                }
            }
        }
    }
}


void GameState::end_of_round_calculation()
{
    update_shortest_distances();

    
    std::vector<int> drive_distance(4, 0); //stores the total amount each player has to drive from this cell
    bool enough_wood_for_everyone; //checks if this cell has enough wood, provided it is shared

    for (auto cell : cells_list)
    {
        enough_wood_for_everyone = true;
        int num_trucks = cell->get_num_trucks();
        auto trucks = cell->get_trucks();

        if (cell->get_concessionaire() == -1)
        //shared cells need to be handled differently
        {
            if (num_trucks == 2)
            {
                if (!(trucks[0].first == trucks[1].first))
                {
                    //calculate the total amount of wood that is being logged
                    int sum = 0;
                    trucks[0].second == TruckType::Big ? sum += 2 : sum += 1;
                    trucks[1].second == TruckType::Big ? sum += 2 : sum += 1;

                    //check if there is enough wood for everyone
                    if (sum > cell->get_forest_cover())
                    {
                        enough_wood_for_everyone = false;
                        cell->log_wood(sum);
                    }
                }
            }
            else if (num_trucks == 3)
            {
                if (!(trucks[0].first == trucks[1].first && trucks[1].first == trucks[2].first))
                {
                    int sum = 0;
                    trucks[0].second == TruckType::Big ? sum += 2 : sum += 1;
                    trucks[1].second == TruckType::Big ? sum += 2 : sum += 1;
                    trucks[2].second == TruckType::Big ? sum += 2 : sum += 1;
                    if (sum > cell->get_forest_cover())
                    {
                        enough_wood_for_everyone = false;
                        cell->log_wood(sum);
                    }
                }
            }
        }
        //iterate through all trucks on the cell and log the wood
        for (auto truck : trucks)
        {
            //get the shortest distance for fuel costs
            drive_distance[truck.first] += cell->get_shortest_distance(num_trucks - 1);

            //log the wood depending on the truck type
            if (truck.second == TruckType::Big && enough_wood_for_everyone)
            {
                int wood = cell->log_wood(2);
                players[truck.first]->add_to_balance(wood * wood_price);
                players[truck.first]->add_to_total_logged(wood);
            }
            else if (truck.second == TruckType::Small && enough_wood_for_everyone)
            {
                int wood = cell->log_wood(1);
                players[truck.first]->add_to_balance(wood * wood_price);
                players[truck.first]->add_to_total_logged(wood);
            }
        }
        cell->regrow_forest();
    }

    //reduce the balance of the players by the fuel costs
    for (int i = 0; i < max_num_players; ++i)
    {
        players[i]->reduce_balance((unsigned int)((drive_distance[i] + 4) / 5));
    }

    //update wood price if biodiversity has dropped  by 50%
    current_biodiversity_number = calculate_biodiversity();
    if (current_biodiversity_number < initial_biodiversity_number * 0.5)
    {
        set_wood_price(1);
    }

    return;
}

// Serialization

std::shared_ptr<const GameState> GameState::from_json(const rapidjson::Value &json)
{

    if (json.HasMember("game_id") && json.HasMember("current_player_idx") && json.HasMember("started") && json.HasMember("phase_end_list") && json.HasMember("current_biodiversity") && json.HasMember("wood_price") && json.HasMember("phase") && json.HasMember("current_round") && json.HasMember("playerArray") && json.HasMember("cellVector"))
    {
        std::array<bool, max_num_players> deserialized_end_phase_list;
        for (int i = 0; i < max_num_players; i++)
        {
            deserialized_end_phase_list.at(i) = json["phase_end_list"].GetArray()[i].GetBool();
        }

        std::array<std::shared_ptr<Player>, max_num_players> deserialized_players;
        int i = 0;
        for (auto &serialized_player : json["playerArray"].GetArray())
        {
            if (serialized_player.IsNull())
            {
                deserialized_players.at(i) = nullptr;
            }
            else
            {
                deserialized_players.at(i) = Player::deserialize(serialized_player.GetObject());
            }
            i++;
        }

        std::vector<std::shared_ptr<Cell>> deserialized_cells;
        for (auto &serialized_cell : json["cellVector"].GetArray())
        {
            if (serialized_cell.IsNull())
            {
                deserialized_cells.push_back(nullptr);
            }
            else
            {
                deserialized_cells.push_back(Cell::deserialize(serialized_cell.GetObject()));
            }
        }

        return std::make_shared<const GameState>(GameState(json["game_id"].GetString(),
                                                           json["current_player_idx"].GetInt(),
                                                           json["started"].GetBool(),
                                                           deserialized_end_phase_list,
                                                           json["current_biodiversity"].GetDouble(),
                                                           deserialized_players,
                                                           deserialized_cells,
                                                           json["wood_price"].GetInt(),
                                                           static_cast<phases>(json["phase"].GetInt()),
                                                           json["current_round"].GetInt()));
    }
    else
    {
        throw EcologgicalException("Failed to deserialize game_state. Required entries were missing.");
    }
}

void GameState::write_into_json(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const
{
    unique_serializable::write_into_json(json, allocator);

    rapidjson::Value game_id_val(game_id.c_str(), allocator);
    json.AddMember("game_id", game_id_val, allocator);

    json.AddMember("current_player_idx", current_player_idx, allocator);
    json.AddMember("started", started, allocator);
    json.AddMember("current_biodiversity", current_biodiversity_number, allocator);
    json.AddMember("wood_price", wood_price, allocator); // not needed for client (only used in calculations)
    json.AddMember("phase", static_cast<int>(phase), allocator);
    json.AddMember("current_round", current_round, allocator);

    rapidjson::Value endPhaseArray(rapidjson::kArrayType);
    for (bool val : phase_end_list)
    {
        endPhaseArray.PushBack(val, allocator);
    }
    json.AddMember("phase_end_list", endPhaseArray, allocator);

    rapidjson::Value playerArray(rapidjson::kArrayType);

    for (std::shared_ptr<Player> player : players)
    {
        if (player != nullptr)
        {
            rapidjson::Value player_json;
            player->serialize(player_json, allocator);
            playerArray.PushBack(player_json, allocator);
        }
        else
        {
            rapidjson::Value null(rapidjson::kNullType);
            playerArray.PushBack(null, allocator);
        }
    }

    json.AddMember("playerArray", playerArray, allocator);

    rapidjson::Value cellVector(rapidjson::kArrayType);

    for (GameState::cell_ptr_t cell : cells_list)
    {
        if (cell != nullptr)
        {
            rapidjson::Value cell_json;
            cell->serialize(cell_json, allocator);
            cellVector.PushBack(cell_json, allocator);
        }
        else
        {
            rapidjson::Value null(rapidjson::kNullType);
            playerArray.PushBack(null, allocator);
        }
    }

    json.AddMember("cellVector", cellVector, allocator);
}