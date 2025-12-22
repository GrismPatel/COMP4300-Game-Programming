#include "RayCollisionState.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>


RayCollisionState::RayCollisionState(sf::RenderWindow &window): State(window) {
    std::unique_ptr<sf::VertexArray> polygon1 = std::make_unique<sf::VertexArray>(sf::PrimitiveType::LineStrip);
    polygon1->append({{225.f, 225.f}});
    polygon1->append({{275.f, 225.f}});
    polygon1->append({{275.f, 275.f}});
    polygon1->append({{225.f, 275.f}});
    polygon1->append({{225.f, 225.f}});
    m_polygons.push_back(std::move(polygon1));

    std::unique_ptr<sf::VertexArray> polygon2 = std::make_unique<sf::VertexArray>(sf::PrimitiveType::LineStrip);
    polygon2->append({{500.f, 370.f}, sf::Color::Red});
    polygon2->append({{700.f, 450.f}, sf::Color::Red});
    polygon2->append({{400.f, 450.f}, sf::Color::Red});
    polygon2->append({{500.f, 370.f}, sf::Color::Red});
    m_polygons.push_back(std::move(polygon2));

    std::unique_ptr<sf::VertexArray> polygon3 = std::make_unique<sf::VertexArray>(sf::PrimitiveType::LineStrip);
    polygon3->append({{300.f, 300.f}});
    polygon3->append({{325.f, 325.f}});
    polygon3->append({{325.f, 350.f}});
    polygon3->append({{275.f, 350.f}});
    polygon3->append({{275.f, 325.f}});
    polygon3->append({{300.f, 300.f}});
    m_polygons.push_back(std::move(polygon3));

    std::unique_ptr<sf::VertexArray> polygon4 = std::make_unique<sf::VertexArray>(sf::PrimitiveType::LineStrip);
    polygon4->append({{300.f, 500.f}});
    polygon4->append({{500.f, 500.f}});
    polygon4->append({{500.f, 600.f}});
    polygon4->append({{300.f, 600.f}});
    polygon4->append({{300.f, 500.f}});
    m_polygons.push_back(std::move(polygon4));

    /*
    Currently hardcoding but this needs to be updated based on position movement

    This whole logic should be moved to update().

    1. Get angle from source to all vertecies then +- 0.00001 radian and store them in vector
    2. sort it
    3. create lines from source to infinite on those sorted angles. source.x + (distance)*cos(angle)
    4. does it intersect.
    */
   m_startPoint = std::make_unique<sf::Vector2f>(5.f, 10.f);

    
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

        if (event->is<sf::Event::KeyPressed>()) {
            switch (event->getIf<sf::Event::KeyPressed>()->code) {
                case sf::Keyboard::Key::Right: {
                    std::cout << "Right: " << std::endl;
                    (*m_startPoint).x += 5.f;
                }
                default: { break; }
            };
        };
        m_angles.clear();
        m_triangles.clear();

        for (std::unique_ptr<sf::VertexArray>& polygon :m_polygons) {
            size_t currentIndex = 0;
            while (currentIndex < polygon->getVertexCount() -1) {
                float angle = getAngle(
                    *m_startPoint,
                    (*polygon)[currentIndex].position
                );
                /*
                std::cout <<
                "Angle from start position to this: " <<
                (*polygon)[currentIndex].position.x << " " << (*polygon)[currentIndex].position.y << " " << angle <<
                std::endl;
                */
                m_angles.push_back(angle -0.001);
                m_angles.push_back(angle);
                m_angles.push_back(angle +0.001);
                currentIndex += 1;
            };
        };
        std::sort(
            m_angles.begin(), m_angles.end(), [](const float a, const float b) {return a < b ;}
        );

        std::vector<sf::Vector2f> raysEnd;
        for (const float m_angle: m_angles) {
            raysEnd.push_back({m_startPoint->x+1000*std::cos(m_angle), m_startPoint->y+1000*std::sin(m_angle)});
        };

        for (std::unique_ptr<sf::VertexArray>& m_polygon : m_polygons) {
            for (int polygonIndex = 0; polygonIndex < m_polygon->getVertexCount() -1; polygonIndex++) {
                for (int index = 0; index < raysEnd.size(); index ++ ) {
                    Intersect resultIntersect = didItIntersect(
                        {*m_startPoint},
                        {raysEnd[index]},
                        (*m_polygon)[polygonIndex],
                        (*m_polygon)[polygonIndex+1]
                    );
                    if (resultIntersect.doesItIntersect) {
                        raysEnd[index] = resultIntersect.position;
                    };
                };
            };
        };

        for (int index = 1; index < raysEnd.size(); index ++) {
            sf::VertexArray triangleLines(sf::PrimitiveType::Triangles);
            triangleLines.append({*m_startPoint});
            triangleLines.append({raysEnd[index]});
            triangleLines.append({raysEnd[index -1]});
            m_triangles.push_back(triangleLines);
        };
    };
};


void RayCollisionState::render() {
    m_window.clear();

    for (std::unique_ptr<sf::VertexArray>& polygon: m_polygons) {
        m_window.draw(*polygon);
    };
    for (sf::VertexArray triangle: m_triangles) {
        m_window.draw(triangle);
    };

    m_window.display();
};


RayCollisionState::~RayCollisionState() {
    
};
