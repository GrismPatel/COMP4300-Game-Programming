#include "State.hpp"


State::State(sf::RenderWindow& window)
:m_window(window), m_pause(false)
{};


std::unique_ptr<State> State::getNextState() {
    return std::move(m_nextState);
};


bool State::isCurrentStatePaused() {
    return m_pause;
}
