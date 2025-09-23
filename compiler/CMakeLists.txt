file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    "src/*.cpp" 
    "src/*/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS 
    "include/*.hpp" 
    "include/*/*.hpp"
)

add_library(compiler STATIC ${SOURCES})

target_include_directories(compiler PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(compiler PUBLIC 
    common 
    frontend 
    codegen_common
)

if(ENABLE_LLVM)
    target_link_libraries(compiler PUBLIC codegen_llvm)
endif()

target_sources(compiler PRIVATE ${HEADERS})
