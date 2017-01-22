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

int Renderer::PollKeys(unsigned char* keys)
{
    SDL_Event event;    
    SDL_PollEvent( &event );
    switch( event.type ){
        case SDL_KEYDOWN:
            switch( event.key.keysym.sym ){
                    case SDLK_1:
                        keys[0x1] = 1;                       
                        break;
                    case SDLK_2:
                        keys[0x2] = 1;                       
                        break;
                    case SDLK_3:
                        keys[0x3] = 1;                       
                        break;
                     case SDLK_4:
                        keys[0xC] = 1;                       
                        break;
                    case SDLK_a:
                        keys[0x4] = 1;
                        break;
                    case SDLK_e:
                        keys[0x6] = 1;                       
                        break;
                    case SDLK_z:
                        keys[0x5] = 1;                       
                        break;
                    case SDLK_r:
                        keys[0xD] = 1;                       
                        break;
                    case SDLK_q:
                        keys[0x7] = 1;                       
                        break;
                    case SDLK_s:
                        keys[0x8] = 1;                       
                        break;
                    case SDLK_d:
                        keys[0x9] = 1;                       
                        break;
                    case SDLK_f:
                        keys[0xE] = 1;                       
                        break; 
                    case SDLK_w:
                        keys[0xA] = 1;                       
                        break;
                    case SDLK_x:
                        keys[0x0] = 1;                       
                        break;
                    case SDLK_c:
                        keys[0xB] = 1;                       
                        break;
                    case SDLK_v:
                        keys[0xC] = 1;                       
                        break;       
            }            
        break;                    
        case SDL_KEYUP:
            switch( event.key.keysym.sym ){
                    case SDLK_1:
                        keys[0x1] = 0;                       
                        break;
                    case SDLK_2:
                        keys[0x2] = 0;                       
                        break;
                    case SDLK_3:
                        keys[0x3] = 0;                       
                        break;
                     case SDLK_4:
                        keys[0xC] = 0;                       
                        break;
                    case SDLK_a:
                        keys[0x4] = 0;
                        break;
                    case SDLK_e:
                        keys[0x6] = 0;                       
                        break;
                    case SDLK_z:
                        keys[0x5] = 0;                       
                        break;
                    case SDLK_r:
                        keys[0xD] = 0;                       
                        break;
                    case SDLK_q:
                        keys[0x7] = 0;                       
                        break;
                    case SDLK_s:
                        keys[0x8] = 0;                       
                        break;
                    case SDLK_d:
                        keys[0x9] = 0;                       
                        break;
                    case SDLK_f:
                        keys[0xE] = 0;                       
                        break; 
                    case SDLK_w:
                        keys[0xA] = 0;                       
                        break;
                    case SDLK_x:
                        keys[0x0] = 0;                       
                        break;
                    case SDLK_c:
                        keys[0xB] = 0;                       
                        break;
                    case SDLK_v:
                        keys[0xC] = 0;                       
                        break;       
            }
        break;
        case SDL_QUIT:
            return SDL_QUIT;
    }
}