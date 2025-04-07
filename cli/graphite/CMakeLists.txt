file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*/*.cpp" "src/*.cpp")
message(STATUS "Found sources: ${SOURCES}")

add_executable(graphite ${SOURCES})

target_include_directories(graphite PUBLIC include)
target_link_libraries(graphite PUBLIC cli_common)
