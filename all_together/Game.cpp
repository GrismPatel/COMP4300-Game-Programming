#include "Game.hpp"
#include <fstream>


Game::Game() {
    m_window.create(sf::VideoMode({800, 600}), "All Together");

    std::ifstream fin("tile.txt");
    TileConfig tileConfig;

    while (fin >> tileConfig.x >> tileConfig.y) {
        m_tileConfigs.push_back(tileConfig);
    };

    setupLayout();
    spawnPlayer();
};

void Game::run() {
    while (m_window.isOpen()) {
        m_gameFrame++;
        m_entityManager.update();
        
        sUserInput(m_player);
        sAnimation(m_player);
        sMovement(m_player);
        sRender();
    };
};

Game::~Game() {};


void Game::setupLayout() {
    std::shared_ptr<sf::Texture> tileTexture = std::make_shared<sf::Texture>();
    if (!(tileTexture->loadFromFile("tile.png"))) {
        std::cout << "Cannot load Tile texture." << std::endl;
    };

    for (TileConfig tileConfig: m_tileConfigs) {
        std::cout << tileConfig.x << tileConfig.y << std::endl;
        Entity* tileEntity = m_entityManager.createEntity("Tile");

        tileEntity->cSprite = std::make_unique<CSprite>();
        tileEntity->cSprite->texture = std::make_shared<sf::Texture>();
        tileEntity->cSprite->texture = tileTexture;

        tileEntity->cTransform = std::make_unique<CTransform>();
        tileEntity->cTransform->position = { tileConfig.x * 1.f, tileConfig.y * 1.f };
    };
};


void Game::spawnPlayer() {
    m_player = m_entityManager.createEntity("Player");

    m_player->cSprite = std::make_unique<CSprite>();
    m_player->cSprite->texture = std::make_shared<sf::Texture>();
    if (!(m_player->cSprite->texture->loadFromFile("player.png"))) {
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
    m_player->cTransform->position = {400.f, 400.f};
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
    entity->cTransform->position.x += entity->cTransform->velocity.x;
    entity->cTransform->position.y += entity->cTransform->velocity.y;
    entity->cTransform->velocity = {0.f, 0.f};
};

void Game::sRender() {
    sf::Color backgroundColor(194, 178, 128);
    m_window.clear(backgroundColor);
    const EntityVector& entities = m_entityManager.getEntities();
    for (const std::unique_ptr<Entity>& entity: entities) {
        if (entity->cSprite != nullptr) {
            sf::Sprite sprite(*(entity->cSprite->texture));
            if (entity->cSprite->textureRect != sf::IntRect()) {
                sprite.setTextureRect(entity->cSprite->textureRect);
            };
            sprite.setPosition({
                entity->cTransform->position.x,
                entity->cTransform->position.y
            });
            m_window.draw(sprite);
        };
    };
    m_window.display();
};
