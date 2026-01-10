#include <iostream>
#include <memory>
#include "Components.hpp"


class Entity {
    private:
        bool m_is_active = true;
        size_t m_id = 0;
        std::string m_tag = "default";
    public:
        Entity(size_t id, std::string tag);

        std::unique_ptr<CSprite>    cSprite;
        std::unique_ptr<CAnimation> cAnimation;
        std::unique_ptr<CTransform> cTransform;
        std::unique_ptr<CLifespan>  cLifespan;

        bool isActive() const;
        const std::string getTag() const;
        const size_t getId() const;
        void destroy();

        ~Entity();
};
