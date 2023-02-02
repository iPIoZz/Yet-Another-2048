#ifndef SDLPROJECT_SDLFUNCTIONS_H
#define SDLPROJECT_SDLFUNCTIONS_H

#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "functions.h"

int colours[12][3] = {{60,  60,  60}, // couleur de la puissance à l'index n
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

void setFontPath(char fontPath[80])
{
    char newFontPath[80] = "";
    strcpy(newFontPath, getenv("windir"));
    strcat(newFontPath, "\\Fonts\\micross.ttf");

    FILE* f = fopen(newFontPath, "r");
    if(f == NULL)
        SDL_ExitWithError("Invalid font path");
    else
        strcpy(fontPath, newFontPath);
    fclose(f);
}

void emptyGrid(int** grid)
{
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            *(*(grid + i) + j) = 0;
        }
    }
}

void resetGrid(int** grid, time_t* lastResetTime)
{
    time_t currentTime = time(NULL);
    if(currentTime - *lastResetTime <= 1 && *lastResetTime != 0)
    {
        *lastResetTime = currentTime;
        emptyGrid(grid);
    }
    else
    {
        *lastResetTime = currentTime;
    }
}

void renderButton(SDL_Renderer* renderer, int row, int column, int value, int base)
{
    char fontPath[80];
    setFontPath(fontPath);

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
    }
}

void renderNextNumber(SDL_Renderer* renderer, int value, int base)
{
    char fontPath[80];
    setFontPath(fontPath);

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
    setFontPath(fontPath);


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
    setFontPath(fontPath);


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
    setFontPath(fontPath);

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

void handleEvent(SDL_Event e, SDL_Window* window, SDL_Renderer* renderer, int** grid, int* nextNumber, int* score, char* filePath, time_t* lastResetTime)
{
    while(SDL_PollEvent(&e) != 0)
    {
        if(e.type == SDL_MOUSEBUTTONDOWN && e.button.clicks == SDL_BUTTON_LEFT) // Si l'utilisateur fait clic gauche
        {
            int xPos = e.button.x; // Obtention de la position du curseur relative à la fenêtre
            int yPos = e.button.y; // --
            int xCoords,yCoords;

            if((yPos >= 204 && yPos <= 242)) // Hauteur dans laquelle se trouvent les boutons de sauvegarde et d'ouverture
            {
                if((xPos >= 550 && xPos <= 588)) //Position du bouton de sauvegarde
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
                if(xPos >= 592 && xPos <= 630) //Position du bouton d'ouverture de sauvegarde
                {
                    openSaveFile(grid, nextNumber, score, filePath);
                }
                if(xPos >= 634 && xPos <= 672 && time(NULL) - *lastResetTime >= 1) //Position de réinitalisation de grille
                {
                    resetGrid(grid, lastResetTime);
                }
            }

            getButtonCoords(xPos, yPos, &xCoords, &yCoords); // Obtention de la case cliquée

            if(xCoords >= 0 && yCoords >= 0) // Si on a cliqué sur une case (la valeur -1 est retournée si on ne clique sur aucune case)
            {
                int frameValue = *(*(grid + xCoords) + yCoords); // valeur de la case cliquée
                if(frameValue == 0) // Si la case est vide
                {
                    *(*(grid + xCoords) + yCoords) = *nextNumber; // Pose le prochain nombre à la case cliquée

                    int rows[16] = {}; // Cases identiques
                    int columns[16] = {}; // --
                    int result = checkSurroundings(grid, xCoords, yCoords, rows, columns); // Fait une première vérification pour voir si fusion est possible
                    while(result >= 2) // Tant qu'il peut y avoir des fusions
                    {
                        for(int i = 0; i < result; i++) // Boucle à travers tous les résultats
                        {
                            *(*(grid + *(rows+i)) + *(columns+i)) = 0; // Vide les cases identiques
                            *(rows+i) = NULL; // Supprime ces cases du tableau qui possède les cases identiques
                            *(columns+i) = NULL; // --
                        }
                        *(*(grid + xCoords) + yCoords) += 1; // Ajoute un à la valeur de la case cliquée (si la base est de 2 la valeur convertie passera par exemple de 8 à 16)
                        *score += *(*(grid + xCoords) + yCoords); // On ajoute au score la nouvelle valeur de la case cliquée

                        result = checkSurroundings(grid, xCoords, yCoords, rows, columns); // On refait une vérification pour voir si la fusion est possible et si elle l'est la boucle continuera
                    }

                    if(*(*(grid + xCoords) + yCoords) >= 11) // Vérifie si la valeur obtenue est supérieure ou égale à 11 (2^11 = 2048)
                    {
                        SDL_ShowSimpleMessageBox(-1, "Yet Another 2048", "Vous avez gagné!", window); // Déclare la victoire
                        emptyGrid(grid);
                    }else if(checkLoss(grid)) // Si l'utilisateur a perdu
                    {
                        SDL_ShowSimpleMessageBox(-1, "Yet Another 2048", "Vous avez perdu!", window); // Déclare la défaite
                        emptyGrid(grid);
                    }
                    generateNextNumber(5, nextNumber); // Génération de la valeur de la prochaine case à poser
                }
            }

        }
        if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) // Touche Q pour fermer le jeu
        {
            destroyTTF(window, renderer); // Détruit le système SDL
            exit(0); // Ferme le jeu
        }
        if((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_o)) // Touche O pour ouvrir un fichier
        {
            openSaveFile(grid, nextNumber, score, filePath); // Ouvre un fichier
        }
        if((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s)) // Touche S pour sauvegarder un fichier
        {
            if(filePath[0] != 0) { // Si le fichier de sauvegarde a déjà été selectionné
                saveCurrent(grid, nextNumber, score, filePath); // Sauvegarde
            }
            else{ // Si le fichier de sauvegarde n'a pas déjà été selectionné
                const char* lFilterPatterns[1] = {"*.dat"};
                char* selection = tinyfd_saveFileDialog( // ouverture du dialogue d'ouverture de fichier
                        "Save progress",
                        "C:\\",
                        1,
                        lFilterPatterns,
                        NULL
                );
                if(selection != NULL) // Si ce qui a été sélectionné n'est pas null
                {
                    saveCurrent(grid, nextNumber, score, selection); // Sauvegarde
                }
            }
        }
        if((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r && time(NULL) - *lastResetTime >= 1)) // Touche R pour réinitailiser la grille
        {   // vérification pour être sûr que la personne n'a pas faite exprès d'appuyer sur la touche plusieurs fois
            resetGrid(grid, lastResetTime); // réinitialisation de la grille
        }
    }
}

#endif //SDLPROJECT_SDLFUNCTIONS_H
