#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                   // TODO: See src/process.cpp
  std::string User();          // TODO: See src/process.cpp
  std::string Command();       // TODO: See src/process.cpp
  float CpuUtilization();      // TODO: See src/process.cpp
  std::string Ram();           // TODO: See src/process.cpp
  long int UpTime();           // TODO: See src/process.cpp
  bool operator<(Process& a);  // TODO: See src/process.cpp

  // mutator functions
  void setPid_(int);
  void setUser_();
  void setCommand_();

  void updateCpuUtil_();
  void updateRam_();
  void updateUpTime_();

  // accessor functions
  int getPid_();
  std::string getUser_();
  std::string getCommand_();

  float getCpuUtil_();
  std::string getRam_();
  long int getUpTime_();

  Process(int);

  // TODO: Declare any necessary private members
 private:
  int Pid_;
  std::string User_;
  std::string Command_;
  float CpuUtilization_;
  std::string Ram_;
  long int UpTime_;
};

#endif