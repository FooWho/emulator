#pragma once
#include <vector>
#include "types.hpp"

class AbstractMemory; // Forward declaration

class Bus {
 protected:
  struct MemoryMapping {
      WORD startAddress;
      WORD endAddress;
      AbstractMemory *device;
      MemoryMapping(WORD start, WORD end, AbstractMemory* vmDevice) : startAddress(start), endAddress(end), device(vmDevice) {};
  };
  std::vector<MemoryMapping> memory_map;
    
 public:
  Bus();
  Bus *attachMemory(AbstractMemory *memory, WORD startAddress, WORD endAddress);
  virtual BYTE readByte(WORD address) const;
  WORD readWord(WORD address) const;
  virtual void writeByte(WORD address, BYTE data);
  void writeWord(WORD address, WORD data);
};