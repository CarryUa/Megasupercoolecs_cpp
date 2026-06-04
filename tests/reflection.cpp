#include <gtest/gtest.h>
#include <test_configs.h>
#include <MSCE/msce_macros.h>
#include <MSCE/Reflection/reflection.h>

struct TestTypeWithReflection
{
    int test_int = 15556;
    bool test_bool = true;
    std::string test_str = "Hello World!";

    MSCE_DEFINE_REFLECTED_OBJECT(::TestTypeWithReflection, test_int, test_bool, test_str)
};

MSCE_REGISTER_REFLECTED_OBJECT(::TestTypeWithReflection)

struct TestTypeWithReflectionDerived : public TestTypeWithReflection
{
    int derived_int = 0;

    MSCE_DEFINE_REFLECTED_OBJECT(::TestTypeWithReflectionDerived, test_int, test_bool, test_str, derived_int)

    virtual void vtable_shifts_stuff_fuck()
    {
    }
};

MSCE_REGISTER_REFLECTED_OBJECT(::TestTypeWithReflectionDerived)

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
    TestTypeWithReflectionDerived td;
    EXPECT_EQ(t.get_type_info(), msce::get_reflection_of_type<TestTypeWithReflection>());
    EXPECT_EQ(t.get_type_info(), msce::get_reflection_of_type("::TestTypeWithReflection"));
    EXPECT_EQ(t.get_type_info(), msce::get_reflection_of_type(typeid(TestTypeWithReflection)));

    EXPECT_TRUE(t.get_type_info().is_class());
    EXPECT_FALSE(t.get_type_info().is_numeric());
    EXPECT_FALSE(t.get_type_info().is_pointer());
    EXPECT_FALSE(t.get_type_info().is_reference());
    EXPECT_FALSE(t.get_type_info().is_array());
    EXPECT_FALSE(t.get_type_info().is_enum());
    EXPECT_FALSE(t.get_type_info().is_function());
    EXPECT_FALSE(t.get_type_info().is_void());

    ASSERT_TRUE(t.get_type_info().has_member_named("test_int"));
    ASSERT_TRUE(t.get_type_info().has_member_named("test_bool"));
    ASSERT_TRUE(t.get_type_info().has_member_named("test_str"));
    ASSERT_FALSE(t.get_type_info().has_member_named("dkgiowewehsdgsDIGUdFiusdfsdfygui"));

    EXPECT_EQ(t.test_int, t.get_type_info().get_member_value<int>(t, "test_int"));
    EXPECT_EQ(t.test_bool, t.get_type_info().get_member_value<bool>(t, "test_bool"));
    EXPECT_EQ(t.test_str, t.get_type_info().get_member_value<std::string>(t, "test_str"));

    EXPECT_EQ(td.get_type_info(), msce::get_reflection_of_type<TestTypeWithReflectionDerived>());
    EXPECT_EQ(td.get_type_info(), msce::get_reflection_of_type("::TestTypeWithReflectionDerived"));
    EXPECT_EQ(td.get_type_info(), msce::get_reflection_of_type(typeid(TestTypeWithReflectionDerived)));

    EXPECT_TRUE(td.get_type_info().is_class());
    EXPECT_FALSE(td.get_type_info().is_numeric());
    EXPECT_FALSE(td.get_type_info().is_pointer());
    EXPECT_FALSE(td.get_type_info().is_reference());
    EXPECT_FALSE(td.get_type_info().is_array());
    EXPECT_FALSE(td.get_type_info().is_enum());
    EXPECT_FALSE(td.get_type_info().is_function());
    EXPECT_FALSE(td.get_type_info().is_void());

    ASSERT_TRUE(td.get_type_info().has_member_named("test_int"));
    ASSERT_TRUE(td.get_type_info().has_member_named("test_bool"));
    ASSERT_TRUE(td.get_type_info().has_member_named("test_str"));
    ASSERT_TRUE(td.get_type_info().has_member_named("derived_int"));
    ASSERT_FALSE(td.get_type_info().has_member_named("dkgiowewehsdgsDIGUdFiusdfsdfygui"));

    static msce::Logger l("Test");
    l.log_debug("\nMemory Address of object: 0x{:x}\n----------------test_int: 0x{:x}\n---------------test_bool: 0x{:x}\n----------------test_str: 0x{:x}\n-------------derived_int: 0x{:x}\n",
                reinterpret_cast<uintptr_t>(&td), reinterpret_cast<uintptr_t>(&td.test_int), reinterpret_cast<uintptr_t>(&td.test_bool), reinterpret_cast<uintptr_t>(&td.test_str), reinterpret_cast<uintptr_t>(&td.derived_int));
    EXPECT_EQ(td.test_int, td.get_type_info().get_member_value<int>(td, "test_int"));
    EXPECT_EQ(td.test_bool, td.get_type_info().get_member_value<bool>(td, "test_bool"));
    EXPECT_EQ(td.test_str, td.get_type_info().get_member_value<std::string>(td, "test_str"));
    EXPECT_EQ(td.derived_int, td.get_type_info().get_member_value<int>(td, "derived_int"));

    for (int i = 0; i < TEST_ITERATIONS; i++)
    {
        int ri = rand();
        int ri2 = rand();
        bool rb = rand() % 2 == 0;
        std::string rstr = std::format("{:x}", rand());

        t.get_type_info().set_member_value(t, "test_int", ri);
        t.get_type_info().set_member_value(t, "test_bool", rb);
        t.get_type_info().set_member_value(t, "test_str", rstr);

        EXPECT_EQ(t.test_int, ri);
        EXPECT_EQ(t.test_bool, rb);
        EXPECT_EQ(t.test_str, rstr);

        td.get_type_info().set_member_value(td, "test_int", ri);
        td.get_type_info().set_member_value(td, "test_bool", rb);
        td.get_type_info().set_member_value(td, "test_str", rstr);
        td.get_type_info().set_member_value(td, "derived_int", ri2);

        EXPECT_EQ(td.test_int, ri);
        EXPECT_EQ(td.test_bool, rb);
        EXPECT_EQ(td.test_str, rstr);
        EXPECT_EQ(td.derived_int, ri2);
    }
}
