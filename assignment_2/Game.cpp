#include <iostream>
#include <string>
#include "Game.h"

#include "MathHelper.cpp"
#include <fstream>


void Game::spawnPlayer() {
    m_player = m_entityManager.addEntity("player");

    m_player->cTransform = std::make_shared<CTransform>(
        CTransform{{m_windowConfig.windowWidth/2.0f, m_windowConfig.windowHeight/2.0f}, {0.0f, 0.0f}, 0.0f}
    );

    m_player->cShape = std::make_shared<CShape>(
        m_playerConfig.shapeRadius,
        m_playerConfig.shapeVertices,
        sf::Color(m_playerConfig.fillRed, m_playerConfig.fillGreen, m_playerConfig.fillBlue),
        sf::Color(m_playerConfig.outlineRed, m_playerConfig.outlineGreen, m_playerConfig.outlineBlue),
        m_playerConfig.outlineThickness
    );

    m_player->cInput = std::make_shared<CInput>();
};


void Game::spawnEnemy() {
    std::shared_ptr<Entity> enemyEntity = m_entityManager.addEntity("enemy");

    sf::Vector2f enemyPosition = getRandomPosition(m_windowConfig.windowWidth, m_windowConfig.windowHeight, m_enemyConfig.shapeRadius);
    float speed = getRandomNumberBetweenRange(m_enemyConfig.minSpeed, m_enemyConfig.maxSpeed);
    float enemyDegree = getRandomNumberBetweenRange(0, 360);

    sf::Vector2f enemyVelocity = getVelocity(speed, enemyDegree);

    enemyEntity->cTransform = std::make_shared<CTransform>(
        CTransform{enemyPosition, enemyVelocity, 0.0f}
    );

    int enemyVertices = (int)getRandomNumberBetweenRange(
        m_enemyConfig.minVertices,
        m_enemyConfig.maxVertices
    );
    int red = (int)getRandomNumberBetweenRange(0, 255);
    int blue = (int)getRandomNumberBetweenRange(0, 255);
    int green = (int)getRandomNumberBetweenRange(0, 255);

    enemyEntity->cShape = std::make_shared<CShape>(
        m_enemyConfig.shapeRadius,
        enemyVertices,
        sf::Color(red, blue, green),
        sf::Color(m_enemyConfig.outlineRed, m_enemyConfig.outlineGreen, m_enemyConfig.outlineBlue),
        m_enemyConfig.outlineThickness
    );
    enemyEntity->cScore = std::make_shared<CScore>(
        CScore{enemyVertices*100}
    );
    m_lastEnemySpawnTime = m_currentFrame;
};


void Game::spawnSmallEnemies(std::shared_ptr<Entity> enemyEntity) {
    std::size_t numberOfSides = enemyEntity->cShape->circle.getPointCount();
    sf::Vector2f enemyPosition = enemyEntity->cTransform->position;
    float baseAngle = 360.0f/numberOfSides;
    float entityAngle = 0.0f;

    sf::Color enemyFillColor = enemyEntity->cShape->circle.getFillColor();

    for (int eachSide = 1; eachSide < numberOfSides +1; eachSide++) {
        std::shared_ptr<Entity> smallEnemy = m_entityManager.addEntity("small_enemy");

        float speed = getRandomNumberBetweenRange(m_enemyConfig.minSpeed, m_enemyConfig.maxSpeed);
        entityAngle += baseAngle;

        sf::Vector2f smallEnemyVelocity = getVelocity(speed, entityAngle);

        smallEnemy->cTransform = std::make_shared<CTransform>(
            CTransform{enemyPosition, smallEnemyVelocity, 0.0f}
        );

        smallEnemy->cShape = std::make_shared<CShape>(
            m_enemyConfig.shapeRadius,
            numberOfSides,
            enemyFillColor,
            sf::Color(m_enemyConfig.outlineRed, m_enemyConfig.outlineGreen, m_enemyConfig.outlineBlue),
            m_enemyConfig.outlineThickness
        );

        smallEnemy->cLifespan = std::make_shared<CLifespan>(
            CLifespan{m_enemyConfig.smallLifespan, m_enemyConfig.smallLifespan}
        );
    };
};


