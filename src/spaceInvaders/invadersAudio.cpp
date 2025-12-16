#include "invadersAudio.hpp"

InvadersAudio::InvadersAudio() {
    buffers[0] = sf::SoundBuffer();
    buffers[0].loadFromFile("roms/spaceInvaders/audio/0.wav");
    sounds[0] = sf::Sound();
    sounds[0].setBuffer(buffers[0]);
    sounds[0].play();
    buffers[1] = sf::SoundBuffer();
    buffers[1].loadFromFile("roms/spaceInvaders/audio/1.wav");
    sounds[1] = sf::Sound();
    sounds[1].setBuffer(buffers[1]);
    sounds[1].play();
    buffers[2] = sf::SoundBuffer();
    buffers[2].loadFromFile("roms/spaceInvaders/audio/2.wav");
    sounds[2] = sf::Sound();
    sounds[2].setBuffer(buffers[2]);
    sounds[2].play();
    buffers[3] = sf::SoundBuffer();
    buffers[3].loadFromFile("roms/spaceInvaders/audio/3.wav");
    sounds[3] = sf::Sound();
    sounds[3].setBuffer(buffers[3]);
    sounds[3].play();
}

void InvadersAudio::writeData(BYTE port, BYTE data) {
    return;
}

BYTE InvadersAudio::readData(BYTE port) {
    return 0;
}