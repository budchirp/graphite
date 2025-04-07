file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*/*.cpp" "src/*.cpp")
message(STATUS "Found sources: ${SOURCES}")

add_executable(graphitec ${SOURCES})

target_include_directories(graphitec PUBLIC include)
target_link_libraries(graphitec PUBLIC cli_common)
