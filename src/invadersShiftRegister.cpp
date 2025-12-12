#include "invadersShiftRegister.hpp"
#include "intel8080TestHelper.hpp"
#include "intel8080.hpp"
#include "types.hpp"
#include <cstdio>
#include <stdexcept>

invadersShiftRegister::invadersShiftRegister()
{
    offset = 0x00;
    data.xy = 0x00;
}

void invadersShiftRegister::writeData(BYTE port, BYTE data)
{   
    switch (port) {
        case 0x02:
            printf("Shift offset: %02X\n", data);
            offset = data;
            break;
        case 0x04:
            //printf("Shift 0x04\n");
            this->data.y = this->data.x;
            this->data.x = data;
            printf("Shfit register in: %02X : %04X\n", data, this->data.xy);
            break;
        default:
            throw std::runtime_error("Attempt to write to invalid port " + std::to_string(port));
    }
    
}

BYTE invadersShiftRegister::readData(BYTE port)
{
    if (port == 0x03) {
        //printf("Reading Shift Register\n");
        WORD result = data.xy << offset;
        result = result >> 8;
        result = result & 0x00FF;
        printf("Shift register offset: %02X and out: %02X\n", offset, (BYTE)result);
        return (BYTE)result;
    }
    throw std::runtime_error("Attempt to read from invalid port " + std::to_string(port));
}