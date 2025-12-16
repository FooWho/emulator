#pragma once
#include "types.hpp"
#include "intel8080.hpp"
#include "bus.hpp"
#include "ram.hpp"
#include "rom.hpp"
#include "invadersShiftRegister.hpp"
#include "dummyPeripheral.hpp"
#include "spaceInvadersButtonDeck.hpp"
#include <SFML/Graphics.hpp>
#include <array>

class LunarRescue {
  private:
    Bus *bus;
    Intel8080 *cpu;
    Ram *workingRam;
    Ram *videoRam;
    std::array<Rom*, 6> programRom;
    invadersShiftRegister *shiftRegister;
    DummyPeripheral *dummyPeripheral;
    SpaceInvadersButtonDeck *p1ButtonDeck;
    SpaceInvadersButtonDeck *p2ButtonDeck;

    sf::Texture screen;
    sf::Sprite spriteScreen; 
    sf::Clock clock;
    sf::Time elapsedTime;
    sf::Time interruptTimer;

  public:
    LunarRescue();
    ~LunarRescue();
    void Initialize();
    void Run();
    void screenUpdate();
};
