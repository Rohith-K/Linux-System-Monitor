#ifndef PROCESS_H
#define PROCESS_H

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

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

 private:
  int Pid_;
  std::string User_;
  std::string Command_;
  float CpuUtilization_;
  std::string Ram_;
  long int UpTime_;
};

#endif