#include "State.hpp"


struct Intersect {
    bool         doesItIntersect;
    sf::Vector2f position;
};


class RayCollisionState: public State {
    private:
        std::vector<sf::Vertex>* m_vertices;
        std::vector<sf::Vertex>* m_raysEnd;
        sf::VertexArray*         m_vertexArray;
        sf::VertexArray*         m_rays;
    public:
        RayCollisionState(sf::RenderWindow &window);

        void update() override;
        void render() override;

        /*
        The function does not modify anything.
        */
        Intersect didItIntersect(
            const sf::Vertex& rayStart,
            const sf::Vertex& rayEnd,
            const sf::Vertex& lineStart,
            const sf::Vertex& lineEnd
        ) const;

        float getAngle(
            const sf::Vector2f &startPosition,
            const sf::Vector2f &endPosition
        ) const;

        sf::Vector2f getPositionFromRadian(
            const float &distance,
            const float &radian
        );

        ~RayCollisionState();
};
