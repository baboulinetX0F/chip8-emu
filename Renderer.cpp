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
        SDL_CreateWindowAndRenderer(SCREEN_WIDTH,SCREEN_HEIGHT, NULL, &_window, &_renderer);
        if (_window == NULL)
            printf( "SDL could not create window ! SDL_Error: %s\n", SDL_GetError() );        
    }
}

const SDL_Window* Renderer::GetWindow()
{
    return _window;
}

void Renderer::SetWindowTitle(const char* newTitle)
{
    SDL_SetWindowTitle(_window, newTitle);
}

void Renderer::Render(unsigned char* gfx)
{    
    SDL_RenderClear(_renderer);
    for (unsigned int y = 0; y < 32; y++)
    {
        for (unsigned int x = 0; x < 64; x++)
        {
            if (gfx[x + (y*64)] != 0)
            {
               // Scale original 64x32 display to a correct resolution
               for (int yi = 0; yi < RENDER_SCALE; yi++)
               {
                    for (int xi = 0; xi < RENDER_SCALE; xi++)
                    {
                        SDL_RenderDrawPoint(_renderer,x*RENDER_SCALE + xi, y*RENDER_SCALE + yi);
                    } 
               }
            }
        }
    }    
    SDL_RenderPresent(_renderer);
}

void Renderer::PollKeys(unsigned char* keys)
{
    SDL_Event event;    
    SDL_PollEvent( &event );
    switch( event.type ){
        case SDL_KEYDOWN:
            switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        keys[0x5] = 1;
                        printf("Key Pressed : %d\n", event.key.keysym.sym);
                        break;                    
                    default:
                        break;
            }            
        break;                    
        case SDL_KEYUP:
            switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        keys[0x5] = 0;
                        break;                    
            }
        break;
    }
}