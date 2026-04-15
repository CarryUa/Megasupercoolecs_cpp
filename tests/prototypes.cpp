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

    EXPECT_EQ(default_tp.id, tp1_deserialized->id);
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
}

TEST(PrototypeTests, ByNameFieldsTest)
{
    auto protoMan = PrototypeManager::instance;

    auto tp1 = *(protoMan->enumerate_prototypes<TestPrototype1>().begin());

    EXPECT_EQ(tp1->test_bool, tp1->get_field<bool>("test_bool"));
    EXPECT_EQ(tp1->test_int, tp1->get_field<int>("test_int"));
    EXPECT_EQ(tp1->test_str, tp1->get_field<std::string>("test_str"));

    bool bool_value_start = tp1->test_bool;
    int int_value_start = tp1->test_int;
    char *str_value_start = new char[tp1->test_str.size()];
    tp1->test_str.copy(str_value_start, tp1->test_str.size(), 0);

    tp1->set_field("test_bool", !tp1->test_bool);
    EXPECT_NE(bool_value_start, tp1->test_bool);

    tp1->set_field("test_str", tp1->test_str + "Hello World!");
    EXPECT_NE(str_value_start, tp1->test_str);

    tp1->set_field("test_int", tp1->test_int + 1);
    EXPECT_NE(int_value_start, tp1->test_int);

    delete[] str_value_start;
}