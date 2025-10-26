#pragma once
#include "types.h"

class Intel8080;
class VirtualMemory;


class Bus {
 private:
  Intel8080 *cpu;
  VirtualMemory *memory;
    
 public:
  Bus();
  Bus *attachCpu(Intel8080 *cpu);
  Bus *attachMemory(VirtualMemory* memory);
  BYTE read(WORD address);
};