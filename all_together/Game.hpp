#pragma once
#include "SFML/Graphics.hpp"
#include <memory>
#include "Entity.hpp"
#include "EntityManager.hpp"


class Game {
    private:
        sf::RenderWindow m_window;
        sf::Texture      m_texture;
        Entity*          m_player;
        EntityManager    m_entityManager;
        int              m_gameFrame = 0;

        void spawnPlayer();
    public:
        Game();
        void run();
        ~Game();

        void sUserInput(Entity* entity);
        void sAnimation(Entity* entity);
        void sMovement(Entity* entity);
        void sRender();
};
