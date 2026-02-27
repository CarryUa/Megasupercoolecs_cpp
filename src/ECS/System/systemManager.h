#ifndef _MSCE_SYSTEM_MANAGER_H_
#define _MSCE_SYSTEM_MANAGER_H_
#include <vector>
#include <map>
#include <typeindex>
#include <functional>
#include <ECS/System/system.h>

class SystemManager
{
private:
    static map<type_index, function<unique_ptr<System>()>> _registered_type_constructor_pairs;

public:
    /// @brief Creates all previously registered systems.
    SystemManager();

    /// @brief List of all existing system instances.
    vector<unique_ptr<System>> AllSystems;

    /// @brief Registers the system for creation during SystemManager construction.
    /// @tparam TSys The type of system being registered.
    template <typename TSys>
    static void RegisterSystem()
    {
        SystemManager::_registered_type_constructor_pairs[typeid(TSys)] = []() -> unique_ptr<System>
        {
            return unique_ptr<System>(make_unique<TSys>().release());
        };
    }

    /// @brief Initializes all system using System.Init();
    void InitAllSystems();

    /// @brief Updated all systems using System.Update(delta_t);
    void UpdateAllSystems();

    /// @brief Returns system of given type.
    /// @tparam TSys The requested system type.
    /// @return The type of system found in SystemManager::AllSystems or nullptr on faliure.
    template <typename TSys>
    TSys *GetSystem()
    {
        for (auto &sys : this->AllSystems)
        {
            TSys *return_sys = dynamic_cast<TSys *>(sys.get());
            if (return_sys == nullptr)
            {
                cout << "couldn't cast " << typeid((sys.get())).name() << " to " << typeid(TSys *).name() << endl;
                continue;
            }

            return return_sys;
        }
        return nullptr;
    }
};
#endif //_MSCE_SYSTEM_MANAGER_H_