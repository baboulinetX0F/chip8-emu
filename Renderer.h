#include <SDL2/SDL.h>

const static int SCREEN_WIDTH = 640;
const static int SCREEN_HEIGHT = 320;
const static int RENDER_SCALE = SCREEN_WIDTH / 64;

class Renderer
{
    private:
        SDL_Window *_window;
        SDL_Renderer *_renderer;

        void Initialize();
        
    public:
        Renderer();
        ~Renderer();

        const SDL_Window* GetWindow();
        void SetWindowTitle(const char* newTitle);

        void Render(unsigned char* gfx);
        void PollKeys(unsigned char* input);
};