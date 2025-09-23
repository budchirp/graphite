file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    "src/*.cpp" 
    "src/*/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS 
    "include/*.hpp" 
    "include/*/*.hpp"
)

find_package(LLVM CONFIG)

if(LLVM_FOUND)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

    include_directories(${LLVM_INCLUDE_DIRS})
    add_definitions(${LLVM_DEFINITIONS})
else()
    message(WARNING "LLVM not found.")
    message(FATAL_ERROR "LLVM is required for codegen_llvm")
endif()

add_library(codegen_llvm STATIC ${SOURCES})

target_include_directories(codegen_llvm PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${LLVM_INCLUDE_DIRS}
)

target_compile_definitions(codegen_llvm PUBLIC ${LLVM_DEFINITIONS})

target_link_libraries(codegen_llvm PUBLIC 
    common 
    frontend 
    codegen_common
    LLVM
)

target_sources(codegen_llvm PRIVATE ${HEADERS})
