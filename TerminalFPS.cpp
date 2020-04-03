/*
Link to origin - https://github.com/OneLoneCoder/CommandLineFPS
*/

#include <iostream>
#include <wchar.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <string>
#include <ncurses.h>
#include <cmath>


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
            case 'w':
                fPlayerX += std::sinf(fPlayerAngle) * 15.0f * fElapsedTime;
                fPlayerY += std::cosf(fPlayerAngle) * 15.0f * fElapsedTime;

                if (map[static_cast<int>(fPlayerY) * nMapWidth + static_cast<int>(fPlayerX)] == '#')
                {
                    fPlayerX -= std::sinf(fPlayerAngle) * 15.0f * fElapsedTime;
                    fPlayerY -= std::cosf(fPlayerAngle) * 15.0f * fElapsedTime; 
                }
                break;
            case 'W':
                fPlayerX += std::sinf(fPlayerAngle) * 15.0f * fElapsedTime;
                fPlayerY += std::cosf(fPlayerAngle) * 15.0f * fElapsedTime;

                if (map[static_cast<int>(fPlayerY) * nMapWidth + static_cast<int>(fPlayerX)] == '#')
                {
                    fPlayerX -= std::sinf(fPlayerAngle) * 15.0f * fElapsedTime;
                    fPlayerY -= std::cosf(fPlayerAngle) * 15.0f * fElapsedTime; 
                }
                break;
            case 'd':
                fPlayerAngle += 50.0f * fElapsedTime;
                break;
            case 'D':
                fPlayerAngle += 50.0f * fElapsedTime;
                break;
            case 's':
                fPlayerX -= std::sinf(fPlayerAngle) * 15.0f * fElapsedTime;
                fPlayerY -= std::cosf(fPlayerAngle) * 15.0f * fElapsedTime;

                if (map[static_cast<int>(fPlayerY) * nMapWidth + static_cast<int>(fPlayerX)] == '#')
                {
                    fPlayerX += std::sinf(fPlayerAngle) * 15.0f * fElapsedTime;
                    fPlayerY += std::cosf(fPlayerAngle) * 15.0f * fElapsedTime; 
                }
                break;
            case 'S':
                fPlayerX -= std::sinf(fPlayerAngle) * 15.0f * fElapsedTime;
                fPlayerY -= std::cosf(fPlayerAngle) * 15.0f * fElapsedTime;

                if (map[static_cast<int>(fPlayerY) * nMapWidth + static_cast<int>(fPlayerX)] == '#')
                {
                    fPlayerX += std::sinf(fPlayerAngle) * 15.0f * fElapsedTime;
                    fPlayerY += std::cosf(fPlayerAngle) * 15.0f * fElapsedTime; 
                }
                break;
            default:
                break;
        }

        //Render image
        for (int x = 0; x < nScreenWidth; x++)
        {
            float fRayAngle = (fPlayerAngle - fFOV / 2.0f) + (static_cast<float>(x) / static_cast<float>(nScreenWidth)) * fFOV;

            float fDistanceToWall = 0;
            bool bHitWall = false;
            bool bBoundary = false;

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

                        std::vector<std::pair<float, float> > p;

                        for (int tx = 0; tx < 2; tx++)
                            for (int ty = 0; ty < 2; ty++)
                            {
                                float vy = static_cast<float>(nTestY) + ty - fPlayerY;
                                float vx = static_cast<float>(nTestX) + tx - fPlayerX;
                                float distance = std::sqrt(vx * vx + vy * vy);
                                float dot = (fEyeX * vx / distance) + (fEyeY * vy / distance);
                                p.push_back(std::make_pair(distance, dot));
                            }

                        std::sort(p.begin(), p.end(), [](const std::pair<float, float> &left, const std::pair<float, float> &right) {return left.first < right.first;});

                        float fBound = 0.01f;
                        if (std::acos(p.at(0).second) < fBound)
                            bBoundary = true;
                        if (std::acos(p.at(1).second) < fBound)
                            bBoundary = true;
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

            if (bBoundary)
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
                    float b = 1.0f - ((static_cast<float>(y) - nScreenHeight / 2.0f) / (static_cast<float>(nScreenHeight) / 2.0f));
                    if (b < 0.25)
                        nShade = '#';
                    else if (b < 0.5)
                        nShade = 'x';
                    else if (b < 0.75)
                        nShade = '.';
                    else if (b < 0.9)
                        nShade = '-';
                    else
                        nShade = ' ';
                    
                    screen[y * nScreenWidth + x] = nShade;
                }
                
            }

        }
        // swprintf(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", fPlayerX, fPlayerY, fPlayerAngle, 1.0f / fElapsedTime);
        for (int nx = 0; nx < nMapWidth; nx++)
			for (int ny = 0; ny < nMapWidth; ny++)
			{
				screen[(ny + 1) * nScreenWidth + nx] = map[ny * nMapWidth + nx];
			}
		screen[(static_cast<int>(fPlayerX + 1)) * nScreenWidth + static_cast<int>(fPlayerY)] = 'P';

        mvaddwstr(0, 0, screen);
        refresh();
    }

    endwin();
    free(screen);
    return 0;
}
