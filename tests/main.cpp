#include "system_registry.h"
#include <gtest/gtest.h>
#include <test_constants.h>

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    // Random seed, i just rolled my face all over the keyboard
    srand(RAND_FUNCTION_SEED);

    register_all();

    return RUN_ALL_TESTS();
}