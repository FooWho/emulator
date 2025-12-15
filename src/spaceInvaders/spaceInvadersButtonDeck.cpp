#include "spaceInvadersButtonDeck.hpp"
#include "types.hpp"

SpaceInvadersButtonDeck::SpaceInvadersButtonDeck() {
    data = 0x08;
}

void SpaceInvadersButtonDeck::writeData(BYTE port, BYTE data) {
    this->data = data;
}

BYTE SpaceInvadersButtonDeck::readData(BYTE port) {
    return data;
}

void SpaceInvadersButtonDeck::reset() {
    data = 0x08;
}

void SpaceInvadersButtonDeck::insertCoin() {
    data = data | 0x01;
}

void SpaceInvadersButtonDeck::pressP1() {
    data = data | 0x04;
}

void SpaceInvadersButtonDeck::pressP2() {
    data = data | 0x02;
}

void SpaceInvadersButtonDeck::pressLeft() {
    data = data | 0x20;
}

void SpaceInvadersButtonDeck::pressRight() {
    data = data | 0x40;
}

void SpaceInvadersButtonDeck::pressShot() {
    data = data | 0x10;
}