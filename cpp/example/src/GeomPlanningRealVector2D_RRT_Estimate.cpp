#include <ompl/geometric/SimpleSetup.h>
#include <ompl/geometric/planners/rrt/RRTEstimate.h>
#include <ompl/base/spaces/RealVectorStateSpace.h>
#include <ompl/base/PlannerData.h>
#include <cmath>
#include <iostream>
#include <fstream>

#include "IO/FileSave.hpp"

namespace ob = ompl::base;
namespace og = ompl::geometric;

// Return true if the state is valid, false if the state is invalid
bool isStateValid(const ob::State *state)
{

  const ob::RealVectorStateSpace::StateType *state_2d = state->as<ob::RealVectorStateSpace::StateType>();
  
  const double &x(state_2d->values[0]), &y(state_2d->values[1]);
  // State is invalid when it is inside a 1x1 box
  // centered at the origin:
  if(std::fabs(x)<0.5 && std::fabs(y)<0.5)
    return false;
  // Otherwise, the state is valid:
  return true;
}

// Print a vertex to file
void printEdge(std::ostream &os, const ob::StateSpacePtr &space, const ob::PlannerDataVertex &vertex)
{
  std::vector<double> reals;
  if(vertex!=ob::PlannerData::NO_VERTEX)
  {
    space->copyToReals(reals, vertex.getState());
    for(size_t j(0); j<reals.size(); ++j)  os<<" "<<reals[j];
  }
}

void planWithSimpleSetup(void)
{
  // Construct the state space where we are planning
  ob::StateSpacePtr space(new ob::RealVectorStateSpace(2));

  ob::RealVectorBounds bounds(2);
  bounds.setLow(-1);
  bounds.setHigh(1);
  space->as<ob::RealVectorStateSpace>()->setBounds(bounds);
  
  // Instantiate SimpleSetup
  og::SimpleSetup ss(space);

  // Setup the StateValidityChecker
  ss.setStateValidityChecker(boost::bind(&isStateValid, _1));

  // Setup Start and Goal
  ob::ScopedState<ob::RealVectorStateSpace> start(space);
  start->values[0] = -0.9;
  start->values[1] = -0.9;
  std::cout << "start: "; start.print(std::cout);

  ob::ScopedState<ob::RealVectorStateSpace> goal(space);
  goal->values[0] = 0.9;
  goal->values[1] = 0.9;
  std::cout << "goal: "; goal.print(std::cout);

  ss.setStartAndGoalStates(start, goal);

  ob::PlannerPtr planner(new og::RRTEstimate(ss.getSpaceInformation()));
  ss.setPlanner(planner);

  std::cout << "----------------" << std::endl;

  // Execute the planning algorithm
  ob::PlannerStatus solved = ss.solve(1.0);

  // If we have a solution,
  if (solved)
  {
    // Print the solution path (that is not simplified yet) to a file
    std::string filename;
    filename = setFilename("GeomPlanningRealVector2D_RRT_Estimate", "path0"); 
    std::ofstream ofs0(filename.c_str());
    ss.getSolutionPath().printAsMatrix(ofs0);

    // Simplify the solution
    ss.simplifySolution();
    std::cout << "----------------" << std::endl;
    std::cout << "Found solution:" << std::endl;
    // Print the solution path to screen
    ss.getSolutionPath().print(std::cout);

    // Print the solution path to a file
    filename = setFilename("GeomPlanningRealVector2D_RRT_Estimate", "path"); 
    std::ofstream ofs(filename.c_str());
    ss.getSolutionPath().printAsMatrix(ofs);

    // Get the planner data to visualize the vertices and the edges
    ob::PlannerData pdat(ss.getSpaceInformation());
    ss.getPlannerData(pdat);

    // Print the vertices to file
    filename = setFilename("GeomPlanningRealVector2D_RRT_Estimate", "vertics"); 
    std::ofstream ofs_v(filename.c_str());
    for(unsigned int i(0); i<pdat.numVertices(); ++i)
    {
      printEdge(ofs_v, ss.getStateSpace(), pdat.getVertex(i));
      ofs_v<<std::endl;
    }

    // Print the edges to file
    filename = setFilename("GeomPlanningRealVector2D_RRT_Estimate", "edges"); 
    std::ofstream ofs_e(filename.c_str());
    std::vector<unsigned int> edge_list;
    for(unsigned int i(0); i<pdat.numVertices(); ++i)
    {
      unsigned int n_edge= pdat.getEdges(i,edge_list);
      for(unsigned int i2(0); i2<n_edge; ++i2)
      {
        printEdge(ofs_e, ss.getStateSpace(), pdat.getVertex(i));
        ofs_e<<std::endl;
        printEdge(ofs_e, ss.getStateSpace(), pdat.getVertex(edge_list[i2]));
        ofs_e<<std::endl;
        ofs_e<<std::endl<<std::endl;
      }
    }
  }
  else
    std::cout << "No solution found" << std::endl;
}

int main()
{
  planWithSimpleSetup();
  return 0;
}
