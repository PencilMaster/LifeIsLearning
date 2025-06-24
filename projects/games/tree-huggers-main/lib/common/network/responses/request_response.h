#ifndef ECOLOGGICAL_REQUEST_RESPONSE_H
#define ECOLOGGICAL_REQUEST_RESPONSE_H

#include <string>
#include <memory>
#include "server_response.h"
#include <rapidjson/document.h>

// Represents a RequestResponse, sent as a response to a client request
class RequestResponse : public ServerResponse {
private:
    bool _success;                               // Indicates whether the request was successful
    std::string _err;                            // Error message, if applicable
    std::string _req_id;                         // Request ID associated with this response
    rapidjson::Value* _state_json;               // json file containing the game state in json format

    // Private constructor for deserialization
    RequestResponse(base_class_properties props, std::string req_id, bool success, rapidjson::Value* state_json, std::string err);

public:
    // Constructor for creating a new RequestResponse
    RequestResponse(std::string game_id, std::string req_id, bool success, rapidjson::Value* state_json, std::string err);

    ~RequestResponse() = default;

    /** @brief serialize the response into json format for network transmission
     *  @param json the json file to write the response into
     *  @param allocator parameter needed by rapidjson
     *  @return void
    **/
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

    /** @brief deserialize the response from json format
     *  @param json the json file to read the json format message from
     *  @return pointer to the deserialized RequestResponse object
    **/
    static RequestResponse* from_json(const rapidjson::Value& json);

    // Various Getters
    [[nodiscard]] bool get_success() const { return _success; }
    [[nodiscard]] rapidjson::Value* get_state() const { return _state_json; }
    [[nodiscard]] std::string get_err() const { return _err; }
    [[nodiscard]] bool has_state() const { return _state_json!= nullptr; }

};

#endif // ECOLOGGICAL_REQUEST_RESPONSE_H