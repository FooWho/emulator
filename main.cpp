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
    Intel8080 cpu;
    Bus bus;
    Rom rom(0x2000); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM

    cpu.reset();

    cpu.attachBus(bus.attachCpu(&cpu));
    bus.attachMemory(&rom, 0x0000, 0x1FFF);
    bus.attachMemory(&ram, 0x2000, 0x3FFF); 

    FILE *file = fopen("/home/jasonelison/Workspace/invaders.bin", "rb");
    if (!file) {
        spdlog::error("Failed to open ROM file.");
        return EXIT_FAILURE;
    }
    std::vector<BYTE> romData(0x2000);
    size_t bytesRead = fread(romData.data(), 1, romData.size(), file);
    if (bytesRead != romData.size()) {
        spdlog::error("Failed to read complete ROM file.");
        return EXIT_FAILURE;
    }
    rom.romLoad(romData);
    fclose(file);

    char input;
    printf("Starting Intel 8080 Emulator. Press 'q' to quit.\n");


    int step = 1;
    while (true) {
        if (step % 5000 == 0) {
            scanf(" %c", &input);
            if (input == 'q') {
                break;
            }
        }
        cpu.step();
        cpu.printState();
        printf("Step: %d\n\n", step++);
    }

    return EXIT_SUCCESS;
}
