#include <ompl/geometric/SimpleSetup.h>
#include <ompl/base/spaces/SE2StateSpace.h>

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

namespace ob = ompl::base;
namespace og = ompl::geometric;

// ファイル名指定用関数
std::string setFilename(const std::string filepath)
{
  std::string command = "mkdir -p " + filepath;
  std::cout << command.c_str() << std::endl;
  const int ret = system(command.c_str());
  if (-1 == ret)
{
    printf("Error creating directory!n");
    exit(1);
}
  time_t now = time(NULL);
  struct tm *pnow = localtime(&now);
  std::ostringstream os ;
  
  os << filepath.c_str();
  os << std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_year%100 << std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_mon+1 <<  std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_mday <<  "_";
  os <<  std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_hour <<  std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_min <<  std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_sec << ".dat";
  return os.str();
}

// 状態チェック関数
// 状態が取りうるかどうかを返す
bool isStateValid(const ob::State *state)
{
  const ob::SE2StateSpace::StateType *state_2d = state->as<ob::SE2StateSpace::StateType>();
  const double &x(state_2d->getX()), &y(state_2d->getY());

  if(std::fabs(x)<0.5 && std::fabs(y)<0.5)
    return false;

  return true;
}

void planWithSimpleSetup(void)
{
  // StateSpaceの定義
  ob::StateSpacePtr space(new ob::SE2StateSpace());

  // 状態の上限・下限を設定し登録
  ob::RealVectorBounds bounds(2);
  bounds.setLow(-1);
  bounds.setHigh(1);
  space->as<ob::SE2StateSpace>()->setBounds(bounds);

  // SimpleSetupのインスタンス化
  og::SimpleSetup ss(space);
  ss.setStateValidityChecker(boost::bind(&isStateValid, _1));

  // start goalの設定
  ob::ScopedState<ob::SE2StateSpace> start(space);
  start->setXY(-0.9, -0.9);
  std::cout << "start:";
  start.print(std::cout);

  ob::ScopedState<ob::SE2StateSpace> goal(space);
  goal->setXY(0.9, 0.9);
  std::cout << "goal";
  goal.print(std::cout);
  
  // start goal の登録
  ss.setStartAndGoalStates(start, goal);

  std::cout << "------------------------------------" << std::endl;
  
  // プランナの実行.引数は待ち時間[s]
  ob::PlannerStatus solved = ss.solve(1.0);

  if(solved){
    ss.simplifySolution();
    std::cout << "------------------------------------" << std::endl;
    std::cout << " Found the Solution" << std::endl;
    ss.getSolutionPath().print(std::cout);
    std::cout <<"------------------------------------" << std::endl;
    
    std::string filename;
    filename = setFilename("../result/GeomPlanningSE2/"); 
    std::ofstream ofs(filename.c_str());
    ss.getSolutionPath().printAsMatrix(ofs);
    std::cout << "Result Path is saved in ";
    std::cout << filename.c_str() << std::endl;
  }
  else{
    std::cout << "No solution found" << std::endl;
  }
}

int main(int argv, char** argc)
{
  planWithSimpleSetup();
  std::cout << argc[0] << std::endl;
  return 0;
}
