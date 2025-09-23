file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    "src/*.cpp" 
    "src/*/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS 
    "include/*.hpp" 
    "include/*/*.hpp"
)

add_library(common STATIC ${SOURCES})

target_include_directories(common PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_sources(common PRIVATE ${HEADERS})
