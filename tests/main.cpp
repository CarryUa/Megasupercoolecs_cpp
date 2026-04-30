#include <test_configs.h>
#include <gtest/gtest.h>
#include "system_registry.hpp"

#include <chrono>

int main(int argc, char **argv)
{

    testing::InitGoogleTest(&argc, argv);
    srand(RAND_FUNCTION_SEED);

    register_all();

    SystemManager sysMan;
    sysMan.init_all_systems();

    static ComponentManager compMan;
    static EntityManager entMan;
    static PrototypeManager protoMan;
    static Logger log("GLOBAL");

    auto t = std::chrono::high_resolution_clock::now();
    log.log_info("Hello world{}", "!!!!");
    log.log_info("Current runtime is {} nanosec.", std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - t));

    return RUN_ALL_TESTS();
}