#include <gtest/gtest.h>
#include <test_configs.h>
#include <MSCE/msce_macros.h>
#include <MSCE/Reflection/reflection.h>

struct TestTypeWithReflection
{
    int test_int = 15556;
    bool test_bool = true;
    std::string test_str = "Hello World!";

    static const ::msce::Type &type_info;
};
MSCE_INITIALIZE_REFLECTED_OBJECT(::TestTypeWithReflection, test_int, test_bool, test_str)

TEST(ReflectionTests, IntegralsTest)
{
    const auto &t_char = msce::get_reflection_of_type<char>();
    const auto &t_uchar = msce::get_reflection_of_type<unsigned char>();
    const auto &t_short = msce::get_reflection_of_type<short>();
    const auto &t_ushort = msce::get_reflection_of_type<unsigned short>();
    const auto &t_int = msce::get_reflection_of_type<int>();
    const auto &t_uint = msce::get_reflection_of_type<unsigned int>();
    const auto &t_long = msce::get_reflection_of_type<long>();
    const auto &t_ulong = msce::get_reflection_of_type<unsigned long>();

    EXPECT_TRUE(t_char.is_numeric());
    EXPECT_TRUE(t_char.is_integer());
    EXPECT_FALSE(t_char.is_unsigned());
    EXPECT_FALSE(t_char.is_class());
    EXPECT_FALSE(t_char.is_pointer());
    EXPECT_FALSE(t_char.is_reference());
    EXPECT_FALSE(t_char.is_array());
    EXPECT_FALSE(t_char.is_enum());
    EXPECT_FALSE(t_char.is_function());
    EXPECT_FALSE(t_char.is_void());

    EXPECT_TRUE(t_uchar.is_numeric());
    EXPECT_TRUE(t_uchar.is_integer());
    EXPECT_TRUE(t_uchar.is_unsigned());
    EXPECT_FALSE(t_uchar.is_class());
    EXPECT_FALSE(t_uchar.is_pointer());
    EXPECT_FALSE(t_uchar.is_reference());
    EXPECT_FALSE(t_uchar.is_array());
    EXPECT_FALSE(t_uchar.is_enum());
    EXPECT_FALSE(t_uchar.is_function());
    EXPECT_FALSE(t_uchar.is_void());

    EXPECT_TRUE(t_short.is_numeric());
    EXPECT_TRUE(t_short.is_integer());
    EXPECT_FALSE(t_short.is_unsigned());
    EXPECT_FALSE(t_short.is_class());
    EXPECT_FALSE(t_short.is_pointer());
    EXPECT_FALSE(t_short.is_reference());
    EXPECT_FALSE(t_short.is_array());
    EXPECT_FALSE(t_short.is_enum());
    EXPECT_FALSE(t_short.is_function());
    EXPECT_FALSE(t_short.is_void());

    EXPECT_TRUE(t_ushort.is_numeric());
    EXPECT_TRUE(t_ushort.is_integer());
    EXPECT_TRUE(t_ushort.is_unsigned());
    EXPECT_FALSE(t_ushort.is_class());
    EXPECT_FALSE(t_ushort.is_pointer());
    EXPECT_FALSE(t_ushort.is_reference());
    EXPECT_FALSE(t_ushort.is_array());
    EXPECT_FALSE(t_ushort.is_enum());
    EXPECT_FALSE(t_ushort.is_function());
    EXPECT_FALSE(t_ushort.is_void());

    EXPECT_TRUE(t_int.is_numeric());
    EXPECT_TRUE(t_int.is_integer());
    EXPECT_FALSE(t_int.is_unsigned());
    EXPECT_FALSE(t_int.is_class());
    EXPECT_FALSE(t_int.is_pointer());
    EXPECT_FALSE(t_int.is_reference());
    EXPECT_FALSE(t_int.is_array());
    EXPECT_FALSE(t_int.is_enum());
    EXPECT_FALSE(t_int.is_function());
    EXPECT_FALSE(t_int.is_void());

    EXPECT_TRUE(t_uint.is_numeric());
    EXPECT_TRUE(t_uint.is_integer());
    EXPECT_TRUE(t_uint.is_unsigned());
    EXPECT_FALSE(t_uint.is_class());
    EXPECT_FALSE(t_uint.is_pointer());
    EXPECT_FALSE(t_uint.is_reference());
    EXPECT_FALSE(t_uint.is_array());
    EXPECT_FALSE(t_uint.is_enum());
    EXPECT_FALSE(t_uint.is_function());
    EXPECT_FALSE(t_uint.is_void());

    EXPECT_TRUE(t_long.is_numeric());
    EXPECT_TRUE(t_long.is_integer());
    EXPECT_FALSE(t_long.is_unsigned());
    EXPECT_FALSE(t_long.is_class());
    EXPECT_FALSE(t_long.is_pointer());
    EXPECT_FALSE(t_long.is_reference());
    EXPECT_FALSE(t_long.is_array());
    EXPECT_FALSE(t_long.is_enum());
    EXPECT_FALSE(t_long.is_function());
    EXPECT_FALSE(t_long.is_void());

    EXPECT_TRUE(t_ulong.is_numeric());
    EXPECT_TRUE(t_ulong.is_integer());
    EXPECT_TRUE(t_ulong.is_unsigned());
    EXPECT_FALSE(t_ulong.is_class());
    EXPECT_FALSE(t_ulong.is_pointer());
    EXPECT_FALSE(t_ulong.is_reference());
    EXPECT_FALSE(t_ulong.is_array());
    EXPECT_FALSE(t_ulong.is_enum());
    EXPECT_FALSE(t_ulong.is_function());
    EXPECT_FALSE(t_ulong.is_void());
}
TEST(ReflectionTests, FloatingPointTest)
{
    const auto &t_float = msce::get_reflection_of_type<float>();
    const auto &t_double = msce::get_reflection_of_type<double>();
    const auto &t_ldouble = msce::get_reflection_of_type<long double>();

    EXPECT_TRUE(t_float.is_numeric());
    EXPECT_TRUE(t_float.is_floating_point());
    EXPECT_FALSE(t_float.is_unsigned());
    EXPECT_FALSE(t_float.is_class());
    EXPECT_FALSE(t_float.is_pointer());
    EXPECT_FALSE(t_float.is_reference());
    EXPECT_FALSE(t_float.is_array());
    EXPECT_FALSE(t_float.is_enum());
    EXPECT_FALSE(t_float.is_function());
    EXPECT_FALSE(t_float.is_void());

    EXPECT_TRUE(t_double.is_numeric());
    EXPECT_TRUE(t_double.is_floating_point());
    EXPECT_FALSE(t_double.is_unsigned());
    EXPECT_FALSE(t_double.is_class());
    EXPECT_FALSE(t_double.is_pointer());
    EXPECT_FALSE(t_double.is_reference());
    EXPECT_FALSE(t_double.is_array());
    EXPECT_FALSE(t_double.is_enum());
    EXPECT_FALSE(t_double.is_function());
    EXPECT_FALSE(t_double.is_void());

    EXPECT_TRUE(t_ldouble.is_numeric());
    EXPECT_TRUE(t_ldouble.is_floating_point());
    EXPECT_FALSE(t_ldouble.is_unsigned());
    EXPECT_FALSE(t_ldouble.is_class());
    EXPECT_FALSE(t_ldouble.is_pointer());
    EXPECT_FALSE(t_ldouble.is_reference());
    EXPECT_FALSE(t_ldouble.is_array());
    EXPECT_FALSE(t_ldouble.is_enum());
    EXPECT_FALSE(t_ldouble.is_function());
    EXPECT_FALSE(t_ldouble.is_void());
}

