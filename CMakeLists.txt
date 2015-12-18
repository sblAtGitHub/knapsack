cmake_minimum_required(VERSION 2.8)

option(TARGET_MIC "compile for Intel Xeon Phi")

set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -Wall -Werror -openmp -DNDEBUG")
if (TARGET_MIC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmic")
endif()

find_package(Boost 1.56.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(knapsack knapsackLib.hpp knapsackLib.cpp main.cpp)

