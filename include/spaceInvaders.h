#pragma once
#include "types.h"
#include "intel8080.h"
#include "bus.h"
#include "ram.h"
#include "rom.h"
#include "invadersShiftRegister.h"
#include "intel8080Interrupt.h"
#include <SFML/Graphics.hpp>

class SpaceInvaders {

    private:
        Bus *bus;
        Intel8080 *cpu;

        Ram *workingRam;
        Ram *videoRam;
        Rom *programRom;
        invadersShiftRegister *shiftRegister;

        sf::Clock clock;
        sf::Time elapsedTime;
        sf::Time interruptTimer;

    public:
        SpaceInvaders();
        ~SpaceInvaders();
        void Initialize();
        void Run();


};
