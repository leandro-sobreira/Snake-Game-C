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

#define windowW 480
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
    SDL_Color color;
} tSnake;

tSnake initPlayer(){
    tSnake player =
    {
        1,   // start size
        {{2, 2}}, // start X Y
        2,   // start movedir
        false,
        0,
        {0, 230, 0}, // head Color
    };

    return player;
}

void createApple(tSnake player, tObj *apple){
    tObj blankSp[(cellsNum*cellsNum)+1];
    bool flag = false;
    int l = 0;

    for (int i = 1; i <= cellsNum; i++)
    {
        for (int j = 1; j <= cellsNum; j++)
        {
            flag = false;
            for (int k = 0; k < player.size; k++)
            {
                if (player.pos[k].x == i && player.pos[k].y == j)
                {
                    flag = true;
                }   
            }
            if (!flag)
            {
                blankSp[l].x = i;
                blankSp[l].y = j;
                l++;
            }
        }
    }
    
    *apple = blankSp[rand()%l];
}

void resetGame(tSnake *player){
    *player = initPlayer();
}

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

    
    
    tSnake player = initPlayer();

    tObj apple; //= {(rand()%(cellsNum-1))+1, (rand()%(cellsNum-1))+1}; 
    createApple(player, &apple);
    printf("|%d, %d|\n", apple.x, apple.y);
    bool gameOver = false;

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
                if (event.key.keysym.sym == SDLK_r)
                {
                    resetGame(&player);
                    createApple(player, &apple);
                }
                
                if (player.moved)
                {
                    if (event.key.keysym.sym == SDLK_w && player.moveDir != 3 && player.moveDir != 1)
                    {
                        player.moveDir = 1;
                        player.moved = false;
                    } else if (event.key.keysym.sym == SDLK_d && player.moveDir != 4 && player.moveDir != 2)
                    {
                        player.moveDir = 2;
                        player.moved = false;
                    } else if (event.key.keysym.sym == SDLK_s && player.moveDir != 1 && player.moveDir != 3)
                    {
                        player.moveDir = 3;
                        player.moved = false;
                    } else if (event.key.keysym.sym == SDLK_a && player.moveDir != 2 && player.moveDir != 4)
                    {
                        player.moveDir = 4;
                        player.moved = false;
                    }
                }                        
            }      
        }
        //Move player segs
        for (int i = player.size; i >= 1; i--)
        {
            player.pos[i] = player.pos[i-1];
        }

        //Move Player
        switch (player.moveDir)
        {
            case 1:
                player.pos[0].y += -1;
                player.moved = true;
            break;

            case 2:
                player.pos[0].x += 1;
                player.moved = true;
            break;

            case 3:
                player.pos[0].y += 1;
                player.moved = true;
            break;

            case 4:
                player.pos[0].x += -1;
                player.moved = true;
            break;
        }     
           
         //Eat apple
        if (player.pos[0].x == apple.x && player.pos[0].y == apple.y)
        {
            createApple(player, &apple);
            printf("|%d, %d|\n", apple.x, apple.y);
            player.size++;
        }

        //Colision
        if (player.pos[0].x > cellsNum || player.pos[0].x < 1)
        {
            gameOver = true;
        }else if (player.pos[0].y > cellsNum || player.pos[0].y < 1)
        {
            gameOver = true;
        }

        for (int i = 1; i <= player.size; i++)
        {
            if (player.pos[0].x == player.pos[i].x && player.pos[0].y == player.pos[i].y)
            {
                gameOver = true;
            }            
        }

        //Game Over
        if(gameOver){
            resetGame(&player);
            createApple(player, &apple);
            gameOver = false;
            SDL_Delay(1000);
        }
        
        // Render Objects
        for (int i = 1; i <= cellsNum; i++)
        {
            cell.x = centerGridW + i*cellSize;
            for (int j = 1; j <= cellsNum; j++)
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
                for (int k = player.size-1; k >= 0; k--)
                {
                    if (i == player.pos[k].x && j == player.pos[k].y){
                        SDL_SetRenderDrawColor(renderer, k*150/player.size, player.color.g, k*150/player.size, 255);
                    }
                }
                
                SDL_RenderFillRect(renderer, &cell);               // Fill the Cells
            }
            
        }     

        // BACKGROUND
        SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255); // Background Collor
        SDL_RenderPresent(renderer);                        // Fill the Screen with background collor

        SDL_Delay(200);
    }

    // END OF PROGRAM
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}