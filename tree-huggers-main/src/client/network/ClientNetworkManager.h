#ifndef ECOLOGGICAL_CLIENTNETWORKMANAGER_H
#define ECOLOGGICAL_CLIENTNETWORKMANAGER_H


#include <string>
#include "ResponseListenerThread.h"
#include "../../../lib/common/network/requests/client_request.h"


class ClientNetworkManager {

public:
    /** 
		@brief initialize the client network manager
    **/
    static void init(const std::string& host, const uint16_t port);

    /** 
		@brief sends a request to the server
        @param request request to send
    **/
    static void sendRequest(const ClientRequest& request);

    /** 
		@brief parse an incoming response message
        @param message the message to parse
    **/
    static void parseResponse(const std::string& message);

private:
    /** 
		@brief tries to connect to a server
        @returns true if successful, false if not
    **/
    static bool connect(const std::string& host, const uint16_t port);


    static sockpp::tcp_connector* _connection;

    static bool _connectionSuccess;
    static bool _failedToConnect;

};


#endif //ECOLOGGICAL_CLIENTNETWORKMANAGER_H