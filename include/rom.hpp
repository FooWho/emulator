#pragma once
#include <vector>
#include "types.hpp"
#include "virtualMemory.hpp"

class Rom : public VirtualMemory
{
 private:
    std::vector<BYTE> romem;
 
 protected:

 public:
   Rom(WORD size);
   Rom(std::vector<BYTE> initData);
   Rom(WORD size, std::vector<BYTE> initData);
   BYTE read(WORD address) const override;
   void write(WORD address, BYTE data) override;
   WORD memSize() const override;
   void romLoad(const std::vector<BYTE>& buffer);
};