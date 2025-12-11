#pragma once
#include "types.h"


class intel8080PeripheralDevice {

public:
    virtual ~intel8080PeripheralDevice() = default;
    virtual void writeData(BYTE port, BYTE data) = 0;
    virtual BYTE readData(BYTE port) = 0;
};