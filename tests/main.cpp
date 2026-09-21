#include <gtest/gtest.h>
#include <test_configs.h>

#include <MSCE/system.h>
#include <MSCE/entity.h>
#include <MSCE/component.h>

#include <MSCE/prototype.h>
#include <MSCE/event.h>
#include <MSCE/Types/enum.h>

#include <chrono>

using namespace msce;

int main(int argc, char **argv)
{
  static auto log = Logger("TESTS");
  static EventManager evMan;
  static SystemManager sysMan;
  static ComponentManager compMan;
  static EntityManager entMan;
  static PrototypeManager protoMan;
  static EnumManager enum_man;

  log.log_info("Starting tests");
  sysMan.init_all_systems();

  testing::InitGoogleTest(&argc, argv);
  srand(RAND_FUNCTION_SEED);

  int result = RUN_ALL_TESTS();

  log.log_info("Tests finished {}",
               result == 0 ? "Successfuly" : "Un-successfuly");

  return result;
}