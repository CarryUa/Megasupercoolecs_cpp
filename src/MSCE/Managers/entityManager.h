#ifndef MSCE_ENTITY_MANAGER_H_
#define MSCE_ENTITY_MANAGER_H_

#include <vector>
#include <memory>

#include <MSCE/ECS/entity.h>
#include <MSCE/Common/smartPointerList.h>
#include <MSCE/Common/Interfaces/Singleton.hpp>

namespace msce
{
    class EntityManager : public Singleton<EntityManager>
    {
    private:
        SmartUniquePointerList<Entity> entities_;

    public:
        /// @brief Creates new entity.
        /// @return Pointer to newly created entity.
        Entity *create_entity();

        /// @brief Copies an entity and all it's components.
        /// @return Pointer to newly created entity.
        Entity *copy_entity(Entity *other);

        bool destroy_entity(Entity *other);

        /// @brief Finds entity class by id.
        /// @return Pointer to an entity with given id or nullptr.
        Entity *get_entity(size_t id);
    };
}

#endif // MSCE_ENTITY_MANAGER_H_