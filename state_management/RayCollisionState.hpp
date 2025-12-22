#include "State.hpp"


struct Intersect {
    bool         doesItIntersect;
    sf::Vector2f position;
};


class RayCollisionState: public State {
    private:
        std::vector<std::unique_ptr<sf::VertexArray>> m_polygons;
        std::unique_ptr<sf::Vector2f> m_startPoint;
        std::vector<float> m_angles;
        std::vector<sf::VertexArray> m_triangles;
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
