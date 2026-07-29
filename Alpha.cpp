#include <SFML/Graphics.hpp>
#include <cstdlib>

int main()
{
    const int ROWS = 64;
    const int COLS = 64;
    const int cellSize = 10;

    int grid[ROWS][COLS];

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            grid[row][col] = rand() % 2;
        }
    }

    sf::RenderWindow window(
        sf::VideoMode({COLS * cellSize, ROWS * cellSize}),
        "Grid"
    );

    sf::RectangleShape cell({
        cellSize - 1.f,
        cellSize - 1.f
    });

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();

        for (int row = 0; row < ROWS; row++)
        {
            for (int col = 0; col < COLS; col++)
            {
                cell.setPosition({
                    col * cellSize,
                    row * cellSize
                });

                if (grid[row][col] == 0)
                    cell.setFillColor(sf::Color::White);
                else
                    cell.setFillColor(sf::Color::Green);

                window.draw(cell);
            }
        }

        window.display();
    }
}