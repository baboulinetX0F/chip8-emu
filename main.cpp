#include <iostream>
#include <SDL2/SDL.h>

#include "chip8.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Error : No filepath argument passed\nChip8 Emulator Usage : chip8bin <file>\n";
        return -1;
    }
    
    chip8 chip;

    chip.Initialize();
    if (chip.LoadProgram(argv[1]) == -1)
        return -1;    

    while (true)
    {
        chip.Cycle();
        if(chip._drawFlag)
            chip.Draw();
        chip.PollKeys();
    }
     
    return 0;
}