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
        _window = SDL_CreateWindow( "CHIP-8", SDL_WINDOWPOS_UNDEFINED,
                     SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if (_window == NULL)
            printf( "SDL could not create window ! SDL_Error: %s\n", SDL_GetError() ); 
        else
        {
            _screenSurface = SDL_GetWindowSurface(_window);
            SDL_FillRect( _screenSurface, NULL, 
                                SDL_MapRGB( _screenSurface->format, 0xFF, 0xFF, 0xFF));
            SDL_UpdateWindowSurface(_window);
        }
    }
}

const SDL_Window* Renderer::GetWindow()
{
    return _window;
}