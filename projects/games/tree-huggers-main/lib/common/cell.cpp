#include "cell.h"
#include "exceptions/EcologgicalException.h"

#include <iostream>

// Constructor with cell type
Cell::Cell(int list_index, CellType celltype) : Cell(list_index) {
    cell_type = celltype;
}

// Constructor with default cell type (Forest)
Cell::Cell(int list_index) {
    forest_cover_max = 10;
    forest_cover = forest_cover_max;
    num_roads = 0;
    trucks = {};
    shortest_distances = {0,0,0};
    cell_id = list_index;
    cell_type = Forest;
    concessionaire = -1;
}

// Deserialization Constructor
Cell::Cell(int cell_id, CellType cell_type, int forest_cover, int forest_cover_max, int num_roads,
            std::vector<std::pair<int, TruckType>> truckArray, std::array<unsigned int, 3> distArray,
            int concessionaire) : cell_id(cell_id), cell_type(cell_type), forest_cover(forest_cover), 
            forest_cover_max(forest_cover_max), num_roads(num_roads), trucks(truckArray), shortest_distances(distArray),
            concessionaire(concessionaire) {}


bool Cell::is_road_placeable() const {
    return num_roads < 3 && cell_type != City;
}

// Place a road in the cell
bool Cell::place_road() {
    if (!is_road_placeable())
        return false;
    else {
        this->num_roads += 1;
        this->forest_cover_max -= std::min(2, this->forest_cover_max);
        this->forest_cover = std::min(forest_cover, forest_cover_max);
        return true;
    }
}

bool Cell::is_truck_placeable(std::pair<int, TruckType> truck) const {
    if(truck.first == concessionaire || concessionaire == -1) { // if the player is the concessionaire or shared cell
        size_t num_trucks = trucks.size();
        if(num_trucks < 3 && shortest_distances[num_trucks] != 0) { // access number check
           
            return true;
        }
    }
    return false;
}

// Place a truck in the cell
bool Cell::place_truck(std::pair<int, TruckType> truck) {
    if(is_truck_placeable(truck)) {
        trucks.push_back(truck);
        return true;
    }
    return false;
}

bool Cell::are_trucks_removable(int n_big_trucks, int n_small_trucks, int player) const {
    int big_trucks = 0;
    int small_trucks = 0;
    for (unsigned int i = 0; i < this->trucks.size(); ++i) {
        if (trucks.at(i) == std::make_pair(player, Big)) {
            big_trucks += 1;
        } else if (trucks.at(i) == std::make_pair(player, Small)) {
            small_trucks += 1;
        }
    }
    return n_big_trucks <= big_trucks && n_small_trucks <= small_trucks;
}

// Remove trucks from the cell
bool Cell::remove_trucks(int n_big_trucks, int n_small_trucks, int player) {
    
    if (are_trucks_removable(n_big_trucks, n_small_trucks, player)) {
        unsigned int size = trucks.size();

        for (unsigned int n = 0; n < n_big_trucks; ++n) { // removing specified number of big trucks
            for (unsigned int i = 0; i < size; ++i) {
                if (trucks.at(i) == std::make_pair(player, Big)) {
                    std::swap(trucks[i], trucks[trucks.size() - 1]);
                    trucks.pop_back();
                    size -= 1;
                    break;
                }
            }
        }

        for (unsigned int n = 0; n < n_small_trucks; ++n) { // removing specified number of small trucks
            for (unsigned int i = 0; i < size; ++i) {
                if (trucks.at(i) == std::make_pair(player, Small)) {
                    std::swap(trucks[i], trucks[trucks.size() - 1]);
                    trucks.pop_back();
                    size -= 1;
                    break;
                }
            }
        }

        return true;
    }
    return false;
}

std::vector<std::pair<int, TruckType>> Cell::get_trucks()const{
    return trucks;
}

// Get the forest cover
int Cell::get_forest_cover() const {
    return forest_cover;
}

// Get the cell type
CellType Cell::get_cell_type() const {
    return cell_type;
}

// Set the cell type
void Cell::set_cell_type(CellType type) {
    cell_type = type;
}

// Get the cell ID
int Cell::get_cell_id() const {
    return cell_id;
}

// Set the concessionaire
void Cell::set_concessionaire(int player) {
    concessionaire = player;
}

// Get the concessionaire
int Cell::get_concessionaire() const {
    return concessionaire;
}

// Get the number of roads
int Cell::get_num_roads() const {
    return num_roads;
}

