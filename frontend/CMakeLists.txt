file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*/*.cpp" "src/*.cpp")
message(STATUS "Found sources: ${SOURCES}")

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*/*.hpp" "include/*.hpp")
message(STATUS "Found headers: ${HEADERS}")

add_library(frontend STATIC "")

target_sources(frontend
  PRIVATE ${SOURCES}
  PUBLIC ${HEADERS}
)

target_include_directories(frontend PUBLIC include)
target_link_libraries(frontend PUBLIC common compiler)
