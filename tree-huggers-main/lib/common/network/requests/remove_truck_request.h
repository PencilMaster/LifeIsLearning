#ifndef REMOVE_TRUCK_REQUEST_H
#define REMOVE_TRUCK_REQUEST_H

#include "client_request.h"
#include <string>

// Represents a request to remove a truck from the game board
class RemoveTruckRequest : public ClientRequest {
public:
    // Constructor for creating a `RemoveTruckRequest`
    RemoveTruckRequest(std::string client_id, std::string game_id, int cell_id, int big_trucks, int small_trucks);

    /** @brief serialize the request into json format for network transmission
     *  @param json the json file to write the request into
     *  @param allocator parameter needed by rapidjson
     *  @return void
    **/
    void write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const override;

    /** @brief deserialize the request from json format
     *  @param json the json file to read the json format message from
     *  @return pointer to the deserialized RemoveTruckRequest object
    **/
    static RemoveTruckRequest* from_json(const rapidjson::Value& json);

    //Deserialization Constructor
    RemoveTruckRequest(base_class_properties props, int cell_id, int big_trucks, int small_trucks);

    //getters for the request's member variables
    int get_cell_id() const { return this->_cell_id; }
    int get_big_trucks() const { return this->_big_trucks; }
    int get_small_trucks() const { return this->_small_trucks; }

private:
    int _cell_id;                 // The ID of the cell from which trucks will be removed
    int _big_trucks;              // Number of big trucks to remove
    int _small_trucks;            // Number of small trucks to remove
};

#endif // REMOVE_TRUCK_REQUEST_H