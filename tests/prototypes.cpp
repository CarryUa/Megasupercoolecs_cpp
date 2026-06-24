#include <test_configs.h>

#include <gtest/gtest.h>
#include <MSCE/Prototypes/prototype.hpp>
#include <MSCE/Managers/prototypeManager.h>

#include <filesystem>
using namespace msce;

enum class TestEnum : int
{
    Bebebe = 123123,
    BRUH = 55555,
    NONONO = -1
};
MSCE_CEREAL_GENERATE_ENUM_SERIALIZE_METHODS(TestEnum, Bebebe, BRUH, NONONO)

struct TestPrototype1 : public msce::IPrototype
{
    int test_int = 15556;
    bool test_bool = true;
    TestEnum test_enum = TestEnum::NONONO;

    std::string test_str = "Hello World!";

public:
    MSCE_DEFINE_PROTOTYPE(TestPrototype1, test_int, test_bool, test_str)
};
MSCE_REGISTER_PROTOTYPE(TestPrototype1, TestPrototype1, test_int, test_bool, test_str)

TEST(PrototypeTests, PrototypeSerializationTest)
{
    auto protoMan = PrototypeManager::instance;
    const std::string cereal_file_path = "/tmp/test_prototype.cereal0";

    auto tp1 = PrototypeManager::instance->create_new_prototype_instance<TestPrototype1>("TestPrototype1", "test_prototype_0");
    tp1->test_enum = TestEnum::Bebebe;
    protoMan->serialize_prototype(cereal_file_path, tp1->id);
    PrototypeManager::instance->delete_prototype("test_prototype_0");

    protoMan->deserialize_prototype(cereal_file_path);
    TestPrototype1 *tp1_deserialized = protoMan->get_prototype<TestPrototype1>("test_prototype_0");

    TestPrototype1 *tp1_serialized = dynamic_cast<TestPrototype1 *>(tp1);

    EXPECT_EQ(tp1_serialized->id, tp1_deserialized->id);
    EXPECT_EQ(tp1_serialized->test_bool, tp1_deserialized->test_bool);
    EXPECT_EQ(tp1_serialized->test_int, tp1_deserialized->test_int);
    EXPECT_EQ(tp1_serialized->test_str, tp1_deserialized->test_str);
    EXPECT_EQ(tp1_serialized->test_enum, tp1_deserialized->test_enum);
    PrototypeManager::instance->delete_prototype("test_prototype_0");
}

TEST(PrototypeTests, PrototypeFileConsistencyTest)
{
    auto protoMan = PrototypeManager::instance;
    const std::string cereal_file_path = "/tmp/test_prototype.cereal";

    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {
        IPrototype *tp = PrototypeManager::instance->create_new_prototype_instance("TestPrototype1", "test_prototype_" + std::to_string(i));
        protoMan->serialize_prototype(cereal_file_path + std::to_string(i), tp->id);
        protoMan->deserialize_prototype(cereal_file_path + std::to_string(i));
        ASSERT_EQ(std::filesystem::remove((cereal_file_path + std::to_string(i)).c_str()), true);
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
    EXPECT_GE(protoMan->registered_prototypes_ref.enumerate_registry().size(), 1);

    EXPECT_EQ(protoMan->registered_factories_ref.enumerate_registry().size(),
              protoMan->registered_prototypes_ref.enumerate_registry().size());
}

TEST(PrototypeTests, PrototypeByIdCreationTest)
{
    auto protoMan = PrototypeManager::instance;

    auto dtp1 = protoMan->create_new_prototype_instance("TestPrototype1", "dynamic_test_prototype_1");

    auto dtp1_concrete = protoMan->create_new_prototype_instance<TestPrototype1>("TestPrototype1", "dynamic_test_prototype_concrete_1");

    EXPECT_NE(dtp1, nullptr) << "Something went wrong during prototype creation. See the log output for details.";
    EXPECT_NE(dtp1_concrete, nullptr) << "Something went wrong during prototype creation. See the log output for details.";

    EXPECT_EQ(dtp1->id, "dynamic_test_prototype_1") << "The id wasn't properly assigned to new prototype!";
    EXPECT_EQ(dtp1_concrete->id, "dynamic_test_prototype_concrete_1") << "The id wasn't properly assigned to new prototype with template implementation!";

    auto dtp1_same_id = protoMan->create_new_prototype_instance("TestPrototype1", "dynamic_test_prototype_1");
    EXPECT_EQ(dtp1_same_id, nullptr) << "New prototype has probably overwritten the old one with same Id!";
}

TEST(PrototypeTests, PrototypeDeletionTest)
{
    auto protoMan = PrototypeManager::instance;

    auto new_proto = protoMan->create_new_prototype_instance<TestPrototype1>("TestPrototype1", "deletion_test_prototype_1");
    size_t start_size = protoMan->enumerate_prototypes().size();

    ASSERT_GE(start_size, 1) << "Prototypes count must be non-zero at this point of testing.";
    protoMan->delete_prototype(new_proto->id);
    EXPECT_LT(protoMan->enumerate_prototypes().size(), start_size) << "By-id deletion failed!";

    try
    {
        new_proto = protoMan->create_new_prototype_instance<TestPrototype1>("TestPrototype1", "deletion_test_prototype_1");
        start_size = protoMan->enumerate_prototypes().size();

        protoMan->delete_prototype(new_proto);
        EXPECT_LT(protoMan->enumerate_prototypes().size(), start_size) << "By-pointer deletion failed!";
    }
    catch (std::exception &ex)
    {
        FAIL() << "By-id failed, used id is still taken-up.";
    }
}
