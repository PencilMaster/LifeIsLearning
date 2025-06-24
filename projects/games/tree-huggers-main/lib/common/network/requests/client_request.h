#ifndef ECOLOGGICAL_CLIENT_REQUEST_H
#define ECOLOGGICAL_CLIENT_REQUEST_H

#include <string>
#include <unordered_map>
#include <rapidjson/document.h>
#include <memory>
#include "../../serialization/serializable.h"
#include "../../serialization/uuid_generator.h"
#include "../../serialization/json_utils.h"

// Enumeration representing different request types in the Ecologgical game
// Each type corresponds to a specific action or request initiated by the client
enum RequestType {
    server_connect,  
    start_game,              
    place_road,              
    buy_truck,               
    place_truck,             
    remove_truck,            
    end_phase                
};

// Base class for all client requests, providing common properties and functions
class ClientRequest : public serializable {
protected:

    // Structure holding common properties for all request types.
    // These properties will be shared across all specific request classes.
    struct base_class_properties {
        RequestType _type;        // Type of request, as defined in the `RequestType` enum
        std::string _req_id;      
        std::string _client_id;   
        std::string _game_id;     
    };

    // Protected members holding the request data
    RequestType _type;            
    std::string _req_id;          
    std::string _client_id;       
    std::string _game_id;         

    // Protected constructor, used only by subclasses, initializes base properties.
    explicit ClientRequest(base_class_properties);

    // Static helper function to create a `base_class_properties` struct with provided values.
    static base_class_properties create_base_class_properties(RequestType type, const std::string &req_id, const std::string &client_id, const std::string &game_id);

    // Extracts common base properties from a JSON object and returns them in a struct.
    static base_class_properties extract_base_class_properties(const rapidjson::Value& json);

    [[nodiscard]] virtual std::string to_string() const;

private:

    // Static mapping of string request types to `RequestType` enums, used for deserialization.
    static const std::unordered_map<std::string, RequestType> _string_to_request_type;

    // Static mapping of `RequestType` enums to string representations, used for serialization.
    static const std::unordered_map<RequestType, std::string> _request_type_to_string;

public:
    
    // Getter functions for the fields
    [[nodiscard]] RequestType get_type() const { return this->_type; }
    [[nodiscard]] std::string get_req_id() const { return this->_req_id; }
    [[nodiscard]] std::string get_game_id() const { return this->_game_id; }
    [[nodiscard]] std::string get_client_id() const { return this->_client_id; }

    /** @brief deserialize the request from json format
     *  @param json the json file to read the json format message from
     *  @return pointer to the deserialized ClientRequest object
    **/
    static ClientRequest* from_json(const rapidjson::Value& json);

    /** @brief serialize the request into json format for network transmission
     *  @param json the json file to write the request into
     *  @param allocator parameter needed by rapidjson
     *  @return void
    **/
    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
};

#endif