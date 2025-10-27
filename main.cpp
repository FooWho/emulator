#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include "intel8080.h"
#include "rom.h"
#include "ram.h"
#include "bus.h"




int main(int argc, char *argv[])
{
    Intel8080 cpu;
    Bus bus;
    Rom rom(0x2000); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM

    cpu.attachBus(bus.attachCpu(&cpu));
    rom.attachBus(bus.attachMemory(&rom));
    ram.attachBus(bus.attachMemory(&ram));

    rom.test();
    ram.test();
    cpu.test();

    printf("All tests completed.\n");
    return 0;
}
