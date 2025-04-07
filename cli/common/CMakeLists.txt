file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*/*.cpp" "src/*.cpp")
message(STATUS "Found sources: ${SOURCES}")

include(FetchContent)
FetchContent_Declare(
  argman
  GIT_REPOSITORY https://github.com/budchirp/argman
  GIT_TAG        main
)
FetchContent_MakeAvailable(argman)

add_library(cli_common STATIC ${SOURCES})

target_include_directories(cli_common PUBLIC include)

target_link_libraries(cli_common PUBLIC argman)
target_link_libraries(cli_common PUBLIC compiler)
