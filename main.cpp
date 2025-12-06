#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <spdlog/spdlog.h>
#include "intel8080.h"
#include "rom.h"
#include "ram.h"
#include "bus.h"
#include "intel8080TestHelper.h"




int main(int argc, char *argv[])
{
    
    Intel8080 cpu;
    Bus bus;
    //Rom rom(0x2000); // 8KB ROM
    Ram ram(0xFFFF); // 8KB RAM

    cpu.reset();

    cpu.attachBus(bus.attachCpu(&cpu));
    //bus.attachMemory(&rom, 0x0000, 0x1FFF);
    bus.attachMemory(&ram, 0x0000, 0xFFFE); 

    std::ifstream file("/home/jelison/Workspace/cpudiag.bin", std::ios::binary | std::ios::ate);

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        // CRITICAL: Load at 0x0100 (CP/M TPA start address)
        for (int i = 0; i < size; i++) {
            bus.writeByte(0x0100 + i, (uint8_t)buffer[i]);
        }
    }
    
    // Inject "OS" shim at 0x0005
    // When the CPU calls 0x0005, we want it to return immediately 
    // so we can intercept it in the emulator loop.
    // 0xC9 is the opcode for RET
    bus.writeByte(0x0005, 0xC9);

    Intel8080TestHelper::setRegisterPC(cpu, 0x0100);
    Intel8080TestHelper::setRegisterSP(cpu, 0xF000);

    bool finished = false;
    while (!finished) {
        if (Intel8080TestHelper::getRegisterPC(cpu) == 0x0689) {
            int x = 5;
        }
        cpu.step();
        //std::cout << "Did " << (int)(Intel8080TestHelper::getOpcode(cpu)) << "" << std::endl;
        // ---------------------------------------------------------
        // CP/M BDOS Shim (The "Magic" Part)
        // ---------------------------------------------------------
        // CPUDiag prints by calling address 0x0005.
        // When we see PC == 0x0005, we look at Register C to see what to do.
        if (Intel8080TestHelper::getRegisterPC(cpu) == 0x0005) {
            
            // C = 9: Print String (DE points to string, ends with '$')
            if (Intel8080TestHelper::getRegisterC(cpu) == 9) {
                WORD addr = Intel8080TestHelper::getRegisterDE(cpu);
                char c = bus.readByte(addr);
                while (c != '$') {
                    std::cout << c;
                    addr++;
                    c = bus.readByte(addr);
                }
            }
            
            // C = 2: Print Character (E contains the char)
            else if (Intel8080TestHelper::getRegisterC(cpu) == 2) {
                std::cout << (char)Intel8080TestHelper::getRegisterE(cpu);
            }
            
            // Note: Since we put a RET (0xC9) at 0x0005 in RAM, 
            // the CPU will automatically execute that RET and go back 
            // to the main program. We just "peeked" to see what to print.
        }
        
        // Check for "JP 0000" (Warm Boot) - This indicates the test is done
        if (Intel8080TestHelper::getRegisterPC(cpu) == 0x0000) {
            std::cout << "\n\n*** Terminated (Jumped to 0x0000) ***" << std::endl;
            finished = true;
        }
    }

    return 0;

    

    /*

    FILE *file = fopen("/home/jelison/Workspace/invaders.bin", "rb");
    if (!file) {
        spdlog::error("Failed to open ROM file.");
        return EXIT_FAILURE;
    }
    std::vector<BYTE> romData(0x2000);
    size_t bytesRead = fread(romData.data(), 1, romData.size(), file);
    if (bytesRead != romData.size()) {
        spdlog::error("Failed to read complete ROM file.");
        return EXIT_FAILURE;
    }
    rom.romLoad(romData);
    fclose(file);

    char input;
    printf("Starting Intel 8080 Emulator. Press 'q' to quit.\n");


    int step = 1;
    while (true) {
        cpu.step();
        cpu.printState();
        printf("Step: %d\n\n", step);
        /*
        if (step % 5000 == 0) {
            scanf(" %c", &input);
            if (input == 'q') {
                break;
            }
        }
        step++;
    }

    return EXIT_SUCCESS;*/
}
