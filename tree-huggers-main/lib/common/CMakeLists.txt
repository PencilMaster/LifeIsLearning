
# Create a library from the source files
# add_library(common cell.cpp player.cpp game_state.cpp)

include_directories(
${CMAKE_SOURCE_DIR}/lib/third-party/rapidjson/include
${CMAKE_SOURCE_DIR}/lib/third-party/sockpp/include
)

set(COMMON_SOURCE_FILES
cell.cpp 
player.cpp 
game_state.cpp 
network/requests/buy_truck_request.cpp
network/requests/client_request.cpp
network/requests/end_phase_request.cpp
network/requests/place_road_request.cpp
network/requests/place_truck_request.cpp
network/requests/remove_truck_request.cpp
network/requests/server_connect_request.cpp
network/requests/start_game_request.cpp
network/responses/full_state_response.cpp
network/responses/request_response.cpp
network/responses/server_response.cpp
serialization/unique_serializable.cpp
serialization/json_utils.h
serialization/serializable_value.h
serialization/serializable.h
serialization/uuid_generator.h
serialization/value_type_helpers.h
serialization/vector_utils.h
exceptions/EcologgicalException.h
)

#add_library(common_client ${COMMON_SOURCE_FILES})
#target_compile_definitions(common_client PRIVATE ECOLOGGICAL_CLIENT=1)

#add_library(common_server ${COMMON_SOURCE_FILES})

add_library(common ${COMMON_SOURCE_FILES})
