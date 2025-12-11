#pragma once
#include <vector>
#include "types.hpp"
#include "virtualMemory.hpp"

class Ram : public VirtualMemory
{
 private:
    std::vector<BYTE> rwmem;
 
 protected:

 public:
   Ram(WORD size);
   Ram(std::vector<BYTE> initData);
   Ram(WORD size, std::vector<BYTE> initData);
   BYTE read(WORD address) const override;
   void write(WORD address, BYTE data) override;
   WORD memSize() const override;
};