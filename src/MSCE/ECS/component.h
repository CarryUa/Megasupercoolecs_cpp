#ifndef _MSCE_COMPONENT_H_
#define _MSCE_COMPONENT_H_
#include <iostream>
#include <MSCE/msce_macros.h>

#pragma region Components Macros
#define MSCE_DEFINE_COMPONENT(Type, ...)                                                                        \
private:                                                                                                        \
    MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(::msce::BaseComponent<Type>, __VA_ARGS__)                    \
public:                                                                                                         \
    /**  @brief This overload is used by cereal. All components must be instantiated using ComponentManager. */ \
    Type() {}                                                                                                   \
    Type(size_t id) : BaseComponent(id) {}

#define MSCE_REGISTER_COMPONENT(Type) \
    CEREAL_REGISTER_TYPE(Type)        \
    CEREAL_REGISTER_POLYMORPHIC_RELATION(::msce::BaseComponent<Type>, Type)

#pragma endregion

namespace msce
{
    class IComponent
    {
    protected:
        friend class ComponentManager;
        size_t id_;

    public:
        /// @brief This overload is used by cereal.All components must be instantiated using ComponentManager
        IComponent() {}
        IComponent(size_t id) { this->id_ = id; }

        virtual ~IComponent() = default;
        virtual IComponent *clone() = 0;

        friend class ::cereal::access;
        template <class Archive>
        void save(Archive &ar) const {}
        template <class Archive>
        void load(Archive &ar) {}
    };

    template <typename TComp>
    class BaseComponent : public IComponent
    {
    public:
        /// @brief This overload is used by cereal. All components must be instantiated using ComponentManager.
        BaseComponent() {}
        BaseComponent(size_t id) : IComponent(id) {}
        /// @brief This method is used by ComponentManager, dont relly on it for cloning components
        /// @return
        IComponent *clone() override
        {
            return new TComp(static_cast<const TComp &>(*this));
        }
    };
}

CEREAL_REGISTER_TYPE(msce::IComponent)

#endif // _MSCE_COMPONENT_H_