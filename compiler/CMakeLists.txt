file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*/*.cpp" "src/*.cpp")
message(STATUS "Found sources: ${SOURCES}")

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*/*.hpp" "include/*.hpp")
message(STATUS "Found headers: ${HEADERS}")

add_library(compiler STATIC "")

target_sources(compiler
  PRIVATE ${SOURCES}
  PUBLIC ${HEADERS}
)

target_include_directories(compiler PUBLIC include)
target_link_libraries(compiler PUBLIC common frontend codegen_common codegen_llvm)
