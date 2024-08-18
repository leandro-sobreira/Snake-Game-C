#include <SDL2/SDL.h>
#include <time.h>
#include <stdbool.h>

/*
====================== SNAKE GAME ======================
By Leandro Sobreira
Commented in English to pratice
====================X= SNAKE GAME =X====================
*/

/* ============= CODE TO COMPILE =================

g++ .\Snake.c -I ..\SDL2\x86_64-w64-mingw32\include\ -L ..\SDL2\x86_64-w64-mingw32\lib\ -lmingw32 -lSDL2main -lSDL2

   ==========X== CODE TO COMPILE ==X============== */

#define windowW 640
#define windowH 480

#define cellsNum 20  // Number of cells in the grid (cellNum*cellNum)
#define gridSize 400 //  Size of grid (in pixels)

#define centerGridH ((windowH - gridSize) / 2) - (gridSize / cellsNum)
#define centerGridW ((windowW - gridSize) / 2) - (gridSize / cellsNum)

typedef struct
{
    int size;
    int x[(cellsNum * cellsNum) + 1];
    int y[(cellsNum * cellsNum) + 1];
    int moveDir; // 1 = UP, 2 = RIGHT, 3 = DOWN, 4 = LEFT
    int moved;
    int score;
    SDL_Color hRGB;
    SDL_Color tRGB;
} Snake;

Snake player1 =
    {
        1,   // start size
        {2}, // start X
        {2}, // start Y
        2,   // start movedir
        false,
        0,
        {0, 180, 0}, // head Color
        {0, 255, 0}  // tail Color
};

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING); // Initialize all resources of SDL
    SDL_Window *window = NULL;
    window = SDL_CreateWindow(                          // Create the window
        "Snake Game!",                                  // Name of the window
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // Centralize the window
        windowW, windowH,                               // Size of the window
        SDL_WINDOW_SHOWN);                              // Show the window
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Rect cell;
    cell.h = gridSize / cellsNum;
    cell.w = gridSize / cellsNum;

    while (true)
    {
        SDL_RenderClear(renderer); // Clear the Screen

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            while (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) // Verify if the user close the game or press ESC
            {
                exit(0); // Stop the program
            }
        }

        // Objects
        for (int i = 1; i < cellsNum; i++)
        {
            for (int j = 1; j < cellsNum; j++)
            {
                cell.x = centerGridW + i*(gridSize / cellsNum);
                cell.y = centerGridH + j*(gridSize / cellsNum);


                // render Cells
                if ((i+j) % 2 == 0)
                {
                    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Defaut color
                } else {
                    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255); // Defaut color
                }

                // Render Player
                if (i == player1.x && j == player1.y){
                    SDL_SetRenderDrawColor(renderer, player1.hRGB, 255); // Defaut color
                    
                }
                
                
                SDL_RenderFillRect(renderer, &cell);               // Fill the Cells
            }
            
        }
        



        // BACKGROUND
        SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255); // Background Collor
        SDL_RenderPresent(renderer);                        // Fill the Screen with background collor

        SDL_Delay(100);
    }

    // END OF PROGRAM
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}