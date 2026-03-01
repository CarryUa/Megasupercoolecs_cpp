#ifndef _MSCE_SYSTEM_REGISTRY_HPP_
#define _MSCE_SYSTEM_REGISTRY_HPP_
#include <ECS/Managers/systemManager.h>
using namespace msce;

void register_all_systems()
{
    SystemManager::register_system<System>();
}

#endif // _MSCE_SYSTEM_REGISTRY_HPP_
