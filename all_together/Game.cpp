#include "Game.hpp"


Game::Game() {
    m_window.create(sf::VideoMode({800, 600}), "All Together");

    m_player = m_entityManager.createEntity("Player");

    m_player->cSprite = std::make_unique<CSprite>();
    m_player->cSprite->texture = std::make_unique<sf::Texture>();
    if (!(m_player->cSprite->texture->loadFromFile("Sprite.png"))) {
        std::cout << "Cannot load texture" << std::endl;
    };

    m_player->cAnimation = std::make_unique<CAnimation>();
    m_player->cAnimation->animationPosition = 3;
    m_player->cAnimation->totalFramesInAnimation = 27;
    m_player->cAnimation->totalAnimationsInSprite = 4;
    m_player->cAnimation->animationTime = 1;
    m_player->cAnimation->singleFrameWidth = m_player->cSprite->texture->getSize().x/27;
    m_player->cAnimation->singleFrameHeigth = m_player->cSprite->texture->getSize().y/4;
};

void Game::run() {
    while (m_window.isOpen()) {
        /*
        Other system
        1. input
        2. Physics
        3. Animation (should update Csprite component)
        4. Collision
        */
        sAnimation(m_player);
        
        while (const std::optional event = m_window.pollEvent() ) {
            if (event->is<sf::Event::Closed>()) {
                m_window.close();
            };
        };

        /*
        Render system
        The draw happens in render system. and sprite is created in render system
        using Csprite.
        */
        sf::Sprite sprite(*(m_player->cSprite->texture), m_player->cSprite->textureRect);
        m_window.clear();
        m_window.draw(sprite);
        m_window.display();

    };
};

Game::~Game() {};

void Game::sAnimation(Entity* entity) {
    m_gameFrame++;
    int currentFrame = (m_gameFrame/entity->cAnimation->animationTime) % entity->cAnimation->totalFramesInAnimation;
    entity->cSprite->textureRect = {
        {entity->cAnimation->singleFrameWidth*currentFrame, entity->cAnimation->singleFrameHeigth*entity->cAnimation->animationPosition},
        {entity->cAnimation->singleFrameWidth, entity->cAnimation->singleFrameHeigth}
    };
};
