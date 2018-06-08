cmake_minimum_required(VERSION 2.6)
project(OMPL_EXAMPLE)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -std=c++11")

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)


# For OMPL
#set(CMAKE_PREFIX_PATH /usr/local/)
find_package(OMPL REQUIRED)
include_directories(${OMPL_INCLUDE_DIRS})
link_directories(${OMPL_LIBRARY_DIRS})
add_definitions(${OMPL_DEFINITIONS})

# For Boost
find_package(Boost COMPONENTS system REQUIRED)

# Common
add_subdirectory(src/IO)

# add_executable(CtrlPlanningSE2 src/CtrlPlanningSE2.cpp)
# target_link_libraries(CtrlPlanningSE2 FileSave ${Boost_LIBRARIES} ${OMPL_LIBRARIES} m)

# add_executable(CtrlPlanningSE2_RRT src/CtrlPlanningSE2_RRT.cpp)
# target_link_libraries(CtrlPlanningSE2_RRT FileSave ${Boost_LIBRARIES} ${OMPL_LIBRARIES} m)

# add_executable(GeomPlanningRealVector2D_RRT_Connect_Estimate src/GeomPlanningRealVector2D_RRT_Connect_Estimate.cpp)
# target_link_libraries(GeomPlanningRealVector2D_RRT_Connect_Estimate FileSave ${Boost_LIBRARIES} ${OMPL_LIBRARIES} m)

# add_executable(GeomPlanningRealVector2D_RRT src/GeomPlanningRealVector2D_RRT.cpp)
# target_link_libraries(GeomPlanningRealVector2D_RRT FileSave ${Boost_LIBRARIES} ${OMPL_LIBRARIES} m)


# add_executable(GeomPlanningRealVector2D_RRT_Estimate src/GeomPlanningRealVector2D_RRT_Estimate.cpp)
# target_link_libraries(GeomPlanningRealVector2D_RRT_Estimate FileSave ${Boost_LIBRARIES} ${OMPL_LIBRARIES} m)

# add_executable(GeomPlanningSE2 src/GeomPlanningSE2.cpp)
# target_link_libraries(GeomPlanningSE2 FileSave ${Boost_LIBRARIES} ${OMPL_LIBRARIES} m)

# add_executable(GeomPlanningSE2_Info src/GeomPlanningSE2_Info.cpp)
# target_link_libraries(GeomPlanningSE2_Info FileSave ${Boost_LIBRARIES} ${OMPL_LIBRARIES} m)

# add_executable(GeomPlanningSE2_PRM src/GeomPlanningSE2_PRM.cpp)
# target_link_libraries(GeomPlanningSE2_PRM FileSave ${Boost_LIBRARIES} ${OMPL_LIBRARIES} m)

# add_executable(GeomPlanningSE2_SBL src/GeomPlanningSE2_SBL.cpp)
# target_link_libraries(GeomPlanningSE2_SBL FileSave ${Boost_LIBRARIES} ${OMPL_LIBRARIES} m)
