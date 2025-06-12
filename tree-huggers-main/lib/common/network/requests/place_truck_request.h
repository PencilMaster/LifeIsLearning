#ifndef PLACE_TRUCK_REQUEST_H
#define PLACE_TRUCK_REQUEST_H

#include "client_request.h"
#include "../../cell.h"
#include <string>

// Represents a request to place a truck on the game board
class PlaceTruckRequest : public ClientRequest {
public:
    // Constructor for creating a `PlaceTruckRequest`
    PlaceTruckRequest(std::string client_id, std::string game_id, int cell_id, TruckType truck_type);

    /** @brief serialize the request into json format for network transmission
     *  @param json the json file to write the request into
     *  @param allocator parameter needed by rapidjson
     *  @return void
    **/
    void write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const override;

    /** @brief deserialize the request from json format
     *  @param json the json file to read the json format message from
     *  @return pointer to the deserialized PlaceTruckRequest object
    **/
    static PlaceTruckRequest* from_json(const rapidjson::Value& json);

    //Deserialization Constructor
    PlaceTruckRequest(base_class_properties props, int cell_id, TruckType truck_type);

    //Getters for the request's member variables
    int get_cell_id() const { return this->_cell_id; }
    TruckType get_truck_type() const { return this->_truck_type; }

private:
    int _cell_id;               // The ID of the cell where the truck will be placed
    TruckType _truck_type;      // Type of the truck being placed (e.g., "big_truck" or "small_truck")
};

#endif // PLACE_TRUCK_REQUEST_H