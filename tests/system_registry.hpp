#ifndef _MSCE_TESTS_SYSTEM_REGISTRY_
#define _MSCE_TESTS_SYSTEM_REGISTRY_
#include <Managers/systemManager.h>
#include <ECS/system.h>

using namespace msce;

void register_all()
{
    SystemManager::register_system<System>();
}

#endif //_MSCE_TESTS_SYSTEM_REGISTRY