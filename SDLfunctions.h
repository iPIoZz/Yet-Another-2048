//
// Created by iploz on 22/11/2022.
//

#ifndef SDLPROJECT_SDLFUNCTIONS_H
#define SDLPROJECT_SDLFUNCTIONS_H

#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "functions.h"

int colours[12][3] = {{60,  60,  60},
                      {238, 228, 218},
                      {237, 224, 198},
                      {242, 177, 121},
                      {245, 149, 99},
                      {246, 124, 96},
                      {246, 94,  59},
                      {237, 207, 115},
                      {237, 204, 98},
                      {237, 200, 80},
                      {237, 197, 63},
                      {237,194,45}
};

void SDL_ExitWithError(char message[])
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    exit(EXIT_FAILURE);
}

SDL_Rect createRect(int x, int y, int width, int height)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    return rect;
}

void renderButton(SDL_Renderer* renderer, int row, int column, int value, int base)
{
    char fontPath[80];
    strcpy(fontPath, getenv("APPDATA"));
    strcat(fontPath, "\\YA2048\\resources\\micross.ttf");

    int convertedValue = convertValue(base, value);

    SDL_Rect button = createRect(3+51*row, 3+51*column, 48, 48);

    SDL_SetRenderDrawColor(renderer, *(*(colours+value)), *(*(colours+value)+1), *(*(colours+value)+2), SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &button);

    if(value != 0)
    {
        TTF_Font* font = TTF_OpenFont(fontPath, 12);

        SDL_Color White = { 249, 246, 242 };
        SDL_Color Black = { 119, 110, 101 };
        SDL_Surface* text;
        if(value <= 2)
        {
            text = TTF_RenderText_Blended(font, intToString(convertedValue), Black);
        }else{
            text = TTF_RenderText_Blended(font, intToString(convertedValue), White);
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, text);
        if(texture == NULL)
            SDL_ExitWithError("Texturea");

        int xButtonCenter = button.x + (int)(button.w / 2) - (int)(text->w / 2); //3 + 7/2 = 6.5
        int yButtonCenter = button.y + (int)(button.h / 2) - (int)(text->h / 2); //3 + 14/2 = 10


        SDL_Rect dst;
        dst.x = xButtonCenter;
        dst.y = yButtonCenter;
        dst.w = text->w;
        dst.h = text->h;

        if(SDL_RenderCopy(renderer, texture, NULL, &dst) != 0)
            SDL_ExitWithError("Render copy");

        SDL_FreeSurface(text);
        SDL_DestroyTexture(texture);
        TTF_CloseFont(font);
    }
}

void renderNextNumber(SDL_Renderer* renderer, int value, int base)
{
    char fontPath[80];
    strcpy(fontPath, getenv("APPDATA"));
    strcat(fontPath, "\\YA2048\\resources\\micross.ttf");

    int convertedValue = convertValue(base, value);

    SDL_Rect button;
    button.x = 550;
    button.y = 3;
    button.w = 64;
    button.h = 64;

    SDL_SetRenderDrawColor(renderer, colours[value][0], colours[value][1], colours[value][2], SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &button);

    TTF_Font* font = TTF_OpenFont(fontPath, 25);
    if(font == NULL)
        SDL_ExitWithError("Font");

    SDL_Color White = { 249, 246, 242 };
    SDL_Color Black = { 119, 110, 101 };
    SDL_Surface* text;
    if(value <= 2)
    {
        text = TTF_RenderText_Blended(font, intToString(convertedValue), Black);
    }else{
        text = TTF_RenderText_Blended(font, intToString(convertedValue), White);
    }

    if(text == NULL)
        SDL_ExitWithError("TextNextNumber");

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, text);
    if(texture == NULL)
        SDL_ExitWithError("Textureb");

    int xButtonCenter = button.x + (int)(button.w / 2) - (int)(text->w / 2);
    int yButtonCenter = button.y + (int)(button.h / 2) - (int)(text->h / 2);

    SDL_Rect dst;
    dst.x = xButtonCenter;
    dst.y = yButtonCenter;
    dst.w = text->w;
    dst.h = text->h;

    if(SDL_RenderCopy(renderer, texture, NULL, &dst) != 0)
        SDL_ExitWithError("Render copy");

    SDL_FreeSurface(text);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);

    TTF_Font* titleFont = TTF_OpenFont(fontPath, 11);
    SDL_Surface* titleText = TTF_RenderText_Blended(titleFont, "Next number", White);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleText);

    SDL_Rect titleDst;
    titleDst.x = button.x;
    titleDst.y = button.y + button.h;
    titleDst.w = titleText->w;
    titleDst.h = titleText->h;

    if(SDL_RenderCopy(renderer, titleTexture, NULL, &titleDst) != 0)
        SDL_ExitWithError("Render copy");

    SDL_FreeSurface(titleText);
    SDL_DestroyTexture(titleTexture);
    TTF_CloseFont(titleFont);
}

