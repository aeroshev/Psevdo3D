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
#include <cstring>

uint8_t ScreenWidth = 120;
uint8_t ScreenHeight = 40;

int main()
{
    std::string message = "Just a string";
    initscr(); //инициализируем библиотеку
    cbreak();  //Не использовать буфер для функции getch()
    raw();
    nonl();
    noecho(); //Не печатать на экране то, что набирает пользователь на клавиатуре
    curs_set(0); //Убрать курсор
    keypad(stdscr, TRUE); //Активировать специальные клавиши клавиатуры (например, если хотим использовать горячие клавиши)
    if (has_colors() == FALSE) //На практике столкнулся с линуксом, на котором не было поддержки цвета. 
    {
        endwin();
        puts("\nYour terminal does not support color");
        return 1;
    }
    start_color(); //Активируем поддержку цвета
    use_default_colors(); //Фон stscr будет "прозрачным"
    WINDOW *my_window = newwin(ScreenHeight, ScreenWidth, 0, 0);                   // Переход в curses-режим
    mvwprintw(my_window, ScreenHeight / 2, (ScreenWidth - message.length()) / 2, "%s", message.c_str());
    refresh();                   // Вывод приветствия на настоящий экран
    getch();                     // Ожидание нажатия какой-либо клавиши пользователем
    endwin();                    // Выход из curses-режима. Обязательная команда.
    return 0;
}
