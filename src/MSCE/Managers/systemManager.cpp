#include "systemManager.h"
#include <MSCE/built_in_system_registry.hpp>
#include <typeindex>

using namespace std;
using namespace msce;

map<type_index, function<unique_ptr<System>()>> SystemManager::_registered_type_constructor_pairs;

SystemManager::SystemManager()
{

    register_built_in_systems();

    for (const auto &pair : SystemManager::_registered_type_constructor_pairs)
    {
        this->AllSystems.push_back(pair.second());
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
