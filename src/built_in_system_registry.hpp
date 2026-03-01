#ifndef _MSCE_BUILT_IN_SYSTEM_REGISTRY_H_
#define _MSCE_BUILT_IN_SYSTEM_REGISTRY_H_
#include <ECS/system.h>
#include <Managers/systemManager.h>

using namespace msce;

void register_built_in_systems()
{
    SystemManager::register_system<System>();
}

#endif
