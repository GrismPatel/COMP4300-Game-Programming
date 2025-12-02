#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"
#include <memory>


class State {
    protected:
        sf::RenderWindow&      m_window;
        // State*&                 m_nextState;
        std::unique_ptr<State> m_nextState;
        bool                   m_pause;
    public:
        State(sf::RenderWindow& window);
        virtual void update() = 0;
        virtual void render() = 0;

        std::unique_ptr<State> getNextState();
        bool isCurrentStatePaused();
        virtual ~State()      = default;
};
