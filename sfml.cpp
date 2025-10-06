#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

int main() {

    // create window
    const int width = 800;
    const int height = 600;
    sf::RenderWindow window(sf::VideoMode({width, height}), "SFML works!");

    // window.setFramerateLimit(60);
    
    
    // create circle shape
    sf::CircleShape shape(50);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition({0.0f, 0.0f});
    float shapeMoveSpeed = 0.01f;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // event->getIf<sf::Event::KeyPressed>() returns a pointer
            if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                std::cout << "key pressed: " << static_cast<int>(keyEvent->code) << std::endl;
            }
        }

        window.clear();

        shape.setPosition({
            shape.getPosition().x + shapeMoveSpeed,
            shape.getPosition().y + 0
        });
        sf::Vector2f center = shape.getOrigin();
        std::cout << "Geometric center: " << center.x << ", " << center.y << "\n";
        window.draw(shape);
        window.display();
    }
}



