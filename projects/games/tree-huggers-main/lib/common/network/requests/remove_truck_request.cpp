#include "remove_truck_request.h"
#include "../../serialization/uuid_generator.h"

// Constructor for creating a `RemoveTruckRequest` with client ID, game ID, cell ID and number of big and small trucks
RemoveTruckRequest::RemoveTruckRequest(std::string client_id, std::string game_id, int cell_id, int big_trucks, int small_trucks)
    : ClientRequest(ClientRequest::create_base_class_properties(
          RequestType::remove_truck, uuid_generator::generate_uuid_v4(), client_id, game_id)),
      _cell_id(cell_id), _big_trucks(big_trucks), _small_trucks(small_trucks) {}

// Private constructor for deserialization
RemoveTruckRequest::RemoveTruckRequest(base_class_properties props, int cell_id, int big_trucks, int small_trucks)
    : ClientRequest(props), _cell_id(cell_id), _big_trucks(big_trucks), _small_trucks(small_trucks) {}

// Serializes the `RemoveTruckRequest` to JSON
void RemoveTruckRequest::write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::write_into_json(json, allocator);
    json.AddMember("cell_id", _cell_id, allocator);
    json.AddMember("big_trucks", _big_trucks, allocator);
    json.AddMember("small_trucks", _small_trucks, allocator);
}

// Deserializes the JSON to get the `RemoveTruckRequest` 
RemoveTruckRequest* RemoveTruckRequest::from_json(const rapidjson::Value& json) {
    if (json.HasMember("cell_id") && json.HasMember("big_trucks") && json.HasMember("small_trucks")) {
        return new RemoveTruckRequest(
            ClientRequest::extract_base_class_properties(json),
            json["cell_id"].GetInt(),
            json["big_trucks"].GetInt(),
            json["small_trucks"].GetInt()
        );
    } else {
        throw std::runtime_error("Could not parse RemoveTruckRequest from JSON. Missing fields.");
    }
}