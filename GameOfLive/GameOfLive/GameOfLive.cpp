// GameOfLife.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <windows.h>
// #include <time.h>

const int size = 30;
const int windowSize = 900;
float cellSize = windowSize / size;

sf::Color deathCellColor = sf::Color(0, 0, 0);
sf::Color deathCellBorderColor = sf::Color(76, 131, 153);
sf::Color lifeCellColor = sf::Color(129, 195, 215);
sf::Color lifeCellBorderColor = sf::Color(76, 131, 153);

bool newFieldReady = true;
bool startChangingField = false;
bool setingField = true;

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

void nextLifeCycle(bool **field, sf::RenderWindow *window)
{
    bool **nextField = new bool *[size];
    for (int i = 0; i < size; i++)
        nextField[i] = new bool[size];

    while (window -> isOpen())
    {
        start:
        while (setingField)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                int counter = 0;
                if (field[(i + size + 1) % size][(j + size) % size])
                    counter++;
                if (field[(i + size + 1) % size][(j + size + 1) % size])
                    counter++;
                if (field[(i + size) % size][(j + size + 1) % size])
                    counter++;
                if (field[(i + size - 1) % size][(j + size + 1) % size])
                    counter++;
                if (field[(i + size - 1) % size][(j + size) % size])
                    counter++;
                if (field[(i + size - 1) % size][(j + size - 1) % size])
                    counter++;
                if (field[(i + size) % size][(j + size - 1) % size])
                    counter++;
                if (field[(i + size + 1) % size][(j + size - 1) % size])
                    counter++;
                
                if (!field[(i + size) % size][(j + size) % size] && counter == 3)
                    nextField[i][j] = true;
                else if (field[(i + size) % size][(j + size) % size] && (counter == 3 || counter == 2))
                    nextField[i][j] = true;
                else
                    nextField[i][j] = false;
            }
        }

        if (!setingField)
        {
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
        }
        else
        {
            goto start;
        }
        startChangingField = false;
        newFieldReady = true;
    }
    return;
}

void consoleDraw(bool field[size][size])
{
    while (true)
    {
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
        newFieldReady = false;
        startChangingField = true;
    }
}

void graphDraw(sf::RenderWindow *window, bool **field)
{
    // window->setActive(true);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            sf::RectangleShape rectangle(sf::Vector2f(cellSize, cellSize));
            rectangle.setOutlineThickness(1);
            rectangle.setPosition(cellSize * j, cellSize * i);

            if (field[i][j])
            {
                rectangle.setFillColor(lifeCellColor);
                rectangle.setOutlineColor(lifeCellBorderColor);
            }
            else
            {
                rectangle.setFillColor(deathCellColor);
                rectangle.setOutlineColor(deathCellBorderColor);
            }

            window->draw(rectangle);
        }
    }

    while (window->isOpen())
    {
        window->clear();
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                sf::RectangleShape rectangle(sf::Vector2f(cellSize, cellSize));
                rectangle.setOutlineThickness(1);
                rectangle.setPosition(cellSize * j, cellSize * i);

                if (field[i][j])
                {
                    rectangle.setFillColor(lifeCellColor);
                    rectangle.setOutlineColor(lifeCellBorderColor);
                }
                else
                {
                    rectangle.setFillColor(deathCellColor);
                    rectangle.setOutlineColor(deathCellBorderColor);
                }

                window->draw(rectangle);
            }
        }
        newFieldReady = false;
        startChangingField = true;
        window->display();
    }
    return;
}

void help(){
    sf::RenderWindow window(sf::VideoMode(500, 110), "How to play!");
    sf::Font font;
    sf::Text text;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        exit(0);
    }
    text.setFont(font);
    text.setString("When game start it on pause.\nPause changing - Space Bar\nWhen game on pause you can draw cells");
    text.setCharacterSize(24); 
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(10, 5));
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }
        window.clear();
        window.draw(text);
        window.display();
    }
}

int main()
{
    bool **field = new bool *[size];
    for (int i = 0; i < size; i++)
        field[i] = new bool[size];

    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "GameOfLife!", sf::Style::Titlebar | sf::Style::Close);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            field[i][j] = false;
        }
    }

    window.setActive(false);
    std::thread helper(*help);
    std::thread draw(*graphDraw, &window, field);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::thread cycleLife(nextLifeCycle, field, &window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                // draw.join();
                // cycleLife.join();
                exit(0);
            }
            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                setingField = !setingField;
                if (setingField)
                {
                    deathCellColor = sf::Color(0, 0, 0);
                    deathCellBorderColor = sf::Color(76, 131, 153);
                    lifeCellColor = sf::Color(129, 195, 215);
                    lifeCellBorderColor = sf::Color(76, 131, 153);
                }
                else
                {
                    deathCellColor = sf::Color(0, 0, 0);
                    deathCellBorderColor = sf::Color(40, 40, 40);
                    lifeCellColor = sf::Color(129, 195, 215);
                    lifeCellBorderColor = sf::Color(76, 131, 153);
                }
            }
            if (setingField && event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                field[(int)(localPosition.y / cellSize)][(int)(localPosition.x / cellSize)] = !field[(int)(localPosition.y / cellSize)][(int)(localPosition.x / cellSize)];
            }
        }
    }
}