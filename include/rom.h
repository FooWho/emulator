#pragma once
#include <vector>
#include "types.h"
#include "virtualMemory.h"

class Bus; // Forward declaration

class Rom : public VirtualMemory
{
 private:
    std::vector<BYTE> romem;
    Bus *bus;
 
 protected:

 public:
   Rom(WORD size);
   Rom *attachBus(Bus *bus);
   BYTE read(WORD address) override;
   void write(WORD address, BYTE data) override;
   WORD memSize() override;
   void romLoad(const std::vector<BYTE>& buffer);
   void test();
};