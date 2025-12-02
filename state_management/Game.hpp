#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"
#include "StateMachine.hpp"
#include "MenuState.hpp"
#include <memory>


class Game {
    private:
        sf::RenderWindow m_window;
        StateMachine m_stateMachine = StateMachine(std::make_unique<MenuState>(m_window));
    public:
        Game();
        void run();
};
