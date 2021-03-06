#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "FileSave.hpp"

// ファイル名指定用関数
std::string setFilename(const std::string dir_name)
{
  // Set Time info
  time_t now = time(NULL);
  struct tm *pnow = localtime(&now);
  std::ostringstream time_s;
  // add Year-Month-Day
  time_s << pnow->tm_year+1900 << "-" << std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_mon+1 << "-" << std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_mday << "-";
  // add Hour-Minuits-Seconds
  time_s << std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_hour << "-" << std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_min << "-" << std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_sec;

  // File path
  std::string file_path = std::getenv("OMPL_APPS_DIR");
  file_path = file_path + "/result/" +  dir_name + "/" + time_s.str() + "/";
  // Make Directory
  std::string command = "mkdir -p " + file_path;             
  int ret = system(command.c_str());
  if (ret<0){
    std::cout << "!!! Error creating directory !!!" << std::endl;
    exit(1);
  }

  std::ostringstream os ; 
  os << file_path.c_str();
  os << "result.dat";
  
  return os.str();
}


std::string setFilename(const std::string dir_name, const std::string file_name)
{
  // Set Time info
  time_t now = time(NULL);
  struct tm *pnow = localtime(&now);
  std::ostringstream time_s;
  // add Year-Month-Day
  time_s << pnow->tm_year+1900 << "-" << std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_mon+1 << "-" << std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_mday << "-";
  // add Hour-Minuits-Seconds
  time_s << std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_hour << "-" << std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_min << "-" << std::setw( 2 ) << std::setfill( '0' ) << pnow->tm_sec;

  // File path
  std::string file_path = std::getenv("OMPL_APPS_DIR");
  file_path = file_path + "/result/" +  dir_name + "/" + time_s.str() + "/";
  // Make Directory
  std::string command = "mkdir -p " + file_path;             
  int ret = system(command.c_str());
  if (ret<0){
    std::cout << "!!! Error creating directory !!!" << std::endl;
    exit(1);
  }
  
  std::ostringstream os ; 
  os << file_path.c_str();
  os << file_name.c_str() << ".dat";
  return os.str();
}
