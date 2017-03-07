cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

set(ENV{OMPL_CPP_SAMPLE_DIR} ./)

project(OMPL_NAIST_SAMPLE)

# For OMPL
find_package(OMPL REQUIRED)
include_directories(${OMPL_INCLUDE_DIRS})
link_directories(${OMPL_LIBRARY_DIRS})
add_definitions(${OMPL_DEFINITIONS})

add_executable(GeomPlanningSE2 GeomPlanningSE2.cpp)
target_link_libraries(GeomPlanningSE2 ${OMPL_LIBRARIES} m)
