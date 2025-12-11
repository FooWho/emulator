#pragma once
#include "types.hpp"
#include "intel8080.hpp"
#include "bus.hpp"
#include "ram.hpp"
#include "rom.hpp"
#include "invadersShiftRegister.hpp"
#include "dummyPeripheral.hpp"
#include "intel8080Interrupt.hpp"
#include <SFML/Graphics.hpp>

class SpaceInvaders {

    private:
        Bus *bus;
        Intel8080 *cpu;

        Ram *workingRam;
        Ram *videoRam;
        Rom *programRom;
        invadersShiftRegister *shiftRegister;
        DummyPeripheral *dummyPeripheral;



        sf::Clock clock;
        sf::Time elapsedTime;
        sf::Time interruptTimer;

    public:
        SpaceInvaders();
        ~SpaceInvaders();
        void Initialize();
        void Run();


};
