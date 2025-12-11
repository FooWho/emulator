#pragma once
#include "intel8080PeripheralDevice.h"

class invadersShiftRegister : public intel8080PeripheralDevice {
    public:
        invadersShiftRegister();
        ~invadersShiftRegister() = default;
        void writeData(BYTE port, BYTE data) override;
        BYTE readData(BYTE port) override;
};
