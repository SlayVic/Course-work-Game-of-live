#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <chrono>

const clock_t simSpeed = 30; //Min time between turns

const int size = 40;                // size of side of field
const int windowSize = 920;         // window size of side
float cellSize = windowSize / size; // cell square size

sf::Color deathCellColor = sf::Color(0, 0, 0);
sf::Color lifeCellColor = sf::Color(129, 195, 215);
sf::Color borderColor = sf::Color(76, 131, 153);
const int borderWide = 1;

bool startChangingField = false;
bool setingField = true;

bool **nextField = new bool *[size];
bool **oldField = new bool *[size];
int currentTurn = 0;
int maxTurn = 50;

bool stopDraw = false;

void pastTurnUpdate(bool **field);

// Set all cells as dead
void clearField(bool **field)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            field[i][j] = false;
            oldField[i][j] = false;
            currentTurn = 0;
        }
    }
}

void nextLifeCycle(bool **field, bool wait = false, bool old = false)
{
    if (!old)
        pastTurnUpdate(field);

    auto oldTime = clock();             // get time of start
    auto deltaTime = clock() - oldTime; // passed time

    // calculate next turn
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int counter = 0; // count live cells around cell

            /*
                 (n + size + shift) % size
                 math funcion that make infinit move around field from 0 to size [0,size)
                 n - current number
                 size - max acceptable, not included number
                 shift - changing n value

                 when n >= size, it go to start
                 when n < 0, it go to end

                 examples: 
                 n=5, size = 10, shift = 7
                 5+7=12, we dont accept it so
                 12%size = 2, cool

                 n=3, size = 6, shift = -5
                 3+shift(-5)=-2
                 -2+size(6)=4
                 4 % 6 = 4
                */

            // count amount of live cells around
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
            // ------------------------------------------------------

            if (!field[(i + size) % size][(j + size) % size] && counter == 3) // become live if 3 live cells around
            {
                nextField[i][j] = true;
                continue;
            }
            if (field[(i + size) % size][(j + size) % size] && (counter == 3 || counter == 2)) // stay live if 2 or 3 live cells around
            {
                nextField[i][j] = true;
                continue;
            }
            // dead
            nextField[i][j] = false;
        }
    }

    // wait if calculate end to fast, and we want to make simulation slower
    if (wait)
        do
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            deltaTime = clock() - oldTime;
        } while (deltaTime < simSpeed);

    oldTime = clock();
    if (wait)
        while (!startChangingField) // Wait while window drawing old field
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

    for (int i = 0; i < size; i++) // change old field with new
    {
        for (int j = 0; j < size; j++)
        {
            field[i][j] = nextField[i][j];
        }
    }
    currentTurn++;
}

// Next turn thread
void nextLifeCycleThread(bool **field, sf::RenderWindow *window)
{
    while (window->isOpen()) // while open
    {
        nextLifeCycle(field, true);
        while (setingField) // wait if game on pause
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        startChangingField = false; // Say that field ready to draw
    }
}

void pastTurnSetField(bool **field)
{
    if (currentTurn > 0)
    {
        currentTurn = 0;
        for (int i = 0; i < size; i++) // change old field with new
        {
            for (int j = 0; j < size; j++)
            {
                oldField[i][j] = field[i][j];
            }
        }
    }
}

void pastTurnUpdate(bool **field)
{
    if (currentTurn >= maxTurn)
    {
        nextLifeCycle(oldField, false, true);
        currentTurn = maxTurn;
    }
}

void pastTurn(bool **field)
{
    stopDraw = true;
    if (currentTurn > 0)
    {
        for (int i = 0; i < size; i++) // change old field with new
        {
            for (int j = 0; j < size; j++)
            {
                field[i][j] = oldField[i][j];
            }
        }

        int turn = currentTurn - 1;
        currentTurn = 0;
        for (int i = 0; i < turn; i++)
        {
            nextLifeCycle(field);
            currentTurn;
        }
    }
    stopDraw = false;
}

