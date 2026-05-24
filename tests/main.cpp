#include <gtest/gtest.h>
#include <test_configs.h>

#include <MSCE/Managers/systemManager.h>
#include <MSCE/Managers/componentManager.h>
#include <MSCE/Managers/entityManager.h>
#include <MSCE/Managers/prototypeManager.h>
#include <MSCE/Managers/eventManager.h>

#include <chrono>

using namespace msce;

int main(int argc, char **argv)
{
    static auto log = Logger("GLOBAL");
    static SystemManager sysMan;
    static ComponentManager compMan;
    static EntityManager entMan;
    static PrototypeManager protoMan;
    static EventManager evMan;

    log.log_info("Starting tests");
    sysMan.init_all_systems();

    testing::InitGoogleTest(&argc, argv);
    srand(RAND_FUNCTION_SEED);

    int result = RUN_ALL_TESTS();

    log.log_info("Tests finished {}", result == 0 ? "Successfuly" : "Un-successfuly");

    return result;
}