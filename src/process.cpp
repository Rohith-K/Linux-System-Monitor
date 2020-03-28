#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() {
  // return 0.0;
  return getPid_();
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  // return 0.0;
  updateCpuUtil_();
  return getCpuUtil_();
}

// TODO: Return the command that generated this process
string Process::Command() {
  // return string();
  return getCommand_();
}

// TODO: Return this process's memory utilization
string Process::Ram() {
  updateRam_();
  // return string();
  return getRam_();
}

// TODO: Return the user (name) that generated this process
string Process::User() {
  // return string();
  return getUser_();
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  updateUpTime_();
  // return 0;
  return getUpTime_();
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process& a) {
  if (this->getCpuUtil_() < a.getCpuUtil_()) return true;
  return false;
}

void Process::setPid_(int pid) { Pid_ = pid; }

void Process::setUser_() { User_ = LinuxParser::User(Pid_); }

void Process::setCommand_() { Command_ = LinuxParser::Command(Pid_); }

void Process::updateCpuUtil_() {}

void Process::updateRam_() { Ram_ = LinuxParser::Ram(Pid_); }

void Process::updateUpTime_() { UpTime_ = LinuxParser::UpTime(Pid_); }

int Process::getPid_() { return Pid_; }
std::string Process::getUser_() { return User_; }
std::string Process::getCommand_() { return Command_; }

float Process::getCpuUtil_() { return CpuUtilization_; }
std::string Process::getRam_() { return Ram_; }
long int Process::getUpTime_() { return UpTime_; }

Process::Process(int num) {
  setPid_(num);
  setUser_();
  setCommand_();
  updateCpuUtil_();
  updateRam_();
  updateUpTime_();
}
