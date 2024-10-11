#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL2_gfxPrimitives.h>

//argc argv kell, az sdl megköveteli
int main(int argc, char** argv) {
    //inicializálni kell az SDL dolgait
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0 /*negatívat ad vissza ha hiba történt*/) {
        printf(SDL_GetError()); //SDL_GetError() visszad egy sztringet ami a legutóbbi hibát írja le.
    }

    //SDL-el ha több flaget szeretnénk megadni, akkor a bitenkénti 'vagy' ( | ) operátort használjuk.
    if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0 /*0-t ad vissza ha hiba történt*/) {
        printf(SDL_GetError());
    }

    if(TTF_Init() == -1 /*-1et ad vissza ha hiba történt*/) {
        printf(SDL_GetError());
    }

    if(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0 /*0-t ad vissza ha hiba történt*/) {
        printf(SDL_GetError());
    }

    //Csinálunk egy új ablakot és megjegyezzük a pointert amit az SDL ad rá
    SDL_Window* window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
    if(window == NULL) { //pointereket visszaadó függvények ált. NULL-t adnak vissza hiba esetén, ezt az SDL is követi
        printf(SDL_GetError());
    }
    //Csinálunk egy renderert ami az előbb készített window-hoz lesz kötve
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        printf(SDL_GetError());
    }
    //Az új rendererrel csinálunk egy textúrát
    SDL_Texture* texture = IMG_LoadTexture(renderer, "test.png");
    if(texture == NULL) {
        printf(SDL_GetError());
    }
    //az SDL_FRect struktúra egy lebegőpontos négyszöget ír le.
    //Ez lesz a kirajzolt textúránk pozíciója.
    SDL_FRect pos = {10.0f, 10.0f, 50.0f, 50.0f};

    //Ebben a struktúrában fogjuk tárolni az eventekre jellemző adatokat
    SDL_Event event;

    //téglalapok a rajzoláshoz
    const SDL_FRect rect = {300, 200, 50, 100};
    const SDL_FRect rect_filled = {600, 100, 70, 90};
    while(true) {
        //Amíg van valamilyen event addig egyesével megnézzük őket
        while (SDL_PollEvent(&event)) {
            switch (event.type) { //az event típusa alapján döntjük el, hogy mit csinálunk
                //ha az event típusa SDL_QUIT, azaz minden ablakot bezártunk
                //akkor töröljük a lefoglalt dolgokat(nem free() -vel, hanem az SDL saját függvényeivel!)
                case SDL_QUIT:
                    SDL_DestroyWindow(window);
                    SDL_DestroyTexture(texture);
                    SDL_DestroyRenderer(renderer);
                    SDL_Quit();//lezár minden iniciaizált subsystem-et.
                    return 0;

                //Ha billentyűleütés történik akkor megnézzük, hogy a W volt -e
                case SDL_KEYDOWN:
                    if(event.key.keysym.scancode == SDL_SCANCODE_W) {
                        printf("W lenyomva\n");
                    }
                    break;
                //Ha billentyűfelengedés történik akkor megnézzük, hogy a W volt -e
                case SDL_KEYUP:
                    if(event.key.keysym.scancode == SDL_SCANCODE_W) {
                        printf("W felengedve\n");
                    }
                    break;
                //minden más esetben nem érdekel mi történt, megyünk tovább
                default:
                    break;
            }
        }

        //beállítjuk a draw colort, ezzel fogja kitölteni az üres részeket a renderer, valamit ezzel a színnel fog rajzolni
        SDL_SetRenderDrawColor(renderer, 122, 248, 255, SDL_ALPHA_OPAQUE);

        //törlünk mindent ami a rendereren van
        SDL_RenderClear(renderer);

        //itt beállítjuk pirosra a draw colort
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        //rajzolunk egy téglalapot
        SDL_RenderDrawRectF(renderer, &rect);
        //kitöltünk egy másik téglalapot
        SDL_RenderFillRectF(renderer, &rect_filled);

        //"rámásoljuk" a rendererre a textúrát
        SDL_RenderCopyF(renderer, texture, NULL, &pos);

        //renderelünk mindent ami a rendererre másolva lett
        SDL_RenderPresent(renderer);
    }
}