#include "Renderer.h"

Renderer::Renderer()
{
    Initialize();
}

Renderer::~Renderer()
{    
    SDL_DestroyWindow( _window );
    SDL_Quit();
}

void Renderer::Initialize()
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() ); 
    else {
        _window = SDL_CreateWindowAndRenderer(SCREEN_WIDTH,SCREEN_HEIGHT, NULL, _window, _renderer);
        if (_window == NULL)
            printf( "SDL could not create window ! SDL_Error: %s\n", SDL_GetError() ); 
        else
        {
            
        }
    }
}

const SDL_Window* Renderer::GetWindow()
{
    return _window;
}

void Renderer::Render(unsigned char* gfx)
{
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}