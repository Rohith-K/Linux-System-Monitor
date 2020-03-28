#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();

 private:
  long PrevIdle;
  long Idle;
  long PrevNonIdle;
  long NonIdle;
  long PrevTotal;
  long Total;
};

#endif