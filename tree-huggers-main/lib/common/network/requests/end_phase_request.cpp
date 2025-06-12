#include "end_phase_request.h"
#include "../../serialization/uuid_generator.h"

// Constructor for creating an `EndPhaseRequest` with client ID and game ID
EndPhaseRequest::EndPhaseRequest(std::string client_id, std::string game_id)
    : ClientRequest(ClientRequest::create_base_class_properties(
          RequestType::end_phase, uuid_generator::generate_uuid_v4(), client_id, game_id)) {}

// Private constructor for deserialization
EndPhaseRequest::EndPhaseRequest(base_class_properties props)
    : ClientRequest(props) {}

// Serializes the `EndPhaseRequest` to JSON
void EndPhaseRequest::write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::write_into_json(json, allocator);
}

// Deserializes the JSON to get the `EndPhaseRequest` 
EndPhaseRequest* EndPhaseRequest::from_json(const rapidjson::Value& json) {
    return new EndPhaseRequest(ClientRequest::extract_base_class_properties(json));
}