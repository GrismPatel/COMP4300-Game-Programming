#include <iostream>
#include "MenuState.hpp"
#include "AnimationState.hpp"
#include "PhysicsState.hpp"
#include "RayCollisionState.hpp"
#include <memory>


MenuState::MenuState(sf::RenderWindow &window): State(window) {
    if (!m_font.openFromFile("Manrope-VariableFont_wght.ttf")) {
        std::cout << "Error loading font" << std::endl;
    };
    std::vector<std::string> menuStrings = {
        "Menu State",
        "Animation State",
        "Physics State",
        "RayColliion State"
    };

    float positionY = 0.0f;
    for (std::string menuString: menuStrings) {
        std::unique_ptr<sf::Text> text1 = std::make_unique<sf::Text>(m_font);
        text1->setString(menuString);
        text1->setPosition({
            text1->getPosition().x,
            text1->getPosition().y + positionY
        });
        m_texts.push_back(std::move(text1));
        positionY += 40.0f;
    };
};


void MenuState::update() {
    while ( const std::optional event = m_window.pollEvent() ) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        };

        if (event->is<sf::Event::KeyPressed>()) {
            switch (event->getIf<sf::Event::KeyPressed>()->code) {
                case sf::Keyboard::Key::A: {
                    std::cout << "A Key is pressed" << std::endl;
                    m_nextState = std::make_unique<AnimationState>(m_window);
                    // change m_nextState to Animation State
                    break;
                }
                case sf::Keyboard::Key::P: {
                    std::cout << "P Key is pressed" << std::endl;
                    m_nextState = std::make_unique<PhysicsState>(m_window);
                    // change m_nextState to Physics State
                    break;
                }
                case sf::Keyboard::Key::R: {
                    std::cout << "R Key is pressed" << std::endl;
                    m_nextState = std::make_unique<RayCollisionState>(m_window);
                    break;
                }
                default: { break ;}
            }
        };
    };
};


void MenuState::render() {
    m_window.clear();
    for (const std::unique_ptr<sf::Text>& textPtr: m_texts) {
        m_window.draw(*textPtr);
    };

    m_window.display();
};


MenuState::~MenuState() {
    std::cout << "Shutting Menu State down." << std::endl;
    // for (sf::Text* textPtr : m_texts) {
    //     std::cout << "deleting text ptr" << std::endl;
    //     delete textPtr;
    // };
    // m_texts.clear();
};
