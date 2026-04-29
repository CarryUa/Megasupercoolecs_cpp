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

    EXPECT_EQ(twr.test_bool, std::any_cast<bool>(twr.get_field_any("test_bool")));
    EXPECT_EQ(twr.test_int, std::any_cast<int>(twr.get_field_any("test_int")));
    EXPECT_EQ(twr.test_str, std::any_cast<std::string>(twr.get_field_any("test_str")));

    twr.set_field_any("test_bool", std::any(false));
    EXPECT_EQ(twr.test_bool, false);

    twr.set_field_any("test_int", std::any(42));
    EXPECT_EQ(twr.test_int, 42);

    twr.set_field_any("test_str", std::any(std::string("Updated String")));
    EXPECT_EQ(twr.test_str, "Updated String");

    twr.set_field_any("test_bool", std::any(true));
    EXPECT_EQ(twr.test_bool, true);
    EXPECT_EQ(twr.get_field<bool>("test_bool"), true);

    twr.set_field_any("test_int", std::any(999));
    EXPECT_EQ(twr.get_field<int>("test_int"), 999);

    twr.set_field_any("test_str", std::any(std::string("New Value")));
    EXPECT_EQ(twr.get_field<std::string>("test_str"), "New Value");

    delete[] str_value_start;
}

TEST(ReflectionTests, RTTI_Test)
{
    auto twrd = new TestTypeWithReflectionDerived();
    auto downcast = dynamic_cast<TestTypeWithReflection *>(twrd);

    auto base_class_fields = TestTypeWithReflection::get_field_name_type_pairs_static();
    auto derived_class_fields = downcast->get_field_name_type_pairs();

    EXPECT_NE(base_class_fields.size(), derived_class_fields.size()) << "RTTI-deduced derived reflection returned same amount of fields, as base class.";
    EXPECT_EQ(twrd->get_field_name_type_pairs_static().size(), derived_class_fields.size());
    EXPECT_EQ(downcast->get_field_name_type_pairs_static().size(), base_class_fields.size());

    auto derived_fields_checklist = TestTypeWithReflectionDerived::get_field_name_type_pairs_static();

    for (const auto &[name, type] : derived_class_fields)
    {
        derived_fields_checklist.erase(name);
    }
    EXPECT_EQ(derived_fields_checklist.size(), 0) << "Some or all fields are missing in TestTypeWithReflectionDerived RTTI output.";

    EXPECT_EQ(downcast->get_field<int>("test_int"), twrd->test_int);
    EXPECT_EQ(downcast->get_field<bool>("test_bool"), twrd->test_bool);
    EXPECT_EQ(downcast->get_field<std::string>("test_str"), twrd->test_str);

    // get_field() isn't virtual.
    // EXPECT_EQ(downcast->get_field<int>("derived_test_int"), twrd->derived_test_int);
    // EXPECT_EQ(downcast->get_field<bool>("derived_test_bool"), twrd->derived_test_bool);
    // EXPECT_EQ(downcast->get_field<std::string>("derived_test_str"), twrd->derived_test_str);

    downcast->set_field("test_int", 99999);
    EXPECT_EQ(twrd->test_int, 99999);

    downcast->set_field("test_bool", false);
    EXPECT_EQ(twrd->test_bool, false);

    downcast->set_field("test_str", std::string("Modified base field"));
    EXPECT_EQ(twrd->test_str, "Modified base field");

    EXPECT_EQ(std::any_cast<int>(downcast->get_field_any("test_int")), twrd->test_int);
    EXPECT_EQ(std::any_cast<bool>(downcast->get_field_any("test_bool")), twrd->test_bool);
    EXPECT_EQ(std::any_cast<std::string>(downcast->get_field_any("test_str")), twrd->test_str);

    EXPECT_EQ(std::any_cast<int>(downcast->get_field_any("derived_test_int")), twrd->derived_test_int);
    EXPECT_EQ(std::any_cast<bool>(downcast->get_field_any("derived_test_bool")), twrd->derived_test_bool);
    EXPECT_EQ(std::any_cast<std::string>(downcast->get_field_any("derived_test_str")), twrd->derived_test_str);

    downcast->set_field_any("test_int", std::any(55555));
    EXPECT_EQ(twrd->test_int, 55555);

    downcast->set_field_any("test_bool", std::any(true));
    EXPECT_EQ(twrd->test_bool, true);

    downcast->set_field_any("test_str", std::any(std::string("Any modified base")));
    EXPECT_EQ(twrd->test_str, "Any modified base");

    downcast->set_field_any("derived_test_int", std::any(77777));
    EXPECT_EQ(twrd->derived_test_int, 77777);

    downcast->set_field_any("derived_test_bool", std::any(false));
    EXPECT_EQ(twrd->derived_test_bool, false);

    downcast->set_field_any("derived_test_str", std::any(std::string("Any modified derived")));
    EXPECT_EQ(twrd->derived_test_str, "Any modified derived");

    delete twrd;
}

#undef STRINGIFY