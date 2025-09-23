file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    "src/*.cpp" 
    "src/*/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS 
    "include/*.hpp" 
    "include/*/*.hpp"
)

add_library(codegen_common STATIC ${SOURCES})

target_include_directories(codegen_common PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(codegen_common PUBLIC 
    common 
    frontend
)

target_sources(codegen_common PRIVATE ${HEADERS})
