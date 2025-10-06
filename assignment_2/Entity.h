#pragma once
#include <memory>
#include <string>
#include <iostream>
#include "Components.h"


class Entity {
    private:
        friend class EntityManager;

        bool m_is_active = true;
        size_t m_id = 0;
        std::string m_tag = "default";

    Entity(const size_t id, const std::string& tag);
    public:

        // component point
        std::shared_ptr<CTransform> cTransform;
        std::shared_ptr<CShape> cShape;
        std::shared_ptr<CCollision> cCollision;
        std::shared_ptr<CScore> cScore;
        std::shared_ptr<CLifespan> cLifespan;
        std::shared_ptr<CInput> cInput;

        // privatemember access
        bool isActive() const;
        const std::string& getTag() const;
        const size_t getId() const;
        void destroy();
};
