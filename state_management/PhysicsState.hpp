#pragma once

#include "State.hpp"


class PhysicsState: public State {
    private:
        
        /*This should be entity; add each entity should have component (position, ) to hold the data.*/
        /*
        This is just telling m_moveableShape is type sf::RectangeShape pointer, it does not assign anything to it.
        You have to manually assign/create it by new() before using it.
        */
        sf::RectangleShape* m_moveableShape;
        sf::CircleShape*    m_stationaryShape;
        sf::Vector2f        m_previousMoveableShapePosition;
    public:
        PhysicsState(sf::RenderWindow& window);

        void update() override;
        void render() override;

        /*
        This should be texture so both args should be same type
        */
        const sf::Vector2f getOverlap(
            const sf::Vector2f& entityOnePosition,
            const sf::Vector2f& entityOneSize,
            const sf::Vector2f& entityTwoPosition,
            const sf::Vector2f& entityTwoSize
        ) const;

        ~PhysicsState();
};
