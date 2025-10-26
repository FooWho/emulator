#pragma once
#include <vector>
#include "types.h"
#include "virtualMemory.h"

class Bus;


class Rom : public VirtualMemory
{
 private:
    std::vector<BYTE> romem;
    Bus *bus;
 
 protected:

 public:
   Rom(unsigned int size);
   Rom *attachBus(Bus *bus);
   BYTE read(WORD address) override;
   void write(WORD address, BYTE data) override;
   void test();
};