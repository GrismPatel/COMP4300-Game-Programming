#pragma once
#include <SFML/Graphics.hpp>


struct CSprite {
    std::shared_ptr<sf::Texture> texture;
    sf::IntRect                  textureRect;
};

struct CAnimation {
    int    animationPosition;
    size_t totalFramesInAnimation;  /*number of animations in single line */
    size_t totalAnimationsInSprite; /*Unique animations in sprite*/
    int    animationTime;
    int    singleFrameWidth;        /*texture width/totalFramesInAnimation*/
    int    singleFrameHeigth;       /*texture height/totalAnimationsInSprite*/
    int    currentFrame = 0;
    bool   isPlaying = false;
};

struct CTransform {
    sf::Vector2f size;
    sf::Vector2f position;
    sf::Vector2f previousPosition;
    sf::Vector2f velocity = {0.f, 0.f};
    float        angle = 0;
};

struct CLifespan {
    float health;
};
