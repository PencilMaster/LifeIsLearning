cmake_minimum_required(VERSION 3.16.3)
project(Ecologgical_Client)

#set(ROOT ${CMAKE_SOURCE_DIR}/../..)
#include_directories(${ROOT}/lib/third-party/sockpp/include)

set(SERVER_SOURCE_FILES
    main.cpp
    game_instance_manager.cpp
    game_instance.cpp
    player_manager.cpp
    request_handler.cpp
    server_network_manager.cpp
    )

add_executable(Ecologgical_Server ${SERVER_SOURCE_FILES})

#PRINT_NETWORK_MESSAGES=1 for debugging
target_compile_definitions(Ecologgical_Server PRIVATE PRINT_NETWORK_MESSAGES=1 ECOLOGGICAL_SERVER=1 RAPIDJSON_HAS_STDSTRING=1)

# link common
target_link_libraries(Ecologgical_Server common)

target_include_directories(Ecologgical_Server PRIVATE ${INCLUDE_DIRS_GENERAL})

# link sockpp
# link_directories(${ROOT}/lib/third-party/sockpp/lib)
if(WIN32)
    message("Detected Win32")

    #target_link_libraries(Ecologgical_Client PRIVATE sockpp)
    target_link_libraries(Ecologgical_Server ${CMAKE_SOURCE_DIR}/lib/third-party/sockpp/cmake-build-debug/sockpp-static.lib)
    
    # Necessary to get sockets working under Windows (with MingW)
    target_link_libraries(Ecologgical_Server wsock32 ws2_32)
else()
    message("Not Win32, so probably a Linux") # We assume it's Linux in this case

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    # target_link_libraries(Ecologgical_Server sockpp)

    target_link_libraries(Ecologgical_Server ${CMAKE_SOURCE_DIR}/lib/third-party/sockpp/build/libsockpp.so Threads::Threads)
endif()

