#include "invadersShiftRegister.h"
#include "intel8080TestHelper.h"
#include "intel8080.h"
#include "types.h"
#include <cstdio>

invadersShiftRegister::invadersShiftRegister()
{
    printf("Yes\n");
}

void invadersShiftRegister::writeData(BYTE port, BYTE data)
{   

}

BYTE invadersShiftRegister::readData(BYTE port)
{
    return 0x00;
}