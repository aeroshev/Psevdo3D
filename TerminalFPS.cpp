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

int nScreenWidth = 178;
int nScreenHeight = 52;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerAngle = 0.0f;
//Field of view
float fFOV = 3.1459 / 4.0;
float fDepth = 16.0f;

int nMapHeight = 16;
int nMapWidth = 16;

using chclock = std::chrono::system_clock;

int main()
{
    // std::ofstream log("log.txt", std::ios_base::ate);

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

    setlocale(LC_ALL, "");
    initscr();
    noecho(); 
    curs_set(0); 
    nodelay(stdscr, TRUE);
    cbreak();
    refresh();

    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    screen[nScreenWidth * nScreenHeight - 1] = '\0';

    getmaxyx(stdscr, nScreenHeight, nScreenWidth);

    auto tp1 = chclock::now();
    auto tp2 = chclock::now();

    //Game Loop
    while(1)
    {
        tp2 = chclock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

        //Control
        int symbol = getch();
        switch (symbol)
        {
            case 'a':
                fPlayerAngle -= 50.0f * fElapsedTime;
                break;
            case 'A':
                fPlayerAngle -= 50.0f * fElapsedTime;
                break;
            // case 'w':
            //     break;
            case 'd':
                fPlayerAngle += 50.0f * fElapsedTime;
                break;
            case 'D':
                fPlayerAngle += 50.0f * fElapsedTime;
                break;
            // case 's':
            //     break;
            default:
                break;
        }

        //Render image
        for (int x = 0; x < nScreenWidth; x++)
        {
            float fRayAngle = (fPlayerAngle - fFOV / 2.0f) + (static_cast<float>(x) / static_cast<float>(nScreenWidth)) * fFOV;

            float fDistanceToWall = 0;
            bool bHitWall = false;

            float fEyeX = std::sinf(fRayAngle);
            float fEyeY = std::cosf(fRayAngle);

            while(!bHitWall && fDistanceToWall < fDepth)
            {
                fDistanceToWall += 0.1f;

                int nTestX = static_cast<int>(fPlayerX + fEyeX * fDistanceToWall);
                int nTestY = static_cast<int>(fPlayerY + fEyeY * fDistanceToWall);

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

            int nCelling = static_cast<float>(nScreenHeight / 2.0) - nScreenHeight / (static_cast<float>(fDistanceToWall));
            int nFloor = nScreenHeight - nCelling;

            short nShade = ' ';
			if (fDistanceToWall <= fDepth / 4.0f)			
                nShade = 0x2588;
			else if (fDistanceToWall < fDepth / 3.0f)		
                nShade = 0x2593;
			else if (fDistanceToWall < fDepth / 2.0f)		
                nShade = 0x2592;
			else if (fDistanceToWall < fDepth)				
                nShade = 0x2591;
			else
                nShade = ' ';

            for (int y = 0; y < nScreenHeight; y++)
            {
                if (y <= nCelling)
                {
                    screen[y * nScreenWidth + x] = ' ';
                }
                else if (y > nCelling && y <= nFloor)
                {
                    screen[y * nScreenWidth + x] = static_cast<wchar_t>(nShade);
                }
                else
                {
                    screen[y * nScreenWidth + x] = ' ';
                }
                
            }

        }
        mvaddwstr(0, 0, screen);
        refresh();
    }

    endwin();
    free(screen);
    return 0;
}
