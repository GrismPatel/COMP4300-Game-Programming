#pragma once
#include "SFML/Graphics.hpp"
#include <memory>
#include "Entity.hpp"
#include "EntityManager.hpp"


class Game {
    private:
        sf::RenderWindow m_window;
        Entity*          m_player;
        EntityManager    m_entityManager;
    public:
        Game();
        void run();
        ~Game();
};
