void graphDraw(sf::RenderWindow *window, bool **field)
{
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
                    rectangle.setOutlineColor(borderColor);
                }
                else
                {
                    rectangle.setFillColor(deathCellColor);
                    rectangle.setOutlineColor(borderColor);
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