#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include "intel8080.hpp"
#include "rom.hpp"
#include "ram.hpp"
#include "bus.hpp"
#include "spaceInvaders.hpp"
#include "intel8080TestHelper.hpp"

int main(int argc, char *argv[]) {
    SpaceInvaders game;

    game.Initialize();

    game.Run();

    return 0;
    
}
