// GameOfLife.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
// #include <time.h>

static int size=20;
bool newFieldReady = true;
bool startChangingField = false;

void clear()
{
    COORD topLeft = {0, 0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}

void nextLifeCycle(bool **field)
{
    while (true)
    {
        // *newFieldReady = false;
        bool **nextField = new bool *[size];
        for (int i = 0; i < size; i++)
            nextField[i] = new bool[size];

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                int counter = 0;
                if (field[(i + size + 1) % size][(j + size) % size])
                {
                    counter++;
                }
                if (field[(i + size + 1) % size][(j + size + 1) % size])
                {
                    counter++;
                }
                if (field[(i + size) % size][(j + size + 1) % size])
                {
                    counter++;
                }
                if (field[(i + size - 1) % size][(j + size + 1) % size])
                {
                    counter++;
                }
                if (field[(i + size - 1) % size][(j + size) % size])
                {
                    counter++;
                }
                if (field[(i + size - 1) % size][(j + size - 1) % size])
                {
                    counter++;
                }
                if (field[(i + size) % size][(j + size - 1) % size])
                {
                    counter++;
                }
                if (field[(i + size + 1) % size][(j + size - 1) % size])
                {
                    counter++;
                }

                if (!field[(i + size) % size][(j + size) % size] && counter == 3)
                {
                    nextField[i][j] = true;
                }
                else if (field[(i + size) % size][(j + size) % size] && (counter == 3 || counter == 2))
                {
                    nextField[i][j] = true;
                }
                else
                {
                    nextField[i][j] = false;
                }
            }
        }

        while (!startChangingField)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                field[i][j] = nextField[i][j];
            }
        }

        startChangingField = false;
        newFieldReady = true;

        for (int i = 0; i < size; ++i)
        {
            delete[] nextField[i];
        }
        delete[] nextField;
    }
}

void consoleDraw(bool **field)
{
    while(true){
        while (!newFieldReady)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        clear();
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                std::cout << (field[i][j] ? " #" : " .");
            }
            std::cout << "\n";
        }
        newFieldReady=false;
        startChangingField = true;
    }
}

void graphDraw(bool **field){

}

int main()
{
    // std::cout << "Hello World!\n";
    bool **field = new bool *[size];
    for (int i = 0; i < size; i++)
        field[i] = new bool[size];

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            field[i][j] = false;
        }
    }
    field[0][1] = true;
    field[1][2] = true;
    field[2][0] = true;
    field[2][1] = true;
    field[2][2] = true;

    // consoleDraw(field, sizeXY, &startChangingField, &newFieldReady);
    std::thread draw(consoleDraw, field);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::thread cycleLife(nextLifeCycle,field);
    while (true)
    {
    }

    // nextLifeCycle(field,sizeXY);
    // field[15][950] = true;
    // std::cout << field[15][951] << "\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
