#include "intel8080PeripheralDevice.hpp"
#include "types.hpp"

class DummyPeripheral : public intel8080PeripheralDevice {
    public:
        DummyPeripheral() = default;
        ~DummyPeripheral() = default;
        void writeData(BYTE port, BYTE data) override;
        BYTE readData(BYTE port) override;
};