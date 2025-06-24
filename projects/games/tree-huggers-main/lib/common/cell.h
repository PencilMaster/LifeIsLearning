#ifndef CELL_H
#define CELL_H

#include <vector>
#include <array>
#include <utility>
#include <memory>
#include "player.h"

enum TruckType {
    Big,
    Small
};

enum CellType {
    City,
    Forest
};


class Cell {

public:
//typedefs
    using player_id_t = Player::Player_id_type;
    using player_idx_t = Player::Player_idx_type;

private:
//class variables
    // Type of the cell (City or Forest)
    CellType cell_type;

    // Current forest cover of the cell
    int forest_cover;

    // Maximum forest cover the cell can have
    int forest_cover_max;

    // Number of roads placed on the cell
    int num_roads;

    // Unique identifier for the cell
    int cell_id;

    // Vector storing pairs of player_idx (truck owner) and truck type present on the cell
    std::vector<std::pair<int, TruckType>> trucks;

    // Vector storing the shortest distances to other cells
    std::array<unsigned int, 3> shortest_distances;

    // Concessionaire of the cell
    int concessionaire;

public:
//class methods
    // Constructor to initialize a Cell with a list index and cell type
    Cell(int list_index, CellType celltype);

    // Constructor to initialize a Cell with a list index and default cell type (Forest)
    Cell(int list_index);

    // Deserialization Constructor for comfort
    Cell(int cell_id, CellType cell_type, int forest_cover, int forest_cover_max, int num_roads,
            std::vector<std::pair<player_idx_t, TruckType>> truckArray, std::array<unsigned int, 3> distArray,
            int concessionaire);


    /**
     * @brief Place a road on the cell. Checks if less than 3 roads are placed.
     * @return true if the road is placed, false otherwise
     */
    bool place_road();

    /**
     * @brief Check if a road can be placed on the cell.
     * @return true if a road can be placed, false otherwise
     */
    bool is_road_placeable() const;

    /**
     * @brief Place a truck on the cell. Checks number of trucks placed and number of roads and concession
     * @param truck a pair of player_idx (the player the truck belong to) and truck type
     * @return true if the truck is placed, false otherwise
     */
    bool place_truck(std::pair<player_idx_t, TruckType> truck);

    /**
     * @brief Check if a truck can be placed on the cell. Checks number of trucks placed and number of roads and concession
     * @param truck a pair of player_idx (the player the truck belongs to) and truck type
     * @return true if the truck can be placed, false otherwise
     */
    bool is_truck_placeable(std::pair<player_idx_t, TruckType> truck) const;

    /**
     * @brief Remove trucks from the cell. Checks wheter the player has enough trucks placed on the cell.
     * @param big_trucks number of big trucks to remove
     * @param small_trucks number of small trucks to remove
     * @param player idx of the player requesting the removal
     * @return true if trucks are removed, false otherwise
     */
    bool remove_trucks(int big_trucks, int small_trucks, int player);

    /**
     * @brief Check if trucks can be removed from the cell. Checks wheter the player has enough trucks placed on the cell.
     * @param big_trucks number of big trucks to remove
     * @param small_trucks number of small trucks to remove
     * @param player idx of the player requesting the removal
     * @return true if trucks can be removed, false otherwise
     */
    bool are_trucks_removable(int big_trucks, int small_trucks, int player) const;

    /**
     * @brief Attempt to log the specified amount of wood in the cell, capped by the amount present
     * @param amount amount of wood that is attempted to be logged
     * @return amount of wood actually logged
     */
    int log_wood(int amount);

    /**
     * @brief Regrow the forest in the cell. By 1, up to max forest cover and only if the forest cover is not 0.
     */
    void regrow_forest();

    /**
     * @brief Get the forest cover of the cell.
     * @return the current forest cover
     */
    int get_forest_cover() const;

    /**
     * @brief Set the forest cover of the cell.
     * @param forest_cover the new forest cover value
     */
    void set_forest_cover(int forest_cover);

    /**
     * @brief Get the maximum forest cover of the cell.
     * @return the maximum forest cover
     */
    int get_forest_cover_max() const;

    /**
     * @brief Set the maximum forest cover of the cell. Also checks if the forest cover is greater than the max forest cover.
     * @param forest_cover_max the new maximum forest cover value
     */
    void set_forest_cover_max(int forest_cover_max);

    /**
     * @brief Get the number of roads on the cell.
     * @return the number of roads
     */
    int get_num_roads() const;

    /**
     * @brief Set the number of roads on the cell.
     * @param num_roads the new number of roads
     */
    void set_num_roads(int num_roads);

    /**
     * @brief Get the type of the cell.
     * @return the cell type
     */
    CellType get_cell_type() const;

    /**
     * @brief Set the type of the cell.
     * @param type the new cell type
     */
    void set_cell_type(CellType type);

    /**
     * @brief Get the unique identifier of the cell.
     * @return the cell id
     */
    int get_cell_id() const;

    /**
     * @brief Set the concessionaire of the cell.
     * @param player the idx of the player to set as concessionaire
     */
    void set_concessionaire(int player);

    /**
     * @brief Get the concessionaire of the cell.
     * @return the concessionaire idx
     */
    player_idx_t get_concessionaire() const;

    /**
     * @brief Get the shortest distance to the closest city.
     * @param index the index of the distance to get, index = (numtrucks - 1)
     * @return the shortest distance
     */
    unsigned int get_shortest_distance(size_t index) const;

    /**
     * @brief Set the shortest distance to the closest city.
     * @param index the index of the distance to set, index = (numtrucks - 1)
     * @param distance the new shortest distance
     */
    void set_shortest_distance(size_t index, unsigned int distance);

    /**
     * @brief Get the number of trucks on the cell.
     * @return the number of trucks
     */
    size_t get_num_trucks();

    /**
     * @brief Get the trucks on the cell.
     * @return a vector of pairs of player_idx (truck owner) and truck type
     */
    std::vector<std::pair<int, TruckType>> get_trucks() const;

    /**
     * @brief Serialize the cell to a JSON object.
     * @param jsonObject the JSON object to serialize into
     */
    void serialize(rapidjson::Value& jsonObject, rapidjson::Document::AllocatorType& allocator) const;

    /**
     * @brief Deserialize the cell from a JSON object.
     * @param jsonObject the JSON object to deserialize from
     * @return a shared pointer to the deserialized cell
     */
    static std::shared_ptr<Cell> deserialize(const rapidjson::Value& jsonObject);
};


#endif // CELL_H