#include "systemManager.h"
#include <MSCE/built_in_system_registry.hpp>
#include <typeindex>

using namespace std;
using namespace msce;

Registry<std::type_index, std::function<std::unique_ptr<System>()>> SystemManager::_system_registry;

SystemManager::SystemManager()
{

    register_built_in_systems();

    for (const auto &[type, constructor] : SystemManager::_system_registry.enumerate_registry())
    {
        this->AllSystems.push_back(constructor());
        this->AllSystems.back().get()->p_sys_man = this;
    }

    this->_time_sys = this->get_system<TimeSystem>();
}

void SystemManager::init_all_systems()
{
    for (const auto &system : this->AllSystems)
    {
        system->pre_init();
    }
    for (const auto &system : this->AllSystems)
    {
        system->init();
    }
}

void SystemManager::update_all_systems()
{
    for (auto &system : this->AllSystems)
    {
        if (system->active)
            system->update(this->_time_sys->get_delta_time());
    }
}
