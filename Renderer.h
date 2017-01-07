#include <SDL2/SDL.h>

const static int SCREEN_WIDTH = 640;
const static int SCREEN_HEIGHT = 320;

class Renderer
{
    private:
        SDL_Window* _window;
        SDL_Surface* _screenSurface;

        void Initialize();
        
    public:
        Renderer();
        ~Renderer();

        const SDL_Window* GetWindow();
};