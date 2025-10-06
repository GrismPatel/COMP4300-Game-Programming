#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include "Entity.h"


typedef std::vector<std::shared_ptr<Entity>> EntityVector;
typedef std::map<std::string, EntityVector> EntityMap;


class EntityManager {
    private:
        EntityVector m_entities;
        EntityVector m_toAdd;
        EntityMap m_entityMap;
        size_t m_totalEntities = 0;
    public:
        EntityManager();

        std::shared_ptr<Entity> addEntity(const std::string& tag);
        void update();
        const EntityVector& getEntities();
        EntityVector& getEntitiesByTag(const std::string& tag);
};
