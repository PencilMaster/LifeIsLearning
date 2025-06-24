#ifndef SERVER_CONNECT_REQUEST_H
#define SERVER_CONNECT_REQUEST_H

#include "client_request.h"
#include <string>

// Represents a request from the client to connect to the server
class ServerConnectRequest : public ClientRequest {
public:
    // Public constructor for creating a `ServerConnectRequest` with client ID and name
    ServerConnectRequest(std::string client_id, std::string name);

    /** @brief serialize the request into json format for network transmission
     *  @param json the json file to write the request into
     *  @param allocator parameter needed by rapidjson
     *  @return void
    **/
    void write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const override;

    /** @brief deserialize the request from json format
     *  @param json the json file to read the json format message from
     *  @return pointer to the deserialized ServerConnectRequest object
    **/
    static ServerConnectRequest* from_json(const rapidjson::Value& json);

    // Getter for the Client's name
    std::string get_client_name();

private:
    std::string _client_name;  // Name of the client requesting connection
    
    // Private constructor for deserialization use only
    ServerConnectRequest(ClientRequest::base_class_properties props, std::string client_name);
};

#endif // SERVER_CONNECT_REQUEST_H