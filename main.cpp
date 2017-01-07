#include <iostream>
#include <SDL2/SDL.h>

#include "chip8.h"

int main(int argc, char** argv)
{
    chip8 chip;

    chip.Initialize();
    chip.LoadProgram("roms/INVADERS");

    for (int i = 0; i < 10; i++)
        chip.Cycle();
    
    // Debug display    
    for (unsigned int i = 0; i < 16; i++)
        printf("V[%d] : %#06x\n", i, chip.getDataRegister()[i]);

    SDL_Delay( 2000 );   
    return 0;
}