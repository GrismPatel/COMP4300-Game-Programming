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

        
        sUserInput();
        sAnimation();
        sCollision();
        sMovement();
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
        tileEntity->cTransform->size = { 
            tileTexture->getSize().x  * 1.f,  tileTexture->getSize().y  * 1.f
        };
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
    m_player->cTransform->size = {
        m_player->cAnimation->singleFrameWidth*1.f,
        m_player->cAnimation->singleFrameHeigth*1.f
    };
};

void Game::sUserInput() {
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

void Game::sAnimation() {
    const EntityVector& entities = m_entityManager.getEntities();

    for (const std::unique_ptr<Entity>& entity: entities) {
        if (entity->cAnimation != nullptr && entity->cSprite != nullptr) {
            if (entity->cAnimation->isPlaying) {
                entity->cAnimation->currentFrame = (
                    m_gameFrame/entity->cAnimation->animationTime
                ) % entity->cAnimation->totalFramesInAnimation;
            };
            entity->cSprite->textureRect = {
                {
                    entity->cAnimation->singleFrameWidth*entity->cAnimation->currentFrame,
                    entity->cAnimation->singleFrameHeigth*entity->cAnimation->animationPosition
                },
                {
                    entity->cAnimation->singleFrameWidth,
                    entity->cAnimation->singleFrameHeigth
                }
            };
        };
    };
};

void Game::sMovement() {
    const EntityVector& entities = m_entityManager.getEntities();

    for (const std::unique_ptr<Entity>& entity: entities) {
        entity->cTransform->position.x += entity->cTransform->velocity.x;
        entity->cTransform->position.y += entity->cTransform->velocity.y;
        entity->cTransform->velocity = {0.f, 0.f};
    };
};

void Game::sCollision() {
    const EntityPtrVector& playersEntities = m_entityManager.getEntitiesByTag("Player");
    const EntityPtrVector& tilesEntities = m_entityManager.getEntitiesByTag("Tile");

    for ( Entity* playerEntity: playersEntities ) {
        for (Entity* tileEntity: tilesEntities) {
            const sf::Vector2f currentOverlap = getOverlap(
                {
                    playerEntity->cTransform->position.x + (playerEntity->cTransform->size.x/2.f),
                    playerEntity->cTransform->position.y + (playerEntity->cTransform->size.y/2.f)
                },
                playerEntity->cTransform->size,
                {
                    tileEntity->cTransform->position.x + (tileEntity->cTransform->size.x/2.f),
                    tileEntity->cTransform->position.y + (tileEntity->cTransform->size.y/2.f),
                },
                tileEntity->cTransform->size
            );
            if (currentOverlap.x > 0 and currentOverlap.y > 0) {
                std::cout << "playerEntity->cTransform->position: " << playerEntity->cTransform->position.x << " " << playerEntity->cTransform->position.y << std::endl;
                std::cout << "playerEntity->size: " << playerEntity->cAnimation->singleFrameWidth << " " << playerEntity->cAnimation->singleFrameHeigth*1.f << std::endl;
                std::cout << "tileEntity->cTransform->position: " << tileEntity->cTransform->position.x << " " << tileEntity->cTransform->position.y << std::endl;
                std::cout << "tileEntity->size: " << tileEntity->cSprite->texture->getSize().x << " " << tileEntity->cSprite->texture->getSize().y << std::endl;
                std::cout << "currentOverlap: " << currentOverlap.x << " " << currentOverlap.y << std::endl;
            };
            /*
            2. if condition then previous overlap.
            3. identified collision so if top, bottom, left, right we change velocity
            */
        };
    };
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

const sf::Vector2f Game::getOverlap(
    const sf::Vector2f& entityOnePosition,
    const sf::Vector2f& entityOneSize,
    const sf::Vector2f& entityTwoPosition,
    const sf::Vector2f& entityTwoSize
) {
    sf::Vector2f delta = {
        std::abs(entityOnePosition.x - entityTwoPosition.x),
        std::abs(entityOnePosition.y - entityTwoPosition.y)
    };

    return {
        (entityOneSize.x*0.5f) + (entityTwoSize.x*0.5f) - delta.x,
        (entityOneSize.y*0.5f) + (entityTwoSize.y*0.5f) - delta.y
    };
};
