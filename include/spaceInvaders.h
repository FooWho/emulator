#pragma once
#include "types.h"
#include "intel8080.h"
#include "bus.h"
#include "ram.h"
#include "rom.h"

class SpaceInvaders {

    private:
        Bus *bus;
        Intel8080 *cpu;
        Ram *workingRam;
        Ram *videoRam;
        Rom *programRom;

    public:
        SpaceInvaders();
        ~SpaceInvaders();
        void Run();


};
