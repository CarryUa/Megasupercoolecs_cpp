#ifndef MSCE_TESTS_SYSTEM_REGISTRY_
#define MSCE_TESTS_SYSTEM_REGISTRY_
#include <test_configs.h>

using namespace msce;

void register_all()
{
    SystemManager::register_system<TestSystem1>();
    SystemManager::register_system<System>();
}

#endif // MSCE_TESTS_SYSTEM_REGISTRY