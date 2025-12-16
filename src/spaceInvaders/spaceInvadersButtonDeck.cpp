#include "spaceInvadersButtonDeck.hpp"
#include "types.hpp"

SpaceInvadersButtonDeck::SpaceInvadersButtonDeck(BYTE resetValue) {
    this->resetValue = resetValue;
    data = this->resetValue;
}

void SpaceInvadersButtonDeck::writeData(BYTE port, BYTE data) {
    this->data = data;
}

BYTE SpaceInvadersButtonDeck::readData(BYTE port) {
    return data;
}

void SpaceInvadersButtonDeck::reset() {
    data = resetValue;
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