#ifndef MSCE_I_PROTOTYPE_H_
#define MSCE_I_PROTOTYPE_H_
#include <string>
#include <MSCE/logger.h>
#include <MSCE/Types/Collections/registry.hpp>
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

#define MSCE_DEFINE_PROTOTYPE(Type, ...)                                            \
    MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(::msce::IPrototype, __VA_ARGS__) \
    MSCE_GENERATE_REFLECTION_METHODS_DERIVED(Type, ::msce::IPrototype, __VA_ARGS__)

#define MSCE_REGISTER_PROTOTYPE(Type, Name)                                                                                                    \
    CEREAL_REGISTER_TYPE(Type)                                                                                                                 \
    CEREAL_REGISTER_POLYMORPHIC_RELATION(::msce::IPrototype, Type)                                                                             \
    namespace msce                                                                                                                             \
    {                                                                                                                                          \
        template <>                                                                                                                            \
        class Registration<Type>                                                                                                               \
        {                                                                                                                                      \
        public:                                                                                                                                \
            [[gnu::used]] Registration()                                                                                                       \
            {                                                                                                                                  \
                static Logger logger("StaticPrototypeRegistration");                                                                           \
                get_g_registered_prototypes().register_entry(#Name, std::cref(typeid(Type)));                                                  \
                get_g_registered_prototype_factories().register_entry(#Name,                                                                   \
                                                                      []()                                                                     \
                                                                      {                                                                        \
                                                                          return std::make_unique<Type>();                                     \
                                                                      });                                                                      \
                logger.log_info("Registered prototype({}) with name: '{}'", get_g_registered_prototypes().enumerate_registry().size(), #Name); \
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
        MSCE_GENERATE_REFLECTION_METHODS(IPrototype, id)
    };
}
CEREAL_REGISTER_TYPE(msce::IPrototype)

#endif // MSCE_I_PROTOTYPE_H_