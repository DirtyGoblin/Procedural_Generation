#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>


const int ROWS = 64;
const int COLS = 64;
const int blockSize = 16;
const int spacing = 16;
const float cellSize = 10.f;

const int CONTROL_ROWS = ROWS / blockSize + 1;
const int CONTROL_COLS = COLS / blockSize + 1;

float frequency = 1.0f;
float amplitude = 1.0f;
int seed = 0;

struct Gradient
{
    float x;
    float y;
};

Gradient control[CONTROL_ROWS][CONTROL_COLS];

Gradient gradient(int x, int y) {
    int hash = x * 73856093 + y * 19349663 + seed * 83492791;
    hash ^= hash >> 13;
    hash *= 1274126177;
    hash ^= hash >> 16;
    int direction = hash % 8;
    direction = (direction + 8) % 8;

    switch (direction)
    {
        case 0: return {1.0f, 0.0f};
        case 1: return {0.7071f, 0.7071f};
        case 2: return {0.0f, 1.0f};
        case 3: return {-0.7071f, 0.7071f};
        case 4: return {-1.0f, 0.0f};
        case 5: return {-0.7071f, -0.7071f};
        case 6: return {0.0f, -1.0f};
        case 7: return {0.7071f, -0.7071f};
        default: return {1.0f, 0.0f};
    }
}

float dot(Gradient g, float dx, float dy)
{
    return g.x * dx + g.y * dy;
}

float fade(float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float a, float b, float t)
    {
        return a + (b-a)*t;
    }

float perlin(float x, float y)
    {
        int cx = std::floor(x);
        int cy = std::floor(y);
        float tx = x - cx;
        float ty = y - cy;

        float A = dot(gradient(cx, cy), tx, ty);
        float B = dot(gradient(cx + 1, cy), tx - 1.0f, ty);
        float C = dot(gradient(cx, cy + 1), tx, ty - 1.0f);
        float D = dot(gradient(cx + 1, cy + 1), tx - 1.0f, ty - 1.0f);

        float top = lerp(A, B, fade(tx));
        float bottom = lerp(C, D, fade(tx));

       // std::cout << cx << " " << cy << std::endl;
        return lerp(top, bottom, fade(ty));
    }

/*void generateGradients()
{
        for (int y = 0; y < CONTROL_ROWS; y++)
    {
        for (int x = 0; x < CONTROL_COLS; x++)
        {
            int direction = rand() % 8;
            switch(direction)
        {
            case 0:
             control[y][x].x = 1.0f;
             control[y][x].y = 0.0f;
            break;

            case 1:
                control[y][x].x = 0.7071f;
                control[y][x].y = 0.7071f;
            break;

            case 2:
                control[y][x].x = 0.0f;
                control[y][x].y = 1.0f;
            break;

            case 3:
                control[y][x].x = -0.7071f;
                control[y][x].y = 0.7071f;
            break;

            case 4:
                control[y][x].x = -1.0f;
                control[y][x].y = 0.0f; 
              break;

            case 5:
                control[y][x].x = -0.7071f;
                control[y][x].y = -0.7071f;
            break;

            case 6:
                control[y][x].x = 0.0f;
                control[y][x].y = -1.0f;
            break;

            case 7:
                control[y][x].x = 0.7071f;
                control[y][x].y = -0.7071f;
            break;
        }
        }
    } 
}
*/
void generateTerrain(float heightMap[ROWS][COLS])
{
    /*for (int x = 0; x < COLS; x++)
    {
        for (int y = 0; y < ROWS; y++)
        {
            heightMap[y][x] = static_cast<float>(rand()) / RAND_MAX;
        }
    }*/

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {

                {

                float sampleX = (col) / float(blockSize);
                float sampleY = (row) / float(blockSize);

                float value = perlin(sampleX * frequency, sampleY * frequency);
                value = (value + 1.0f) / 2.0f;
                heightMap[row][col] = value * amplitude;

                
                }
            
         }    
    }
}

int main()
{
    srand(time(nullptr));
    float heightMap[ROWS][COLS];

    sf::Font font("Consola.ttf");

    sf::Text info(font);

    info.setCharacterSize(20);
    info.setFillColor(sf::Color::White);
    info.setPosition({10.f, 10.f});

    //generateGradients();
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
        if (key->code == sf::Keyboard::Key::Z)
        {
            frequency = 1.0f;
            amplitude = 1.0f;
            //generateGradients();
            generateTerrain(heightMap);
        }
        if (key->code == sf::Keyboard::Key::X)
        {
            seed = rand();
            //generateGradients();
            generateTerrain(heightMap);
        }
        if (key->code == sf::Keyboard::Key::Q)
        {
            frequency += 0.1f;
            //generateGradients();
            generateTerrain(heightMap);
        }
        if (key->code == sf::Keyboard::Key::A)
        {
            frequency -= 0.1f;
            if (frequency < 0.1f) frequency = 0.1f;
            //generateGradients();
            generateTerrain(heightMap);
        }
        if (key->code == sf::Keyboard::Key::W)
        {
            amplitude += 0.1f;
            //generateGradients();
            generateTerrain(heightMap);
        }
        if (key->code == sf::Keyboard::Key::S)
        {
            amplitude -= 0.1f;
            if (amplitude < 0.1f) amplitude = 0.1f;
            //generateGradients();
            generateTerrain(heightMap);
        }
        if (key->code == sf::Keyboard::Key::Space)
        {
            //generateGradients();
            generateTerrain(heightMap);
        }
    }
}

info.setString(
    "Frequency: " + std::to_string(frequency) +
    "\nAmplitude: " + std::to_string(amplitude) +
    "\nSeed: " + std::to_string(seed)
);

        window.clear();

        for (int row = 0; row < ROWS; row++)
        {
            for (int col = 0; col < COLS; col++)
            {
                cell.setPosition({
                    col * cellSize,
                    row * cellSize
                });
                
            
cell.setFillColor(sf::Color(
    static_cast<std::uint8_t>(heightMap[row][col] * 255.f),
    static_cast<std::uint8_t>(heightMap[row][col] * 255.f),
    static_cast<std::uint8_t>(heightMap[row][col] * 255.f)
));
                
                window.draw(cell);
            }
        }
        window.draw(info);
        window.display();
    }
}