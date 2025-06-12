# Get the relative path of the current source directory
file(RELATIVE_PATH _relpath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "adding unit tests found in ${_relpath}")

# Include directories for the client source code
include_directories (
    ${CMAKE_SOURCE_DIR}/lib/common
)

# Add the client_test executable with the necessary source files
add_executable(cell_test cell_test.cpp)

# Link Google Test libraries
#find_package(GTest REQUIRED)
target_link_libraries(cell_test GTest::gtest GTest::gtest_main common)

# Discover and register tests with Google Test
gtest_discover_tests(cell_test properties test_discovery_timeout 600)


# Add the client_test executable with the necessary source files
add_executable(player_test player_test.cpp)

# Link Google Test libraries
#find_package(GTest REQUIRED)
target_link_libraries(player_test GTest::gtest GTest::gtest_main common)

# Discover and register tests with Google Test
gtest_discover_tests(player_test properties test_discovery_timeout 600)


# Add the client_test executable with the necessary source files
add_executable(game_state_test game_state_test.cpp)

# Link Google Test libraries
#find_package(GTest REQUIRED)
target_link_libraries(game_state_test GTest::gtest GTest::gtest_main common)

# Discover and register tests with Google Test
gtest_discover_tests(game_state_test properties test_discovery_timeout 600)

