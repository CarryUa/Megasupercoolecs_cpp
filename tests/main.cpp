#include "system_registry.hpp"
#include <gtest/gtest.h>
#include <test_constants.h>

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    srand(RAND_FUNCTION_SEED);

    register_all();

    return RUN_ALL_TESTS();
}