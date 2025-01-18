cmake_minimum_required(VERSION 3.25)

project(graphite VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

add_compile_options(
  -Wall -Wextra -Wpedantic -Wno-sign-compare
)

include(FetchContent)

FetchContent_Declare(
  argman
  GIT_REPOSITORY https://github.com/budchirp/argman
  GIT_TAG        main
)
FetchContent_MakeAvailable(argman)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*/*.hpp" "include/*.hpp")
message(STATUS "Found headers: ${HEADERS}")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*/*.cpp" "src/*.cpp")
message(STATUS "Found sources: ${SOURCES}")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE argman)

target_link_libraries(${PROJECT_NAME} PRIVATE LLVM)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE=1)
  target_compile_options(${PROJECT_NAME} PRIVATE -Og -g --debug -fsanitize=address)
  target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(${PROJECT_NAME} PRIVATE -O3 -flto -DNDEBUG)
  target_link_options(${PROJECT_NAME} PRIVATE -flto)
endif()
