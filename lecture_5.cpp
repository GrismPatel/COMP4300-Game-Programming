#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Vec2: public sf::Vector2f {
public:
    float x;
    float y;

    Vec2(): sf::Vector2f() {};
    Vec2(float vec_x, float vec_y): sf::Vector2f(vec_x, vec_y), x(vec_x), y(vec_y) {};

    // returns vec2 reference which is going to return reference to the object itself.
    Vec2& add(const Vec2& rhs) {
        x += rhs.x;
        y += rhs.y;
        // return reference to the object itself.
        return *this;
    }
};

int main () {
    Vec2 vector_1(1.0, 2.0);
    Vec2 vector_2(1.0, 2.0);

    vector_1.add(vector_2);

    std::cout << vector_1.x << " " << vector_1.y << std::endl;
    return 0;
}
