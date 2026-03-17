#ifndef _MSCE_SYSTEM_MANAGER_H_
#define _MSCE_SYSTEM_MANAGER_H_
#include <Common/Interfaces/iSingleton.hpp>
#include <ECS/system.h>
#include <BuiltInSystems/timeSystem.h>
#include <vector>
#include <map>
#include <memory>
#include <typeindex>
#include <functional>
#include <iostream>

namespace msce
{

    /// @brief Manages the life-cycle of all registered systems.
    /// All systems should be registered before SystemManager is created.
    /// Only one instance of SystemManager can exist in a programs, or else constructor will throw std::runtime_error.
    class SystemManager : public ISingleton<SystemManager>
    {
    private:
        TimeSystem *_time_sys;
        static std::map<std::type_index, std::function<std::unique_ptr<System>()>> _registered_type_constructor_pairs;

    public:
        /// @brief Creates all previously registered systems.
        /// @exception throws is another instance of SystemManager exists in the program.
        SystemManager();

        /// @brief Deletes all previously created systems.
        ~SystemManager();

        /// @brief List of all existing system instances.
        std::vector<std::unique_ptr<System>> AllSystems;

        /// @brief Registers the system for creation during SystemManager construction.
        /// @tparam TSys The type of system being registered.
        template <typename TSys>
        static void register_system()
        {
            SystemManager::_registered_type_constructor_pairs[typeid(TSys)] = []() -> std::unique_ptr<System>
            {
                return std::unique_ptr<System>(std::make_unique<TSys>().release());
            };
        }

        /// @brief Initializes all system using System.Init();
        void init_all_systems();

        /// @brief Updated all systems using System.Update(delta_t);
        void update_all_systems();

        /// @brief Returns system of given type.
        /// @tparam TSys The requested system type.
        /// @return The type of system found in SystemManager::AllSystems or nullptr on faliure.
        template <typename TSys>
        TSys *get_system()
        {
            for (auto &sys : this->AllSystems)
            {
                TSys *return_sys = dynamic_cast<TSys *>(sys.get());
                if (return_sys == nullptr)
                {
                    std::cout << "Couldn't cast " << typeid((sys.get())).name() << " to " << typeid(TSys *).name() << std::endl;
                    continue;
                }

                return return_sys;
            }
            return nullptr;
        }
    };
}
#endif //_MSCE_SYSTEM_MANAGER_H_