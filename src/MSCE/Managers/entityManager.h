#ifndef MSCE_ENTITY_MANAGER_H_
#define MSCE_ENTITY_MANAGER_H_
#include <MSCE/ECS/entity.h>
#include <MSCE/Common/Interfaces/iSingleton.hpp>
#include <memory>
namespace msce
{
    class EntityManager : public ISingleton<EntityManager>
    {
    private:
        std::vector<std::unique_ptr<Entity>> _entities;

    public:
        EntityManager();

        /// @brief Creates new entity.
        /// @return Pointer to newly created entity.
        Entity *create_entity();

        /// @brief Copies an entity and all it's components.
        /// @return Pointer to newly created entity.
        Entity *copy_entity(Entity *other);

        void destroy_entity();

        /// @brief Finds entity class by id.
        /// @return Pointer to an entity with given id or nullptr.
        Entity *get_entity(size_t id);

        size_t get_entity_id(Entity *ent);
    };
}

#endif // MSCE_ENTITY_MANAGER_H_