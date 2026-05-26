#ifndef _MSCE_SYSTEM_MANAGER_H_
#define _MSCE_SYSTEM_MANAGER_H_
#include <MSCE/Types/singleton.hpp>
#include <MSCE/logger.h>
#include <MSCE/Types/Collections/registry.hpp>
#include <vector>
#include <map>
#include <memory>
#include <typeindex>
#include <functional>
#include <iostream>
#include <MSCE/ECS/system.h>
#include <MSCE/BuiltIns/timeSystem.h>

namespace msce
{

    /// @brief Manages the life-cycle of all registered systems.
    /// All systems should be registered before SystemManager is created.
    /// Only one instance of SystemManager can exist in a programs, or else constructor will throw std::runtime_error.
    class SystemManager : public Singleton<SystemManager>
    {
    private:
        inline static Logger logger = Logger("SystemManager");
        Registry<std::type_index, std::function<std::unique_ptr<System>()>> &system_registry_ref_;
        TimeSystem *time_sys_;

    public:
        /// @brief Creates all previously registered systems.
        /// @exception throws is another instance of SystemManager exists in the program.
        SystemManager();

        /// @brief List of all existing system instances.
        std::vector<std::unique_ptr<System>> all_systems;

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
            for (auto &sys : this->all_systems)
            {
                TSys *return_sys = dynamic_cast<TSys *>(sys.get());
                if (return_sys == nullptr)
                    continue;

                return return_sys;
            }
            return nullptr;
        }
    };
}

#endif //_MSCE_SYSTEM_MANAGER_H_