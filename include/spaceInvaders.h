#pragma once
#include "types.h"
#include "intel8080.h"
#include "bus.h"
#include "ram.h"
#include "rom.h"
#include <SFML/Graphics.hpp>

class SpaceInvaders {

    private:
        Bus *bus;
        Intel8080 *cpu;
        Ram *workingRam;
        Ram *videoRam;
        Rom *programRom;
        sf::Clock clock;
        sf::Time elapsedTime;

    public:
        SpaceInvaders();
        ~SpaceInvaders();
        void Initialize();
        void Run();


};
