#include <gtest/gtest.h>
#include <test_configs.h>
#include <MSCE/msce_macros.h>
#include <MSCE/Reflection/reflection.h>

enum class TestEnum8 : uint8_t
{
    TESTV1 = 255,
    TESTV2 = 211,
    TESTV3 = 155,
};

enum class TestEnum32 : uint32_t
{
    TESTV1 = 15155,
    TESTV2 = 171245,
    TESTV3 = 1251,
};

namespace msce::internal
{
    template <>
    struct TypeRegistration<TestEnum8>
    {
        inline static constexpr msce::Type type = msce::Type("TestEnum8", sizeof(TestEnum8), ::msce::internal::compute_type_traits<TestEnum8>(), {}, typeid(TestEnum8));
        inline static constexpr const msce::Type &get_type() { return type; }
        static void register_self()
        {
            static const ::msce::Type &r = []()
            { Logger logger("StaticTypeRegistration"); ::msce::internal::get_g_reflection_types_registry().register_entry("TestEnum8", ::std::cref(TypeRegistration<TestEnum8>::type)); logger.log_info("Successfully reflected type '{}'", TypeRegistration<TestEnum8>::type.get_name_str()); return TypeRegistration<TestEnum8>::type; }();
        }
        [[gnu::used]] TypeRegistration() { register_self(); }
    };
    inline static TypeRegistration<TestEnum8> refl_t_reg539;
}
MSCE_REFLECT_FUNDAMENTAL(TestEnum32)

struct TestTypeWithReflection
{
    int test_int = 15556;
    bool test_bool = true;
    std::string test_str = "Hello World!";

    MSCE_REFLECTION_DEFINE_CLASS(::TestTypeWithReflection)
};

MSCE_REFLECT_CLASS(::TestTypeWithReflection, test_int, test_bool, test_str)

struct TestTypeWithReflectionDerived : public TestTypeWithReflection
{
    int derived_int = 0;

    MSCE_REFLECTION_DEFINE_CLASS(::TestTypeWithReflectionDerived)

    virtual void vtable_shifts_stuff_fuck()
    {
    }
};

MSCE_REFLECT_CLASS(::TestTypeWithReflectionDerived, test_int, test_bool, test_str, derived_int)

TEST(ReflectionTests, FundamentalOperations)
{

    const auto &t_int = typeof(int);
    int v = 0;
    void *vp = &v;

    for (int i = 0; i < TEST_ITERATIONS; i++)
    {
        v = rand();

        EXPECT_EQ(v, t_int.get_value<int>(v));
        EXPECT_EQ(v, t_int.get_value<int>(vp));

        int r = rand();
        t_int.set_value(v, r);
        EXPECT_EQ(r, t_int.get_value<int>(v));
        EXPECT_EQ(r, t_int.get_value<int>(vp));
        EXPECT_EQ(v, t_int.get_value<int>(v));
        EXPECT_EQ(v, t_int.get_value<int>(vp));

        r = rand();
        t_int.set_value(vp, r);
        EXPECT_EQ(r, t_int.get_value<int>(v));
        EXPECT_EQ(r, t_int.get_value<int>(vp));
        EXPECT_EQ(v, t_int.get_value<int>(v));
        EXPECT_EQ(v, t_int.get_value<int>(vp));

        try
        {
            EXPECT_EQ(v, t_int.get_value<int64_t>(vp));

            FAIL() << "Invalid reflection use cases didn't threw predicted exceptions.";
        }
        catch (const std::runtime_error &e)
        {
        }
    }
}

TEST(ReflectionTests, IntegralsTest)
{
    const auto &t_char = typeof(char);
    const auto &t_uchar = typeof(unsigned char);
    const auto &t_short = typeof(short);
    const auto &t_ushort = typeof(unsigned short);
    const auto &t_int = typeof(int);
    const auto &t_uint = typeof(unsigned int);
    const auto &t_long = typeof(long);
    const auto &t_ulong = typeof(unsigned long);

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

TEST(ReflectionTests, EnumTest)
{
    TestEnum8 e8 = TestEnum8::TESTV1;
    TestEnum32 e32 = TestEnum32::TESTV1;

    const auto &t8 = msce::get_reflection_of_type<TestEnum8>();
    const auto &t32 = msce::get_reflection_of_type<TestEnum32>();

    EXPECT_TRUE(t8.is_enum());
    EXPECT_FALSE(t8.is_numeric());
    EXPECT_FALSE(t8.is_floating_point());
    EXPECT_FALSE(t8.is_unsigned());
    EXPECT_FALSE(t8.is_class());
    EXPECT_FALSE(t8.is_pointer());
    EXPECT_FALSE(t8.is_reference());
    EXPECT_FALSE(t8.is_array());
    EXPECT_FALSE(t8.is_function());
    EXPECT_FALSE(t8.is_void());

    EXPECT_TRUE(t32.is_enum());
    EXPECT_FALSE(t32.is_numeric());
    EXPECT_FALSE(t32.is_floating_point());
    EXPECT_FALSE(t32.is_unsigned());
    EXPECT_FALSE(t32.is_class());
    EXPECT_FALSE(t32.is_pointer());
    EXPECT_FALSE(t32.is_reference());
    EXPECT_FALSE(t32.is_array());
    EXPECT_FALSE(t32.is_function());
    EXPECT_FALSE(t32.is_void());

    for (int i = 0; i < TEST_ITERATIONS; i++)
    {
        EXPECT_EQ(e8, t8.get_value<TestEnum8>(e8));
        t8.set_value(e8, TestEnum8::TESTV2);
        EXPECT_EQ(e8, t8.get_value<TestEnum8>(e8));

        EXPECT_EQ(e32, t32.get_value<TestEnum32>(e32));
        t32.set_value(e32, TestEnum32::TESTV2);
        EXPECT_EQ(e32, t32.get_value<TestEnum32>(e32));
    }
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
