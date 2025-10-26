#include "rom.h"

Rom::Rom(unsigned int size)
{
    romem = std::vector<BYTE>(size, 0);
    bus = nullptr;
} 

Rom *Rom::attachBus(Bus *bus)
{
    this->bus = bus;
    return this;
}

BYTE Rom::read(WORD address)
{
    // Implementation for reading from ROM
    return romem[address];
}

void Rom::write(WORD address, BYTE data)
{
    // ROM is read-only; writing is not allowed
}

void Rom::test()
{
    romem[0] = 0x00;
    romem[1] = 0x01; // Example data
}