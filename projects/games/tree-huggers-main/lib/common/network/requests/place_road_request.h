#ifndef PLACE_ROAD_REQUEST_H
#define PLACE_ROAD_REQUEST_H

#include "client_request.h"
#include <string>
#include <memory>

class PlaceRoadRequest : public ClientRequest {
public:
    // Constructor for creating a `PlaceRoadRequest`
    PlaceRoadRequest(std::string client_id, std::string game_id, int cell_id);

    /** @brief serialize the request into json format for network transmission
     *  @param json the json file to write the request into
     *  @param allocator parameter needed by rapidjson
     *  @return void
    **/
    void write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const override;

    /** @brief deserialize the request from json format
     *  @param json the json file to read the json format message from
     *  @return pointer to the deserialized PlaceRoadRequest object
    **/
    static PlaceRoadRequest* from_json(const rapidjson::Value& json);

    //Deserialization Constructor
    PlaceRoadRequest(ClientRequest::base_class_properties props, int cell_id);

    //Getter for the request's member variable
    int get_cell_id() const { return this->_cell_id; }

private:
    int _cell_id;
};

#endif // PLACE_ROAD_REQUEST_H