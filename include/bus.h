#pragma once
#include <vector>
#include <memory>
#include <optional>
#include "types.h"

class VirtualMemory; // Forward declaration

class Bus {
 private:
  struct MemoryMapping {
      WORD startAddress;
      WORD endAddress;
      VirtualMemory *device;
      MemoryMapping(WORD start, WORD end, VirtualMemory* vmDevice) : startAddress(start), endAddress(end), device(vmDevice) {};
      //MemoryMapping() = default;
      ~MemoryMapping() = default;
  };
  std::vector<MemoryMapping> memory_map;
    
 public:
  Bus();
  Bus *attachMemory(VirtualMemory *memory, WORD startAddress, WORD endAddress);
  BYTE readByte(WORD address) const;
  WORD readWord(WORD address) const;
  void writeByte(WORD address, BYTE data);
  void writeWord(WORD address, WORD data);
  Bus(const Bus&) = delete;
  Bus& operator=(const Bus&) = delete;
};