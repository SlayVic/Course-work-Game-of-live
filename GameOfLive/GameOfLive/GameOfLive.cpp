// GameOfLife.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <SFML/Graphics.hpp>
// #include <time.h>

const clock_t simSpeed = 20;

const int size = 35;
const int windowSize = 910;
float cellSize = windowSize / size;

sf::Color deathCellColor = sf::Color(0, 0, 0);
sf::Color lifeCellColor = sf::Color(129, 195, 215);
sf::Color borderColor = sf::Color(76, 131, 153);
const int borderWide = 1;

bool newFieldReady = true;
bool startChangingField = false;
bool setingField = true;

void clearField(bool **field)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            field[i][j] = false;
        }
    }
}

void nextLifeCycle(bool **field, sf::RenderWindow *window)
{
    auto oldTime = clock();
    auto deltaTime = clock() - oldTime;

    bool **nextField = new bool *[size];
    for (int i = 0; i < size; i++)
        nextField[i] = new bool[size];

    while (window->isOpen())
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

        do
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            deltaTime = clock() - oldTime;
        } while (deltaTime < simSpeed);

        oldTime = clock();

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

void graphDraw(sf::RenderWindow *window, bool **field)
{
    static sf::VertexArray quads;
    static sf::VertexBuffer buffer(sf::Quads);

    buffer.create(size * size * 4);
    quads.setPrimitiveType(sf::Quads);
    buffer.setUsage(sf::VertexBuffer::Usage::Stream);
    quads.resize(size * size * 4);
    while (window->isOpen())
    {
        window->clear(borderColor);

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                sf::Vertex *quad = &quads[(i + j * size) * 4];
                quad[0].position = sf::Vector2f(j * cellSize + borderWide, i * cellSize + borderWide);
                quad[1].position = sf::Vector2f((j + 1) * cellSize, i * cellSize + borderWide);
                quad[2].position = sf::Vector2f((j + 1) * cellSize, (i + 1) * cellSize);
                quad[3].position = sf::Vector2f(j * cellSize + borderWide, (i + 1) * cellSize);

                if (field[i][j])
                {
                    quad[0].color = lifeCellColor;
                    quad[1].color = lifeCellColor;
                    quad[2].color = lifeCellColor;
                    quad[3].color = lifeCellColor;
                }
                else
                {
                    quad[0].color = deathCellColor;
                    quad[1].color = deathCellColor;
                    quad[2].color = deathCellColor;
                    quad[3].color = deathCellColor;
                }
            }
        }

        buffer.update(&quads[0]);
        window->draw(buffer);
        newFieldReady = false;
        startChangingField = true;
        window->display();
    }
}

void help()
{
    sf::RenderWindow window(sf::VideoMode(500, 130), "How to play!");
    sf::Font font;
    sf::Text text;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        exit(0);
    }
    text.setFont(font);
    text.setString("When game start it on pause.\nPause changing - Space Bar\nWhen game on pause you can draw cells\n'C' - clear field");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(10, 5));
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear();
        window.draw(text);
        window.display();
    }
}

void windowEvent(sf::RenderWindow &window, bool **field)
{
    bool toSet = true;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // draw.join();
                // cycleLife.join();
                exit(0);
            }
            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                setingField = !setingField;
                if (setingField)
                {
                    borderColor = sf::Color(76, 131, 153);
                }
                else
                {
                    borderColor = sf::Color(40, 40, 40);
                }
            }
            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::C))
            {
                setingField = true;
                deathCellColor = sf::Color(0, 0, 0);
                borderColor = sf::Color(76, 131, 153);
                lifeCellColor = sf::Color(129, 195, 215);
                borderColor = sf::Color(76, 131, 153);
                std::this_thread::sleep_for(std::chrono::milliseconds(25));
                clearField(field);
            }

            if (setingField && event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                toSet = !field[(int)(localPosition.y / cellSize)][(int)(localPosition.x / cellSize)];
            }

            if (setingField && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                if (localPosition.x > 0 && localPosition.x < windowSize)
                    if (localPosition.y > 0 && localPosition.y < windowSize)
                        if (field[(int)(localPosition.y / cellSize)][(int)(localPosition.x / cellSize)] != toSet)
                            field[(int)(localPosition.y / cellSize)][(int)(localPosition.x / cellSize)] = toSet;
            }
        }
    }
}

int main()
{
    bool **field = new bool *[size];
    for (int i = 0; i < size; i++)
        field[i] = new bool[size];

    clearField(field);

    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "GameOfLife!", sf::Style::Titlebar | sf::Style::Close);

    window.setActive(false);
    std::thread helper(*help);
    std::thread draw(*graphDraw, &window, field);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::thread cycleLife(nextLifeCycle, field, &window);

    windowEvent(window, field);
}