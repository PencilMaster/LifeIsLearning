# 'googletest' is the subproject name
project(googletest)

include(GoogleTest)
# Add GoogleTest as a subdirectory
# 'lib' is the folder with Google Test sources
add_subdirectory(lib)

# Add test subdirectories
add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)

# Include GoogleTest headers
include_directories(${gtest_SOURCE_DIR}/include)