// Draw mein window
void graphDraw(sf::RenderWindow *window, bool **field)
{
    static sf::VertexArray quads;
    static sf::VertexBuffer buffer(sf::Quads);

    buffer.create(size * size * 4);                   // Create vertex buffer with size
    quads.setPrimitiveType(sf::Quads);                // Set shape type
    buffer.setUsage(sf::VertexBuffer::Usage::Stream); // Set buffer settings
    quads.resize(size * size * 4);                    // Set vertex array size
    while (window->isOpen())
    {
        while (stopDraw) // wait if game on pause
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        window->clear(borderColor); // clear windows with borderColor as beckground color

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                sf::Vertex *quad = &quads[(i + j * size) * 4];                                         // take array of one quad
                quad[0].position = sf::Vector2f(j * cellSize + borderWide, i * cellSize + borderWide); // set vertices positions
                quad[1].position = sf::Vector2f((j + 1) * cellSize, i * cellSize + borderWide);        // and protect some
                quad[2].position = sf::Vector2f((j + 1) * cellSize, (i + 1) * cellSize);               // void space
                quad[3].position = sf::Vector2f(j * cellSize + borderWide, (i + 1) * cellSize);        // to make borders

                // Set color by cell state
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

        buffer.update(&quads[0]);  // Update buffer
        window->draw(buffer);      // Draw buffer on window
        startChangingField = true; // Say to calculate next live cycle
        window->display();         // Update display
    }
}

// Helping window
void help()
{
    sf::RenderWindow window(sf::VideoMode(500, 180), "How to play!", sf::Style::Titlebar | sf::Style::Close); // Create helping window
    sf::Font font;
    sf::Text text;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) // load font
    {
        exit(0); // If cant load, crush program
    }
    text.setFont(font);                                                                                                                                                    // set font
    text.setString("When game start it on pause.\n\\
Pause changing - Space Bar\n\\
When game on pause you can draw cells\n\\
'C' - clear field\n\\
Right arrow to make only 1 turn\n\\
Left Arrow will make time rerun"); // Set text
    text.setCharacterSize(24);                                                                                                                                             // set text size
    text.setFillColor(sf::Color::White);                                                                                                                                   // set text color
    text.setPosition(sf::Vector2f(10, 5));                                                                                                                                 // set position, to make some void space around text

    while (window.isOpen()) // While window is open
    {
        sf::Event event;
        while (window.pollEvent(event)) // read event
        {
            if (event.type == sf::Event::Closed) // Is press 'X'
            {
                window.close(); // close window
            }
        }
        window.clear();    // clear window
        window.draw(text); // draw text
        window.display();  // display text
    }
}

void windowEvent(sf::RenderWindow &window, bool **field)
{
    bool toSet = true; // memorizer of cell state to draw
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) // read event
        {
            if (event.type == sf::Event::Closed) // if press 'X'
            {
                exit(0); // close program
            }
            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) // if press space
            {
                setingField = !setingField; // change pause mod(field setting mod)
                if (setingField)
                {
                    borderColor = sf::Color(76, 131, 153); // color of paused borders
                }
                else
                {
                    borderColor = sf::Color(40, 40, 40); // regular border color
                }
            }
            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::C)) // press 'C' - clear
            {
                setingField = true;                                         // change mod to pause(field setting mod)
                borderColor = sf::Color(76, 131, 153);                      // color to pause border color
                std::this_thread::sleep_for(std::chrono::milliseconds(25)); // wait
                clearField(field);                                          // clear field
            }

            if (setingField)
            {
                if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left)) // on LMB click
                {
                    pastTurnSetField(field);
                    sf::Vector2i localPosition = sf::Mouse::getPosition(window);                          // Get position of mouse
                    toSet = !field[(int)(localPosition.y / cellSize)][(int)(localPosition.x / cellSize)]; // memorize not cliced cell state
                }

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) // LBM pressed
                {
                    sf::Vector2i localPosition = sf::Mouse::getPosition(window); // Get position
                    if (localPosition.x > 0 && localPosition.x < windowSize)     // If in window at x coords
                        if (localPosition.y > 0 && localPosition.y < windowSize) // If in window at y coords
                        {
                            field[(int)(localPosition.y / cellSize)][(int)(localPosition.x / cellSize)] = toSet; // Set cell state to memorized state
                            oldField[(int)(localPosition.y / cellSize)][(int)(localPosition.x / cellSize)] = toSet;
                            currentTurn = 0;
                        }
                }

                if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    nextLifeCycle(field, false);
                }
                if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    pastTurn(field);
                }
            }
        }
    }
}

int main()
{
    for (int i = 0; i < size; i++)
        oldField[i] = new bool[size];

    for (int i = 0; i < size; i++)
        nextField[i] = new bool[size];

    bool **field = new bool *[size]; // make array of arrays
    for (int i = 0; i < size; i++)   // place array to all array of arrays objects
        field[i] = new bool[size];

    clearField(field); // make field dead

    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "GameOfLife!", sf::Style::Titlebar | sf::Style::Close); // Make mein window

    window.setActive(false);                                    // Make work with threads possible
    std::thread helper(*help);                                  // Control helping window thread
    std::thread draw(*graphDraw, &window, field);               // Drawind thread
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // wait
    std::thread cycleLife(nextLifeCycleThread, field, &window); // calculate next turn thread

    windowEvent(window, field); // Main window event system
}