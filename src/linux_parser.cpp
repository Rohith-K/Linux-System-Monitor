#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read Operating System from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read Kernel Version from the filesystem
string LinuxParser::Kernel() {
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    string kernel, version, number;
    string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> kernel >> version >> number;
    return (kernel + " " + version + " " + number);
  }
  return string();
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  float MemTotal, MemFree;
  if (stream.is_open()) {
    for (int i = 0; i < 2; ++i) {
      string line, fieldName, value;
      std::getline(stream, line);
      std::istringstream linestream(line);
      if (i == 0) {
        linestream >> fieldName >> value;
        MemTotal = std::stol(value);
      }
      if (i == 1) {
        linestream >> fieldName >> value;
        MemFree = std::stol(value);
      }
    }
  }
  return (MemTotal - MemFree) / MemTotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long upTime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    string value;
    linestream >> value;
    upTime = std::stof(value);
  }
  return upTime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    string field, user, nice, system;
    linestream >> field >> user >> nice >> system;
    return std::stol(system);
  }
  return 0;
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    vector<string> fieldValues;
    int count = 0;
    while (linestream.good()) {
      // values greater than the 22nd field are not needed
      if (count >= 22) break;
      string value;
      linestream >> value;
      fieldValues.push_back(value);
      count++;
    }
    long utime = std::stol(fieldValues[13]);
    long stime = std::stol(fieldValues[14]);
    long cutime = std::stol(fieldValues[15]);
    long cstime = std::stol(fieldValues[16]);

    // total time spent for the process [include the time from children
    // processes]
    long total_time = utime + stime + cutime + cstime;
    return total_time;
  }
  return 0;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    string field, user, nice, system, idle, iowait, irq, softirq, steal;
    linestream >> field >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal;
    long activeJiffies = std::stol(user) + std::stol(nice) + std::stol(system) +
                         std::stol(irq) + std::stol(softirq) + std::stol(steal);
    return activeJiffies;
  }
  return 0;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    string field, user, nice, system, idle, iowait;
    linestream >> field >> user >> nice >> system >> idle >> iowait;
    long idleJiffies = std::stol(idle) + std::stol(iowait);
    return idleJiffies;
  }
  return 0;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    string line;
    string field, user, nice, system, idle, iowait, irq, softirq, steal, guest,
        guest_nice;
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> field >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
    vector<string> aggUtil{user, nice,    system, idle,  iowait,
                           irq,  softirq, steal,  guest, guest_nice};
    return aggUtil;
  }
  return {};
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    string line;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string fieldName, value;
      linestream >> fieldName >> value;
      if (fieldName == "processes"){ 
        int total_procs = std::stoi(value);
        return total_procs;
        }
    }
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    string line;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string value, fieldName;
      linestream >> fieldName >> value;
      if (fieldName == "procs_running"){ 
        int runningProcs = std::stoi(value);
        return runningProcs;
        }
    }
  }
  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    string cmdline;
    std::getline(stream, cmdline);
    return cmdline;
  }
  return string();
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (stream.good()) {
      string line;
      std::getline(stream, line);
      std::istringstream linestream(line);
      string fieldName, value;
      linestream >> fieldName >> value;
      if (fieldName == "VmSize:") {
        int ramInt = std::stoi(value);
        ramInt /= 1000;
        return std::to_string(ramInt);
      };
    }
  }
  return string();
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (stream.good()) {
      string line;
      std::getline(stream, line);
      std::istringstream linestream(line);
      string fieldName, value;
      linestream >> fieldName >> value;
      if (fieldName == "Uid:") return value;
    }
  }
  return string();
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uVal = LinuxParser::Uid(pid);
  int uid = std::stoi(uVal);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    string delimiter = ":x:" + (to_string(uid));
    while (stream.good()) {
      string line;
      std::getline(stream, line);
      std::size_t pos = line.find(delimiter);
      if (pos != string::npos) {
        string token;
        token = line.substr(0, pos);
        return token;
      }
    }
  }
  return string();
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    string line;
    vector<string> elemVec;
    std::getline(stream, line);
    std::istringstream linestream(line);
    string elem;
    while (elemVec.size() < 22) {
      linestream >> elem;
      elemVec.push_back(elem);
    }
    long upTime = (std::stol(elemVec[elemVec.size() - 1]));
    return upTime;
  }
  return 0;
}