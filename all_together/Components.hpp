#include <SFML/Graphics.hpp>


struct CSprite {
    std::unique_ptr<sf::Texture> texture;
    sf::IntRect                  textureRect;
};

struct CAnimation {
    std::string animationName;
    size_t      totalFramesInAnimation;
};

struct CTransform {
    sf::Vector2f position;
    sf::Vector2f velocity = {0.f, 0.f};
    float        angle = 0;
};

struct CLifespan {
    float health;
};
