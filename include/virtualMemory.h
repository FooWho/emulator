#pragma once
#include "types.h"

class VirtualMemory
{
 private:

 public:
    virtual BYTE read(WORD address) = 0;
    virtual ~VirtualMemory() = default;
    virtual void write(WORD address, BYTE data) = 0;
    virtual WORD memSize() = 0;
};