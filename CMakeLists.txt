cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(cpptext VERSION 0.1 LANGUAGES CXX)

# require c++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# setup compiler flags
set(MY_DEBUG_OPTIONS -g -O2 -Wall -pthread)

# find threading package
find_package(Threads)

# find GSL and GSLCBLAS
find_package(GSL REQUIRED)

# find OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# find Boost requirements if needed
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#find_package(Boost 1.60 REQUIRED COMPONENTS program_options log random timer filesystem serialization)
#include_directories(${Boost_INCLUDE_DIRS})
#link_directories(${Boost_LIBRARY_DIRS})

add_subdirectory(src)
