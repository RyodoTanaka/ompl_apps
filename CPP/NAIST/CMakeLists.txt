cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

project(OMPL_NAIST_SAMPLE)

# For OMPL
find_package(OMPL REQUIRED)
include_directories(${OMPL_INCLUDE_DIRS})
link_directories(${OMPL_LIBRARY_DIRS})
add_definitions(${OMPL_DEFINITIONS})

# Common
add_subdirectory(src/IO)

add_executable(GeomPlanningSE2 src/GeomPlanningSE2.cpp)
target_link_libraries(GeomPlanningSE2 FileSave ${OMPL_LIBRARIES} m)

add_executable(CtrlPlanningSE2 src/CtrlPlanningSE2.cpp)
target_link_libraries(CtrlPlanningSE2 FileSave ${OMPL_LIBRARIES} m)