void Game::spawnBullet(std::shared_ptr<Entity> entity, sf::Vector2f target) {
    std::shared_ptr<Entity> bulletEntity = m_entityManager.addEntity("bullet");

    float bulletDegree = getAngle(entity->cTransform->position, target);
    sf::Vector2f bulletVelocity = getVelocity(m_bulletConfig.speed, bulletDegree);

    bulletEntity->cTransform = std::make_shared<CTransform>(
        CTransform{entity->cTransform->position, bulletVelocity, 0.0}
    );
    bulletEntity->cShape = std::make_shared<CShape>(
        m_bulletConfig.shapeRadius,
        20,
        sf::Color(m_bulletConfig.fillRed, m_bulletConfig.fillGreen, m_bulletConfig.fillBlue),
        sf::Color(m_bulletConfig.outlineRed, m_bulletConfig.outlineGreen, m_bulletConfig.outlineBlue),
        m_bulletConfig.outlineThickness
    );
    bulletEntity->cLifespan = std::make_shared<CLifespan>(
        CLifespan{m_bulletConfig.lifespan, m_bulletConfig.lifespan}
    );
}


void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity) {
    for (std::shared_ptr<Entity> enemyEntity : m_entityManager.getEntitiesByTag("enemy")) {
        sf::Vector2f enemyPosition = enemyEntity->cTransform->position;

        sf::Vector2f entityPosition = entity->cTransform->position;

        float distance = pow((enemyPosition.y - entityPosition.y), 2.0) + pow((enemyPosition.x - entityPosition.x), 2.0);
        std::cout << "what is the distance: " << distance << std::endl;

        if (distance <= pow(300.f, 2)) {
            std::cout << "goes in? " << std::endl;
            enemyEntity->destroy();
            spawnSmallEnemies(enemyEntity);
            m_score += enemyEntity->cScore->score;
        };
    };
};


void Game::sEnemySpawner() {
    if (m_currentFrame - m_lastEnemySpawnTime >= 100) {
        spawnEnemy();
    }
};


void Game::sLifeSpan(std::shared_ptr<Entity> entity) {
    if (entity->cLifespan != nullptr) {
        entity->cLifespan->remaining -= 1;

        if (entity->cLifespan->remaining == 0) {
            entity->destroy();
        }
        
        float remainingFrame = float(entity->cLifespan->remaining);
        float totalFrame = float(entity->cLifespan->total);
        sf::Color entityColor = entity->cShape->circle.getFillColor();
        entityColor.a = ((remainingFrame/totalFrame)  * 255);
        entity->cShape->circle.setFillColor(entityColor);
    }
};


void Game::sRender() {

    m_text = std::make_shared<sf::Text>(m_font, "Score: "+std::to_string(m_score));
    sf::Text zealot_timer(m_font, "Zealot Timer: "+std::to_string(m_zealot_timer/60));
    zealot_timer.setPosition({
        m_windowConfig.windowWidth - zealot_timer.getLocalBounds().size.x,
        zealot_timer.getPosition().y,
    });
    m_window.clear(sf::Color::Black);

    for (std::shared_ptr<Entity> entity: m_entityManager.getEntities()) {

        sf::FloatRect bounds = entity->cShape->circle.getLocalBounds();
        float shapeWidth = bounds.size.x;
        float shapeHeight = bounds.size.y;
        if (entity->getTag() != "player" && entity->getTag() != "small_enemy") {
            if (entity->cTransform->position.y <= 0 || (entity->cTransform->position.y + shapeHeight) >= m_window.getSize().y) {
                entity->cTransform->velocity.y *= -1;
            };

            if (entity->cTransform->position.x <= 0 || (entity->cTransform->position.x + shapeWidth) >= m_window.getSize().x) {
                entity->cTransform->velocity.x *= -1;
            };
        } else if (entity->getTag() == "player") {
            // if player goes out of bound, spawn again at starting position.
            if (
                entity->cTransform->position.y + shapeHeight <= 0
                || entity->cTransform->position.y >= m_window.getSize().y
                || entity->cTransform->position.x + shapeWidth <= 0
                || entity->cTransform->position.x >= m_window.getSize().x
            ) {
                entity->destroy();
                spawnPlayer();
            };
        };

        entity->cTransform->angle += 1.0f;
        entity->cShape->circle.setRotation(
            sf::degrees(entity->cTransform->angle)
        );

        m_window.draw(entity->cShape->circle);
    };
    m_window.draw(*m_text);
    m_window.draw(zealot_timer);

    m_window.display();
};


