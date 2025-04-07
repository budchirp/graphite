cmake_minimum_required(VERSION 3.25)

project(graphite VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

add_compile_options(
  -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wnull-dereference -Wnon-virtual-dtor -Woverloaded-virtual -Wdouble-promotion -fstack-protector-strong -fno-omit-frame-pointer -pie -fPIE -Wno-sign-compare -Wno-shadow
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG_MODE=1)
  add_compile_options(-Og -g --debug -fsanitize=address)
  add_link_options(-fsanitize=address)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_options(-O3 -flto -DNDEBUG)
  add_link_options(-flto)
endif()

add_subdirectory(common)
add_subdirectory(frontend)
add_subdirectory(compiler)
add_subdirectory(codegen)
add_subdirectory(cli)
