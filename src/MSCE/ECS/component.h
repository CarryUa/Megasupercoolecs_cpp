#ifndef _MSCE_COMPONENT_H_
#define _MSCE_COMPONENT_H_
#include <iostream>
#include <MSCE/msce_macros.h>

#pragma region Components Macros
#define MSCE_DEFINE_COMPONENT(Type, ...)                                                                        \
private:                                                                                                        \
    MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(::msce::BaseComponent<Type>, __VA_ARGS__)                    \
    /**  @brief This overload is used by cereal. All components must be instantiated using ComponentManager. */ \
    Type() {}

#define MSCE_REGISTER_COMPONENT(Type) \
    CEREAL_REGISTER_TYPE(Type)        \
    CEREAL_REGISTER_POLYMORPHIC_RELATION(::msce::BaseComponent<Type>, Type)

#pragma endregion

namespace msce
{
    class ComponentManager;
    /**
     * @brief Component interface used for storage.
     */
    class IComponent
    {
    protected:
        friend class ComponentManager;

        /// @brief This overload is used by cereal.All components must be instantiated using ComponentManager
        IComponent() {}

    public:
        virtual ~IComponent() = default;
        virtual IComponent *clone() = 0;

        friend class ::cereal::access;
        /**  @brief Cereal's save function. Refer to their docs for more info.*/
        template <class Archive>
        void save(Archive &ar) const {}
        template <class Archive>
        /**  @brief Cereal's load function. Refer to their docs for more info.*/
        void load(Archive &ar)
        {
        }
    };

    /**
     * @brief Base type for all components.
     */
    template <typename TComp>
    class BaseComponent : public IComponent
    {
    protected:
        friend class ComponentManager;

        /**
         * @returns A pointer to the copy.
         */
        IComponent *clone() override final
        {
            return new TComp(static_cast<const TComp &>(*this));
        }
        /// @brief This overload is used by cereal. All components must be instantiated using ComponentManager.
        BaseComponent() {}

    public:
    };
}

CEREAL_REGISTER_TYPE(msce::IComponent)

#endif // _MSCE_COMPONENT_H_