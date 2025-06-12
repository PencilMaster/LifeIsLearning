#include "full_state_response.h"
#include "../../serialization/json_utils.h"
#include <stdexcept> // For std::runtime_error
#include "../../exceptions/EcologgicalException.h"


// Private constructor for deserialization
FullStateResponse::FullStateResponse(base_class_properties props, rapidjson::Value* state_json)
    : ServerResponse(props),
      _state_json(state_json) {}

// Public constructor for creating a full state response
FullStateResponse::FullStateResponse(std::string game_id, const GameState& state)
    : ServerResponse(ServerResponse::create_base_class_properties(ResponseType::full_state_msg, game_id)) {
    _state_json = state.to_json(); // Convert game_state to JSON
}

// Serializes the FullStateResponse into JSON format
void FullStateResponse::write_into_json(rapidjson::Value& json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator) const {
    ServerResponse::write_into_json(json, allocator);
    // Add the state JSON
    if (_state_json) {
        json.AddMember("state_json", *_state_json, allocator);
    }
}

// deserializes the JSON file to get a FullStateResponse
FullStateResponse *FullStateResponse::from_json(const rapidjson::Value& json) {
    if (json.HasMember("state_json")) {
        return new FullStateResponse(ServerResponse::extract_base_class_properties(json),
                                       json_utils::clone_value(json["state_json"].GetObject()));
    } else {
        throw EcologgicalException("Could not parse FullStateResponse from json. state is missing.");
    }
}

// Accessor for the state JSON
const rapidjson::Value* FullStateResponse::get_state_json() const {
    return _state_json;
}
