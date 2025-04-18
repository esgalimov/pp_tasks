cmake_minimum_required(VERSION 3.22)

project(pi_counter)

set(CMAKE_CXX_COMPILER "mpic++")

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)


set(CMAKE_CXX_FLAGS_DEBUG "-O0 -DDEBUG -Wunused-result")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -Wunused-result")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(pi_counter ${CMAKE_CURRENT_SOURCE_DIR}/pi_main.cc)
add_executable(cond_counter ${CMAKE_CURRENT_SOURCE_DIR}/cond_main.cc)
add_executable(connect_time ${CMAKE_CURRENT_SOURCE_DIR}/connect_time.cc)

target_sources(cond_counter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/differentiator_lib/source/diff.cpp)
target_sources(cond_counter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/differentiator_lib/source/expr.cpp)
target_sources(cond_counter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/differentiator_lib/source/get.cpp)
target_sources(cond_counter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/differentiator_lib/source/latex.cpp)
target_sources(cond_counter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/differentiator_lib/source/print.cpp)
target_sources(cond_counter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/differentiator_lib/source/read_preorder.cpp)
target_sources(cond_counter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/differentiator_lib/source/simplify.cpp)
target_sources(cond_counter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/differentiator_lib/source/tree_debug.cpp)
target_sources(cond_counter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/differentiator_lib/source/tree.cpp)


target_include_directories(pi_counter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(cond_counter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(cond_counter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/differentiator_lib/include)
