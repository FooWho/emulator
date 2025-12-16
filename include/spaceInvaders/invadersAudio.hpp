#pragma once
#include <SFML/Audio.hpp>
#include <array>
#include "intel8080PeripheralDevice.hpp"
#include "types.hpp"

class InvadersAudio : public Intel8080PeripheralDevice {
  private:
    std::array<sf::SoundBuffer, 19> buffers;
    std::array<sf::Sound, 19> sounds;

  public:
    InvadersAudio();
    void writeData(BYTE port, BYTE data) override;
    BYTE readData(BYTE port) override;
};