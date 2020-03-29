#include "system.h"

#include "process.h"
#include "processor.h"

using std::size_t;
using std::string;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
std::vector<Process>& System::Processes() {
  updateProcesses();
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

void System::updateProcesses() {
  processes_.clear();
  std::vector<int> pids = LinuxParser::Pids();
  for (auto& eachProcess : pids) {
    processes_.push_back(Process(eachProcess));
  }
  std::sort(processes_.begin(), processes_.end());
}