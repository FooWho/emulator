#include "spaceInvaders.hpp"
#include "intel8080TestHelper.hpp"
#include <fstream>
#include <cstdio>
#include <SFML/Graphics.hpp>
#include <vector>



SpaceInvaders::SpaceInvaders() {
    programRom = new Rom(0x2000);
    workingRam = new Ram(0x400);
    videoRam = new Ram(0x1C00);
    bus = new SpaceInvadersBus();
    cpu = new Intel8080();
    shiftRegister = new invadersShiftRegister();
    p1ButtonDeck = new SpaceInvadersButtonDeck();
    p2ButtonDeck = new SpaceInvadersButtonDeck();
    dummyPeripheral = new DummyPeripheral();

    bus->attachMemory(programRom, 0x0000, 0x1FFF);
    bus->attachMemory(workingRam, 0x2000, 0x23FF);
    bus->attachMemory(videoRam, 0x2400, 0x3FFF);
    cpu->attachBus(bus); 

    cpu->attachInputPeripheral(p1ButtonDeck, 0x01);
    cpu->attachInputPeripheral(p2ButtonDeck, 0x02);
    cpu->attachInputPeripheral(shiftRegister, 0x03);

    cpu->attachOutputPeripheral(shiftRegister, 0x02);
    cpu->attachOutputPeripheral(dummyPeripheral, 0x03);
    cpu->attachOutputPeripheral(shiftRegister, 0x04);
    cpu->attachOutputPeripheral(dummyPeripheral, 0x05);
    cpu->attachOutputPeripheral(dummyPeripheral, 0x06);

    screen.create(224, 256);
    spriteScreen.setTexture(screen);
    spriteScreen.setPosition(0, 0);
    
}

SpaceInvaders::~SpaceInvaders() {
    delete programRom;
    delete workingRam;
    delete videoRam;
    delete shiftRegister;
    delete p1ButtonDeck;
    delete p2ButtonDeck;
    delete dummyPeripheral;
    delete bus;
    delete cpu;
}

void SpaceInvaders::Initialize() {
    cpu->reset();
    sf::Uint8* pixels = new sf::Uint8[224 * 256 * 4];
    for (int i = 0; i < 224 * 256 * 4; i += 4) {
        pixels[i] = 0;
        pixels[i + 1] = 0;
        pixels[i + 2] = 0;
        pixels[i + 3] = 255;
    }
    screen.update(pixels);
    spriteScreen.setTexture(screen);
    
    delete[] pixels;

    std::vector<BYTE> programRomData(0x2000);
    FILE *file = fopen("/home/jelison/Workspace/invaders.bin", "rb");
    if (!file) {
        throw std::runtime_error("Failed to open ROM file");
    }
    size_t bytesRead = fread(programRomData.data(), 1, programRomData.size(), file);
    if (bytesRead != programRomData.size()) {
        throw std::runtime_error("Failed to read ROM file");
    }
    fclose(file);

    programRom->romLoad(programRomData);

    cpu->reset();
    clock.restart();
}


void SpaceInvaders::Run() {
    sf::RenderWindow window(sf::VideoMode(224, 256), "Space Invaders");

    int cycle_count = 0;
    clock.restart();
    elapsedTime = sf::Time::Zero;
    interruptTimer = sf::Time::Zero;
    bool flop = false;
    WORD pc = 0x0000;
    while (window.isOpen())
    {   
        sf::Time dt = clock.restart();
        elapsedTime += dt;
        interruptTimer += dt;

        if (interruptTimer.asMicroseconds() >= 8333) {
            interruptTimer -= sf::microseconds(8333);
            if (flop) {
                cpu->interrupt(0x02);
                screenUpdate();
            } else {
                cpu->interrupt(0x01);
            }
            flop = !flop;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            p1ButtonDeck->insertCoin();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
            p1ButtonDeck->pressP1();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
            p1ButtonDeck->pressP2();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            p1ButtonDeck->pressLeft();
            p2ButtonDeck->pressLeft();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            p1ButtonDeck->pressRight();
            p2ButtonDeck->pressRight();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            p1ButtonDeck->pressShot();
            p2ButtonDeck->pressShot();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }
  
        cycle_count = elapsedTime.asMicroseconds() * 2;
        int cycles_executed = 0;
        while (cycles_executed < cycle_count) {
             cycles_executed += cpu->step();
        }
        elapsedTime = sf::Time::Zero;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(spriteScreen);
        window.display();
    
    }
}

void SpaceInvaders::screenUpdate() {
    // 224 * 256 * 4 bytes
    std::vector<sf::Uint8> pixels(224 * 256 * 4, 0); // Initialize to Black/Transparent

    // Set Alpha to 255 (Opaque)
    for (size_t i = 3; i < pixels.size(); i += 4) {
        pixels[i] = 255;
    }

    for (int x = 0; x < 224; x++) {
        for (int y = 0; y < 256; y++) {
            // VRAM is 256x224 (rotated). Map VRAM(x,y) to Screen(x, 255-y)
            WORD vramOffset = x * 32 + (y / 8);
            BYTE byte = videoRam->read(vramOffset);
            
            if (byte & (1 << (y % 8))) {
                int screenY = 255 - y;
                int screenX = x;
                int idx = (screenY * 224 + screenX) * 4;
                pixels[idx] = 255;     // R
                pixels[idx + 1] = 255; // G
                pixels[idx + 2] = 255; // B
            }
        }
    }
    screen.update(pixels.data());
    spriteScreen.setTexture(screen);
}