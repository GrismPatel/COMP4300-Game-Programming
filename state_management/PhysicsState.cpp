#include "PhysicsState.hpp"
#include <cmath>


PhysicsState::PhysicsState(sf::RenderWindow& window): State(window) {
    std::cout << "Physics State Init" << std::endl;
    m_moveableShape = new sf::RectangleShape;
    m_moveableShape->setSize({50.0, 40.0});
    m_previousMoveableShapePosition = {0.0, 0.0};
    m_moveableShape->setPosition(m_previousMoveableShapePosition);
    m_moveableShape->setOrigin({m_moveableShape->getSize().x*0.5f, m_moveableShape->getSize().y*0.5f});
    std::cout << m_moveableShape->getPosition().x << std::endl;
    std::cout << m_moveableShape->getPosition().y << std::endl;

    m_stationaryShape = new sf::CircleShape;
    m_stationaryShape->setRadius(50.0);
    m_stationaryShape->setPosition({
        300.0,
        300.0
    });
    m_stationaryShape->setOrigin({50.0f, 50.0f});
    std::cout << m_stationaryShape->getPosition().x << std::endl;
    std::cout << m_stationaryShape->getPosition().y << std::endl;
};


const sf::Vector2f PhysicsState::getOverlap(
    const sf::Vector2f& entityOnePosition,
    const sf::Vector2f& entityOneSize,
    const sf::Vector2f& entityTwoPosition,
    const sf::Vector2f& entityTwoSize
) const {

    sf::Vector2f delta = {
        std::abs(entityOnePosition.x - entityTwoPosition.x),
        std::abs(entityOnePosition.y - entityTwoPosition.y)
    };

    return {
        (entityOneSize.x*0.5f) + (entityTwoSize.x*0.5f) - delta.x,
        (entityOneSize.y*0.5f) + (entityTwoSize.y*0.5f) - delta.y
    };
};


void PhysicsState::update() {
    while ( const std::optional event = m_window.pollEvent() ) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        };

        if (event->is<sf::Event::KeyPressed>()) {
            m_previousMoveableShapePosition = m_moveableShape->getPosition();
            switch (event->getIf<sf::Event::KeyPressed>()->code) {
                case sf::Keyboard::Key::Up: {
                    std::cout << "Up" << std::endl;
                    /*
                    This should be in the movement method.
                    Also need to check if you are running out of window.
                    */
                    m_moveableShape->setPosition({
                        m_previousMoveableShapePosition.x,
                        m_previousMoveableShapePosition.y - 10.0f,
                    });
                    break;
                }
                case sf::Keyboard::Key::Down: {
                    std::cout << "Down" << std::endl;
                    m_moveableShape->setPosition({
                        m_previousMoveableShapePosition.x,
                        m_previousMoveableShapePosition.y + 10.0f,
                    });
                    break;
                }
                case sf::Keyboard::Key::Right: {
                    std::cout << "Right" << std::endl;
                    m_moveableShape->setPosition({
                        m_previousMoveableShapePosition.x + 10.0f,
                        m_previousMoveableShapePosition.y
                    });
                    break;
                }
                case sf::Keyboard::Key::Left: {
                    std::cout << "Left" << std::endl;
                    m_moveableShape->setPosition({
                        m_previousMoveableShapePosition.x - 10.0f,
                        m_previousMoveableShapePosition.y
                    });
                    break;
                }
                default: { break; }
            };
        };
        sf::Vector2f currentOverlap = getOverlap(
            m_moveableShape->getPosition(),
            m_moveableShape->getSize(),
            m_stationaryShape->getPosition(),
            {
                m_stationaryShape->getRadius()*2.0f,
                m_stationaryShape->getRadius()*2.0f
            }

        );
        std::cout << "currentOverlap.x: " << currentOverlap.x << std::endl;
        std::cout << "currentOverlap.y: " << currentOverlap.y << std::endl;
        if (currentOverlap.x > 0 and currentOverlap.y > 0) {
            
            sf::Vector2f previousOverlap = getOverlap(
                m_previousMoveableShapePosition,
                m_moveableShape->getSize(),
                m_stationaryShape->getPosition(),
                {
                    m_stationaryShape->getRadius()*2.0f,
                    m_stationaryShape->getRadius()*2.0f
                }
            );
            std::cout << "previousOverlap.x: " << previousOverlap.x << std::endl;
            std::cout << "previousOverlap.y: " << previousOverlap.y << std::endl;
            /*
            if previous overlap.x > 0 and (new position y > old position y) then it is going down.
            if previous overlap.x > 0 and (new position y < old position y) then it is going up.
            if previous overlap.y > 0 and (new position x > old position x) then it is going left.
            if previous overlap.y > 0 and (new position x < old position x) then it is going right.
            */

            if (previousOverlap.x >= 0 and (m_moveableShape->getPosition().y > m_previousMoveableShapePosition.y)) {
                std::cout << "coming from up and collided" << std::endl;
                m_moveableShape->setPosition({
                    m_moveableShape->getPosition().x,
                    m_moveableShape->getPosition().y-currentOverlap.y
                });
            }
            else if (previousOverlap.x >= 0 and (m_moveableShape->getPosition().y < m_previousMoveableShapePosition.y )) {
                std::cout << "coming from down and collided" << std::endl;
                m_moveableShape->setPosition({
                    m_moveableShape->getPosition().x,
                    m_moveableShape->getPosition().y + currentOverlap.y
                });
            }
            else if (previousOverlap.y >= 0 and (m_moveableShape->getPosition().x > m_previousMoveableShapePosition.x)) {
                std::cout << "coming from left and collided" << std::endl;
                m_moveableShape->setPosition({
                    m_moveableShape->getPosition().x-currentOverlap.x,
                    m_moveableShape->getPosition().y
                });
            }
            else if (previousOverlap.y >= 0 and (m_moveableShape->getPosition().x < m_previousMoveableShapePosition.x)) {
                std::cout << "coming from right and collided" << std::endl;
                m_moveableShape->setPosition({
                    m_moveableShape->getPosition().x + currentOverlap.x,
                    m_moveableShape->getPosition().y
                });
            };
        };
    };
};


void PhysicsState::render() {
    m_window.clear();

    m_window.draw(*m_moveableShape);
    m_window.draw(*m_stationaryShape);

    m_window.display();
};


PhysicsState::~PhysicsState() {
    std::cout << "Closing PhysicsState" << std::endl;
    delete m_moveableShape;
    delete m_stationaryShape;
};
