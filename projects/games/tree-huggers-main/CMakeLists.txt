cmake_minimum_required(VERSION 3.15)
project(Ecologgical) # Project name

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories

set(INCLUDE_DIRS_GENERAL
    ${CMAKE_SOURCE_DIR}/lib/common
    ${CMAKE_SOURCE_DIR}/lib/third-party/sockpp/include
    ${CMAKE_SOURCE_DIR}/lib/third-party/rapidjson/include
)


# add Sub directories

# source code
add_subdirectory(${CMAKE_SOURCE_DIR}/src/client)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/server)

# libraries - common package
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/common)

# unit tests
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_target(tests)
    add_subdirectory(${CMAKE_SOURCE_DIR}/tests/unit/googletest EXCLUDE_FROM_ALL)
    add_dependencies(tests cell_test player_test game_state_test game_instance_test)
endif()

# Find wxWidgets library
find_package(wxWidgets COMPONENTS core base net REQUIRED)

# Copy assets to binary directory
#file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Coverage flags for debugging
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

