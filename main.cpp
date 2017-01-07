#include <iostream>

#include "chip8.h"

int main(int argc, char** argv)
{
    chip8 chip;

    chip.LoadProgram("roms/INVADERS");
    std::cout << "Hello World\n";
    return 0;
}