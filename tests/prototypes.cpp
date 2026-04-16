#include <test_configs.h>
using namespace msce;

TEST(PrototypeTests, PrototypeSerializationTest)
{
    auto protoMan = PrototypeManager::instance;
    const std::string cereal_file_path = "/tmp/test_prototype.cereal0";

    std::unique_ptr<IPrototype> tp1 = std::make_unique<TestPrototype1>();
    tp1->id = "test_prototype_0"; // Set the base class member

    protoMan->serialize_prototype(cereal_file_path, tp1);

    protoMan->deserialize_prototype(cereal_file_path);
    TestPrototype1 *tp1_deserialized = protoMan->get_prototype<TestPrototype1>("test_prototype_0");

    TestPrototype1 default_tp = TestPrototype1();

    EXPECT_NE(default_tp.id, tp1_deserialized->id);
}

TEST(PrototypeTests, PrototypeEnumerationTest)
{
    auto protoMan = PrototypeManager::instance;
    const std::string cereal_file_path = "/tmp/test_prototype.cereal";

    std::unique_ptr<IPrototype> tp = std::make_unique<TestPrototype1>();
    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {
        tp->id = "test_prototype_" + std::to_string(i);
        protoMan->serialize_prototype(cereal_file_path + std::to_string(i), tp);
    }

    for (size_t i = 1; i < TEST_ITERATIONS; i++)
    {
        protoMan->deserialize_prototype(cereal_file_path + std::to_string(i));
    }

    const auto prototypes = protoMan->enumerate_prototypes();
    EXPECT_EQ(prototypes.size(), TEST_ITERATIONS);

    for (const auto &p : prototypes)
    {
        EXPECT_NE(p, nullptr);
    }
}

TEST(PrototypeTests, PrototypeRegistryTest)
{
    auto protoMan = PrototypeManager::instance;
    EXPECT_GE(protoMan->registered_prototypes.enumerate_registry().size(), 1);

    EXPECT_EQ(protoMan->registered_factories.enumerate_registry().size(),
              protoMan->registered_prototypes.enumerate_registry().size());
}

TEST(PrototypeTests, PrototypeByIdCreationTest)
{
    auto protoMan = PrototypeManager::instance;

    auto dtp1 = protoMan->instantiate_prototype("TestPrototype1", "dynamic_test_prototype_1");

    auto dtp1_concrete = protoMan->instantiate_prototype<TestPrototype1>("TestPrototype1", "dynamic_test_prototype_concrete_1");

    EXPECT_NE(dtp1, nullptr) << "Something went wrong during prototype creation. See the log output for details.";
    EXPECT_NE(dtp1_concrete, nullptr) << "Something went wrong during prototype creation. See the log output for details.";

    EXPECT_EQ(dtp1->id, "dynamic_test_prototype_1") << "The id wasn't properly assigned to new prototype!";
    EXPECT_EQ(dtp1_concrete->id, "dynamic_test_prototype_concrete_1") << "The id wasn't properly assigned to new prototype with template implementation!";

    auto dtp1_same_id = protoMan->instantiate_prototype("TestPrototype1", "dynamic_test_prototype_1");
    EXPECT_EQ(dtp1_same_id, nullptr) << "New prototype has probably overwritten the old one with same Id!";
}

TEST(PrototypeTests, PrototypeDeletionTest)
{
    auto protoMan = PrototypeManager::instance;

    auto new_proto = protoMan->instantiate_prototype<TestPrototype1>("TestPrototype1", "deletion_test_prototype_1");
    size_t start_size = protoMan->enumerate_prototypes().size();

    ASSERT_GE(start_size, 1) << "Prototypes count must be non-zero at this point of testing.";
    protoMan->delete_prototype(new_proto->id);
    EXPECT_LT(protoMan->enumerate_prototypes().size(), start_size) << "By-id deletion failed!";

    try
    {
        new_proto = protoMan->instantiate_prototype<TestPrototype1>("TestPrototype1", "deletion_test_prototype_1");
        start_size = protoMan->enumerate_prototypes().size();

        protoMan->delete_prototype(new_proto);
        EXPECT_LT(protoMan->enumerate_prototypes().size(), start_size) << "By-pointer deletion failed!";
    }
    catch (std::exception &ex)
    {
        FAIL() << "By-id failed, used id is still taken-up.";
    }
}