void renderGrid(SDL_Renderer* renderer, int** grid)
{

    char fontPath[80];
    strcpy(fontPath, getenv("APPDATA"));
    strcat(fontPath, "\\YA2048\\resources\\micross.ttf");


    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            int value = *(*(grid + i)+j);
            if(value == NULL && sizeof(value) == 8)
            {
                printf("ERROR VALUE\t%d\t%d\n", i, j);
            }else{
                renderButton(renderer, i, j, value, 2);
            }
        }
    }
}

void renderCurrentScore(SDL_Renderer* renderer, int score)
{
    char fontPath[80];
    strcpy(fontPath, getenv("APPDATA"));
    strcat(fontPath, "\\YA2048\\resources\\micross.ttf");


    SDL_Rect rect = createRect(550, 120, 80, 80);

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);

    TTF_Font* font = TTF_OpenFont(fontPath, 22);
    if(font == NULL)
        SDL_ExitWithError("renderCurrentScore FONT");
    SDL_Color White = { 249, 246, 242 };
    SDL_Color Black = { 0, 0, 0 };

    SDL_Surface* titleText = TTF_RenderText_Blended(font, "SCORE", White);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleText);


    int xButtonCenter = rect.x + (int)(rect.w / 2) - (int)(titleText->w / 2);

    SDL_Rect titleDst = createRect(xButtonCenter, rect.y, titleText->w, titleText->h);

    if(SDL_RenderCopy(renderer, titleTexture, NULL, &titleDst) != 0)
        SDL_ExitWithError("Render copy score's title");

    SDL_Surface* scoreText = TTF_RenderText_Blended(font, intToString(score), White);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreText);

    xButtonCenter = rect.x + (int)(rect.w / 2) - (int)(scoreText->w / 2);
    int yButtonCenter = rect.y + (int)(rect.h / 2) - (int)(scoreText->h / 2);

    SDL_Rect scoreDst = createRect(xButtonCenter, yButtonCenter + (int)(titleText->h / 2) - 3, scoreText->w, scoreText->h);

    if(SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreDst) != 0)
        SDL_ExitWithError("Render copy score");


    SDL_FreeSurface(titleText);
    SDL_FreeSurface(scoreText);
    SDL_DestroyTexture(titleTexture);
    SDL_DestroyTexture(scoreTexture);

    TTF_CloseFont(font);
}

void renderBestScore(SDL_Renderer* renderer, int best, int score)
{
    char fontPath[80];
    strcpy(fontPath, getenv("APPDATA"));
    strcat(fontPath, "\\YA2048\\resources\\micross.ttf");

    if(score > best)
    {
        setBestScore(score);
        best = score;
    }

    SDL_Rect rect = createRect(634, 120, 80, 80);

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);

    TTF_Font* font = TTF_OpenFont(fontPath, 22);
    if(font == NULL)
        SDL_ExitWithError("renderBestScore FONT");
    SDL_Color White = { 249, 246, 242 };
    SDL_Color Black = { 0, 0, 0 };

    SDL_Surface* titleText = TTF_RenderText_Blended(font, "BEST", White);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleText);


    int xButtonCenter = rect.x + (int)(rect.w / 2) - (int)(titleText->w / 2);

    SDL_Rect titleDst = createRect(xButtonCenter, rect.y, titleText->w, titleText->h);

    if(SDL_RenderCopy(renderer, titleTexture, NULL, &titleDst) != 0)
        SDL_ExitWithError("Render copy score's title");

    SDL_Surface* scoreText = TTF_RenderText_Blended(font, intToString(best), White);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreText);

    xButtonCenter = rect.x + (int)(rect.w / 2) - (int)(scoreText->w / 2);
    int yButtonCenter = rect.y + (int)(rect.h / 2) - (int)(scoreText->h / 2);

    SDL_Rect scoreDst = createRect(xButtonCenter, yButtonCenter + (int)(titleText->h / 2) - 3, scoreText->w, scoreText->h);

    if(SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreDst) != 0)
        SDL_ExitWithError("Render copy score");


    SDL_FreeSurface(titleText);
    SDL_FreeSurface(scoreText);
    SDL_DestroyTexture(titleTexture);
    SDL_DestroyTexture(scoreTexture);

    TTF_CloseFont(font);
}

