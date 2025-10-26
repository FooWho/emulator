#pragma once
#include "types.h"

class VirtualMemory
{
 public:
   virtual BYTE read(WORD address) = 0;
   virtual void write(WORD address, BYTE data) = 0;
};