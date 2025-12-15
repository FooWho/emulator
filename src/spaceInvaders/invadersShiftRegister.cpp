#include "invadersShiftRegister.hpp"
#include "intel8080TestHelper.hpp"
#include "intel8080.hpp"
#include "types.hpp"
#include <cstdio>
#include <stdexcept>

invadersShiftRegister::invadersShiftRegister() {
    offset = 0x00;
    data.xy = 0x00;
}

void invadersShiftRegister::writeData(BYTE port, BYTE data) {   
    switch (port) {
        case 0x02:
            offset = data;
            break;
        case 0x04:
            this->data.y = this->data.x;
            this->data.x = data;
            break;
        default:
            throw std::runtime_error("Attempt to write to invalid port " + std::to_string(port));
    }
    
}

BYTE invadersShiftRegister::readData(BYTE port) {
    if (port == 0x03) {
        WORD result = (data.xy >> (8 - offset)) & 0xFF;
        return (BYTE)result;
    }
    throw std::runtime_error("Attempt to read from invalid port " + std::to_string(port));
}