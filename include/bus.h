#pragma once
#include <array>
#include "types.h"

class Intel8080; // Forward declaration
class VirtualMemory; // Forward declaration
const int MEMORY_MAP_SIZE = 32;


class Bus {
 private:
  Intel8080 *cpu;
  unsigned int device_count;
  std::array<VirtualMemory*, MEMORY_MAP_SIZE> *memory_map;
    
 public:
  Bus();
  Bus *attachCpu(Intel8080 *cpu);
  Bus *attachMemory(VirtualMemory* memory);
  BYTE read(WORD address);
};