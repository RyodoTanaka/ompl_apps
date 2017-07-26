/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2008, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/* Author: Ioan Sucan */

#include "ompl/geometric/planners/ryodo_rrt/RyodoRRT.h"
#include "ompl/base/goals/GoalSampleableRegion.h"
#include "ompl/base/samplers/ObstacleEstimationSampler.h"
#include <ompl/base/spaces/RealVectorStateSpace.h>
#include "ompl/tools/config/SelfConfig.h"
#include <limits>

ompl::base::ValidStateSamplerPtr RyodoRRTallocObstacleEstimationSampler(const ompl::base::SpaceInformation *si)
{
  return ompl::base::ValidStateSamplerPtr(new ompl::base::ObstacleEstimationSampler(si));
}

ompl::geometric::RyodoRRT::RyodoRRT(const base::SpaceInformationPtr &si) : base::Planner(si, "RyodoRRT")
{
    specs_.approximateSolutions = true;
    specs_.directed = true;

    goalBias_ = 0.05;
    maxDistance_ = 0.0;
    lastGoalMotion_ = NULL;

    Planner::declareParam<double>("range", this, &RyodoRRT::setRange, &RyodoRRT::getRange, "0.:1.:10000.");
    Planner::declareParam<double>("goal_bias", this, &RyodoRRT::setGoalBias, &RyodoRRT::getGoalBias, "0.:.05:1.");
}

ompl::geometric::RyodoRRT::~RyodoRRT()
{
    freeMemory();
}

void ompl::geometric::RyodoRRT::clear()
{
    Planner::clear();
    sampler_.reset();
    freeMemory();
    if (nn_)
        nn_->clear();
    lastGoalMotion_ = NULL;
}

void ompl::geometric::RyodoRRT::setup()
{
  Planner::setup();
  tools::SelfConfig sc(si_, getName());
  sc.configurePlannerRange(maxDistance_);

  if (!nn_)
    nn_.reset(tools::SelfConfig::getDefaultNearestNeighbors<Motion*>(si_->getStateSpace()));
  nn_->setDistanceFunction(boost::bind(&RyodoRRT::distanceFunction, this, _1, _2));
}

void ompl::geometric::RyodoRRT::freeMemory()
{
    if (nn_)
    {
        std::vector<Motion*> motions;
        nn_->list(motions);
        for (unsigned int i = 0 ; i < motions.size() ; ++i)
        {
            if (motions[i]->state)
                si_->freeState(motions[i]->state);
            delete motions[i];
        }
    }
}

ompl::base::PlannerStatus ompl::geometric::RyodoRRT::solve(const base::PlannerTerminationCondition &ptc)
{
  checkValidity();
  base::Goal                 *goal   = pdef_->getGoal().get();
  base::GoalSampleableRegion *goal_s = dynamic_cast<base::GoalSampleableRegion*>(goal);

  while (const base::State *st = pis_.nextStart())
    {
      Motion *motion = new Motion(si_);
      si_->copyState(motion->state, st);
      nn_->add(motion);
    }

  if (nn_->size() == 0)
    {
      OMPL_ERROR("%s: There are no valid initial states!", getName().c_str());
      return base::PlannerStatus::INVALID_START;
    }
    
  // if (!sampler_){
  //   si_->setValidStateSamplerAllocator(boost::bind(&RyodoRRTallocObstacleEstimationSampler, _1));
  //   sampler_ = si_->allocValidStateSampler();    
  // }

  if (!sampler_)
    sampler_ = si_->allocStateSampler();

  OMPL_INFORM("%s: Starting planning with %u states already in datastructure", getName().c_str(), nn_->size());

  Motion *solution  = NULL;
  Motion *approxsol = NULL;
  double  approxdif = std::numeric_limits<double>::infinity();
  Motion *rmotion   = new Motion(si_);
  base::State *rstate = rmotion->state;
  base::State *xstate = si_->allocState();

  while (ptc == false)
    {
      /* sample random state (with goal biasing) */
      if (goal_s && rng_.uniform01() < goalBias_ && goal_s->canSample())
        goal_s->sampleGoal(rstate);
      else
        sampler_->sampleUniform(rstate);
      
      /* find closest state in the tree */
      Motion *nmotion = nn_->nearest(rmotion);
      base::State *sstate = nmotion->state;
      base::State *gstate = rstate;
      base::State *mstate = si_->allocState();;
    

      /* find state to add */
      while(1){
        double d = si_->distance(sstate, gstate);
        si_->getStateSpace()->interpolate(sstate, gstate, si_->getStateSpace()->getLongestValidSegmentLength() / d, mstate);
        if(d<=si_->getStateSpace()->getLongestValidSegmentLength()){
          if(si_->isValid(gstate))
            mstate=gstate;
          else
            mstate=sstate;
          break;
        }
        else if(si_->isValid(mstate)){
          sstate=mstate;
        } else {
          mstate = sstate;
          break;
        }
      }
      
      /* create a motion */
      Motion *motion = new Motion(si_);
      si_->copyState(motion->state, mstate);
      motion->parent = nmotion;

      nn_->add(motion);
      double dist = 0.0;
      bool sat = goal->isSatisfied(motion->state, &dist);
      if (sat)
        {
          approxdif = dist;
          solution = motion;
          break;
        }
      if (dist < approxdif)
        {
          approxdif = dist;
          approxsol = motion;
        }
    }

  bool solved = false;
  bool approximate = false;
  if (solution == NULL)
    {
      solution = approxsol;
      approximate = true;
    }

  if (solution != NULL)
    {
      lastGoalMotion_ = solution;

      /* construct the solution path */
      std::vector<Motion*> mpath;
      while (solution != NULL)
        {
          mpath.push_back(solution);
          solution = solution->parent;
        }

      /* set the solution path */
      PathGeometric *path = new PathGeometric(si_);
      for (int i = mpath.size() - 1 ; i >= 0 ; --i)
        path->append(mpath[i]->state);
      pdef_->addSolutionPath(base::PathPtr(path), approximate, approxdif, getName());
      solved = true;
    }

  si_->freeState(xstate);
  if (rmotion->state)
    si_->freeState(rmotion->state);
  delete rmotion;

  OMPL_INFORM("%s: Created %u states", getName().c_str(), nn_->size());

  return base::PlannerStatus(solved, approximate);
}

void ompl::geometric::RyodoRRT::getPlannerData(base::PlannerData &data) const
{
    Planner::getPlannerData(data);

    std::vector<Motion*> motions;
    if (nn_)
        nn_->list(motions);

    if (lastGoalMotion_)
        data.addGoalVertex(base::PlannerDataVertex(lastGoalMotion_->state));

    for (unsigned int i = 0 ; i < motions.size() ; ++i)
    {
        if (motions[i]->parent == NULL)
            data.addStartVertex(base::PlannerDataVertex(motions[i]->state));
        else
            data.addEdge(base::PlannerDataVertex(motions[i]->parent->state),
                         base::PlannerDataVertex(motions[i]->state));
    }
}
