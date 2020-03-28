#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 

    std::vector<std::string> parsed_data = LinuxParser::CpuUtilization();
    std::vector<long> curr_data{};
    for(auto& strElem: parsed_data){
        curr_data.push_back(std::stol(strElem));
    }

    PrevIdle = Idle;
    // Idle = idle + iowait
    Idle = curr_data[3] + curr_data[4];

    PrevNonIdle = NonIdle;
    // NonIdle = user + nice + system + irq + softirq + steal
    NonIdle = curr_data[0] + curr_data[1] + curr_data[2] + \
                curr_data[5] + curr_data[6] + curr_data[7];
    
    PrevTotal = PrevIdle + PrevNonIdle;
    Total = Idle + NonIdle;

    // differentiate
    long del_total = Total - PrevTotal;
    long del_idled = Idle - PrevIdle;

    float cpu_util = (del_total-del_idled)*1.0/del_total; 
    return cpu_util;
}