from ompl import base as ob
from ompl import geometric as og
from ompl import control as oc
import os
import datetime
import math

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

def FMod(x, y):
    if y==0:
        return x;
    return x-y*(x//y)

def RadToNPiPPi(x):
    return FMod(x+math.pi, math.pi*2.)-math.pi

def propagate(start, control, duration, result):
    nx = start.gatX() + control[0]*duration*math.cos(start.getYaw())
    ny = start.getY() + control[0]*duration*math.sin(start.getYaw())
    if abs(nx) > 1.0:
        nx = (nx/abs(nx))*nx
    if abs(ny) > 1.0:
        ny = (ny/abs(ny))*ny
    result().setXY(nx, ny)
    result().setYaw(RadToNPiPPi(start.getYaw()+control[1]*duration))

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

    cSpace = oc.RealVectorControlSpace(space, 2)
    cbounds = ob.RealVectorBounds(2)
    cbounds.low[0] = 0
    cbounds.high[0] = 0.5
    cbounds.low[1] = -2.0
    cbounds.high[1] = 2.0
    cSpace.setBounds(cbounds)
    
    # create a simple setup object
    ss = og.SimpleSetup(cSpace)
    ss.setStateValidityChecker(ob.StateValidityCheckerFn(isStateValid))

    # setup the statepropagator
    ss.setStatePropagator(propagate())

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
