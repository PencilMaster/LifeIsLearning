# # Get the relative path of the current source directory
# file(RELATIVE_PATH _relpath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
# message(STATUS "adding unit tests found in ${_relpath}")

# # Set the client source directory variable
# set(CLIENT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/client)
# set(COMMON_SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/common)

# # Include directories for the source code
# include_directories (
#     ${CLIENT_SOURCE_DIR}
#     ${COMMON_SOURCE_DIR}
# )

# # Add the client_test executable with the necessary source files
# add_executable (gui_test 
#     gui_test.cpp 
#     ${COMMON_SOURCE_DIR}/game_state.cpp
#     ${CLIENT_SOURCE_DIR}/game_window/GameWindow.cpp
#     ${CLIENT_SOURCE_DIR}/connection_panel/ConnectionPanel.cpp
#     ${CLIENT_SOURCE_DIR}/main_game_panel/MainGamePanel.cpp
#     ${CLIENT_SOURCE_DIR}/main_game_panel/GridPanel.cpp
#     ${CLIENT_SOURCE_DIR}/main_game_panel/InfoPanel.cpp
#     ${CLIENT_SOURCE_DIR}/end_screen_panel/EndScreenPanel.cpp
#     ${CLIENT_SOURCE_DIR}/input_field/InputField.cpp
#     ${CLIENT_SOURCE_DIR}/truck_shop/TruckShop.cpp
#     ${CLIENT_SOURCE_DIR}/game_start_dialog/GameStartDialog.cpp
# )

# find_package(wxWidgets COMPONENTS core base net REQUIRED)
# include(${wxWidgets_USE_FILE})

# # Link wxWidgets libraries
# target_link_libraries(gui_test ${wxWidgets_LIBRARIES})
# # Link Google Test libraries
# target_link_libraries(gui_test gtest gtest_main)
# # link common library
# target_link_libraries(gui_test common)

# # Discover and register tests with Google Test
# gtest_discover_tests (gui_test properties test_discovery_timeout 600)

