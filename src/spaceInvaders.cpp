#include "spaceInvaders.h"
#include <fstream>
#include <cstdio>



SpaceInvaders::SpaceInvaders()
{
    programRom = new Rom(0x2000);
    workingRam = new Ram(0x400);
    videoRam = new Ram(0x1C00);
    bus = new Bus();

    std::vector<BYTE> programRomData(0x2000);
    FILE *file = fopen("/home/jelison/Workspace/invaders.bin", "rb");
    if (!file) {
        printf("Oops\n");
        return;
    }
    size_t bytesRead = fread(programRomData.data(), 1, programRomData.size(), file);
    if (bytesRead != programRomData.size()) {
        printf("Oops2\n");
        return;
    }
    fclose(file);

    programRom->romLoad(programRomData);

    bus->attachMemory(programRom, 0x0000, 0x1FFF);
    bus->attachMemory(workingRam, 0x2000, 0x23FF);
    bus->attachMemory(videoRam, 0x2400, 0x3FFF);

    cpu = new Intel8080();
    cpu->reset();
    cpu->attachBus(bus);

    
}

SpaceInvaders::~SpaceInvaders()
{
    delete programRom;
    delete workingRam;
    delete videoRam;
    delete bus;
    delete cpu;
}

void SpaceInvaders::Run()
{
    while (true) {
        cpu->step();
    }
}