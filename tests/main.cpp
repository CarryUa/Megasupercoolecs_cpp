#include <test_configs.h>
#include <gtest/gtest.h>
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
    PrototypeManager protoMan;

    return RUN_ALL_TESTS();
}