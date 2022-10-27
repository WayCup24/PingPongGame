#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "termSymbolNgn.h"          

#define playerTexture '@'

Matrix scene;
Point player, playersVector;
int key, score;
bool isMovingUp;

int randomVector()
{
        int result = 0;
        while(result == 0)
        {
                result = rand() % 3 - 1;
        }
        return result;
}

void playerMove()
{
        pointSum(&player, playersVector);
        if(player.y <= 3 || player.y >= 26)
        {
                if((player.y <= 3 && !isMovingUp) ||
                                (player.y >= 26 && isMovingUp)) 
                        return;
                isMovingUp = !isMovingUp;
                playersVector.y = -playersVector.y;
        }
}

void setPlayer()
{
        Point temp;
        temp.x = player.x;
        temp.y = player.y - 2;
        for(int i = 0; i < 5; i++)
        {
                setPoint(&scene, temp, playerTexture);
                temp.y++;
        }
}

bool traceBall(Point ball)
{
        Point temp;
        temp.x = player.x;
        temp.y = player.y - 2;
        for(int i = 0; i < 5; i++)
        {
                if(ball.y == temp.y && ball.x == temp.x) 
                        return true;
                temp.y++;
        }
        return false;
}

void start()
{
        score = 0;
        srand(time(NULL));
        Point ball;
        ball.x = 30;
        ball.y = 15;
        player.x = 57;
        player.y = 15;
        playersVector.x = 0;
        playersVector.y = -1;
        isMovingUp = true;

        Point ballsVector;
        ballsVector.x = randomVector();
        ballsVector.y = randomVector();

        while(1)
        {
                refreshMatrix(&scene);
                setPoint(&scene, ball, playerTexture);
                setPlayer();
                printMatrix(scene);

                usleep(100000);

                if(kbhit())
                {
                        key = getch();
                        if(key == KEY_UP)
                        {
                                playersVector.y = -playersVector.y;
                                isMovingUp = !isMovingUp;
                        }
                }

                playerMove();
                pointSum(&ball, ballsVector);
                bool inPlayer = traceBall(ball);
                if(ball.x <= 0 || ball.x >= 59 || inPlayer)
                {
                        if(inPlayer)
                        { 
                                ball.x--;
                                score++;
                        }
                        else if(ball.x <= 0) ball.x++;
                        else if(ball.x >= 59)
                        {
                                showGameOver(score);
                                break;
                        }
                        ballsVector.x = -ballsVector.x;
                }
                if(ball.y <= 0 || ball.y >= 29)
                {
                        if(ball.y <= 0) ball.y++;
                        else if(ball.y >= 29) ball.y--;
                        ballsVector.y = -ballsVector.y;
                }
        }
}

int main()
{
        initscr();
        keypad(stdscr, true);
        int selected;
        while(selected != 2)
        {
                selected = showMenu();
                if(selected == 1) start();
        }

        endwin();
        return 0;
}
