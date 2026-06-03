#ifndef MSCE_ENTITY_MANAGER_H_
#define MSCE_ENTITY_MANAGER_H_

#include <vector>
#include <memory>

#include <MSCE/ECS/entity.h>
#include <MSCE/Types/Collections/smartUniquePointerList.hpp>
#include <MSCE/Types/singleton.hpp>

namespace msce
{
    /**
     * @brief This is @ref msce::SmartHandle alias for entity manager.
     */
    using EntityHandle = SmartHandle<SmartUniquePointerList<Entity>, Entity>;

    /**
     * @brief Manages entities lifetime. Always use if for any entity manipulation
     */
    class EntityManager : public Singleton<EntityManager>
    {
    private:
        /// @brief Entity storage.
        SmartUniquePointerList<Entity> entities_;
        inline static Logger logger = Logger("EntityManager");

    public:
        EntityManager();

        /**
         * @brief Creates an empty entity.
         * @return @ref msce::EntityHandle of created entity.
         */
        EntityHandle create_entity();

        /**
         * @brief Copies provided entity.
         * @param other @ref msce::EntityHandle of source entity.
         * @return @ref msce::EntityHandle of the copy of source entity.
         */
        EntityHandle copy_entity(EntityHandle other);

        /**
         * @brief Destroys provided entity.
         * @param other @ref msce::EntityHandle of entity to be destroyed.
         * @note Note that it will also destroy all of this entity's components.
         */
        bool destroy_entity(EntityHandle other);

        /**
         * @brief Gets the entity stored under given id.
         * @param id The id of the entity.
         * @return @ref msce::EntityHandle of an entity under id or nullhandle.
         */
        EntityHandle get_entity(uint32_t id);
    };
}

#endif // MSCE_ENTITY_MANAGER_H_