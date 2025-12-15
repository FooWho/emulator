#pragma once
#include <vector>
#include "types.hpp"
#include "abstractMemory.hpp"

class Ram : public AbstractMemory {
 private:
    std::vector<BYTE> rwmem;
 
 public:
   Ram(WORD size);
   Ram(std::vector<BYTE> initData);
   Ram(WORD size, std::vector<BYTE> initData);
   BYTE read(WORD address) const override;
   void write(WORD address, BYTE data) override;
   WORD memSize() const override;
};