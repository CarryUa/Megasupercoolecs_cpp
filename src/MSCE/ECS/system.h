#ifndef _MSCE_SYSTEM_H_
#define _MSCE_SYSTEM_H_
#include <MSCE/Types/Collections/registry.hpp>
#include <MSCE/msce_macros.h>
#include <MSCE/logger.h>
#include <typeindex>
namespace msce
{
    class System;

    inline Registry<std::type_index, std::function<std::unique_ptr<System>()>> &get_g_system_registry()
    {
        static Registry<std::type_index, std::function<std::unique_ptr<System>()>> reg;
        return reg;
    }
}

#pragma region Systems Macros
#define MSCE_REGISTER_SYSTEM(Type, Name)                                                                                       \
    namespace msce                                                                                                             \
    {                                                                                                                          \
        template <>                                                                                                            \
        class Registration<Type>                                                                                               \
        {                                                                                                                      \
        public:                                                                                                                \
            [[gnu::used]] Registration()                                                                                       \
            {                                                                                                                  \
                static Logger logger("StaticSystemRegistration");                                                              \
                if (get_g_system_registry().is_registered(typeid(Type)))                                                       \
                {                                                                                                              \
                    logger.log_warning("Double registration of {} detected!", typeid(Type).name());                            \
                    return;                                                                                                    \
                }                                                                                                              \
                get_g_system_registry().register_entry(typeid(Type),                                                           \
                                                       []() -> std::unique_ptr<System>                                         \
                                                       {                                                                       \
                                                           return std::unique_ptr<System>(std::make_unique<Type>().release()); \
                                                       });                                                                     \
                logger.log_info("Registered system of type: {}", typeid(Type).name());                                         \
            }                                                                                                                  \
        };                                                                                                                     \
        inline Registration<Type> registered_##Name;                                                                           \
    }

#pragma endregion

namespace msce
{
    // Forward declaration to prevent co-dependency
    class SystemManager;

    class System
    {
    public:
        /// @brief Determains if system is going to recive update() calls. init() and pre_init() are recived regardless.
        bool active = true;

        /// @brief Pointer to SystemManager that created this system. Usefull for getting dependency-systems.
        SystemManager *p_sys_man;

        /// @brief Ran during SystemManager.InitAllSystems before init(). Use it to get dependencies pointers, but dont expect other systems being initialized.
        virtual void pre_init();

        /// @brief Ran during SystemManager.InitAllSystems right after pre_init();
        virtual void init();

        /// @brief Ran each frame of the program.
        /// @param delta_t Time since last frame in seconds.
        virtual void update(double delta_seconds);

        virtual ~System() = default;
    };
}
MSCE_REGISTER_SYSTEM(msce::System, System)

#endif // _MSCE_SYSTEM_H_