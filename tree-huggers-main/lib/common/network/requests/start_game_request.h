#ifndef START_GAME_REQUEST_H
#define START_GAME_REQUEST_H

#include "client_request.h"
#include <string>

class StartGameRequest : public ClientRequest {
public:
    // Public constructor
    StartGameRequest(std::string client_id);

    /** @brief serialize the request into json format for network transmission
     *  @param json the json file to write the request into
     *  @param allocator parameter needed by rapidjson
     *  @return void
    **/
    void write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const override;

    /** @brief deserialize the request from json format
     *  @param json the json file to read the json format message from
     *  @return pointer to the deserialized StartgameRequest object
    **/
    static StartGameRequest* from_json(const rapidjson::Value& json);

private:

    StartGameRequest(base_class_properties props);
};

#endif // START_GAME_REQUEST_H