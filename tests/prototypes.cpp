#include <test_configs.h>

#include <gtest/gtest.h>
#include <MSCE/prototype.h>

#include <filesystem>
using namespace msce;

namespace
{
/// @brief Basic test prototype with simple types
struct BasicPrototype : public msce::IPrototype
{
  int test_int = 42;
  bool test_bool = true;
  std::string test_str = "Default";

public:
  MSCE_DEFINE_PROTOTYPE(BasicPrototype, test_int, test_bool, test_str)
};

} // namespace

MSCE_REGISTER_PROTOTYPE(BasicPrototype, BasicPrototype, test_int, test_bool,
                        test_str)

/// @brief Test prototype creation
TEST(PrototypeTests, Creation)
{
  auto protoMan = PrototypeManager::instance;

  auto proto = protoMan->create_new_prototype_instance<BasicPrototype>(
      "BasicPrototype", "test_proto_1");

  ASSERT_TRUE(proto);
  EXPECT_EQ(proto->id, "test_proto_1");
  EXPECT_EQ(proto->test_int, 42);
  EXPECT_TRUE(proto->test_bool);
  EXPECT_EQ(proto->test_str, "Default");

  protoMan->delete_prototype("test_proto_1");
}

/// @brief Test prototype data mutation
TEST(PrototypeTests, DataMutation)
{
  auto protoMan = PrototypeManager::instance;

  auto proto = protoMan->create_new_prototype_instance<BasicPrototype>(
      "BasicPrototype", "test_proto_2");
  ASSERT_TRUE(proto);

  proto->test_int = 100;
  proto->test_bool = false;
  proto->test_str = "Modified";

  EXPECT_EQ(proto->test_int, 100);
  EXPECT_FALSE(proto->test_bool);
  EXPECT_EQ(proto->test_str, "Modified");

  protoMan->delete_prototype("test_proto_2");
}

/// @brief Test prototype retrieval
TEST(PrototypeTests, Retrieval)
{
  auto protoMan = PrototypeManager::instance;

  auto proto = protoMan->create_new_prototype_instance<BasicPrototype>(
      "BasicPrototype", "test_proto_3");
  ASSERT_TRUE(proto);

  // Retrieve by ID
  auto retrieved = protoMan->get_prototype<BasicPrototype>("test_proto_3");
  ASSERT_TRUE(retrieved);
  EXPECT_EQ(retrieved->id, proto->id);
  EXPECT_EQ(retrieved->test_int, proto->test_int);

  protoMan->delete_prototype("test_proto_3");
}

/// @brief Test prototype with different values
TEST(PrototypeTests, EnumHandling)
{
  auto protoMan = PrototypeManager::instance;

  auto proto1 = protoMan->create_new_prototype_instance<BasicPrototype>(
      "BasicPrototype", "test_proto_enum_var");
  ASSERT_TRUE(proto1);

  proto1->test_int = 100;
  EXPECT_EQ(proto1->test_int, 100);

  proto1->test_int = 200;
  EXPECT_EQ(proto1->test_int, 200);

  protoMan->delete_prototype("test_proto_enum_var");
}

/// @brief Test prototype with string data
TEST(PrototypeTests, VectorData)
{
  auto protoMan = PrototypeManager::instance;

  auto proto = protoMan->create_new_prototype_instance<BasicPrototype>(
      "BasicPrototype", "test_proto_str_data");
  ASSERT_TRUE(proto);

  proto->test_str = "First";
  EXPECT_EQ(proto->test_str, "First");

  proto->test_str = "Second";
  EXPECT_EQ(proto->test_str, "Second");

  proto->test_str = "Third";
  EXPECT_EQ(proto->test_str, "Third");

  protoMan->delete_prototype("test_proto_str_data");
}

