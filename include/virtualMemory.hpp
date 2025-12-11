#pragma once
#include "types.hpp"

class VirtualMemory
{
 private:

 public:
    virtual BYTE read(WORD address) const = 0;
    virtual ~VirtualMemory() = default;
    virtual void write(WORD address, BYTE data) = 0;
    virtual WORD memSize() const = 0;
};