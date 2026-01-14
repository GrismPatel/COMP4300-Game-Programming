#include "Game.hpp"


Game::Game() {
    m_window.create(sf::VideoMode({800, 600}), "All Together");

    spawnPlayer();
};

void Game::run() {
    while (m_window.isOpen()) {
        m_gameFrame++;
        m_entityManager.update();
        /*
        Other system
        2. Physics
        4. Collision
        */
        sUserInput(m_player);
        sAnimation(m_player);
        sMovement(m_player);

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


void Game::spawnPlayer() {
    m_player = m_entityManager.createEntity("Player");

    m_player->cSprite = std::make_unique<CSprite>();
    m_player->cSprite->texture = std::make_unique<sf::Texture>();
    if (!(m_player->cSprite->texture->loadFromFile("Sprite.png"))) {
        std::cout << "Cannot load texture" << std::endl;
    };

    m_player->cAnimation = std::make_unique<CAnimation>();
    m_player->cAnimation->animationPosition = 0;
    m_player->cAnimation->totalFramesInAnimation = 26;
    m_player->cAnimation->totalAnimationsInSprite = 4;
    m_player->cAnimation->animationTime = 10;
    m_player->cAnimation->singleFrameWidth = m_player->cSprite->texture->getSize().x/26;
    m_player->cAnimation->singleFrameHeigth = m_player->cSprite->texture->getSize().y/4;

    m_player->cTransform = std::make_unique<CTransform>();
};

void Game::sUserInput(Entity* entity) {
    while (const std::optional event = m_window.pollEvent() ) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        };

        if (event->is<sf::Event::KeyPressed>()) {
            switch (event->getIf<sf::Event::KeyPressed>()->code) {
                case sf::Keyboard::Key::W: {
                    std::cout << "W key is pressed \n";
                    m_player->cTransform->velocity = {0.0, -5.0f};
                    m_player->cAnimation->animationPosition = 1;
                    m_player->cAnimation->isPlaying = true;
                    break;
                };
                case sf::Keyboard::Key::A: {
                    std::cout << "A key is pressed \n";
                    m_player->cTransform->velocity = {-5.0, 0.0f};
                    m_player->cAnimation->animationPosition = 3;
                    m_player->cAnimation->isPlaying = true;
                    break;
                };
                case sf::Keyboard::Key::S: {
                    std::cout << "S key is pressed \n";
                    m_player->cTransform->velocity = {0.0, 5.0f};
                    m_player->cAnimation->animationPosition = 2;
                    m_player->cAnimation->isPlaying = true;
                    break;
                };
                case sf::Keyboard::Key::D: {
                    std::cout << "D key is pressed \n";
                    m_player->cTransform->velocity = {5.0, 0.0f};
                    m_player->cAnimation->animationPosition = 0;
                    m_player->cAnimation->isPlaying = true;
                    break;
                };
                default: { break; }
            };
        };

        if (event->is<sf::Event::KeyReleased>()) {
            switch (event->getIf<sf::Event::KeyReleased>()->code) {
                case sf::Keyboard::Key::W: {
                    std::cout << "W key is pressed \n";
                    m_player->cAnimation->isPlaying = false;
                    break;
                };
                case sf::Keyboard::Key::A: {
                    std::cout << "A key is pressed \n";
                    m_player->cAnimation->isPlaying = false;
                    break;
                };
                case sf::Keyboard::Key::S: {
                    std::cout << "S key is pressed \n";
                    m_player->cAnimation->isPlaying = false;
                    break;
                };
                case sf::Keyboard::Key::D: {
                    std::cout << "D key is pressed \n";
                    m_player->cAnimation->isPlaying = false;
                    break;
                };
                default: { break; }
            };
        };
    };
};

void Game::sAnimation(Entity* entity) {
    if (entity->cAnimation->isPlaying) {
        m_player->cAnimation->currentFrame = (
            m_gameFrame/m_player->cAnimation->animationTime
        ) % m_player->cAnimation->totalFramesInAnimation;
    };
    entity->cSprite->textureRect = {
        {
            entity->cAnimation->singleFrameWidth*m_player->cAnimation->currentFrame,
            entity->cAnimation->singleFrameHeigth*entity->cAnimation->animationPosition
        },
        {
            entity->cAnimation->singleFrameWidth,
            entity->cAnimation->singleFrameHeigth
        }
    };
};

void Game::sMovement(Entity* entity) {

};
