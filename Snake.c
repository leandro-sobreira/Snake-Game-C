#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

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

#define cellsNum 20  // Number of cells in the grid = (cellNum*cellNum)
#define gridSize 400 //  Size of grid (in pixels)
#define cellSize gridSize/cellsNum

#define centerGridH ((windowH - gridSize) / 2) - (gridSize / cellsNum)
#define centerGridW ((windowW - gridSize) / 2) - (gridSize / cellsNum)


typedef struct
{
    int x;
    int y;
} tObj;

typedef struct
{
    int size;
    tObj pos[(cellsNum*cellsNum)+1];
    int moveDir; // 1 = UP, 2 = RIGHT, 3 = DOWN, 4 = LEFT
    bool moved;
    int score;
    SDL_Color hRGB;
    SDL_Color tRGB;
} tSnake;

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
    cell.h = cellSize;
    cell.w = cellSize;

    tSnake player1 =
    {
        1,   // start size
        {{2, 2}, {1,1}}, // start X Y
        2,   // start movedir
        false,
        0,
        {0, 180, 0}, // head Color
        {0, 255, 0}  // tail Color
    };

    tObj apple = {rand()%cellsNum, rand()%cellsNum};    

    while (true)
    {
        SDL_RenderClear(renderer); // Clear the Screen
        

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) // Verify if the user close the game or press ESC
            {
                exit(0); // Stop the program
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (player1.moved)
                {
                    if (event.key.keysym.sym == SDLK_w && player1.moveDir != 3 && player1.moveDir != 1)
                    {
                        player1.moveDir = 1;
                        player1.moved = false;
                    } else if (event.key.keysym.sym == SDLK_d && player1.moveDir != 4 && player1.moveDir != 2)
                    {
                        player1.moveDir = 2;
                        player1.moved = false;
                    } else if (event.key.keysym.sym == SDLK_s && player1.moveDir != 1 && player1.moveDir != 3)
                    {
                        player1.moveDir = 3;
                        player1.moved = false;
                    } else if (event.key.keysym.sym == SDLK_a && player1.moveDir != 2 && player1.moveDir != 4)
                    {
                        player1.moveDir = 4;
                        player1.moved = false;
                    }
                }                
                
            }
            
        }

        //Move Player
        switch (player1.moveDir)
        {
            case 1:
                player1.pos[0].y += -1;
                player1.moved = true;
            break;

            case 2:
                player1.pos[0].x += 1;
                player1.moved = true;
            break;

            case 3:
                player1.pos[0].y += 1;
                player1.moved = true;
            break;

            case 4:
                player1.pos[0].x += -1;
                player1.moved = true;
            break;
        }     
           
         //Eat apple
        if (player1.pos[0].x == apple.x && player1.pos[0].y == apple.y)
        {
            apple = {rand()%cellsNum, rand()%cellsNum}; 
        }

        // Render Objects
        for (int i = 1; i < cellsNum; i++)
        {
            cell.x = centerGridW + i*cellSize;
            for (int j = 1; j < cellsNum; j++)
            {
                cell.y = centerGridH + j*cellSize;

                // render Cells
                if ((i+j) % 2 == 0) // Chess patern
                {
                    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
                } else {
                    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
                }

                //Render Apple
                if (apple.x == i && apple.y == j)
                {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                } 

                // Render Player
                    if (i == player1.pos[0].x && j == player1.pos[0].y){
                        SDL_SetRenderDrawColor(renderer, player1.hRGB.r, player1.hRGB.g, player1.hRGB.b, 255);
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