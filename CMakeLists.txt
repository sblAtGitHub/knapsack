cmake_minimum_required(VERSION 2.8)

option(TARGET_MIC "compile for Intel Xeon Phi")

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror -fopenmp -qopt-report=5 -qopt-report-phase=vec")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

if (TARGET_MIC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmic")
else()
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -xhost")
endif()

find_package(Boost 1.56.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(knapsack knapsackLib.hpp knapsackLib.cpp test.hpp test.cpp main.cpp)

