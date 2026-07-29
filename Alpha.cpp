#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

const int ROWS = 64;
const int COLS = 64;
const int blockSize = 16;
const int spacing = 16;
const float cellSize = 10.f;

float lerp(float a, float b, float t)
    {
        return a + (b-a)*t;
    }

void generateTerrain(float heightMap[ROWS][COLS])
{
    const int CONTROL_ROWS = ROWS / blockSize + 1;
    const int CONTROL_COLS = COLS / blockSize + 1;

    float control[CONTROL_ROWS][CONTROL_COLS];

    for (int y = 0; y < CONTROL_ROWS; y++)
    {
        for (int x = 0; x < CONTROL_COLS; x++)
        {
            control[y][x] = static_cast<float>(rand()) / RAND_MAX;
        }
    }

    for (int row = 0; row < ROWS; row += blockSize)
    {
        for (int col = 0; col < COLS; col += blockSize)
        {
        int cy = row / blockSize;
        int cx = col / blockSize;

            for(int y=0;y<spacing;y++)
            {
            for(int x=0;x<spacing;x++)
                {
                float tx = x / float(blockSize-1);
                float ty = y / float(blockSize-1);

                float A = control[cy][cx];
                float B = control[cy][cx + 1];
                float C = control[cy + 1][cx];
                float D = control[cy + 1][cx + 1];

                float top = lerp(A,B,tx);
                float bottom = lerp(C,D,tx);
                float value = lerp(top,bottom,ty);

                heightMap[row+y][col+x] = value;
                }
            }
         }    
    }
}

int main()
{
    srand(time(nullptr));
    float heightMap[ROWS][COLS];
    generateTerrain(heightMap);

    sf::RenderWindow window(
        sf::VideoMode({COLS * cellSize, ROWS * cellSize}),
        "Grid2"
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

    if (const auto* key = event->getIf<sf::Event::KeyPressed>())
    {
        if (key->code == sf::Keyboard::Key::Space)
        {
            generateTerrain(heightMap);
        }
    }
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
                
            //float height = grid[row][col] / 256.f;
              // int green = height * 255;
cell.setFillColor(sf::Color(
    static_cast<std::uint8_t>(heightMap[row][col] * 255.f),
    static_cast<std::uint8_t>(heightMap[row][col] * 255.f),
    static_cast<std::uint8_t>(heightMap[row][col] * 255.f)
));
                
                window.draw(cell);
            }
        }

        window.display();
    }
}