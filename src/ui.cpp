#include "common.h"
#include "ui.h"
#include <iostream>
#include <string>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>

const int Y = 4;
const int X = 2;
int termWidth, termHeight;

int selection = 0;

using namespace ui;


std::string fitText(std::string str)
{
    size_t maxlen = termWidth - 3 - X * 2;
    if (str.length() > maxlen)
        str = str.substr(0, maxlen);
    for (int i = 0; str.length() < maxlen; i++)
        str = str + " ";

    return str;
}

void printEntry(int index)
{
    std::cout << "\033[" << (Y + index) <<"B\033[" << X << "C";

    std::string title = fitText(entries[index].title);

    if (index == selection)
        std::cout << "\e[7m → " << title << "\e[27m";
    else
        std::cout << "   " << title;

    std::cout << "\033[" << termWidth - X <<"D\033[" << (Y + index) << "A";

}

void updateSelection(int i)
{
    if (i < 0)
        i = entries.size() - 1;
    if (i >= (int)entries.size())
        i = 0;
    int oldSelection = selection;
    selection = i;
    printEntry(oldSelection);
    printEntry(selection);
}

void ui::initTerminal()
{
    winsize termSize;
    termios termOptions;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &termSize);
    tcgetattr(STDIN_FILENO, &termOptions);
    termOptions.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &termOptions);

    termWidth = termSize.ws_col;
    termHeight = termSize.ws_row;
}



void ui::mainLoop()
{
    for (size_t i = 0; i < entries.size(); i++)
        printEntry(i);

    while (true)
    {
        switch (getchar())
        {
        case '\n':
            entries[selection].run();
            break;
        case '\e':
            if (getchar() == '[')
            {
                switch (getchar())
                {
                case 'A':
                    updateSelection(selection - 1);
                    break;
                case 'B':
                    updateSelection(selection + 1);
                    break;
                case '2':
                    switch (getchar())
                    {
                    case '6':
                        if (getchar() == '~')
                            updateSelection(selection + 1);
                        break;
                    }
                }
            }
            break;
        case '-':
            updateSelection(selection - 1);
            break;
        case '+':
            updateSelection(selection + 1);
            break;
        }
    }
}
