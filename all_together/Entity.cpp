#include "Entity.hpp"


Entity::Entity(
    size_t id, std::string tag
) : m_id(id), m_tag(tag) {};


bool Entity::isActive() const {
    return m_is_active;
};


const std::string Entity::getTag() const {
    return m_tag;
};


const size_t Entity::getId() const {
    return m_id;
};


void Entity::destroy() {
    m_is_active = false;
};


Entity::~Entity() {};
