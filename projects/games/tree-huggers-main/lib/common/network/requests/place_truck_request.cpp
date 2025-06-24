#include "place_truck_request.h"
#include "../../serialization/uuid_generator.h"

// Constructor for creating a `PlaceTruckRequest` with client ID, game ID, cell ID and truck type
PlaceTruckRequest::PlaceTruckRequest(std::string client_id, std::string game_id, int cell_id, TruckType truck_type)
    : ClientRequest(ClientRequest::create_base_class_properties(
          RequestType::place_truck, uuid_generator::generate_uuid_v4(), client_id, game_id)),
      _cell_id(cell_id), _truck_type(truck_type) {}

// Private constructor for deserialization
PlaceTruckRequest::PlaceTruckRequest(base_class_properties props, int cell_id, TruckType truck_type)
    : ClientRequest(props), _cell_id(cell_id), _truck_type(truck_type) {}

// Serializes the `PlaceTruckRequest` to JSON
void PlaceTruckRequest::write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::write_into_json(json, allocator);
    json.AddMember("cell_id", _cell_id, allocator);
    json.AddMember("truck_type", static_cast<int>(_truck_type), allocator);
}

// Deserializes the JSON to get the `PlaceTruckRequest` 
PlaceTruckRequest* PlaceTruckRequest::from_json(const rapidjson::Value& json) {
    if (json.HasMember("cell_id") && json.HasMember("truck_type")) {
        return new PlaceTruckRequest(
            ClientRequest::extract_base_class_properties(json),
            json["cell_id"].GetInt(),
            static_cast<TruckType>(json["truck_type"].GetInt())
        );
    } else {
        throw std::runtime_error("Could not parse PlaceTruckRequest from JSON. Missing fields.");
    }
}