#include "dummyPeripheral.hpp"
#include "types.hpp"
#include <cstdio>

void DummyPeripheral::writeData(BYTE port, BYTE data) {
    return;
}

BYTE DummyPeripheral::readData(BYTE port) {
    return 0x00;
}
