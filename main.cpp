#include <iostream>
#include <SDL2/SDL.h>

#include "chip8.h"

int main(int argc, char** argv)
{
    chip8 chip;

    chip.Initialize();
    chip.LoadProgram("roms/PONG");

    while (true)
    {
        chip.Cycle();
        if(chip._drawFlag)
            chip.Draw();
        chip.PollKeys();
    }
     
    return 0;
}