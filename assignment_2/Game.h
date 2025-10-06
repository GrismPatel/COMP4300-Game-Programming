#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Entity.h"
#include "EntityManager.h"

struct WindowConfig{
    unsigned int windowWidth;
    unsigned int windowHeight;
    unsigned int frame;
    int anything;
};
struct FontConfig{
    std::string fontType;
    int fontSize;
    int fontRedColor;
    int fontGreenColor;
    int fontBlueColor;
};
struct PlayerConfig{
    float shapeRadius;
    int collisionRadius;
    float speed;
    int fillRed;
    int fillGreen;
    int fillBlue;
    int outlineRed;
    int outlineGreen;
    int outlineBlue;
    float outlineThickness;
    int shapeVertices;
};
struct EnemyConfig{
    int shapeRadius;
    int collisionRadius;
    float minSpeed;
    float maxSpeed;
    int outlineRed;
    int outlineGreen;
    int outlineBlue;
    float outlineThickness;
    int minVertices;
    int maxVertices;
    int smallLifespan;
    int spawnInterval;
};
struct BulletConfig{
    int shapeRadius;
    int collisionRadius;
    float speed;
    int fillRed;
    int fillGreen;
    int fillBlue;
    int outlineRed;
    int outlineGreen;
    int outlineBlue;
    int outlineThickness;
    int shapeVertices;
    int lifespan;
};


class Game {
    private:
        sf::RenderWindow m_window;
        EntityManager m_entityManager;
        sf::Font m_font;
        std::shared_ptr<sf::Text> m_text;

        WindowConfig m_windowConfig;
        FontConfig m_fontConfig;
        PlayerConfig m_playerConfig;
        BulletConfig m_bulletConfig;
        EnemyConfig m_enemyConfig;

        int m_score = 0;
        int m_zealot_timer = 600;
        int m_currentFrame = 0;
        int m_lastEnemySpawnTime = 0;
        bool m_paused = false;
        bool m_running = true;

        std::shared_ptr<Entity> m_player;

        // void init() initialize game with config path. private method which reads the config file and sets all config values.
        void setPaused(bool paused);

        // all systems with business logic.
        void sMovement();
        void sUserInput();
        void sLifeSpan(std::shared_ptr<Entity> entity);
        void sRender();
        void sEnemySpawner();
        void sCollision();

        void spawnScore();
        void spawnPlayer();
        void spawnEnemy();
        void spawnSmallEnemies(std::shared_ptr<Entity> entity);  // why spawn bullet and enemies have to pass entity pointer?
        void spawnBullet(std::shared_ptr<Entity> entity, const sf::Vector2f spawnPosition);  // why spawn bullet and enemies have to pass entity pointer?
        void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
    public:
        Game(const std::string & configPath);
        void run();
};
