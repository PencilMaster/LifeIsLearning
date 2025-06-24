#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <memory>
#include <string>

#include "game_instance_manager.h"
#include "../../lib/common/network/requests/client_request.h"
#include "../../lib/common/network/responses/server_response.h"
#include "../../lib/common/network/responses/request_response.h"


class RequestHandler{
public:
  /** @brief handles an incoming request
   *  @param  req client request to handle
   *  @return pointer to request response to send back
   */
  static RequestResponse* handle_request(const ClientRequest& req);
};

#endif // REQUEST_HANDLER_H

