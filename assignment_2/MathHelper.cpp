#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include "Entity.h"


float getRadian(const float &angle) {
    return angle * (M_PI / 180.0f);
};


sf::Vector2f getVelocity(const float &speed, const float &angle) {
    float radian = getRadian(angle);
    return sf::Vector2f {speed*std::cos(radian), speed*std::sin(radian)};
};


float getAngle(const sf::Vector2f &startPosition, const sf::Vector2f &endPosition) {
    float dX = endPosition.x - startPosition.x;
    float dY = endPosition.y - startPosition.y;

    float radian = std::atan2(dY, dX);

    return (180.0f * radian) / M_PI;
};


float getRandomNumberBetweenRange(const float &minValue, const float &maxValue) {
    static std::random_device randomDevice;
    static std::mt19937 gen(randomDevice());

    return std::uniform_real_distribution(minValue, maxValue)(gen);
};


sf::Vector2f getRandomPosition(const float &windowSizeX, const float &windowSizeY, const int &shapeRadius) {

    float positionX = getRandomNumberBetweenRange(0.0f+shapeRadius, windowSizeX-shapeRadius);
    float positionY = getRandomNumberBetweenRange(0.0f+shapeRadius, windowSizeY-shapeRadius);

    return sf::Vector2f{positionX, positionY};
};


bool didCollide(const std::shared_ptr<Entity> entityOne, const std::shared_ptr<Entity> entityTwo) {
    float entityOneRadius = entityOne->cShape->circle.getRadius();
    float entityTwoRadius = entityTwo->cShape->circle.getRadius();

    sf::Vector2f entityOnePosition = entityOne->cTransform->position;
    sf::Vector2f entityTwoPosition = entityTwo->cTransform->position;

    float distance = pow((entityTwoPosition.y - entityOnePosition.y), 2.0) + pow((entityTwoPosition.x - entityOnePosition.x), 2.0);

    return pow(entityOneRadius+entityTwoRadius, 2.0) > distance;
};
