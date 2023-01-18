#include <time.h>
#include "SDLfunctions.h"
#include "functions.h"
#include <SDL.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char **argv)
{
    srand( time( NULL ) );

    int** grid = (int**) malloc(10*sizeof(int*));
    for(int i = 0; i < 10; i++)
    {
        *(grid+i) = (int*) malloc(10*sizeof(int));
        for(int j = 0; j < 10; j++)
        {
            *(*(grid + i) + j) = 0;
        }
    }

    char currentSavePath[80] = "";

    int nextNumber = 1;

    int score = 0;

    SDL_Window* window = SDL_CreateWindow("Yet Another 2048", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    runttf(renderer);

    if(SDL_GetWindowSurface(window) == NULL)
    {
        SDL_ExitWithError("getWindowSurface");
    }

    SDL_Event ev;

    createResourceFiles();
    int bestScore = getBestScore();

    while(1)
    {
        handleEvent(ev, window, renderer, grid, &nextNumber, &score, currentSavePath);
        renderCurrentScore(renderer, score);
        renderBestScore(renderer, bestScore, score);
        renderImage(renderer, 550, 204, 38, 38, "save");
        renderImage(renderer, 592, 204, 38, 38, "open");
        renderGrid(renderer, grid);
        renderNextNumber(renderer, nextNumber, 2);
        SDL_RenderPresent(renderer);
    }
}