void Game::sUserInput() {
    while (const std::optional event = m_window.pollEvent() ) {
        // event triggers when window is closed.
        if (event->is<sf::Event::Closed>()) {
            m_running = false;
            m_window.close();
        }

        // event triggers when key is pressed.
        if (event->is<sf::Event::KeyPressed>()) {
            switch (event->getIf<sf::Event::KeyPressed>()->code) {
                case sf::Keyboard::Key::W:
                    std::cout << "W key is pressed \n";
                    m_player->cInput->up = true;
                    break;
                case sf::Keyboard::Key::S:
                    std::cout << "S key is pressed \n";
                    m_player->cInput->down = true;
                    break;
                case sf::Keyboard::Key::A:
                    std::cout << "A key is pressed \n";
                    m_player->cInput->left = true;
                    break;
                case sf::Keyboard::Key::D:
                    std::cout << "D key is pressed \n";
                    m_player->cInput->right = true;
                    break;
                case sf::Keyboard::Key::P:
                    std::cout << "P Key is pressed \n";
                    m_paused = true;
                    break;
                case sf::Keyboard::Key::Escape:
                    std::cout << "Esc Key is pressed \n";
                    m_running = false;
                    m_window.close();
                    break;
                default: break;
            }
        }

        // event triggers when key is released.
        if (event->is<sf::Event::KeyReleased>()) {
            switch (event->getIf<sf::Event::KeyReleased>()->code) {
                case sf::Keyboard::Key::W:
                    std::cout << "W key is released \n";
                    m_player->cInput->up = false;
                    break;
                case sf::Keyboard::Key::S:
                    std::cout << "S key is released.\n";
                    m_player->cInput->down = false;
                    break;
                case sf::Keyboard::Key::A:
                    std::cout << "A key is released. \n";
                    m_player->cInput->left = false;
                    break;
                case sf::Keyboard::Key::D:
                    std::cout << "D key is released. \n";
                    m_player->cInput->right = false;
                    break;
                default: break;
            }
        }

        // when mouse is clicked.
        if (event->is<sf::Event::MouseButtonPressed>()) {
            sf::Vector2f mouseClickPosition(event->getIf<sf::Event::MouseButtonPressed>()->position);  // converting vi to vf
            switch (event->getIf<sf::Event::MouseButtonPressed>()->button) {
                case sf::Mouse::Button::Left:
                    std::cout << "Left Button was Pressed: " << mouseClickPosition.x << mouseClickPosition.y << "\n";\
                    spawnBullet(m_player, mouseClickPosition);
                    break;
                case sf::Mouse::Button::Right:
                    std::cout << "Right Button was Pressed: " << mouseClickPosition.x << mouseClickPosition.y << "\n";
                    if (m_zealot_timer == 0) {
                        spawnSpecialWeapon(m_player);
                        m_zealot_timer = 600;
                    };
                    break;
                default: break;
            }
        }
    };
};


