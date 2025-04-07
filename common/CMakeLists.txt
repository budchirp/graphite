file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*/*.cpp" "src/*.cpp")
message(STATUS "Found sources: ${SOURCES}")

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*/*.hpp" "include/*.hpp")
message(STATUS "Found headers: ${HEADERS}")

add_library(common STATIC "")

target_sources(common
  PRIVATE ${SOURCES}
  PUBLIC ${HEADERS}
)

target_include_directories(common PUBLIC include)
