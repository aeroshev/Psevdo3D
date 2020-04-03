/*
Link to origin - https://github.com/OneLoneCoder/CommandLineFPS
*/

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <string>
#include <ncurses.h>
#include <cmath>
#include <cstring>
#include <fstream>

uint8_t nScreenWidth = 178;
uint8_t nScreenHeight = 52;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerAngle = 0.0f;
//Field of view
float fFOV = 3.1459 / 4.0;
float fDepth = 16.0f;

uint8_t nMapHeight = 16;
uint8_t nMapWidth = 16;

int main()
{
    std::ofstream log("log.txt", std::ios_base::ate);

    std::string map;
    map += "#########.......";
    map += "#...............";
    map += "#.......########";
    map += "#..............#";
    map += "#......##......#";
    map += "#......##......#";
    map += "#..............#";
    map += "###............#";
    map += "##.............#";
    map += "#......####..###";
    map += "#......#.......#";
    map += "#......#.......#";
    map += "#..............#";
    map += "#......#########";
    map += "#..............#";
    map += "################";

    // setlocale(LC_ALL, "");
    initscr();
    noecho(); 
    curs_set(0); 
    nodelay(stdscr, TRUE);
    cbreak();
    refresh();

    char* screen = new char[nScreenWidth * nScreenHeight];
    screen[nScreenWidth * nScreenHeight - 1] = '\0';
    
    int terminalWidth;
    int terminalHeight;
    getmaxyx(stdscr, terminalHeight, terminalWidth);
    log << terminalHeight << ' ' << terminalWidth << '\n';

    //Game Loop
    while(1)
    {
        int symbol = getch();
        switch (symbol)
        {
            case 'a':
                fPlayerAngle -= 0.1f;
                break;
            // case 'w':
            //     break;
            case 'd':
                fPlayerAngle += 0.1f;
                break;
            // case 's':
            //     break;
            default:
                break;
        }

        for (uint8_t x = 0; x < nScreenWidth; x++)
        {
            float fRayAngle = (fPlayerAngle - fFOV / 2.0f) + (static_cast<float>(x) / static_cast<float>(nScreenWidth)) * fFOV;

            float fDistanceToWall = 0;
            bool bHitWall = false;

            float fEyeX = std::sinf(fRayAngle);
            float fEyeY = std::cosf(fRayAngle);

            while(!bHitWall && fDistanceToWall < fDepth)
            {
                fDistanceToWall += 0.1f;

                int32_t nTestX = static_cast<int32_t>(fPlayerX + fEyeX * fDistanceToWall);
                int32_t nTestY = static_cast<int32_t>(fPlayerY + fEyeY * fDistanceToWall);

                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
                {
                    bHitWall = true;
                    fDistanceToWall = fDepth;
                }
                else
                {
                    if (map[nTestY * nMapWidth + nTestX] == '#')
                    {
                        bHitWall = true;
                    }
                } 
            }

            int32_t nCelling = static_cast<float>(nScreenHeight / 2.0) - nScreenHeight / (static_cast<float>(fDistanceToWall));
            int32_t nFloor = nScreenHeight - nCelling;

            for (int32_t y = 0; y < nScreenHeight; y++)
            {
                if (y <= nCelling)
                {
                    screen[y * nScreenWidth + x] = ' ';
                }
                else if (y > nCelling && y <= nFloor)
                {
                    screen[y * nScreenWidth + x] = '#';
                }
                else
                {
                    screen[y * nScreenWidth + x] = ' ';
                }
                
            }

        }
        mvaddstr(0, 0, screen);
        refresh();
    }

    endwin();
    free(screen);
    return 0;
}