void Game::sMovement() {
    m_player->cTransform->velocity = {0.0f, 0.0f};

    if ( m_player->cInput->up == true ) {
        m_player->cTransform->velocity.y -= m_playerConfig.speed;
    }

    if ( m_player->cInput->down == true ) {
        m_player->cTransform->velocity.y += m_playerConfig.speed;
    }

    if ( m_player->cInput->left == true ) {
        m_player->cTransform->velocity.x -= m_playerConfig.speed;
    }

    if ( m_player->cInput->right == true ) {
        m_player->cTransform->velocity.x += m_playerConfig.speed;
    }

    for (std::shared_ptr<Entity> entity: m_entityManager.getEntities()) {
        sLifeSpan(entity);

        // change entities movement and angle
        entity->cTransform->position.x += entity->cTransform->velocity.x;
        entity->cTransform->position.y += entity->cTransform->velocity.y;
        entity->cShape->circle.setPosition({
            entity->cTransform->position.x,
            entity->cTransform->position.y
        });
    }
};


void Game::sCollision() {
    EntityVector bulletEntities = m_entityManager.getEntitiesByTag("bullet");
    EntityVector enemiesEntities = m_entityManager.getEntitiesByTag("enemy");

    for (std::shared_ptr<Entity> bulletEntity: bulletEntities) {
        for (std::shared_ptr<Entity> enemyEntity: enemiesEntities) {
            if (didCollide(bulletEntity, enemyEntity)) {
                bulletEntity->destroy();
                enemyEntity->destroy();
                spawnSmallEnemies(enemyEntity);
                m_score += enemyEntity->cScore->score;
            };
        };
    };
};


Game::Game(const std::string& configPath) {
    
    std::ifstream fin(configPath);
    std::string word;

    while (fin >> word) {
        if (word == "Window") {
            fin >> m_windowConfig.windowWidth >> m_windowConfig.windowHeight >> m_windowConfig.frame >> m_windowConfig.anything;
        }

        if (word == "Font") {
            fin >> m_fontConfig.fontType >> m_fontConfig.fontSize >> m_fontConfig.fontRedColor >> m_fontConfig.fontGreenColor >> m_fontConfig.fontBlueColor;
        }

        if (word == "Player") {
            fin >> m_playerConfig.shapeRadius >> m_playerConfig.collisionRadius >> m_playerConfig.speed >> m_playerConfig.fillRed >> m_playerConfig.fillGreen >> m_playerConfig.fillBlue
            >> m_playerConfig.outlineRed >> m_playerConfig.outlineGreen >> m_playerConfig.outlineBlue >> m_playerConfig.outlineThickness >> m_playerConfig.shapeVertices;
        }

        if (word == "Enemy") {
            fin >> m_enemyConfig.shapeRadius >> m_enemyConfig.collisionRadius >> m_enemyConfig.minSpeed >> m_enemyConfig.maxSpeed >> m_enemyConfig.outlineRed >> m_enemyConfig.outlineGreen
            >> m_enemyConfig.outlineBlue >> m_enemyConfig.outlineThickness >> m_enemyConfig.minVertices >> m_enemyConfig.maxVertices >> m_enemyConfig.smallLifespan >> m_enemyConfig.spawnInterval;
        }

        if (word == "Bullet") {
            fin >> m_bulletConfig.shapeRadius >> m_bulletConfig.collisionRadius >> m_bulletConfig.speed >> m_bulletConfig.fillRed >> m_bulletConfig.fillGreen >> m_bulletConfig.fillBlue
            >> m_bulletConfig.outlineRed >> m_bulletConfig.outlineGreen >> m_bulletConfig.outlineBlue >> m_bulletConfig.outlineThickness >> m_bulletConfig.shapeVertices >> m_bulletConfig.lifespan;
        }
    };

    m_window.create(sf::VideoMode({
        m_windowConfig.windowWidth, m_windowConfig.windowHeight
    }), "Assignment 2");
    m_window.setFramerateLimit(m_windowConfig.frame);
    
    if (!m_font.openFromFile("Manrope-VariableFont_wght.ttf")) {
        std::cout << "Font does not exists" << std::endl;
        exit(-1);
    }

    spawnPlayer();
};


void Game::run() {
    while (m_window.isOpen()) {

        m_entityManager.update();

        if (!m_paused) {
            sEnemySpawner();
            sMovement();
            sCollision();
            
            m_currentFrame++;
            if (m_zealot_timer != 0) {
                m_zealot_timer--;
            }
        }
        sUserInput();
        sRender();
    }
};
