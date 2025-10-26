#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include "intel8080.h"
#include "rom.h"
#include "bus.h"




int main(int argc, char *argv[])
{
    Intel8080 cpu;
    Bus bus;
    Rom rom(0x2000); // Example ROM size of 8KB

    cpu.attachBus(bus.attachCpu(&cpu));
    rom.attachBus(bus.attachMemory(&rom));

    rom.test();
    cpu.test();

    return 0;
}
