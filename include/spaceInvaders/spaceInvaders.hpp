#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "types.hpp"
#include "intel8080.hpp"
#include "spaceInvadersBus.hpp"
#include "ram.hpp"
#include "rom.hpp"
#include "invadersShiftRegister.hpp"
#include "invadersAudio.hpp"
#include "dummyPeripheral.hpp"
#include "spaceInvadersButtonDeck.hpp"

class SpaceInvaders {
  private:
    std::array<std::unique_ptr<Rom>, 4> programRom;
    std::unique_ptr<Ram> workingRam;
    std::unique_ptr<Ram> videoRam;
    std::unique_ptr<SpaceInvadersBus> bus;
    std::unique_ptr<Intel8080> cpu;
    std::unique_ptr<invadersShiftRegister> shiftRegister;
    std::unique_ptr<DummyPeripheral> dummyPeripheral;
    std::unique_ptr<SpaceInvadersButtonDeck> p1ButtonDeck;
    std::unique_ptr<SpaceInvadersButtonDeck> p2ButtonDeck;
    InvadersAudio *audio;


    sf::Texture screen;
    sf::Sprite spriteScreen; 
    sf::Clock clock;
    sf::Time elapsedTime;
    sf::Time interruptTimer;

  public:
    SpaceInvaders();
    void Initialize();
    void Run();
    void screenUpdate();
};
