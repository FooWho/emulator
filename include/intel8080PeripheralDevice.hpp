#pragma once
#include "types.hpp"


class Intel8080PeripheralDevice {

public:
    virtual ~Intel8080PeripheralDevice() = default;
    virtual void writeData(BYTE port, BYTE data) = 0;
    virtual BYTE readData(BYTE port) = 0;
};