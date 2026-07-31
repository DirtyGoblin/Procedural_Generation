#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

const int WIDTH = 800;
const int HEIGHT = 600;

void drawThickLine(sf::RenderWindow& window,
                   sf::Vector2f p1,
                   sf::Vector2f p2,
                   float thickness,
                   sf::Color color)
{
    sf::Vector2f diff = p2 - p1;

    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    sf::RectangleShape line({length, thickness});
    line.setFillColor(color);

    line.setPosition(p1);

    float angle = std::atan2(diff.y, diff.x) * 180.f / 3.14159265f;
    line.setRotation(sf::degrees(angle));

    // Center the rectangle on the line
    line.setOrigin({0.f, thickness / 2.f});

    window.draw(line);
}

float fade(float t)
{
    // Change this function to experiment!

    //return t;                              // Linear

   // return t * t;                       // Quadratic

   // return t * t * t;                   // Cubic

    
//return t * t * (3 - 2 * t);         // Smoothstep

  //return t*t*t*(t*(t*6-15)+10);       // Perlin Fade

    return 0.5f - 0.5f*cos(t*3.1415926f);// Sinusoidal
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Fade Function");

std::vector<sf::Vector2f> graphPoints;

for (int i = 0; i <= WIDTH; i++)
{
    float x = i / float(WIDTH);
    float y = fade(x);

    graphPoints.push_back({
        x * WIDTH,
        HEIGHT - y * HEIGHT
    });
}

    // Axes
    sf::VertexArray axes(sf::PrimitiveType::Lines, 4);

    // X-axis
    axes[0].position = {0, HEIGHT};
    axes[1].position = {WIDTH, HEIGHT};

    // Y-axis
    axes[2].position = {0, 0};
    axes[3].position = {0, HEIGHT};

    for (int i = 0; i < 4; i++)
        axes[i].color = sf::Color::White;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);

        window.draw(axes);
        const float thickness = 5.f;

for (std::size_t i = 0; i < graphPoints.size() - 1; i++)
{
    drawThickLine(
        window,
        graphPoints[i],
        graphPoints[i + 1],
        thickness,
        sf::Color::Green
    );
}

        window.display();
    }
}