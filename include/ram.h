#pragma once
#include <vector>
#include "types.h"
#include "virtualMemory.h"

class Bus; // Forward declaration


class Ram : public VirtualMemory
{
 private:
    std::vector<BYTE> rwmem;
    Bus *bus;
 
 protected:

 public:
   Ram(WORD size);
   Ram(std::vector<BYTE> initData);
   Ram(WORD size, std::vector<BYTE> initData);
   Ram *attachBus(Bus *bus);
   BYTE read(WORD address) const override;
   void write(WORD address, BYTE data) override;
   WORD memSize() const override;
   void ramLoad(const std::vector<BYTE>& buffer);
};