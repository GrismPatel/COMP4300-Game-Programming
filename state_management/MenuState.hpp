#pragma once

#include "State.hpp"
#include <vector>


class MenuState : public State {
    private:
        sf::Font m_font;
        sf::Text* text1;
        std::vector<std::unique_ptr<sf::Text>> m_texts;
    public:
        MenuState(sf::RenderWindow &window);
        void update() override;
        void render() override;

        ~MenuState();
};
