#include "gtest/gtest.h"
#include "intel8080.h"
#include "ram.h"
#include "rom.h"
#include "bus.h"

// Demonstrate some basic assertions.
TEST(emuTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(emuTest, CustomTest) {
    Intel8080 cpu;
    Bus bus;
    Rom rom(0x2000); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM

    const std::vector<BYTE> buffer = {0xff, 0x00, 0x34, 0x12};
    rom.romLoad(buffer);
    EXPECT_EQ(rom.read(0x0000), 0xff);

    cpu.attachBus(bus.attachCpu(&cpu));
    rom.attachBus(bus.attachMemory(&rom));
    ram.attachBus(bus.attachMemory(&ram));
    rom.test();
}
