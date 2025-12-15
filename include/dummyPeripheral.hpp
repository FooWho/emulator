#include "intel8080PeripheralDevice.hpp"
#include "types.hpp"

class DummyPeripheral : public Intel8080PeripheralDevice {
  public:
    void writeData(BYTE port, BYTE data) override;
    BYTE readData(BYTE port) override;
};