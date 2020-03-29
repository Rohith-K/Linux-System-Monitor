#include <string>
#include <sstream>
#include <iomanip>
#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) { 
    
    int hours = seconds/3600;
    seconds = seconds%3600;
    int minutes = seconds/60;
    seconds = seconds%60;
    
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << hours;

    string time{ss.str()};

    ss.str(string());
    ss << std::setw(2) << std::setfill('0') << minutes;

    time = time + ":" + ss.str();
    ss.str(string());

    ss << std::setw(2) << std::setfill('0') << seconds;
    time = time + ":" + ss.str();

    return time; 
}