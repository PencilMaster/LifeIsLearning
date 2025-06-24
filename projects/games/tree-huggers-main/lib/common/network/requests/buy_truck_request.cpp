#include "buy_truck_request.h"
#include "../../serialization/uuid_generator.h"

// Constructor for creating a `buyTruckRequest` with client ID and number of big and small trucks
BuyTruckRequest::BuyTruckRequest(std::string client_id, int big_trucks, int small_trucks)
    : ClientRequest(ClientRequest::create_base_class_properties(
          RequestType::buy_truck, uuid_generator::generate_uuid_v4(), client_id, "")),
      _big_trucks(big_trucks), _small_trucks(small_trucks) {}

// Deserialization constructor
BuyTruckRequest::BuyTruckRequest(ClientRequest::base_class_properties props, int big_trucks, int small_trucks)
    : ClientRequest(props), _big_trucks(big_trucks), _small_trucks(small_trucks) {}

// Serializes the `BuyTruckRequest` to JSON
void BuyTruckRequest::write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::write_into_json(json, allocator);
    json.AddMember("big_trucks", _big_trucks, allocator);
    json.AddMember("small_trucks", _small_trucks, allocator);
}

// Deserializes the JSON to get the `BuyTruckRequest` 
BuyTruckRequest* BuyTruckRequest::from_json(const rapidjson::Value& json) {
    if (json.HasMember("big_trucks") && json.HasMember("small_trucks")) {
        return new BuyTruckRequest(ClientRequest::extract_base_class_properties(json), json["big_trucks"].GetInt(), json["small_trucks"].GetInt());
    } else {
        throw std::runtime_error("Could not parse buy_truck_request from JSON. Missing fields.");
    }
}