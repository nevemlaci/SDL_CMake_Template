#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL2_gfxPrimitives.h>

int main(int argc, char** argv) {

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf(SDL_GetError());
    }

    if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) {
        printf(SDL_GetError());
    }

    if(TTF_Init() == -1) {
        printf(SDL_GetError());
    }

    if(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
        printf(SDL_GetError());
    }

    SDL_Window* window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
    if(window == NULL) {
        printf(SDL_GetError());
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        printf(SDL_GetError());
    }

    SDL_Texture* texture = IMG_LoadTexture(renderer, "test.png");
    if(texture == NULL) {
        printf(SDL_GetError());
    }

    SDL_FRect pos = {10.0f, 10.0f, 50.0f, 50.0f};

    SDL_Event event;

    const SDL_FRect rect = {300, 200, 50, 100};
    const SDL_FRect rect_filled = {600, 100, 70, 90};
    while(true) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    SDL_DestroyWindow(window);
                    SDL_DestroyTexture(texture);
                    SDL_DestroyRenderer(renderer);
                    SDL_Quit();
                    return 0;

                case SDL_KEYDOWN:
                    if(event.key.keysym.scancode == SDL_SCANCODE_W) {
                        printf("W lenyomva\n");
                    }
                    break;

                case SDL_KEYUP:
                    if(event.key.keysym.scancode == SDL_SCANCODE_W) {
                        printf("W felengedve\n");
                    }
                    break;

                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 122, 248, 255, SDL_ALPHA_OPAQUE);

        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

        SDL_RenderDrawRectF(renderer, &rect);

        SDL_RenderFillRectF(renderer, &rect_filled);

        SDL_RenderCopyF(renderer, texture, NULL, &pos);

        SDL_RenderPresent(renderer);
    }
}