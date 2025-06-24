#include <stdexcept> // For std::runtime_error
#include <iostream>

#include "request_response.h"
#include "../../serialization/json_utils.h"
#include "../../exceptions/EcologgicalException.h"

// Deserialization constructor
RequestResponse::RequestResponse(base_class_properties props, std::string req_id, bool success, rapidjson::Value* state_json, std::string err)
    : ServerResponse(props),
      _req_id(req_id),
      _success(success),
      _state_json(state_json),
      _err(err) {}

// Public constructor for creating a request response
RequestResponse::RequestResponse(std::string game_id, std::string req_id, bool success, rapidjson::Value* state_json, std::string err)
    : ServerResponse(ServerResponse::create_base_class_properties(ResponseType::req_response, game_id)),
      _req_id(req_id),
      _success(success),
      _state_json(state_json),
      _err(err) {}

// Serializes the RequestResponse into JSON format
void RequestResponse::write_into_json(rapidjson::Value& json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator) const {
    ServerResponse::write_into_json(json, allocator);

    // Add error message
    rapidjson::Value err_val(_err.c_str(), allocator);
    json.AddMember("err", err_val, allocator);

    // Add request ID
    rapidjson::Value req_id_val(_req_id.c_str(), allocator);
    json.AddMember("req_id", req_id_val, allocator);

    // Add success status
    json.AddMember("success", _success, allocator);

    // Add state JSON if available
    if (_state_json) {
        json.AddMember("state_json", *_state_json, allocator);
    }
}

// Deserializes the RequestResponse from json
RequestResponse *RequestResponse::from_json(const rapidjson::Value& json) {
   
    if (json.HasMember("err") && json.HasMember("success")) {
        std::string err = json["err"].GetString();

        rapidjson::Value* state_json = nullptr;
        if (json.HasMember("state_json")) {
            
            state_json = json_utils::clone_value(json["state_json"].GetObject());
        }

        return new RequestResponse(
                ServerResponse::extract_base_class_properties(json),
                json["req_id"].GetString(),
                json["success"].GetBool(),
                state_json,
                err);
    } else {
        throw EcologgicalException("Could not parse RequestResponse from json. err or success is missing.");
    }
}
