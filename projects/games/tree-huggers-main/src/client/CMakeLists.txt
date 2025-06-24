cmake_minimum_required(VERSION 3.16.3)
project(Ecologgical_Client)

set(CLIENT_SOURCE_FILES
  main.cpp
  app/Ecologgical.cpp
  GameController.cpp
  network/ClientNetworkManager.cpp
  network/ResponseListenerThread.cpp
  game_window/GameWindow.cpp
  connection_panel/ConnectionPanel.cpp
  lobby_screen_panel/LobbyScreenPanel.cpp
  input_field/InputField.cpp
  truck_shop/TruckShop.cpp
  game_start_dialog/GameStartDialog.cpp
  main_game_panel/MainGamePanel.cpp
  main_game_panel/GridPanel.cpp
  main_game_panel/InfoPanel.cpp
  end_screen_panel/EndScreenPanel.cpp
  main_game_panel/truck_dialog/TruckDialog.cpp
)

add_executable(Ecologgical_Client ${CLIENT_SOURCE_FILES})

#if(PRINT_NETWORK_MESSAGES)
#    target_compile_definitions(Ecologgical_Client PRIVATE PRINT_NETWORK_MESSAGES=1)
#endif()

find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

# add_executable(Ecologgical_Client App/App.cpp)
target_compile_definitions(Ecologgical_Client PRIVATE ECOLOGGICAL_CLIENT=1 RAPIDJSON_HAS_STDSTRING=1)

target_link_libraries(Ecologgical_Client ${wxWidgets_LIBRARIES})
target_link_libraries(Ecologgical_Client common)

target_include_directories(Ecologgical_Client PRIVATE ${INCLUDE_DIRS_GENERAL})

# link sockpp
# link_directories(${ROOT}/lib/third-party/sockpp/lib)
if(WIN32)
    message("Detected Win32")

    #target_link_libraries(Ecologgical_Client PRIVATE sockpp)
    target_link_libraries(Ecologgical_Client ${CMAKE_SOURCE_DIR}/lib/third-party/sockpp/cmake-build-debug/sockpp-static.lib)
    
    # Necessary to get sockets working under Windows (with MingW)
    target_link_libraries(Ecologgical_Client wsock32 ws2_32)
else()
    message("Not Win32, so probably a Linux") # We assume it's Linux in this case

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    # target_link_libraries(Ecologgical_Client sockpp)

    target_link_libraries(Ecologgical_Client ${CMAKE_SOURCE_DIR}/lib/third-party/sockpp/build/libsockpp.so Threads::Threads)
endif()


#install graphics files

set(GRAPHICS_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/main_game_panel/graphics/wood-simple.png
)

# Ensure the destination directory exists
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/graphics)

# Add a custom command to copy the graphics files
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/graphics/wood-simple.png
  COMMAND ${CMAKE_COMMAND} -E copy ${GRAPHICS_FILES} ${CMAKE_CURRENT_BINARY_DIR}/graphics
  DEPENDS ${GRAPHICS_FILES}
  COMMENT "Copying graphics files..."
)

# Add a custom target that depends on the custom command
add_custom_target(copy_graphics_files ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/graphics/wood-simple.png)

# Ensure the install command is still present for the installation step
install(FILES ${GRAPHICS_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/graphics)