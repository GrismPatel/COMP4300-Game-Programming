#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <SFML/Graphics.hpp>


// Store pointer of shape, its speed and shape name
// std::unique_ptr cannot be copied so it can only be movied for exlusive ownership
class ShapeEntity {
private:
public:
    std::unique_ptr<sf::Shape> shape;  // we want to store the pointer of the shape (either circle or rectangle).
    std::vector<float> movingSpeed;
    std::unique_ptr<sf::Text> shapeText;

    ShapeEntity(std::unique_ptr<sf::Shape> shape, std::vector<float> movingSpeed, std::unique_ptr<sf::Text> shapeText)
        :shape(std::move(shape)), movingSpeed(movingSpeed), shapeText(std::move(shapeText)) {};
};


int main () {

    // Init the variables
    unsigned int windowWidth;
    unsigned int windowHeight;
    sf::RenderWindow window;
    sf::Font font;

    std::string circleShapeName;
    float initialCirclePositionX;
    float initialCirclePositionY;
    float initialCircleSpeedX;
    float initialCircleSpeedY;
    int circleRed;
    int circleGreen;
    int circleBlue;
    float circleRadius;

    std::string rectangleShapeName;
    float initialRectanglePositionX;
    float initialRectanglePositionY;
    float initialRectangleSpeedX;
    float initialRectangleSpeedY;
    int rectangleRed;
    int rectangleGreen;
    int rectangleBlue;
    float rectangleWidth;
    float rectangleHeight;

    
    std::vector<ShapeEntity> shapeEntities;
    
    // Read the config file.
    std::ifstream file("config.txt");

    std::string word;
    while (file >> word) {
        if (word == "Window") {
            file >> windowWidth >>  windowHeight;
            window.create(sf::VideoMode({windowWidth, windowHeight}), "Assignment 1");
            window.setFramerateLimit(480);
        }

        if (word == "Font") {
            if (!font.openFromFile("Manrope-VariableFont_wght.ttf")) {
                std::cout << "Font does not exists" << std::endl;
                exit(-1);
            };
        }

        if (word == "Circle") {
            file >> circleShapeName >> initialCirclePositionX >> initialCirclePositionY >> initialCircleSpeedX >> initialCircleSpeedY >> circleRed >> circleGreen >> circleBlue >> circleRadius;

            std::unique_ptr<sf::CircleShape> circleShapePtr = std::make_unique<sf::CircleShape>(circleRadius);
            circleShapePtr->setFillColor(sf::Color(circleRed, circleGreen, circleBlue));
            circleShapePtr->setPosition({initialCirclePositionX, initialCirclePositionY});

            std::unique_ptr<sf::Text> textPtr = std::make_unique<sf::Text>(font, circleShapeName, 18);
            const float textPositionX = initialCirclePositionX + (circleShapePtr->getLocalBounds().size.x - textPtr->getLocalBounds().size.x)/2.0f;
            const float textPositionY = initialCirclePositionY + (circleShapePtr->getLocalBounds().size.y - textPtr->getLocalBounds().size.y)/2.0f;
            textPtr->setPosition({textPositionX, textPositionY});

            // since ShapeEntity class is using unique_ptr it will also need to be moved and not copied. Since vector.push_back by default does copies.
            ShapeEntity shapeEntity(std::move(circleShapePtr), {initialCircleSpeedX, initialCircleSpeedY}, std::move(textPtr));
            shapeEntities.push_back(std::move(shapeEntity));
        }

        if (word == "Rectangle") {
            file >> rectangleShapeName >> initialRectanglePositionX >> initialRectanglePositionY >> initialRectangleSpeedX >> initialRectangleSpeedY >> rectangleRed >> rectangleGreen >> rectangleBlue >> rectangleWidth >> rectangleHeight;

            std::unique_ptr<sf::RectangleShape> rectangleShapePtr = std::make_unique<sf::RectangleShape>(sf::Vector2f({rectangleWidth, rectangleHeight}));
            rectangleShapePtr->setFillColor(sf::Color(rectangleRed, rectangleGreen, rectangleBlue));
            rectangleShapePtr->setPosition({initialRectanglePositionX, initialRectanglePositionY});

            std::unique_ptr<sf::Text> textPtr = std::make_unique<sf::Text>(font, rectangleShapeName, 18);
            const float textPositionX = initialRectanglePositionX + (rectangleShapePtr->getLocalBounds().size.x - textPtr->getLocalBounds().size.x)/2.0f;
            const float textPositionY = initialRectanglePositionY + (rectangleShapePtr->getLocalBounds().size.y - textPtr->getLocalBounds().size.y)/2.0f;
            textPtr->setPosition({textPositionX, textPositionY});

            ShapeEntity ShapeEntity(std::move(rectangleShapePtr), {initialRectangleSpeedX, initialRectangleSpeedY}, std::move(textPtr));
            shapeEntities.push_back(std::move(ShapeEntity));
        }
    }
    file.close();

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent() ) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        for (auto &shapeEntity : shapeEntities) {

            sf::FloatRect bounds = shapeEntity.shape->getLocalBounds();
            float shapeWidth = bounds.size.x;
            float shapeHeight = bounds.size.y;

            if (shapeEntity.shape->getPosition().y <= 0 || (shapeEntity.shape->getPosition().y + shapeHeight) >= windowHeight) {
                shapeEntity.movingSpeed[1] *= -1;
            }

            if (shapeEntity.shape->getPosition().x <= 0 || (shapeEntity.shape->getPosition().x + shapeWidth) >= windowWidth) {
                shapeEntity.movingSpeed[0] *= -1;
            }

            shapeEntity.shape->setPosition({
                shapeEntity.shape->getPosition().x + shapeEntity.movingSpeed[0],
                shapeEntity.shape->getPosition().y + shapeEntity.movingSpeed[1],
            });

            shapeEntity.shapeText->setPosition({
                shapeEntity.shapeText->getPosition().x + shapeEntity.movingSpeed[0],
                shapeEntity.shapeText->getPosition().y + shapeEntity.movingSpeed[1],
            });

            window.draw(*shapeEntity.shape);
            window.draw(*shapeEntity.shapeText);
        }

        window.display();
    }

    return 0;
};
