file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*/*.cpp" "src/*.cpp")
message(STATUS "Found sources: ${SOURCES}")

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*/*.hpp" "include/*.hpp")
message(STATUS "Found headers: ${HEADERS}")

add_library(codegen_common STATIC "")

target_sources(codegen_common
  PRIVATE ${SOURCES}
  PUBLIC ${HEADERS}
)

target_include_directories(codegen_common PUBLIC include)
target_link_libraries(codegen_common PUBLIC common frontend compiler)
