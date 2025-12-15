#pragma once
#include "types.hpp"
#include "intel8080PeripheralDevice.hpp"

class SpaceInvadersButtonDeck : public Intel8080PeripheralDevice {
    private:
        BYTE data;
        bool coinInserted;
        bool p1Pressed;
        bool p2Pressed;
        bool leftPressed;
        bool rightPressed;
        bool shotPressed;
        bool firePressed;


    public:
        SpaceInvadersButtonDeck();
        void writeData(BYTE port, BYTE data) override;
        BYTE readData(BYTE port) override;
        void insertCoin();
        void pressP1();
        void pressP2();
        void pressLeft();
        void pressRight();
        void pressShot();
};