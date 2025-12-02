#include <iostream>
#include "Game.hpp"


Game::Game() {
    m_window.create(sf::VideoMode({800, 600}), "Understanding State");
};


void Game::run() {

    while (m_window.isOpen()) {
        /* update */
        m_stateMachine.update();
        if (!m_stateMachine.isPaused()) {
            /*movement(setPosition) should be seperated out from update*/
            m_stateMachine.changeState();
        };
        /*user input should be seperated out from update*/

        /* render */
        m_stateMachine.draw();
    };
};