// Set the number of roads
void Cell::set_num_roads(int num_roads) {
    this->num_roads = num_roads;
}

// Get the forest cover max
int Cell::get_forest_cover_max() const {
    return forest_cover_max;
}

// Set the forest cover max
void Cell::set_forest_cover_max(int forest_cover_max) {
    if (forest_cover > forest_cover_max) {
        forest_cover = forest_cover_max;
    }
    this->forest_cover_max = forest_cover_max;
}

// Set the forest cover
void Cell::set_forest_cover(int forest_cover) {
    this->forest_cover = forest_cover;
}

// Get the shortest distance
unsigned int Cell::get_shortest_distance(size_t index) const{
    return shortest_distances.at(index);
}

// Set the shortest distance
void Cell::set_shortest_distance(size_t index, unsigned int distance) {
    shortest_distances.at(index) = distance;
}

std::shared_ptr<Cell> Cell::deserialize(const rapidjson::Value& json){
    if(json.HasMember("cell_id") &&
        json.HasMember("cell_type") &&
        json.HasMember("forest_cover") &&
        json.HasMember("forest_cover_max") &&
        json.HasMember("num_roads") &&
        json.HasMember("truckArray") &&
        json.HasMember("distanceArray") &&
        json.HasMember("concessionaire")
        ){
            std::vector<std::pair<int, TruckType>> deserialized_truckArray;
            for (auto &object : json["truckArray"].GetArray()) {
                deserialized_truckArray.push_back(
                    std::pair<int, TruckType>(object["player_idx"].GetInt(),static_cast<TruckType>(object["truck_type"].GetInt()))
                    );
            }

            std::array<unsigned int, 3> deserialized_distanceArray;
            int i = 0;
            for (auto &serialized_distance : json["distanceArray"].GetArray()) {
                deserialized_distanceArray.at(i) = serialized_distance["shortest_distance"].GetInt();
                i++;
            }
            std::shared_ptr<Cell> cell_ptr = std::make_shared<Cell>(
                json["cell_id"].GetInt(),
                static_cast<CellType>(json["cell_type"].GetInt()),
                json["forest_cover"].GetInt(),
                json["forest_cover_max"].GetInt(),
                json["num_roads"].GetInt(),
                deserialized_truckArray,
                deserialized_distanceArray,
                json["concessionaire"].GetInt()
            );
            return cell_ptr;
        }
    else{
        return nullptr;
        throw EcologgicalException("Failed to deserialize Cell. Required entries were missing.");
    }
}

void Cell::serialize(rapidjson::Value& jsonObject, rapidjson::Document::AllocatorType& allocator) const {
        jsonObject.SetObject();

        jsonObject.AddMember("cell_id", cell_id, allocator);
        jsonObject.AddMember("cell_type", static_cast<int>(cell_type), allocator); //should be the same over whole game
        jsonObject.AddMember("forest_cover", forest_cover, allocator);
        jsonObject.AddMember("forest_cover_max", forest_cover_max, allocator); //might not be needed
        jsonObject.AddMember("num_roads", num_roads, allocator);

        rapidjson::Value truckArray(rapidjson::kArrayType);
        for (const auto& pair : trucks) {
            rapidjson::Value pairObject(rapidjson::kObjectType);

            pairObject.AddMember("player_idx", pair.first, allocator); // Owner index
            pairObject.AddMember("truck_type", static_cast<int>(pair.second), allocator); // Truck enum

            truckArray.PushBack(pairObject, allocator);
        }
        jsonObject.AddMember("truckArray", truckArray, allocator);

        //only needed for legality check
        rapidjson::Value distanceArray(rapidjson::kArrayType);
        for (const auto& distance : shortest_distances) {
            rapidjson::Value distObject(rapidjson::kObjectType);

            distObject.AddMember("shortest_distance", distance, allocator);

            distanceArray.PushBack(distObject, allocator);
        }
        jsonObject.AddMember("distanceArray", distanceArray, allocator);

        jsonObject.AddMember("concessionaire", concessionaire, allocator);

    }

// Get the number of trucks
size_t Cell::get_num_trucks() {
    return trucks.size();
}

int Cell::log_wood(int amount) {
    if (amount >= forest_cover) {
        int logged = forest_cover;
        forest_cover = 0;
        forest_cover_max = 0;
        return logged;
    } else {
        forest_cover -= amount;
        return amount;
    }
}

void Cell::regrow_forest() {
    forest_cover = std::min(forest_cover_max, forest_cover + 1);
}