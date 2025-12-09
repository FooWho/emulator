#include "spaceInvaders.h"
#include <fstream>
#include <cstdio>
#include <SFML/Graphics.hpp>



SpaceInvaders::SpaceInvaders()
{
    programRom = new Rom(0x2000);
    workingRam = new Ram(0x400);
    videoRam = new Ram(0x1C00);
    bus = new Bus();
    cpu = new Intel8080();
    bus->attachMemory(programRom, 0x0000, 0x1FFF);
    bus->attachMemory(workingRam, 0x2000, 0x23FF);
    bus->attachMemory(videoRam, 0x2400, 0x3FFF);
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

void SpaceInvaders::Initialize()
{
    cpu->reset();

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

    cpu->reset();
    clock.restart();
}


void SpaceInvaders::Run()
{
    sf::RenderWindow window(sf::VideoMode(224, 256), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    int cycle_count = 0;
    clock.restart();
    elapsedTime = sf::Time::Zero;
    while (window.isOpen())
    {
        elapsedTime += clock.restart();
        if (elapsedTime.asMicroseconds() > 16666) {
            // Video Interrupt

        }
        
        cycle_count = elapsedTime.asMicroseconds() * 2;
        for (int i = 0; i < cycle_count; i++) {
             i += cpu->step();
        }
        elapsedTime = sf::Time::Zero;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    
    }
}