#include "AnimationState.hpp"


AnimationState::AnimationState(sf::RenderWindow& window): State(window) {
    std::cout << "Animation State is init" << std::endl;
    for (std::string action: m_actions) {
        sf::Texture texture;
        if (!texture.loadFromFile(action + ".png")) {
            std::cout << "Cannot load texture" << std::endl;
        };
        m_actionToAnimationMap[action] = std::move(texture);
    };
    m_sprite = new sf::Sprite(m_actionToAnimationMap["Stand"]);
};

/*
when up arrow is pressed, jump animation. when arrow is released, stand animation.
when right arrow is pressed, run animation. when arrow is released, stand animation.
when left arrow is pressed, run animation * -1.0. when arrow is released, stand animation.
when p is pressed goes to pause state
*/
void AnimationState::update() {
    m_gameFrame++ ;
    int frameNumber = ( m_gameFrame/m_animationTime ) % 4;
    auto [width, height] = m_actionToAnimationMap["Run"].getSize();
    int frameWidth = width/4;

    while ( const std::optional event = m_window.pollEvent() ) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        };

        if (event->is<sf::Event::KeyPressed>()) {
            switch ( event->getIf<sf::Event::KeyPressed>()->code ) {
                case sf::Keyboard::Key::Up: {
                    std::cout << "Up key is pressed." << std::endl;
                    // action changes to JUMP
                    m_sprite->setTexture(m_actionToAnimationMap["Jump"]);
                    break;
                }
                case sf::Keyboard::Key::Right: {
                    std::cout << "Right key is pressed." << std::endl;
                    
                    m_sprite->setTexture(m_actionToAnimationMap["Run"]);
                    m_sprite->setTextureRect(sf::IntRect({frameNumber*frameWidth, 0}, {frameWidth, static_cast<int>(height)}));
                    sf::Vector2f currentPosition = m_sprite->getPosition();
                    m_sprite->setPosition({
                        currentPosition.x+1.0f,
                        currentPosition.y
                    });
                    break;
                }
                case sf::Keyboard::Key::Left: {
                    std::cout << "Left key is pressed." << std::endl;
                    // action changes to RUN(-1.0)
                    break;
                }
                case sf::Keyboard::Key::P: {
                    std::cout << "P key is pressed." << std::endl;
                    m_pause = true;
                    break;
                }
                default: { break; }
            };
        };

        if (event->is<sf::Event::KeyReleased>()) {
            switch ( event->getIf<sf::Event::KeyReleased>()->code ) {
                case sf::Keyboard::Key::Up: {
                    std::cout << "Up key is released." << std::endl;
                    m_sprite->setTexture(m_actionToAnimationMap["Stand"]);
                    break;
                }
                case sf::Keyboard::Key::Right: {
                    std::cout << "Right Key is released." << std::endl;
                    const sf::Vector2u standAnimationSize = m_actionToAnimationMap["Stand"].getSize();
                    m_sprite->setTexture(m_actionToAnimationMap["Stand"]);
                    std::cout << "Stand animation is set." << std::endl;
                    // have to override it cause it was using running frame ones.
                    m_sprite->setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(standAnimationSize.x), static_cast<int>(standAnimationSize.y)} ));
                    break;
                }
                default: { break; }
            };
        };
    };
};


void AnimationState::render() {
    m_window.clear();
    m_window.draw(*m_sprite);

    m_window.display();
};


AnimationState::~AnimationState() {
    delete m_sprite;
    std::cout << "Deleting Animation State" << std::endl;
};
