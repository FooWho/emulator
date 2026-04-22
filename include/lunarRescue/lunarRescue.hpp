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
    std::unique_ptr<Bus> bus;
    std::unique_ptr<Intel8080> cpu;
    std::unique_ptr<Ram> workingRam;
    std::unique_ptr<Ram> videoRam;
    std::array<std::unique_ptr<Rom>, 6> programRom;
    std::unique_ptr<invadersShiftRegister> shiftRegister;
    std::unique_ptr<DummyPeripheral> dummyPeripheral;
    std::unique_ptr<SpaceInvadersButtonDeck> p1ButtonDeck;
    std::unique_ptr<SpaceInvadersButtonDeck> p2ButtonDeck;

    sf::Texture screen;
    sf::Sprite spriteScreen; 
    sf::Clock clock;
    sf::Time elapsedTime;
    sf::Time interruptTimer;

  public:
    LunarRescue();
    void Initialize();
    void Run();
    void screenUpdate();
};
