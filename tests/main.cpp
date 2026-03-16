#include <gtest/gtest.h>
#include <test_configs.h>
#include "system_registry.hpp"

int main(int argc, char **argv)
{

    testing::InitGoogleTest(&argc, argv);
    srand(RAND_FUNCTION_SEED);

    register_all();

    SystemManager sysMan;
    sysMan.init_all_systems();

    ComponentManager compMan;
    EntityManager entMan;

    return RUN_ALL_TESTS();
}