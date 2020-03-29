#include "process.h"

// Return this process's ID
int Process::Pid() { return getPid_(); }

int Process::getPid_() { return Pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return getCpuUtil_(); }

float Process::getCpuUtil_() {
  updateCpuUtil_();
  return CpuUtilization_;
}

void Process::updateCpuUtil_() {
  long total_time = LinuxParser::ActiveJiffies(Pid_);
  long uptime = LinuxParser::UpTime();
  long starttime = LinuxParser::UpTime(Pid_);
  long HERTZ = sysconf(_SC_CLK_TCK);
  float seconds = uptime - (starttime / HERTZ);
  CpuUtilization_ = ((total_time / HERTZ) / seconds);
}

// Return the command that generated this process
std::string Process::Command() { return getCommand_(); }

std::string Process::getCommand_() { return Command_; }

// Return this process's memory utilization
std::string Process::Ram() { return getRam_(); }

std::string Process::getRam_() {
  updateRam_();
  return Ram_;
}

void Process::updateRam_() { Ram_ = LinuxParser::Ram(Pid_); }

// Return the user (name) that generated this process
std::string Process::User() { return getUser_(); }

std::string Process::getUser_() { return User_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return getUpTime_(); }

long int Process::getUpTime_() {
  updateUpTime_();
  return UpTime_;
}

void Process::updateUpTime_() { UpTime_ = LinuxParser::UpTime(Pid_); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  if (this->CpuUtilization_ > a.CpuUtilization_) return true;
  return false;
}

void Process::setPid_(int pid) {
  Pid_ = int();
  Pid_ = pid;
}

void Process::setUser_() {
  User_ = std::string();
  User_ = LinuxParser::User(Pid_);
}

void Process::setCommand_() {
  Command_ = std::string();
  Command_ = LinuxParser::Command(Pid_);
}

Process::Process(int num) {
  setPid_(num);
  setUser_();
  setCommand_();
  updateCpuUtil_();
  updateRam_();
  updateUpTime_();
}
