#pragma once
#include "types.hpp"
#include "intel8080PeripheralDevice.hpp"

class SpaceInvadersButtonDeck : public Intel8080PeripheralDevice {
    private:
        BYTE data;
        BYTE resetValue;


    public:
        SpaceInvadersButtonDeck(BYTE resetValue = 0x08);
        void writeData(BYTE port, BYTE data) override;
        BYTE readData(BYTE port) override;
        void reset();
        void insertCoin();
        void pressP1();
        void pressP2();
        void pressLeft();
        void pressRight();
        void pressShot();
};