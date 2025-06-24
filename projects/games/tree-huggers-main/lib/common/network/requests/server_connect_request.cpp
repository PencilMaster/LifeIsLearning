#include "server_connect_request.h"
#include "../../serialization/uuid_generator.h"
#include <stdexcept> // Ensure exceptions are resolved

// Constructor for creating a `ServerConnectRequest` with client ID and name
ServerConnectRequest::ServerConnectRequest(std::string client_id, std::string name)
    : ClientRequest(ClientRequest::create_base_class_properties(
          RequestType::server_connect, uuid_generator::generate_uuid_v4(), client_id, "")),
      _client_name(name) {}

// Private constructor for use in deserialization from JSON
ServerConnectRequest::ServerConnectRequest(ClientRequest::base_class_properties props, std::string client_name)
    : ClientRequest(props), _client_name(client_name) {}

// Serializes the request data into JSON format for network transmission
void ServerConnectRequest::write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::write_into_json(json, allocator);
    rapidjson::Value name_val(_client_name.c_str(), allocator);
    json.AddMember("name", name_val, allocator);
}

// Deserializes the JSON to get the `ServerConnectRequest` 
ServerConnectRequest* ServerConnectRequest::from_json(const rapidjson::Value& json) {
    if (json.HasMember("name") && json["name"].IsString()) {
        // Extracts base properties and creates a new instance with the client name
        return new ServerConnectRequest(ServerConnectRequest(ClientRequest::extract_base_class_properties(json), json["name"].GetString()));
    } else {
        throw std::runtime_error("Could not parse ServerConnectRequest from JSON. 'name' field is missing.");
    }
}

//getter function
std::string ServerConnectRequest::get_client_name(){
    return _client_name;
}