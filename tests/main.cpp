#include <gtest/gtest.h>
#include <test_configs.h>

#include <MSCE/Managers/componentManager.h>
#include <MSCE/Managers/entityManager.h>
#include <MSCE/Managers/systemManager.h>
#include <MSCE/Managers/prototypeManager.h>

#include <chrono>

using namespace msce;

int main(int argc, char **argv)
{

    testing::InitGoogleTest(&argc, argv);
    srand(RAND_FUNCTION_SEED);

    SystemManager sysMan;
    sysMan.init_all_systems();

    static ComponentManager compMan;
    static EntityManager entMan;
    static PrototypeManager protoMan;
    static Logger log("GLOBAL");

    auto t = std::chrono::high_resolution_clock::now();
    log.log_info("Hello world{}", "!!!!");
    log.log_info("Current runtime is {} nanosec.", std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - t));

    int result = RUN_ALL_TESTS();

    log.log_info("Tests finished {}", result == 0 ? "Successfuly" : "Un-successfuly");

    return result;
}