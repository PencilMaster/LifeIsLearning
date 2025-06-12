#ifndef BUY_TRUCK_REQUEST_H
#define BUY_TRUCK_REQUEST_H

#include "client_request.h"
#include <string>


class BuyTruckRequest : public ClientRequest {
public:
    BuyTruckRequest(std::string client_id, int big_trucks, int small_trucks);

    /** @brief serialize the request into json format for network transmission
     *  @param json the json file to write the request into
     *  @param allocator parameter needed by rapidjson
     *  @return void
    **/
    void write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const override;

    /** @brief deserialize the request from json format
     *  @param json the json file to read the json format message from
     *  @return pointer to the deserialized BuyTruckRequest object
    **/
    static BuyTruckRequest* from_json(const rapidjson::Value& json);

    //getter function for the inputs
    int get_big_trucks() const { return this->_big_trucks; }
    int get_small_trucks() const { return this->_small_trucks; }

private:
    int _big_trucks;
    int _small_trucks;
    BuyTruckRequest(ClientRequest::base_class_properties props, int big_trucks, int small_trucks);
};

#endif // BUY_TRUCK_REQUEST_H