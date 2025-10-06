#include <iostream>
#include <string>
#include <memory>
#include "EntityManager.h"


std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag) {
    // this will not work because std::make_shared<Entity>, make_shared will try to construct Entity() and Entity has private constructor
    // which can only be accessed via EntityManager and make_shared is not a friend of Entity.
    // std::shared_ptr<Entity> newEntity = std::make_shared<Entity>(m_totalEntities +1, tag);

    m_totalEntities += 1;
    std::shared_ptr<Entity> newEntity = std::shared_ptr<Entity>(new Entity(m_totalEntities, tag));
    m_toAdd.push_back(newEntity);
    return newEntity;
};


void EntityManager::update() {
    for (std::shared_ptr<Entity>& entity : m_toAdd) {
        m_entities.push_back(entity);
        m_entityMap[entity->getTag()].push_back(entity);
    };
    m_toAdd.clear();

    m_entities.erase(
        std::remove_if(
            m_entities.begin(), m_entities.end(), [](const std::shared_ptr<Entity>& entity) {
                return entity->isActive() == false;
            }
        ), m_entities.end()
    );

    for (auto& [tag, entities]: m_entityMap) {
        entities.erase(
            std::remove_if(
                entities.begin(), entities.end(), [](const std::shared_ptr<Entity>& entity) {
                    return entity->isActive() == false;
                }
            ), entities.end()
        );
    };
};


const EntityVector& EntityManager::getEntities() {
    return m_entities;
};


EntityVector& EntityManager::getEntitiesByTag(const std::string& tag) {
    return m_entityMap[tag];
};


EntityManager::EntityManager() {};
