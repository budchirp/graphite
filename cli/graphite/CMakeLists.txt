file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    "src/*.cpp" 
    "src/*/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS 
    "include/*.hpp" 
    "include/*/*.hpp"
)

add_executable(graphite ${SOURCES})

target_include_directories(graphite PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(graphite PRIVATE cli_common)

target_sources(graphite PRIVATE ${HEADERS})
