#include "EntityManager.hpp"


EntityManager::EntityManager() {};


void EntityManager::createEntity(const std::string& tag) {
    m_totalEntities += 1;
    std::unique_ptr<Entity> newEntity = std::make_unique<Entity>(new Entity(m_totalEntities, tag));
    m_toAdd.push_back(std::move(newEntity));
};


void EntityManager::update() {
    for (std::unique_ptr<Entity>& entity: m_toAdd) {
        Entity* entityPtr = entity.get();
        m_entityMap[entityPtr->getTag()].push_back(entityPtr);
        m_entities.push_back(std::move(entity));
    };
    m_toAdd.clear();

    for (auto& [tag, entities]: m_entityMap) {
        entities.erase(
            std::remove_if(
                entities.begin(), entities.end(), [](const Entity* entity) {
                    return entity->isActive() == false;
                }
            ), entities.end()
        );
    };

    m_entities.erase(
        std::remove_if(
            m_entities.begin(), m_entities.end(), [](const std::unique_ptr<Entity>& entity) {
                return entity->isActive() == false;
            }
        ), m_entities.end()
    );

};


const EntityVector& EntityManager::getEntities() {
    return m_entities;
};


const EntityPtrVector& EntityManager::getEntitiesByTag(const std::string& tag) {
    return m_entityMap[tag];
};
