cmake_minimum_required(VERSION 3.16)
project(advection1d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Numerics library
add_library(advection
  src/grid.cpp
  src/problem.cpp
  src/flux.cpp
  src/time_integrator.cpp
  src/solver.cpp
  src/io.cpp
)
target_include_directories(advection PUBLIC include)
target_compile_options(advection PRIVATE -Wall -Wextra -Wpedantic)

# Executable
add_executable(advection_solver src/main.cpp)
target_link_libraries(advection_solver PRIVATE advection)