void renderImage(SDL_Renderer* renderer, int x, int y, int width, int height, char* imageName)
{
    SDL_Rect rect = createRect(x, y, width, height);

    char filePath[80];
    strcpy(filePath, getenv("APPDATA"));
    strcat(filePath, "\\YA2048\\resources\\");
    strcat(filePath, imageName);
    strcat(filePath, ".bmp");

    SDL_Surface* image = IMG_Load(filePath);
    if(image == NULL)
        SDL_ExitWithError("Save image loading");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

    SDL_Rect dst = createRect(x, y, image->w, image->h);

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderCopy(renderer, texture, NULL, &dst);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
}

void runttf(SDL_Renderer* renderer)
{
    const int SCREEN_WIDTH = 800 ;
    const int SCREEN_HEIGHT = 600 ;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_SetRenderDrawColor(renderer, 32, 32, 32, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

void destroyTTF(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void handleEvent(SDL_Event e, SDL_Window* window, SDL_Renderer* renderer, int** grid, int* nextNumber, int* score, char* filePath)
{
    while(SDL_PollEvent(&e) != 0)
    {
        if(e.type == SDL_MOUSEBUTTONDOWN && e.button.clicks == SDL_BUTTON_LEFT)
        {
            int xPos = e.button.x;
            int yPos = e.button.y;
            int xCoords,yCoords;

            if((yPos >= 204 && yPos <= 242))
            {
                if((xPos >= 550 && xPos <= 588)) //save file button
                {
                    if(filePath[0] != 0) {
                        saveCurrent(grid, nextNumber, score, filePath);
                    }
                    else{
                        const char* lFilterPatterns[1] = {"*.dat"};
                        char* selection = tinyfd_saveFileDialog(
                                        "Save progress",
                                                "C:\\",
                                                1,
                                                lFilterPatterns,
                                                NULL
                        );
                        if(selection != NULL)
                        {
                            saveCurrent(grid, nextNumber, score, selection);
                        }
                    }
                }
                if(xPos >= 592 && xPos <= 630) //open file button
                {
                    openSaveFile(grid, nextNumber, score, filePath);
                }
            }

            getButtonCoords(xPos, yPos, &xCoords, &yCoords);

            if(xCoords >= 0 && yCoords >= 0)
            {
                int frameValue = *(*(grid + xCoords) + yCoords);
                if(frameValue == 0)
                {
                    *(*(grid + xCoords) + yCoords) = *nextNumber;

                    int rows[16] = {};
                    int columns[16] = {};
                    int result = checkSurroundings(grid, xCoords, yCoords, rows, columns);
                    while(result >= 2)
                    {
                        for(int i = 0; i < result; i++)
                        {
                            *(*(grid + *(rows+i)) + *(columns+i)) = 0;
                            *(rows+i) = NULL;
                            *(columns+i) = NULL;
                        }
                        *(*(grid + xCoords) + yCoords) += 1;
                        *score += *(*(grid + xCoords) + yCoords);

                        result = checkSurroundings(grid, xCoords, yCoords, rows, columns);
                    }

                    if(*(*(grid + xCoords) + yCoords) >= 11)
                    {
                        SDL_ShowSimpleMessageBox(-1, "Yet Another 2048", "You have won!", window);
                        for(int i = 0; i < 10; i++)
                        {
                            for(int j = 0; j < 10; j++)
                            {
                                *(*(grid + i) + j) = 0;
                            }
                        }
                    }else if(checkLoss(grid))
                    {
                        SDL_ShowSimpleMessageBox(-1, "Yet Another 2048", "You have lost!", window);
                        for(int i = 0; i < 10; i++)
                        {
                            for(int j = 0; j < 10; j++)
                            {
                                *(*(grid + i) + j) = 0;
                            }
                        }
                    }
                    generateNextNumber(5, nextNumber);
                }
            }

        }
        if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q))
        {
            destroyTTF(window, renderer);
            exit(0);
        }
        if((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_o))
        {
            openSaveFile(grid, nextNumber, score, filePath);
        }
        if((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s))
        {
            if(filePath[0] != 0) {
                saveCurrent(grid, nextNumber, score, filePath);
            }
            else{
                const char* lFilterPatterns[1] = {"*.dat"};
                char* selection = tinyfd_saveFileDialog(
                        "Save progress",
                        "C:\\",
                        1,
                        lFilterPatterns,
                        NULL
                );
                if(selection != NULL)
                {
                    saveCurrent(grid, nextNumber, score, selection);
                }
            }
        }
    }
}

#endif //SDLPROJECT_SDLFUNCTIONS_H
