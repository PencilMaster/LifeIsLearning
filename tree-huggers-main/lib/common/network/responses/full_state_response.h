#ifndef ECOLOGGICAL_FULL_STATE_RESPONSE_H
#define ECOLOGGICAL_FULL_STATE_RESPONSE_H

#include "server_response.h"
#include "../../game_state.h"
#include <memory> // For std::unique_ptr

// Represents a FullStateResponse, sent if anything changed in the game state
class FullStateResponse : public ServerResponse {
private:
    rapidjson::Value* _state_json; // JSON representation of the full game state

public:

    //FullStateResponse(base_class_properties props, std::unique_ptr<rapidjson::Value> state_json);
    FullStateResponse(base_class_properties props, rapidjson::Value* state_json);

    // Constructor for creating a full state response from a game state
    FullStateResponse(std::string game_id, const GameState& state);

    // Default destructor
    ~FullStateResponse() = default;

    // Accessor for the state JSON
    const rapidjson::Value* get_state_json() const;

    /** @brief serialize the response into json format for network transmission
     *  @param json the json file to write the response into
     *  @param allocator parameter needed by rapidjson
     *  @return void
    **/
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

    /** @brief deserialize the response from json format
     *  @param json the json file to read the json format message from
     *  @return pointer to the deserialized FullstateResponse object
    **/
    static FullStateResponse* from_json(const rapidjson::Value& json);

    [[nodiscard]] bool has_state() const { return _state_json!= nullptr; }
    [[nodiscard]] rapidjson::Value* get_state() const { return _state_json; }

};

#endif // ECOLOGGICAL_FULL_STATE_RESPONSE_H