cmake_minimum_required(VERSION 3.22)

project(pi_counter)

set(CMAKE_CXX_COMPILER "mpic++")

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)


set(CMAKE_CXX_FLAGS_DEBUG "-O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(pi_counter ${CMAKE_CURRENT_SOURCE_DIR}/pi_main.cc)
add_executable(cond_counter ${CMAKE_CURRENT_SOURCE_DIR}/cond_main.cc)

target_include_directories(pi_counter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(cond_counter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
