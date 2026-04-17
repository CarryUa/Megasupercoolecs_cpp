#include "test_configs.h"

#define STRINGIFY(x) #x

TEST(ReflectionTests, BasicMethodsTest)
{
    auto twr = TestTypeWithReflection();

    ASSERT_EQ(twr.get_field_name_type_pairs().size(), 3) << "Some or all fields of testing object weren't picked up by reflection.";

    EXPECT_EQ(twr.get_unmangled_type_name(), STRINGIFY(TestTypeWithReflection));

    EXPECT_EQ(twr.test_bool, twr.get_field<bool>("test_bool"));
    EXPECT_EQ(twr.test_int, twr.get_field<int>("test_int"));
    EXPECT_EQ(twr.test_str, twr.get_field<std::string>("test_str"));

    bool bool_value_start = twr.test_bool;
    int int_value_start = twr.test_int;
    char *str_value_start = new char[twr.test_str.size()];
    twr.test_str.copy(str_value_start, twr.test_str.size(), 0);

    twr.set_field("test_bool", !twr.test_bool);
    EXPECT_NE(bool_value_start, twr.test_bool);

    twr.set_field("test_str", twr.test_str + "Hello World!");
    EXPECT_NE(str_value_start, twr.test_str);

    twr.set_field("test_int", twr.test_int + 1);
    EXPECT_NE(int_value_start, twr.test_int);

    delete[] str_value_start;
}

TEST(ReflectionTests, RTTI_Test)
{
    auto twrd = new TestTypeWithReflectionDerived();
    auto downcast = dynamic_cast<TestTypeWithReflection *>(twrd);

    auto base_class_fields = downcast->get_field_name_type_pairs_static();
    auto derived_class_fields = downcast->get_field_name_type_pairs();

    EXPECT_NE(base_class_fields.size(), derived_class_fields.size()) << "RTTI-deduced derived reflection returned same amount of fields, as base class.";
    EXPECT_EQ(twrd->get_field_name_type_pairs_static().size(), derived_class_fields.size());
    EXPECT_EQ(downcast->get_field_name_type_pairs_static().size(), base_class_fields.size());

    delete twrd;
}

#undef STRINGIFY