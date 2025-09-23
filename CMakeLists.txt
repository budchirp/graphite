cmake_minimum_required(VERSION 3.25)

find_program(CMAKE_CXX_COMPILER NAMES g++ clang++ c++)
if(NOT CMAKE_CXX_COMPILER)
    message(FATAL_ERROR "No C++ compiler found. Please install g++ or clang++.")
endif()

project(graphite VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

option(ENABLE_LLVM "Enable LLVM codegen backend" ON)

add_compile_options(
    -Wall -Wextra -Wpedantic 
    -Wconversion -Wsign-conversion 
    -Wnull-dereference -Wnon-virtual-dtor 
    -Woverloaded-virtual -Wdouble-promotion 
    -fstack-protector-strong -fno-omit-frame-pointer
    -Wno-sign-compare -Wno-shadow
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_MODE=1)

    add_compile_options(-Og -g)
    # add_link_options(-fsanitize=address -fsanitize=undefined)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NDEBUG)

    add_compile_options(-O3)
    
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
    if(ipo_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(STATUS "IPO/LTO not supported: ${ipo_error}")
    endif()
endif()

add_subdirectory(common)
add_subdirectory(frontend)
add_subdirectory(compiler)
add_subdirectory(codegen)
add_subdirectory(cli)
