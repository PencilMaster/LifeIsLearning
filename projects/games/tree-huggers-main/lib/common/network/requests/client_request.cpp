#include "client_request.h"
#include "server_connect_request.h"
#include "start_game_request.h"
#include "place_road_request.h"
#include "buy_truck_request.h"
#include "place_truck_request.h"
#include "remove_truck_request.h"
#include "end_phase_request.h"

#include <iostream>

// Mapping of string request types to the corresponding enum values, used for deserialization.
const std::unordered_map<std::string, RequestType> ClientRequest::_string_to_request_type = {
    {"server_connect", RequestType::server_connect},
    {"start_game", RequestType::start_game},
    {"place_road", RequestType::place_road},
    {"buy_truck", RequestType::buy_truck},
    {"place_truck", RequestType::place_truck},
    {"remove_truck", RequestType::remove_truck},
    {"end_phase", RequestType::end_phase}
};

// Mapping of enum request types to their string representations, used for serialization.
const std::unordered_map<RequestType, std::string> ClientRequest::_request_type_to_string = {
    {RequestType::server_connect, "server_connect"},
    {RequestType::start_game, "start_game"},
    {RequestType::place_road, "place_road"},
    {RequestType::buy_truck, "buy_truck"},
    {RequestType::place_truck, "place_truck"},
    {RequestType::remove_truck, "remove_truck"},
    {RequestType::end_phase, "end_phase"}
};

// Protected constructor used by subclasses to initialize common properties.
// `props` is a struct containing the base properties shared across all requests.
ClientRequest::ClientRequest(ClientRequest::base_class_properties props) :
    _type(props._type),
    _req_id(props._req_id),
    _client_id(props._client_id),
    _game_id(props._game_id)
{ }

// Helper function to extract base properties from a JSON object.
// This function reads `client_id`, `game_id`, `req_id`, and `type` from the JSON object
// and maps them to a `base_class_properties` struct, which is used to initialize the request.
ClientRequest::base_class_properties ClientRequest::extract_base_class_properties(const rapidjson::Value& json) {
    // Check if JSON contains the necessary fields
    if (json.HasMember("client_id") && json.HasMember("game_id") && json.HasMember("req_id")) {
        // Retrieve values from JSON and convert them to strings
        std::string client_id = json["client_id"].GetString();
        std::string game_id = json["game_id"].GetString();
        std::string req_id = json["req_id"].GetString();
        // Map the string type to `RequestType` using `_string_to_request_type`
        return create_base_class_properties(
            ClientRequest::_string_to_request_type.at(json["type"].GetString()),
            req_id,
            client_id,
            game_id
        );
    } else {
        throw std::runtime_error("Client Request did not contain client_id, game_id, or req_id");
    }
}

// Creates and returns a `base_class_properties` struct with initialized values
// This function is primarily used to initialize requests with common properties
ClientRequest::base_class_properties ClientRequest::create_base_class_properties(
    RequestType type,
    const std::string &req_id,
    const std::string &client_id,
    const std::string &game_id)
{
    // Fill in the properties struct with provided parameters
    ClientRequest::base_class_properties res;
    res._client_id = client_id;
    res._game_id = game_id;
    res._req_id = req_id;
    res._type = type;
    return res;
}

// Serializes the base request data into a JSON object for transmission over the network
void ClientRequest::write_into_json(rapidjson::Value &json,
                                     rapidjson::Document::AllocatorType &allocator) const {
    // Add the type of request as a string
    rapidjson::Value type_val(_request_type_to_string.at(this->_type).c_str(), allocator);
    json.AddMember("type", type_val, allocator);

    // Add the client ID
    rapidjson::Value client_id_val(_client_id.c_str(), allocator);
    json.AddMember("client_id", client_id_val, allocator);

    // Add the game ID
    rapidjson::Value game_id_val(_game_id.c_str(), allocator);
    json.AddMember("game_id", game_id_val, allocator);

    // Add the request ID
    rapidjson::Value req_id_val(_req_id.c_str(), allocator);
    json.AddMember("req_id", req_id_val, allocator);
}

ClientRequest* ClientRequest::from_json(const rapidjson::Value &json) {
    // Ensure JSON contains a type field
    if (json.HasMember("type") && json["type"].IsString()) {
        const std::string type_str = json["type"].GetString();
        // Convert string type to `RequestType` enum
        const RequestType request_type = ClientRequest::_string_to_request_type.at(type_str);

        // Use a switch statement to determine the type of request and return the appropriate subclass instance
        switch (request_type) {
            case RequestType::server_connect:
                return ServerConnectRequest::from_json(json);
            case RequestType::start_game:
                return StartGameRequest::from_json(json);
            case RequestType::place_road:
                return PlaceRoadRequest::from_json(json);
            case RequestType::buy_truck:
                return BuyTruckRequest::from_json(json);
            case RequestType::place_truck:
                return PlaceTruckRequest::from_json(json);
            case RequestType::remove_truck:
                return RemoveTruckRequest::from_json(json);
            case RequestType::end_phase:
                return EndPhaseRequest::from_json(json);
            default:
                throw std::runtime_error("Encountered unknown ClientRequest type: " + type_str);
        }
    }
    // If type is not found, throw an error
    throw std::runtime_error("Could not determine type of ClientRequest. JSON was:\n");
}

// Helper function for debugging - provides a string representation of the request
std::string ClientRequest::to_string() const {
    return "client_request of type " + ClientRequest::_request_type_to_string.at(_type) +
           " for client_id " + _client_id + " and game_id " + _game_id;
}