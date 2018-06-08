from ompl import base as ob
from ompl import geometric as og
import os
import datetime

today = datetime.date.today()
today_detail = datetime.datetime.today()

def isStateValid(state):
    # "state" is of type SE2StateInternal, so we don't need to use the "()"
    # operator.
    #
    # Some arbitrary condition on the state (note that thanks to
    # dynamic type checking we can just call getX() and do not need
    # to convert state to an SE2State.)
    
    if abs(state.getX())<0.5 and abs(state.getY())<0.5:
        return False;

    return True

def outputData(ssetup, pathname):
    if os.path.exists(pathname) == False:
        os.makedirs(pathname)
    filename = pathname + today.isoformat()
    filename = filename + "-" + str(today_detail.hour).zfill(2) + "-" + str(today_detail.minute).zfill(2) + "-" + str(today_detail.second).zfill(2) + ".dat"
    f = open(filename, 'a')
    f.write( ssetup.getSolutionPath().printAsMatrix())
    f.close()

def plan():
    # create an SE2 state space
    space = ob.SE2StateSpace()
    
    # set lower and upper bounds
    bounds = ob.RealVectorBounds(2)
    bounds.setLow(-1)
    bounds.setHigh(1)
    space.setBounds(bounds)
    
    # create a simple setup object
    ss = og.SimpleSetup(space)
    ss.setStateValidityChecker(ob.StateValidityCheckerFn(isStateValid))

    # set the start state
    start = ob.State(space)
    start().setXY(-0.9, -0.9)

    # set the goal state
    goal = ob.State(space)
    goal().setXY(0.9, 0.9)

    # set the start and goal to planner
    ss.setStartAndGoalStates(start, goal)
    
    # this will automatically choose a default planner with
    # default parameters
    solved = ss.solve(1.0)
    if solved:
        # try to shorten the path
        ss.simplifySolution()
        
        # print the simplified path
        pathname="result/GeomPlanningSE2/"
        outputData(ss, pathname)
        print ss.getSolutionPath().printAsMatrix()

if __name__ == "__main__":
    plan()
