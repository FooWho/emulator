#include "dummyPeripheral.hpp"
#include "types.hpp"
#include <cstdio>

void DummyPeripheral::writeData(BYTE port, BYTE data)
{
    //printf("Writing to port 0x%02X with data 0x%02X\n", port, data);
}

BYTE DummyPeripheral::readData(BYTE port)
{
    //printf("Reading from port 0x%02X\n", port);
    //if (port == 0x01) return 0x01;
    return 0x00;
}
