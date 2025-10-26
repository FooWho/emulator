#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include "intel8080.h"
#include "virtualMemory.h"



int main(int argc, char *argv[])
{
    Intel8080 cpu;
    cpu.test();

    return 0;
}
