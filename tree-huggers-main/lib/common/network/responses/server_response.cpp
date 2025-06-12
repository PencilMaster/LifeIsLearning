#include "server_response.h"
#include "request_response.h"
#include "full_state_response.h"
#include <stdexcept> // For std::runtime_error
#include "../../exceptions/EcologgicalException.h"

// Static mapping for deserialization: string to ResponseType
const std::unordered_map<std::string, ResponseType> ServerResponse::_string_to_response_type = {
    {"req_response", ResponseType::req_response},
    {"state_diff_msg", ResponseType::state_diff_msg},
    {"full_state_msg", ResponseType::full_state_msg}
};

// Static mapping for serialization: ResponseType to string
const std::unordered_map<ResponseType, std::string> ServerResponse::_response_type_to_string = {
    {ResponseType::req_response, "req_response"},
    {ResponseType::state_diff_msg, "state_diff_msg"},
    {ResponseType::full_state_msg, "full_state_msg"}
};

// Constructor for base class properties
ServerResponse::ServerResponse(base_class_properties props)
    : _type(props.type), _game_id(props.game_id) {}

// Helper to create base class properties
ServerResponse::base_class_properties ServerResponse::create_base_class_properties(ResponseType type, const std::string& game_id) {
    return base_class_properties{game_id, type};
}

// Helper to extract base class properties from JSON
ServerResponse::base_class_properties ServerResponse::extract_base_class_properties(const rapidjson::Value& json) {
    if (json.HasMember("type") && json.HasMember("game_id")) {
        std::string game_id = json["game_id"].GetString();
        std::string type_str = json["type"].GetString();

        // Convert string to ResponseType
        auto it = _string_to_response_type.find(type_str);
        if (it == _string_to_response_type.end()) {
            throw std::runtime_error("Unknown response type: " + type_str);
        }

        return create_base_class_properties(it->second, game_id);
    } else {
        throw std::runtime_error("Server Response JSON is missing required fields: 'type' or 'game_id'");
    }
}

// Deserializes the ServerResponse from json
ServerResponse* ServerResponse::from_json(const rapidjson::Value& json) {
    if (json.HasMember("type") && json["type"].IsString()) {
        std::string type = json["type"].GetString();
        ResponseType response_type = ServerResponse::_string_to_response_type.at(type);

        if (response_type == ResponseType::req_response) {
           
            return RequestResponse::from_json(json);
        }
        else if (response_type == ResponseType::full_state_msg) {
            
            return FullStateResponse::from_json(json);
        } else {
            
            throw EcologgicalException("Encountered unknown ServerResponse type " + response_type);
        }
    }
    
    throw EcologgicalException("Could not determine type of ClientRequest");

}

// Serializes the ServerResponse into JSON
void ServerResponse::write_into_json(rapidjson::Value& json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator) const {
    // Serialize the type field
    rapidjson::Value type_val(_response_type_to_string.at(this->_type).c_str(), allocator);
    json.AddMember("type", type_val, allocator);

    // Serialize the game ID
    rapidjson::Value game_id_val(_game_id.c_str(), allocator);
    json.AddMember("game_id", game_id_val, allocator);
}