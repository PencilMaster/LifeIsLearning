#include "start_game_request.h"
#include "../../serialization/uuid_generator.h"

// Public constructor for creating a start_game request with client ID
StartGameRequest::StartGameRequest(std::string client_id)
    : ClientRequest(ClientRequest::create_base_class_properties(
          RequestType::start_game, uuid_generator::generate_uuid_v4(), client_id, "")) {}

// Private constructor for use in deserialization from JSON
StartGameRequest::StartGameRequest(base_class_properties props) 
    : ClientRequest(props) {}

// Serialize the request to JSON
void StartGameRequest::write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::write_into_json(json, allocator);
}

// Deserializes the JSON to get the `StartGameRequest`
StartGameRequest* StartGameRequest::from_json(const rapidjson::Value& json) {
    return new StartGameRequest(ClientRequest::extract_base_class_properties(json));
}