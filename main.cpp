#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <spdlog/spdlog.h>
#include "intel8080.h"
#include "rom.h"
#include "ram.h"
#include "bus.h"




int main(int argc, char *argv[])
{
    spdlog::set_level(spdlog::level::trace);
    Intel8080 cpu;
    Bus bus;
    Rom rom(0x2000); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM

    cpu.attachBus(bus.attachCpu(&cpu));
    rom.attachBus(bus.attachMemory(&rom));
    ram.attachBus(bus.attachMemory(&ram));

    const std::vector<BYTE> buffer = {0xff, 0x00, 0x34, 0x12};
    rom.romLoad(buffer);

    rom.test();
    ram.test();
    cpu.test();

    printf("All tests completed.\n");
    return 0;
}
