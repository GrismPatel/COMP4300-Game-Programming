#pragma once
#include <SFML/Graphics.hpp>


struct CTransform {
    sf::Vector2f position = { 0.0, 0.0 };
    sf::Vector2f velocity = { 0.0, 0.0 };
    float angle = 0;
};

struct CShape {
    sf::CircleShape circle;

    CShape(float radius, int points, const sf::Color &fill, const sf::Color &outline, float thickness)
        : circle(radius, points) {
        circle.setFillColor(fill);
        circle.setOutlineColor(outline);
        circle.setOutlineThickness(thickness);
        circle.setOrigin({radius, radius});
    }
};

struct CCollision {
    float radius = 0;
};

struct CScore {
    int score = 0;
};

struct CLifespan {
    int remaining = 0;
    int total = 0;
};

struct CInput {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool shoot = false;
};
