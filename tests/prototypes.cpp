#include <test_configs.h>
using namespace msce;

TEST(PrototypeTests, PrototypeSerializationTest)
{
    auto protoMan = PrototypeManager::instance;
    const std::string cereal_file_path = "/home/carry/test_prototype.cereal";

    std::unique_ptr<IPrototype> tp1 = std::make_unique<TestPrototype1>();
    tp1->id = "test_proto_1"; // Set the base class member

    protoMan->serialize_prototype(cereal_file_path, tp1);

    protoMan->deserialize_prototype(cereal_file_path);
    TestPrototype1 *tp1_deserialized = protoMan->get_prototype<TestPrototype1>("test_proto_1");

    TestPrototype1 default_tp = TestPrototype1();

    EXPECT_EQ(default_tp.id, tp1_deserialized->id);
}