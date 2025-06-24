# Get the relative path of the current source directory
file(RELATIVE_PATH _relpath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "adding unit tests found in ${_relpath}")

# Include directories for the client source code
set(SERVER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/server)
set(COMMON_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/lib/common)
include_directories (
    ${SERVER_SOURCE_DIR}
    ${COMMON_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/lib/third-party/sockpp/include
    ${CMAKE_SOURCE_DIR}/lib/third-party/rapidjson/include
)

set(SERVER_SOURCE_FILES
    #main.cpp
    ${SERVER_SOURCE_DIR}/game_instance_manager.cpp
    ${SERVER_SOURCE_DIR}/game_instance.cpp
    ${SERVER_SOURCE_DIR}/player_manager.cpp
    ${SERVER_SOURCE_DIR}/request_handler.cpp
    ${SERVER_SOURCE_DIR}/server_network_manager.cpp
    )

# Set the server source directory variable

# Add the client_test executable with the necessary source files
add_executable (game_instance_test game_instance.cpp ${SERVER_SOURCE_FILES})

# Link Google Test libraries
target_link_libraries(game_instance_test common gtest gtest_main common)

# link sockpp
# link_directories(${ROOT}/lib/third-party/sockpp/lib)
if(WIN32)
    message("Detected Win32")

    #target_link_libraries(Ecologgical_Client PRIVATE sockpp)
    target_link_libraries(game_instance_test ${CMAKE_SOURCE_DIR}/lib/third-party/sockpp/cmake-build-debug/sockpp-static.lib)
    
    # Necessary to get sockets working under Windows (with MingW)
    target_link_libraries(game_instance_test wsock32 ws2_32)
else()
    message("Not Win32, so probably a Linux") # We assume it's Linux in this case

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    # target_link_libraries(Ecologgical_Server sockpp)

    target_link_libraries(game_instance_test ${CMAKE_SOURCE_DIR}/lib/third-party/sockpp/build/libsockpp.so Threads::Threads)
endif()

# Discover and register tests with Google Test
gtest_discover_tests (game_instance_test properties test_discovery_timeout 600)


