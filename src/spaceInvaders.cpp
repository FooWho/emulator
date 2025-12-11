#include "spaceInvaders.hpp"
#include "intel8080TestHelper.hpp"
#include "invadersShiftRegister.hpp"
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
    shiftRegister = new invadersShiftRegister();
    dummyPeripheral = new DummyPeripheral();
    bus->attachMemory(programRom, 0x0000, 0x1FFF);
    bus->attachMemory(workingRam, 0x2000, 0x23FF);
    bus->attachMemory(videoRam, 0x2400, 0x3FFF);
    cpu->attachBus(bus); 
    cpu->attachInputPeripheral(dummyPeripheral, 0x01);
    cpu->attachInputPeripheral(dummyPeripheral, 0x02);
    cpu->attachInputPeripheral(shiftRegister, 0x03);

    cpu->attachOutputPeripheral(shiftRegister, 0x02);
    cpu->attachOutputPeripheral(dummyPeripheral, 0x03);
    cpu->attachOutputPeripheral(shiftRegister, 0x04);
    cpu->attachOutputPeripheral(dummyPeripheral, 0x05);
    cpu->attachOutputPeripheral(shiftRegister, 0x06);
    
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
    interruptTimer = sf::Time::Zero;
    bool flop = false;
    WORD pc = 0x0000;
    while (window.isOpen())
    {
        //printf("Credits: %d\n", Intel8080TestHelper::getByteAtAddress(*cpu, 0x20EB));
        
        elapsedTime += clock.restart();
        interruptTimer += (elapsedTime + clock.restart());
        if (interruptTimer.asMicroseconds() > 16666) {
            // Video Interrupt
            if (flop) {
                flop = !flop;
                cpu->interrupt(0x02);
            } else {
                flop = !flop;
                cpu->interrupt(0x01);
            }
            interruptTimer = sf::Time::Zero;
        }
        
        cycle_count = elapsedTime.asMicroseconds() * 2;
        for (int i = 0; i < cycle_count; i++) {
            pc = Intel8080TestHelper::getRegisterPC(*cpu);
            switch (pc) {
            case 0x0008:
                //printf("ScanLine96\n");
                break;
            case 0x0010:
                //printf("ScanLine224\n");
                break;
            case 0x002D:
                //printf("Number of credits in bcd\n");
                break;
            case 0x0038:
                //printf("New number of credits: %d\n", Intel8080TestHelper::getByteAtAddress(*cpu, 0x20EB));
                break;
            case 0x0067:
                //printf("Mark credit as needing registering\n");
            case 0x0765:
                //printf("WaitForStart\n");
                break;
            case 0x08FF:
                //printf("DrawChar\n");
                break;
            case 0x09AD:
                //printf("Print4Digits\n");
                break;
            case 0x09B2:
                //printf("DrawHexByte\n");
                break;
            case 0x0AD7:
                //printf("WaitOnDelay\n");
                break;
            case 0x0ADA:
                //printf("WaitOnDelay (Inner)\n");
                break;
            case 0x17CD:
                //printf("CheckHandleTilt\n");
                break;
            case 0x18D4:
                //printf("init\n");
                break;
            case 0x1947:
                //printf("DrawNumCredits\n");
                break;
            default:
                //printf("default: 0x%04X\n", pc);
                break;
        }
             i += cpu->step();
        }
        interruptTimer += elapsedTime;
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