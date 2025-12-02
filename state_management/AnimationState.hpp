#include "State.hpp"
#include <map>
#include <vector>


class AnimationState: public State {
    private:
        std::vector<std::string>           m_actions = {"Stand", "Jump", "Run"};
        std::map<std::string, sf::Texture> m_actionToAnimationMap;
        sf::Sprite*                        m_sprite;
        int                                m_gameFrame = 0;
        int                                m_animationTime = 10;
    public:
        AnimationState(sf::RenderWindow& window);
        void update() override;
        void render() override;

        ~AnimationState();
};
