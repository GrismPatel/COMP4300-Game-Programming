#include "Game.hpp"


Game::Game() {
    m_window.create(sf::VideoMode({800, 600}), "All Together");
    // m_player = m_entityManager.createEntity("Player");
};

void Game::run() {
    while (m_window.isOpen()) {
        while (const std::optional event = m_window.pollEvent() ) {
            if (event->is<sf::Event::Closed>()) {
                m_window.close();
            };
        };

        m_window.clear();

        m_window.display();

    };
};

Game::~Game() {};
