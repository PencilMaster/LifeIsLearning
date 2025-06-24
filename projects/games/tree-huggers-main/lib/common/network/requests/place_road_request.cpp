#include "place_road_request.h"
#include "../../serialization/uuid_generator.h"

// Constructor for creating a `PlaceRoadRequest` with client ID, game ID and cell ID
PlaceRoadRequest::PlaceRoadRequest(std::string client_id, std::string game_id, int cell_id)
    : ClientRequest(ClientRequest::create_base_class_properties(
          RequestType::place_road, uuid_generator::generate_uuid_v4(), client_id, game_id)),
      _cell_id(cell_id) {}

// Private constructor for deserialization
PlaceRoadRequest::PlaceRoadRequest(base_class_properties props, int cell_id)
    : ClientRequest(props), _cell_id(cell_id) {}

// Serialize the request to JSON
void PlaceRoadRequest::write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::write_into_json(json, allocator);
    json.AddMember("cell_id", _cell_id, allocator);
}

// Deserializes the JSON to get the `PlaceRoadRequest` 
PlaceRoadRequest* PlaceRoadRequest::from_json(const rapidjson::Value& json) {
    if (json.HasMember("cell_id")) {
        return new PlaceRoadRequest(ClientRequest::extract_base_class_properties(json), json["cell_id"].GetInt());
    } else {
        throw std::runtime_error("Could not parse PlaceRoadRequest from JSON. 'cell_id' is missing.");
    }
}