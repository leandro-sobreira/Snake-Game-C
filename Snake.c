#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
            for (int k = 0; k <= player.size; k++)
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
    if (player.size >= cellsNum*cellsNum)
    {
        *apple = {0,0};
    }else{
        *apple = blankSp[rand()%l];
    }
    
}

void resetGame(tSnake *player){
    *player = initPlayer();
}

void AIsnake(tSnake *player, tObj apple){

   tObj opc[2] = {{0,0},{0,0}};
   bool pColision = false;
   float disEuX, disEuY;

    if (player->pos[0].x%2 == 0 && (player->pos[0].y != 1))
    {
        opc[0].y = 1; // Se o player está em uma coluna par ele pode ir para cima
        opc[1].y = -1; 
    } 
    if(player->pos[0].x%2 == 1 && player->pos[0].y != cellsNum) {
        opc[0].y = 3; // Se o player está em uma coluna impar ele pode ir para baixo
        opc[1].y = 1;
    }

    if (player->pos[0].y%2 == 0 && player->pos[0].x != cellsNum)
    {
        opc[0].x = 2; // Se o player está em uma linha par ele pode ir para direita
        opc[1].x = 1;
    } 
    if(player->pos[0].y%2 == 1 && (player->pos[0].x != 1)) {
        opc[0].x = 4; // Se o player está em uma linha impar ele pode ir para esquerda
        opc[1].x = -1;
    }

    if(opc[0].x == 0){
        player->moveDir = opc[0].y;
    } else if(opc[0].y == 0){
        player->moveDir = opc[0].x;
    } else {
        if (player->size > 3)
        {
            for (int i = 1; i < player->size; i++)
            {
                if (player->pos[0].x == player->pos[i].x && player->pos[0].y+opc[1].y == player->pos[i].y)
                {
                    player->moveDir = opc[0].x;
                    pColision = true;
                    break;
                } else if (player->pos[0].x+opc[1].x == player->pos[i].x && player->pos[0].y == player->pos[i].y)
                {
                    player->moveDir = opc[0].y;
                    pColision = true;
                    break;
                }    
            }
        }
        if (!pColision)
        {
            disEuY = pow((player->pos[0].x - apple.x),2) + pow((player->pos[0].y+opc[1].y-apple.y),2);
            disEuX = pow((player->pos[0].x+opc[1].x - apple.x),2) + pow((player->pos[0].y-apple.y),2);
            if (disEuX == disEuY)
            {
                if (rand()%2)
                {
                    player->moveDir = opc[0].x;
                } else {
                    player->moveDir = opc[0].y;
                }
            } else if (disEuX < disEuY)
            {
                player->moveDir = opc[0].x;
            } else {
                player->moveDir = opc[0].y;
            }
        }        
    }
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
    int runs = 0;

    tObj apple; //= {(rand()%(cellsNum-1))+1, (rand()%(cellsNum-1))+1}; 
    createApple(player, &apple);
    bool gameReset = false;

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

        AIsnake(&player, apple);

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
           
        runs++;

         //Eat apple
        if (player.pos[0].x == apple.x && player.pos[0].y == apple.y)
        {
            player.size++;
            if (player.size >= cellsNum*cellsNum) //Wingame
            {
                printf("|runs = %d|\n", runs);
                runs = 0;
                
            } else {
                createApple(player, &apple);
            }
        }

        //Colision
        if (player.pos[0].x > cellsNum || player.pos[0].x < 1)
        {
            gameReset = true;
            printf("Game Over!!!x");
            SDL_Delay(100);
        }else if (player.pos[0].y > cellsNum || player.pos[0].y < 1)
        {
            gameReset = true;
            printf("Game Over!!!y");
            SDL_Delay(100);
        }

        for (int i = 1; i < player.size && !gameReset; i++)
        {
            if (player.pos[0].x == player.pos[i].x && player.pos[0].y == player.pos[i].y)
            {
                gameReset = true;
                printf("moveDir: %d\n", player.moveDir);
                printf("Game Over!!!");
                SDL_Delay(100);
            }            
        }

        //Game Over
        if(gameReset){
            resetGame(&player);
            createApple(player, &apple);
            gameReset = false;
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
                        SDL_SetRenderDrawColor(renderer, 0, 255-(255*(k+1)/(player.size)), 255*(k+1)/(player.size), 255);
                    }
                }
                
                SDL_RenderFillRect(renderer, &cell);               // Fill the Cells
                
            }
            
        }  

        // BACKGROUND
        SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255); // Background Collor
        SDL_RenderPresent(renderer);                        // Fill the Screen with background collor
        
        if(player.size >= cellsNum*cellsNum){
            SDL_Delay(1000);
            gameReset = true;
        }   

        //SDL_Delay(50);
    }

    // END OF PROGRAM
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}