file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*/*.cpp" "src/*.cpp")
message(STATUS "Found sources: ${SOURCES}")

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*/*.hpp" "include/*.hpp")
message(STATUS "Found headers: ${HEADERS}")

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_library(codegen_llvm STATIC "")

target_sources(codegen_llvm
  PRIVATE ${SOURCES}
  PUBLIC ${HEADERS}
)

target_include_directories(codegen_llvm PUBLIC include)

target_link_libraries(codegen_llvm PUBLIC common frontend compiler codegen_common)
target_link_libraries(codegen_llvm PUBLIC LLVM)
