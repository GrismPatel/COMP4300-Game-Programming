#include <iostream>
#include <string>
#include <memory>
#include "EntityManager.h"


typedef std::vector<std::unique_ptr<Entity>> EntityVector;
typedef std::vector<Entity*> EntityPtrVector;


class EntityManager {
    private:
        EntityVector m_entities;
        EntityVector m_toAdd;
        std::map<std::string, EntityPtrVector> m_entityMap;
        size_t m_totalEntities = 0;
    public:
        EntityManager();

        void createEntity(const std::string& tag);
        void update();
        const EntityVector& getEntities();
        const EntityPtrVector& getEntitiesByTag(const std::string& tag);

        ~EntityManager();
};
