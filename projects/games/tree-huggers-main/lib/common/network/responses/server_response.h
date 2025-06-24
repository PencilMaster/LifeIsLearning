#ifndef ECOLOGGICAL_SERVER_RESPONSE_H
#define ECOLOGGICAL_SERVER_RESPONSE_H

#include <string>
#include <unordered_map>
#include <memory> // For unique_ptr
#include "../../serialization/serializable.h"

#include <iostream>

// Enumeration representing different response types in the Ecologgical game
enum ResponseType {
    req_response,     // Response to a specific request
    state_diff_msg,   // State difference message
    full_state_msg    // Full state message
};

// Abstract base class for all responses sent from the server to the client
class ServerResponse : public serializable {
private:
    // Static mapping of string response types to `ResponseType` enums, used for deserialization
    static const std::unordered_map<std::string, ResponseType> _string_to_response_type;

    // Static mapping of `ResponseType` enums to string representations, used for serialization
    static const std::unordered_map<ResponseType, std::string> _response_type_to_string;

protected:
    std::string _game_id;  // Game ID associated with the response
    ResponseType _type;    // Type of the response, as defined in the `ResponseType` enum

    // Structure for common properties shared across all server responses
    struct base_class_properties {
        std::string game_id;  // Game ID associated with the response
        ResponseType type;    // Response type
    };

    // Protected constructor for use by derived classes
    explicit ServerResponse(base_class_properties props);

    // Helper function to create a `base_class_properties` struct
    static base_class_properties create_base_class_properties(ResponseType type, const std::string& game_id);

    // Helper function to extract common properties from a JSON object
    static base_class_properties extract_base_class_properties(const rapidjson::Value& json);

public:
    virtual ~ServerResponse() = default;

    // Accessors for response properties
    [[nodiscard]] ResponseType get_type() const { return _type; }
    [[nodiscard]] std::string get_game_id() const { return _game_id; }

    /** @brief deserialize the response from json format
     *  @param json the json file to read the json format message from
     *  @return pointer to the deserialized ServerResponse object
    **/
    static ServerResponse* from_json(const rapidjson::Value& json);

    /** @brief serialize the response into json format for network transmission
     *  @param json the json file to write the response into
     *  @param allocator parameter needed by rapidjson
     *  @return void
    **/
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override = 0;

};

#endif // ECOLOGGICAL_SERVER_RESPONSE_H