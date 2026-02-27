#include "system_registry.h"
#include <gtest/gtest.h>

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    // Random seed, i just rolled my face all over the keyboard
    srand(0x81049854);

    register_all();

    return RUN_ALL_TESTS();
}