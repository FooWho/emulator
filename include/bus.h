#pragma once
#include <array>
#include <memory>
#include "types.h"

class CPU; // Forward declaration
class VirtualMemory; // Forward declaration
const int MEMORY_MAP_SIZE = 32;


class Bus {
 private:
  struct MemoryMapping {
      WORD startAddress;
      WORD endAddress;
      VirtualMemory* device;
  };
  CPU *cpu;
  unsigned int device_count;
  std::array<MemoryMapping, MEMORY_MAP_SIZE> memory_map;
    
 public:
  Bus();
  Bus *attachCpu(CPU *cpu);
  Bus *attachMemory(VirtualMemory* memory, WORD startAddress, WORD endAddress);
  BYTE readByte(WORD address) const;
  WORD readWord(WORD address) const;
  void writeByte(WORD address, BYTE data);
  void writeWord(WORD address, WORD data);
  Bus(const Bus&) = delete;
  Bus& operator=(const Bus&) = delete;
};