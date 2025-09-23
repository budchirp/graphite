file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    "src/*.cpp" 
    "src/*/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS 
    "include/*.hpp" 
    "include/*/*.hpp"
)

include(FetchContent)

FetchContent_Declare(
  argman
  GIT_REPOSITORY https://github.com/budchirp/argman
  GIT_TAG        main
)
FetchContent_MakeAvailable(argman)

add_library(cli_common STATIC ${SOURCES})

target_include_directories(cli_common PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(cli_common PUBLIC 
    argman::argman
    compiler
)

target_sources(cli_common PRIVATE ${HEADERS})
