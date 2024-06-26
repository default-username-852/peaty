cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(peaty LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-function -pthread -fkeep-inline-functions")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(sources
  solve_mwc.cpp
  colourer.cpp
  graph_colour_solver.cpp
  graph.cpp
  root_node_processing.cpp
  sequential_solver.cpp
  sparse_graph.cpp
  util.cpp)

add_executable(solve_vc ${sources} main.cpp)

set_target_properties(solve_vc
  PROPERTIES
    CXX_STANDARD 14
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)

add_library(peaty STATIC ${sources})

#set_target_properties(peaty PROPERTIES PUBLIC_HEADER graph.h params.h sparse_graph.h)
install(TARGETS peaty
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# MINI_VERSION_ONLY add_definitions(-DWITHOUT_COLOURING_UPPER_BOUND)