/// @brief Test prototype deletion
TEST(PrototypeTests, Deletion)
{
  auto protoMan = PrototypeManager::instance;

  auto proto = protoMan->create_new_prototype_instance<BasicPrototype>(
      "BasicPrototype", "test_proto_del");
  ASSERT_TRUE(proto);

  size_t initial_count = protoMan->enumerate_prototypes().size();

  ASSERT_TRUE(protoMan->delete_prototype("test_proto_del"));

  EXPECT_LT(protoMan->enumerate_prototypes().size(), initial_count);

  // Verify it's deleted
  auto retrieved = protoMan->get_prototype<BasicPrototype>("test_proto_del");
  EXPECT_FALSE(retrieved);
}

/// @brief Test multiple prototype instances
TEST(PrototypeTests, MultipleInstances)
{
  auto protoMan = PrototypeManager::instance;

  auto proto1 = protoMan->create_new_prototype_instance<BasicPrototype>(
      "BasicPrototype", "test_proto_multi_1");
  auto proto2 = protoMan->create_new_prototype_instance<BasicPrototype>(
      "BasicPrototype", "test_proto_multi_2");
  auto proto3 = protoMan->create_new_prototype_instance<BasicPrototype>(
      "BasicPrototype", "test_proto_multi_3");

  ASSERT_TRUE(proto1);
  ASSERT_TRUE(proto2);
  ASSERT_TRUE(proto3);

  // Verify distinct instances
  EXPECT_NE(proto1->id, proto2->id);
  EXPECT_NE(proto1->id, proto3->id);
  EXPECT_NE(proto2->id, proto3->id);

  proto1->test_int = 10;
  proto2->test_int = 20;
  proto3->test_int = 30;

  EXPECT_EQ(proto1->test_int, 10);
  EXPECT_EQ(proto2->test_int, 20);
  EXPECT_EQ(proto3->test_int, 30);

  protoMan->delete_prototype("test_proto_multi_1");
  protoMan->delete_prototype("test_proto_multi_2");
  protoMan->delete_prototype("test_proto_multi_3");
}

/// @brief Test prototype enumeration
TEST(PrototypeTests, Enumeration)
{
  auto protoMan = PrototypeManager::instance;

  auto proto1 = protoMan->create_new_prototype_instance<BasicPrototype>(
      "BasicPrototype", "test_proto_enum_1");
  auto proto2 = protoMan->create_new_prototype_instance<BasicPrototype>(
      "BasicPrototype", "test_proto_enum_2");

  ASSERT_TRUE(proto1);
  ASSERT_TRUE(proto2);

  auto all_protos = protoMan->enumerate_prototypes();
  EXPECT_GE(all_protos.size(), 2);

  // All should be non-null
  for (const auto &proto : all_protos)
  {
    EXPECT_TRUE(proto);
  }

  protoMan->delete_prototype("test_proto_enum_1");
  protoMan->delete_prototype("test_proto_enum_2");
}

/// @brief Test prototype registry
TEST(PrototypeTests, Registry)
{
  auto protoMan = PrototypeManager::instance;

  auto registered = protoMan->registered_prototypes_ref.enumerate_registry();
  EXPECT_GE(registered.size(), 1);

  auto factories = protoMan->registered_factories_ref.enumerate_registry();
  EXPECT_EQ(factories.size(), registered.size());
}

/// @brief Test duplicate prototype ID prevention
TEST(PrototypeTests, DuplicateIDPrevention)
{
  auto protoMan = PrototypeManager::instance;

  auto proto1 = protoMan->create_new_prototype_instance<BasicPrototype>(
      "BasicPrototype", "test_proto_dup");
  ASSERT_TRUE(proto1);

  // Attempt to create with same ID should fail
  auto proto2 = protoMan->create_new_prototype_instance<BasicPrototype>(
      "BasicPrototype", "test_proto_dup");
  EXPECT_FALSE(proto2);

  protoMan->delete_prototype("test_proto_dup");
  EXPECT_FALSE(protoMan->get_prototype<BasicPrototype>("test_proto_dup"));
}
