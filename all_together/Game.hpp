#include "SFML/Graphics.hpp"
#include <memory>


class Game {
    private:
        sf::RenderWindow m_window;
    public:
        Game();
        void run();
        ~Game();
};
