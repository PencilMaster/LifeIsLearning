#ifndef END_PHASE_REQUEST_H
#define END_PHASE_REQUEST_H

#include "client_request.h"
#include <string>
#include <memory>

// Represents a request to end the current phase in the game
class EndPhaseRequest : public ClientRequest {
public:
    // Constructor for creating an `EndPhaseRequest`
    EndPhaseRequest(std::string client_id, std::string game_id);

    /** @brief serialize the request into json format for network transmission
     *  @param json the json file to write the request into
     *  @param allocator parameter needed by rapidjson
     *  @return void
    **/
    void write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const override;

    /** @brief deserialize the request from json format
     *  @param json the json file to read the json format message from
     *  @return pointer to the deserialized EndPhaseRequest object
    **/
    static EndPhaseRequest* from_json(const rapidjson::Value& json);

private:
    // Private constructor used during deserialization
    EndPhaseRequest(base_class_properties props);
};

#endif // END_PHASE_REQUEST_H