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
    Bus *bus;
    Intel8080 *cpu;
    Ram *workingRam;
    Ram *videoRam;
    std::array<Rom*, 4> programRom;
    invadersShiftRegister *shiftRegister;
    DummyPeripheral *dummyPeripheral;
    SpaceInvadersButtonDeck *p1ButtonDeck;
    SpaceInvadersButtonDeck *p2ButtonDeck;
    InvadersAudio *audio;


    sf::Texture screen;
    sf::Sprite spriteScreen; 
    sf::Clock clock;
    sf::Time elapsedTime;
    sf::Time interruptTimer;

  public:
    SpaceInvaders();
    ~SpaceInvaders();
    void Initialize();
    void Run();
    void screenUpdate();
};
