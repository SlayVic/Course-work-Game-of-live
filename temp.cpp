while (window->isOpen())
{
   while (window->isOpen() && setingField)
   {
      sf::Event event;
      while (window->pollEvent(event))
      {
            if (event.type == sf::Event::Closed)
               window->close();

            if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
               sf::Vector2i localPosition = sf::Mouse::getPosition(*window);
               // std::cout << "x: " << localPosition.x << "\ty: " << localPosition.y << "\n";
               field[(int)(localPosition.y / cellSize)][(int)(localPosition.x / cellSize)] = !field[(int)(localPosition.y / cellSize)][(int)(localPosition.x / cellSize)];
               sf::RectangleShape rectangle(sf::Vector2f(cellSize, cellSize));
               rectangle.setOutlineThickness(1);
               rectangle.setPosition(cellSize * (int)(localPosition.x / cellSize), cellSize * (int)(localPosition.y / cellSize));

               if (field[(int)(localPosition.y / cellSize)][(int)(localPosition.x / cellSize)])
               {
                  rectangle.setFillColor(lifeCellColor);
                  rectangle.setOutlineColor(lifeCellBorderColor);
               }
               else
               {
                  rectangle.setFillColor(deathCellColor);
                  rectangle.setOutlineColor(deathCellBorderColor);
               }

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
               window->display();
            }
      }
   }

   while (window->isOpen() && !setingField)
   {
      sf::Event event;
      while (window->pollEvent(event))
      {
            if (event.type == sf::Event::Closed)
               window->close();

            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
               setingField = true;
            }
      }

      // while (!newFieldReady)
      // {
      //     std::this_thread::sleep_for(std::chrono::milliseconds(5));
      // }

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
      window->display();
      newFieldReady = false;
      startChangingField = true;
   }
}