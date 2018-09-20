#include <iostream>
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "configuration.hpp"
#include "game.hpp"
#include "grid.hpp"
#include "marching.hpp"
#include "render.hpp"

#define WINDOW_TITLE "Example OpenGL Context"
#define WINDOW_W 800
#define WINDOW_H 600

#define IMG_INIT_FLAGS (IMG_INIT_PNG | IMG_INIT_JPG)

const bool hardcoded_map[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

int main(int argc, char *argv[])
{
    SDL_version version;

    SDL_GetVersion(&version);

    std::cout << "Linked against SDL version " <<
        (int)version.major << "." <<
        (int)version.minor << "." <<
        (int)version.patch << std::endl;

    const SDL_version *img_version = IMG_Linked_Version();

    std::cout << "Linked against SDL_image version " <<
        (int)img_version->major << "." <<
        (int)img_version->minor << "." <<
        (int)img_version->patch << std::endl;

    const SDL_version *ttf_version = IMG_Linked_Version();

    std::cout << "Linked against SDL_ttf version " <<
              (int)ttf_version->major << "." <<
              (int)ttf_version->minor << "." <<
              (int)ttf_version->patch << std::endl;

    SDL_Window *window = nullptr;
    SDL_GLContext context;

    if(SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        if(IMG_Init(IMG_INIT_FLAGS) == IMG_INIT_FLAGS)
        {
            if(TTF_Init() == 0)
            {
                window = SDL_CreateWindow(
                        WINDOW_TITLE,
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        WINDOW_W,
                        WINDOW_H,
                        SDL_WINDOW_OPENGL);

                if(window != nullptr)
                {
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
                    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

                    context = SDL_GL_CreateContext(window);

                    if(context != nullptr)
                    {
                        GLenum glewStatus = glewInit();

                        if(glewStatus == GLEW_OK)
                        {
                            std::vector<Shape> shapes;
                            Grid<bool> grid(20, 20, hardcoded_map);

                            Game game;

                            game.camera.x = -2.0f;
                            game.camera.y = -2.0f;
                            game.camera.w = WINDOW_W;
                            game.camera.h = WINDOW_H;
                            game.camera.zoom = 20;

                            UpdateProjection(game.camera);

                            const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
                            Uint32 lastTimeStep = SDL_GetTicks();                            
                            bool finished = false;

                            while(!finished)
                            {
                                SDL_Event event;

                                while(SDL_PollEvent(&event) != 0)
                                {
                                    if(event.type == SDL_QUIT)
                                    {
                                        finished = true;
                                        break;
                                    }
                                    else
                                    {
                                        switch(event.type)
                                        {
                                        case SDL_MOUSEBUTTONDOWN:
                                            if(event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED)
                                            {
                                                size_t x = (event.button.x + game.camera.x * game.camera.zoom) / (TILE_W * game.camera.zoom);
                                                size_t y = (event.button.y + game.camera.y * game.camera.zoom) / (TILE_H * game.camera.zoom);

                                                if(x < grid.w && y < grid.h)
                                                {
                                                    bool &tile = grid.getTile(x, y);
                                                    tile = !tile;
                                                    shapes = Marching::solveGrid(grid);
                                                    game.buildWalls(grid);
                                                }
                                            }
                                        }
                                    }
                                }

                                for(Uint32 time = SDL_GetTicks(); lastTimeStep - time > TIME_STEP; lastTimeStep += TIME_STEP)
                                {
                                    game.handleInput(keyboardState);
                                    game.step();
                                }

                                glClear(GL_COLOR_BUFFER_BIT);
                                UpdateProjection(game.camera);
                                RenderGrid(grid);
                                RenderCursor(grid, game.camera);
                                RenderPlayer(*game.player);

                                for(const Shape &shape : shapes)
                                {
                                    RenderShape(shape.points);
                                }

                                for(b2Body *bullet : game.bullets)
                                {
                                    RenderBullet(*bullet);
                                }
                                
                                SDL_GL_SwapWindow(window);

                                SDL_Delay(20);
                            }
                        }
                        else
                        {
                            std::cerr << "glewInit: " << glewGetErrorString(glewStatus) << std::endl;
                        }

                        SDL_GL_DeleteContext(context);
                    }
                    else
                    {
                        std::cerr << "SDL_GL_CreateContext: " << SDL_GetError() << std::endl;
                    }

                    SDL_DestroyWindow(window);
                }
                else
                {
                    std::cerr << "SDL_CreateWindow: " << SDL_GetError() << std::endl;
                }
                TTF_Quit();
            }
            else
            {
                std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
            }
            IMG_Quit();
        }
        else
        {
            std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
        }
        SDL_Quit();
    }
    else
    {
        std::cerr << "SDL_Init: " << SDL_GetError() << std::endl;
    }

    return EXIT_SUCCESS;
}