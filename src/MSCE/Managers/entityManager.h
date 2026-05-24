#ifndef MSCE_ENTITY_MANAGER_H_
#define MSCE_ENTITY_MANAGER_H_

#include <vector>
#include <memory>

#include <MSCE/ECS/entity.h>
#include <MSCE/Types/Collections/smartUniquePointerList.hpp>
#include <MSCE/Types/singleton.hpp>

namespace msce
{
    using EntityHandle = SmartHandle<SmartUniquePointerList<Entity>, Entity>;
    class EntityManager : public Singleton<EntityManager>
    {
    private:
        SmartUniquePointerList<Entity> entities_;

    public:
        /// @brief Creates new entity.
        /// @return Pointer to newly created entity.
        EntityHandle create_entity();

        /// @brief Copies an entity and all it's components.
        /// @return Pointer to newly created entity.
        EntityHandle copy_entity(EntityHandle other);

        bool destroy_entity(EntityHandle other);

        /// @brief Finds entity class by id.
        /// @return Pointer to an entity with given id or nullptr.
        EntityHandle get_entity(uint32_t id);
    };
}

#endif // MSCE_ENTITY_MANAGER_H_