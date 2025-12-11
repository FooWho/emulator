#pragma once
#include "intel8080PeripheralDevice.hpp"
#include "types.hpp"

class invadersShiftRegister : public intel8080PeripheralDevice {
    private:
        BYTE offset;
        union {
            struct {
                BYTE y;
                BYTE x;
            };
            WORD xy;   // Accessed as a 16-bit pair
        } data;
    

    public:
        invadersShiftRegister();
        ~invadersShiftRegister() = default;
        void writeData(BYTE port, BYTE data) override;
        BYTE readData(BYTE port) override;
};
