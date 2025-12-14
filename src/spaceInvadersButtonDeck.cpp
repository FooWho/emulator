#include "spaceInvadersButtonDeck.hpp"
#include "types.hpp"

SpaceInvadersButtonDeck::SpaceInvadersButtonDeck()
{
    data = 0x08;
    coinInserted = false;
    p1Pressed = false;
    p2Pressed = false;
    leftPressed = false;
    rightPressed = false;
    shotPressed = false;
}

void SpaceInvadersButtonDeck::writeData(BYTE port, BYTE data)
{
    this->data = data;
}

BYTE SpaceInvadersButtonDeck::readData(BYTE port)
{
    BYTE result = data;
    if (coinInserted) {
        coinInserted = false;
        data = data & 0xFE;
    }
    if (p1Pressed) {
        p1Pressed = false;
        data = data & 0xFB;
    }  
    if (p2Pressed) {
        p2Pressed = false;
        data = data & 0xFD;
    }
    if (leftPressed) {
        leftPressed = false;
        data = data & 0xDF;
    }  
    if (rightPressed) {
        rightPressed = false;
        data = data & 0xBF;
    }  
    if (shotPressed) {
        shotPressed = false;
        data = data & 0xEF;
    }  
    return result;
}

void SpaceInvadersButtonDeck::insertCoin()
{
    coinInserted = true;
    data = data | 0x01;
}

void SpaceInvadersButtonDeck::pressP1()
{
    p1Pressed = true;
    data = data | 0x04;
}

void SpaceInvadersButtonDeck::pressP2()
{
    p2Pressed = true;
    data = data | 0x02;
}

void SpaceInvadersButtonDeck::pressLeft()
{
    leftPressed = true;
    data = data | 0x20;
}

void SpaceInvadersButtonDeck::pressRight()
{
    rightPressed = true;
    data = data | 0x40;
}

void SpaceInvadersButtonDeck::pressShot()
{
    shotPressed = true;
    data = data | 0x10;
}