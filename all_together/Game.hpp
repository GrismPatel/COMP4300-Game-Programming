#pragma once
#include "SFML/Graphics.hpp"
#include <memory>
#include "Entity.hpp"
#include "EntityManager.hpp"


struct TileConfig {
    int x;
    int y;
};


class Game {
    private:
        sf::RenderWindow         m_window;
        sf::Texture              m_texture;
        Entity*                  m_player;
        std::vector<TileConfig>  m_tileConfigs;
        // Entity*          m_setUp;
        EntityManager            m_entityManager;
        int                      m_gameFrame = 0;

        void setupLayout();
        void spawnPlayer();
    public:
        Game();
        void run();
        ~Game();

        void sUserInput();
        void sAnimation(Entity* entity);
        void sMovement(Entity* entity);
        void sRender();
};
