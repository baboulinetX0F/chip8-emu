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
    
    int last_ticks = SDL_GetTicks();
    int last_delta = 0, step_delta = 0, render_delta = 0;
    while (true)
    {
        last_delta = SDL_GetTicks() - last_ticks;
        last_ticks = SDL_GetTicks();
        step_delta += last_delta;
        render_delta += last_delta;

        if (step_delta >= 1){
            chip.Cycle();
            step_delta--;
        }

        if (render_delta >= (1000/60)) {
            if(chip._drawFlag)
                chip.Draw();
            render_delta -= (1000 / 60);     
        }       
        chip.PollKeys();
        SDL_Delay(1);    
    }
     
    return 0;
}