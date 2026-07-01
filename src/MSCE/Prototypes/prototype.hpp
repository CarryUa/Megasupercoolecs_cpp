#ifndef MSCE_I_PROTOTYPE_H_
#define MSCE_I_PROTOTYPE_H_
#include <string>
#include <MSCE/logger.h>
#include <MSCE/Types/Collections/registry.hpp>
#include <MSCE/Reflection/reflection.h>
#include <MSCE/msce_macros.h>

namespace msce
{
    class IPrototype;
    inline Registry<std::string, std::reference_wrapper<const std::type_info>> &get_g_registered_prototypes()
    {
        static Registry<std::string, std::reference_wrapper<const std::type_info>> reg;
        return reg;
    };
    inline Registry<std::string, std::function<std::unique_ptr<IPrototype>()>> &get_g_registered_prototype_factories()
    {
        static Registry<std::string, std::function<std::unique_ptr<IPrototype>()>> reg;
        return reg;
    };
}

#pragma region Prototype Macros

#define MSCE_DEFINE_PROTOTYPE(ProtoType, ...)                                       \
    MSCE_REFLECTION_DEFINE_CLASS(ProtoType)                                         \
    MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(::msce::IPrototype, __VA_ARGS__) \
    virtual const ::msce::Type &get_type_info_polymorphic() override                \
    {                                                                               \
        static const ::msce::Type &t = ::msce::get_reflection_of_type(#ProtoType);  \
        return t;                                                                   \
    }

#define MSCE_REGISTER_PROTOTYPE(Type, Name, ...)                                                                                               \
    CEREAL_REGISTER_TYPE(Type)                                                                                                                 \
    CEREAL_REGISTER_POLYMORPHIC_RELATION(::msce::IPrototype, Type)                                                                             \
    MSCE_REFLECT_CLASS(Type, __VA_ARGS__)                                                                                                      \
    namespace msce                                                                                                                             \
    {                                                                                                                                          \
        template <>                                                                                                                            \
        class Registration<Type>                                                                                                               \
        {                                                                                                                                      \
        public:                                                                                                                                \
            [[gnu::used]] Registration()                                                                                                       \
            {                                                                                                                                  \
                static Logger logger("StaticPrototypeRegistration");                                                                           \
                get_g_registered_prototypes().register_entry(#Type, std::cref(typeid(Type)));                                                  \
                get_g_registered_prototype_factories().register_entry(#Type,                                                                   \
                                                                      []()                                                                     \
                                                                      {                                                                        \
                                                                          return std::make_unique<Type>();                                     \
                                                                      });                                                                      \
                logger.log_info("Registered prototype({}) with name: '{}'", get_g_registered_prototypes().enumerate_registry().size(), #Type); \
            }                                                                                                                                  \
        };                                                                                                                                     \
        inline Registration<Type> registered_##Name;                                                                                           \
    }

#pragma endregion

namespace msce
{
    /**
     * @brief Interface and base class of all prototypes.
     */
    class IPrototype
    {
    public:
        /// @brief The if of this prototype.
        std::string id = "Invalid default prototype!";

        /// @brief Do I really need to document this?
        virtual ~IPrototype() = default;

    public:
        friend class ::cereal::access;
        /// @brief cereal stuff.
        template <class Archive>
        void save(Archive &ar) const { ar(::cereal::make_nvp("id", id)); }
        /// @brief cereal stuff.
        template <class Archive>
        void load(Archive &ar) { ar(::cereal::make_nvp("id", id)); }
        template <typename T>
        friend constexpr const ::msce::Type &get_reflection_of_type();
        inline static const ::msce::Type &get_type_info()
        {
            static const ::msce::Type &t = ::msce::get_reflection_of_type("::msce::IPrototype");
            return t;
        }

        virtual const ::msce::Type &get_type_info_polymorphic()
        {
            static const ::msce::Type &t = ::msce::get_reflection_of_type("::msce::IPrototype");
            return t;
        }
    };
}

MSCE_REFLECT_CLASS(::msce::IPrototype, id)
CEREAL_REGISTER_TYPE(msce::IPrototype)

#endif // MSCE_I_PROTOTYPE_H_