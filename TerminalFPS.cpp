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

uint8_t nScreenWidth = 120;
uint8_t nScreenHeight = 40;

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
    std::ofstream log("log.txt", std::ios_base::trunc);

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

    initscr(); //инициализируем библиотеку
    noecho(); //Не печатать на экране то, что набирает пользователь на клавиатуре
    curs_set(0); //Убрать курсор
    keypad(stdscr, TRUE); //Активировать специальные клавиши клавиатуры (например, если хотим использовать горячие клавиши)
    use_default_colors(); //Фон stscr будет "прозрачным"
    refresh();

    char* screen = new char[nScreenWidth * nScreenHeight];
    screen[nScreenWidth * nScreenHeight - 1] = '\0';
    WINDOW *win = newwin(nScreenHeight, nScreenWidth, 0, 0);
    nodelay(win, TRUE);
    wrefresh(win);
    refresh();

    int i = 1;
    //Game Loop
    while(i--)
    {
        log << "Loop for x" << '\n';
        for (uint8_t x = 0; x < nScreenWidth; x++)
        {
            float fRayAngle = (fPlayerAngle - fFOV / 2.0f) + (static_cast<float>(x) / static_cast<float>(nScreenWidth)) * fFOV;

            float fDistanceToWall = 0;
            bool bHitWall = false;

            float fEyeX = std::sinf(fRayAngle);
            float fEyeY = std::cosf(fRayAngle);

            log << "BitWall" << '\n';
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
            log << "Distance " << fDistanceToWall << '\n'; 
            log << "Celling " << nCelling << '\n';
            log << "Floor " << nFloor << '\n';

            log << "Y for loop" << '\n';
            for (int32_t y = 0; y < nScreenHeight; y++)
            {
                //TODO
                if (y <= nCelling)
                {
                    screen[y * nScreenWidth + x] = ' ';
                    log << "y <= nCelling " << screen[y * nScreenWidth + x] << '\n';
                }
                else if (y > nCelling && y <= nFloor)
                {
                    screen[y * nScreenWidth + x] = '#';
                    log << "y > nCelling && y <= nFloor " << screen[y * nScreenWidth + x] << '\n';
                }
                else
                {
                    screen[y * nScreenWidth + x] = ' ';
                    log << "y > nFloor " << screen[y * nScreenWidth + x] << '\n';
                }
                
            }

        }
        std::string test = "#####################################################################################################################";
        log << screen << '\n';
        waddstr(win, screen);
        wrefresh(win);
        refresh();
    }
    getch();
                         // Ожидание нажатия какой-либо клавиши пользователем
    delwin(win);
    endwin();                    // Выход из curses-режима. Обязательная команда.
    free(screen);
    return 0;
}
