#include "RayCollisionState.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>


RayCollisionState::RayCollisionState(sf::RenderWindow &window): State(window) {
    m_vertices = new std::vector<sf::Vertex>;
    m_vertices->push_back(sf::Vertex{{225.f, 230.f}});
    m_vertices->push_back(sf::Vertex{{275.f, 230.f}});
    m_vertices->push_back(sf::Vertex{{275.f, 270.f}});
    m_vertices->push_back(sf::Vertex{{225.f, 270.f}});

    m_vertexArray = new sf::VertexArray(sf::PrimitiveType::LineStrip);
    m_rays = new sf::VertexArray(sf::PrimitiveType::Lines);

    m_raysEnd = new std::vector<sf::Vertex>;
    for (int index = 0; index < m_vertices->size(); index++) {
        m_vertexArray->append((*m_vertices)[index]);
    };
    m_vertexArray->append((*m_vertices)[0]);

};


Intersect RayCollisionState::didItIntersect(
    const sf::Vertex& rayStart,
    const sf::Vertex& rayEnd,
    const sf::Vertex& lineStart,
    const sf::Vertex& lineEnd
) const {
    /*
    a------->b
    c------->d
    vec2 r = b-a
    vec2 s = d-c
    float rXs = r.x*s.y - s.x*r.y
    vec2 c-a = c-a
    float t = ((c-a)Xs)/rXs
    float u = ((c-a)Xr)/rXs
    */

    sf::Vector2f rayDiff = rayEnd.position - rayStart.position;
    sf::Vector2f lineDiff = lineEnd.position - lineStart.position;
    float diffCrossProduct = (rayDiff.x * lineDiff.y) - (rayDiff.y * lineDiff.x);
    sf::Vector2f startDiff = lineStart.position - rayStart.position;

    float t = ((startDiff.x * lineDiff.y) - (startDiff.y * lineDiff.x)) / diffCrossProduct;
    float s = ((startDiff.x * rayDiff.y) - (startDiff.y * rayDiff.x)) / diffCrossProduct;

    if (0<= t && t <= 1 && 0 <= s && s <= 1) {
        return Intersect {true, sf::Vector2f(rayStart.position.x+t*rayDiff.x, rayStart.position.y+t*rayDiff.y)};
    };
    return Intersect {false, sf::Vector2f(0.f, 0.f)};
};


float RayCollisionState::getAngle(
    const sf::Vector2f &startPosition,
    const sf::Vector2f &endPosition
) const {
    float dX = endPosition.x - startPosition.x;
    float dY = endPosition.y - startPosition.y;
    return std::atan2(dY, dX);

};


sf::Vector2f RayCollisionState::getPositionFromRadian(
    const float &distance,
    const float &radian
) {
    return sf::Vector2f(distance * std::cos(radian), distance * std::sin(radian));
};


void RayCollisionState::update() {
    while ( const std::optional event = m_window.pollEvent() ) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        };

        m_raysEnd->clear();
        m_rays->clear();

        for (int index = 0; index < m_vertices->size(); index++) {
            m_raysEnd->push_back((*m_vertices)[index]);
        };

        size_t object_index = 0;
        while (object_index +1 < m_vertexArray->getVertexCount()) {
            for (int index = 0; index < m_raysEnd->size(); index++) {
                Intersect returnValue = didItIntersect(
                    {{10.f, 10.f}},
                    (*m_raysEnd)[index],
                    (*m_vertexArray)[object_index],
                    (*m_vertexArray)[object_index +1]
                );
                if (returnValue.doesItIntersect == true) {
                    (*m_raysEnd)[index].position = returnValue.position;
                };
            };
            object_index += 1;
        };

        /*
        [this] is needed because getAngle is non-static method.
        */
        std::sort(
            m_raysEnd->begin(),
            m_raysEnd->end(),
            [this](const sf::Vertex& a, const sf::Vertex& b) {
                return getAngle({10.f, 10.f}, a.position) < getAngle({10.f, 10.f}, b.position);
            }
        );

        // add +- 0.00001 radian change to m_rays
        for (int index =0; index < m_raysEnd->size(); index ++) {
            float distance = std::sqrt(
                std::pow(((*m_raysEnd)[index].position.y - 10.f), 2.0) +
                std::pow(((*m_raysEnd)[index].position.x - 10.f), 2.0)
            );

            float currentAngle = getAngle({10.f, 10.f}, (*m_raysEnd)[index].position);
            sf::Vector2f previousPosition = getPositionFromRadian(distance, currentAngle -0.00001);
            m_rays->append({{10.f, 10.f}});
            m_rays->append({{previousPosition.x + 10.f, previousPosition.y + 10.f}});

            m_rays->append({{10.f, 10.f}});
            m_rays->append((*m_raysEnd)[index]);

            sf::Vector2f nextPosition = getPositionFromRadian(distance, currentAngle -0.00001);
            m_rays->append({{10.f, 10.f}});
            m_rays->append({{nextPosition.x + 10.f, nextPosition.y + 10.f}});
        };
    };
};


void RayCollisionState::render() {
    m_window.clear();

    m_window.draw(*m_vertexArray);
    m_window.draw(*m_rays);

    m_window.display();
};


RayCollisionState::~RayCollisionState() {
    delete m_vertices;
    delete m_vertexArray;
    delete m_rays;
    delete m_raysEnd;
};