TEST(ReflectionTests, CompoundTest)
{
    TestTypeWithReflection t;
    EXPECT_EQ(t.type_info, msce::get_reflection_of_type<TestTypeWithReflection>());
    EXPECT_EQ(t.type_info, msce::get_reflection_of_type("::TestTypeWithReflection"));
    EXPECT_EQ(t.type_info, msce::get_reflection_of_type(typeid(TestTypeWithReflection)));

    EXPECT_TRUE(t.type_info.is_class());
    EXPECT_FALSE(t.type_info.is_numeric());
    EXPECT_FALSE(t.type_info.is_pointer());
    EXPECT_FALSE(t.type_info.is_reference());
    EXPECT_FALSE(t.type_info.is_array());
    EXPECT_FALSE(t.type_info.is_enum());
    EXPECT_FALSE(t.type_info.is_function());
    EXPECT_FALSE(t.type_info.is_void());

    ASSERT_TRUE(t.type_info.has_member_named("test_int"));
    ASSERT_TRUE(t.type_info.has_member_named("test_bool"));
    ASSERT_TRUE(t.type_info.has_member_named("test_str"));
    ASSERT_FALSE(t.type_info.has_member_named("dkgiowewehsdgsDIGUdFiusdfsdfygui"));

    EXPECT_EQ(t.test_int, t.type_info.get_member_value<int>(t, "test_int"));
    EXPECT_EQ(t.test_bool, t.type_info.get_member_value<bool>(t, "test_bool"));
    EXPECT_EQ(t.test_str, t.type_info.get_member_value<std::string>(t, "test_str"));

    for (int i = 0; i < TEST_ITERATIONS; i++)
    {
        int ri = rand();
        bool rb = rand() % 2 == 0;
        std::string rstr = std::format("{:x}", rand());

        t.type_info.set_member_value(t, "test_int", ri);
        t.type_info.set_member_value(t, "test_bool", rb);
        t.type_info.set_member_value(t, "test_str", rstr);

        EXPECT_EQ(t.test_int, ri);
        EXPECT_EQ(t.test_bool, rb);
        EXPECT_EQ(t.test_str, rstr);
    }
}
