#include <gtest/gtest.h>
#include <test_configs.h>
#include <MSCE/msce_macros.h>
#include <MSCE/reflection.h>

namespace
{
/// @brief Test enum for basic reflection
enum class TestEnum8 : uint8_t
{
  Value1 = 255,
  Value2 = 211,
  Value3 = 155,
};

/// @brief Test enum for reflection with various values
enum class TestEnum32 : uint32_t
{
  Value1 = 15155,
  Value2 = 171245,
  Value3 = 1251,
};

/// @brief Test class with reflected members
struct TestTypeWithReflection
{
  int test_int = 100;
  bool test_bool = true;
  std::string test_str = "Default";

  MSCE_REFLECTION_DEFINE_CLASS(::TestTypeWithReflection)
};

/// @brief Derived test class with reflected members
struct TestTypeWithReflectionDerived : public TestTypeWithReflection
{
  int derived_int = 50;

  MSCE_REFLECTION_DEFINE_CLASS(::TestTypeWithReflectionDerived)

  virtual ~TestTypeWithReflectionDerived() = default;
};

} // namespace

namespace msce::internal
{
template <> struct TypeRegistration<TestEnum8>
{
  inline static constexpr msce::Type type =
      msce::Type("TestEnum8", sizeof(TestEnum8),
                 ::msce::internal::compute_type_traits<TestEnum8>(), {},
                 typeid(TestEnum8));
  inline static constexpr const msce::Type &get_type() { return type; }
  static void register_self()
  {
    static const ::msce::Type &r = []()
    {
      Logger logger("StaticTypeRegistration");
      ::msce::internal::get_g_reflection_types_registry().register_entry(
          "TestEnum8", ::std::cref(TypeRegistration<TestEnum8>::type));
      logger.log_info("Successfully reflected type '{}'",
                      TypeRegistration<TestEnum8>::type.get_name_str());
      return TypeRegistration<TestEnum8>::type;
    }();
  }
  [[gnu::used]] TypeRegistration() { register_self(); }
};
inline static TypeRegistration<TestEnum8> refl_t_reg539;
} // namespace msce::internal

MSCE_REFLECT_FUNDAMENTAL(TestEnum32)
MSCE_REFLECT_CLASS(::TestTypeWithReflection, test_int, test_bool, test_str)
MSCE_REFLECT_CLASS(::TestTypeWithReflectionDerived, test_int, test_bool,
                   test_str, derived_int)

/// @brief Test basic fundamental type operations
TEST(ReflectionTests, FundamentalTypeOperations)
{
  const auto &t_int = typeof(int);
  int value = 42;

  EXPECT_EQ(value, t_int.get_value<int>(value));

  int new_value = 100;
  t_int.set_value(value, new_value);
  EXPECT_EQ(value, t_int.get_value<int>(value));
}

/// @brief Test pointer-based fundamental type operations
TEST(ReflectionTests, FundamentalTypeOperationsViaPointer)
{
  const auto &t_int = typeof(int);
  int value = 42;
  void *vp = &value;

  EXPECT_EQ(value, t_int.get_value<int>(vp));

  int new_value = 99;
  t_int.set_value(vp, new_value);
  EXPECT_EQ(value, t_int.get_value<int>(vp));
}

/// @brief Test type mismatch detection
TEST(ReflectionTests, TypeMismatchDetection)
{
  const auto &t_int = typeof(int);
  int value = 42;
  void *vp = &value;

  EXPECT_THROW(t_int.get_value<int64_t>(vp), std::runtime_error);
}

/// @brief Test integer type properties
TEST(ReflectionTests, IntegerTypeProperties)
{
  const auto &t_char = typeof(char);
  const auto &t_int = typeof(int);
  const auto &t_uint = typeof(unsigned int);

  EXPECT_TRUE(t_char.is_numeric());
  EXPECT_TRUE(t_char.is_integer());
  EXPECT_FALSE(t_char.is_unsigned());

  EXPECT_TRUE(t_int.is_numeric());
  EXPECT_TRUE(t_int.is_integer());
  EXPECT_FALSE(t_int.is_unsigned());

  EXPECT_TRUE(t_uint.is_numeric());
  EXPECT_TRUE(t_uint.is_integer());
  EXPECT_TRUE(t_uint.is_unsigned());
}

/// @brief Test that integer types are not classes/pointers
TEST(ReflectionTests, IntegerTypeNegativeProperties)
{
  const auto &t_int = typeof(int);

  EXPECT_FALSE(t_int.is_class());
  EXPECT_FALSE(t_int.is_pointer());
  EXPECT_FALSE(t_int.is_reference());
  EXPECT_FALSE(t_int.is_array());
  EXPECT_FALSE(t_int.is_enum());
  EXPECT_FALSE(t_int.is_function());
  EXPECT_FALSE(t_int.is_void());
}

/// @brief Test floating point type properties
TEST(ReflectionTests, FloatingPointTypeProperties)
{
  const auto &t_float = msce::get_reflection_of_type<float>();
  const auto &t_double = msce::get_reflection_of_type<double>();

  EXPECT_TRUE(t_float.is_numeric());
  EXPECT_TRUE(t_float.is_floating_point());
  EXPECT_FALSE(t_float.is_integer());

  EXPECT_TRUE(t_double.is_numeric());
  EXPECT_TRUE(t_double.is_floating_point());
  EXPECT_FALSE(t_double.is_integer());
}

/// @brief Test enum type properties
TEST(ReflectionTests, EnumTypeProperties)
{
  const auto &t8 = msce::get_reflection_of_type<TestEnum8>();
  const auto &t32 = msce::get_reflection_of_type<TestEnum32>();

  EXPECT_TRUE(t8.is_enum());
  EXPECT_FALSE(t8.is_numeric());
  EXPECT_FALSE(t8.is_class());

  EXPECT_TRUE(t32.is_enum());
  EXPECT_FALSE(t32.is_numeric());
  EXPECT_FALSE(t32.is_class());
}

/// @brief Test enum value operations
TEST(ReflectionTests, EnumValueOperations)
{
  TestEnum8 e8 = TestEnum8::Value1;
  const auto &t8 = msce::get_reflection_of_type<TestEnum8>();

  EXPECT_EQ(e8, t8.get_value<TestEnum8>(e8));

  t8.set_value(e8, TestEnum8::Value2);
  EXPECT_EQ(e8, t8.get_value<TestEnum8>(e8));
}

/// @brief Test class type reflection and member access
TEST(ReflectionTests, ClassTypeReflection)
{
  TestTypeWithReflection t;
  const auto &type_info = t.get_type_info();

  EXPECT_TRUE(type_info.is_class());
  EXPECT_FALSE(type_info.is_numeric());
  EXPECT_FALSE(type_info.is_pointer());
  EXPECT_FALSE(type_info.is_reference());
  EXPECT_FALSE(type_info.is_enum());
}

/// @brief Test class type member queries
TEST(ReflectionTests, ClassMemberQueries)
{
  TestTypeWithReflection t;
  const auto &type_info = t.get_type_info();

  EXPECT_TRUE(type_info.has_member_named("test_int"));
  EXPECT_TRUE(type_info.has_member_named("test_bool"));
  EXPECT_TRUE(type_info.has_member_named("test_str"));
  EXPECT_FALSE(type_info.has_member_named("nonexistent_member"));
}

/// @brief Test class member value retrieval
TEST(ReflectionTests, ClassMemberRetrieval)
{
  TestTypeWithReflection t;
  t.test_int = 42;
  t.test_bool = false;
  t.test_str = "TestValue";

  const auto &type_info = t.get_type_info();

  EXPECT_EQ(42, type_info.get_member_value<int>(t, "test_int"));
  EXPECT_EQ(false, type_info.get_member_value<bool>(t, "test_bool"));
  EXPECT_EQ("TestValue",
            type_info.get_member_value<std::string>(t, "test_str"));
}

/// @brief Test class member value modification
TEST(ReflectionTests, ClassMemberModification)
{
  TestTypeWithReflection t;

  const auto &type_info = t.get_type_info();

  type_info.set_member_value(t, "test_int", 99);
  type_info.set_member_value(t, "test_bool", true);
  type_info.set_member_value(t, "test_str", std::string("Modified"));

  EXPECT_EQ(99, t.test_int);
  EXPECT_TRUE(t.test_bool);
  EXPECT_EQ("Modified", t.test_str);
}

/// @brief Test type reflection by various methods
TEST(ReflectionTests, TypeReflectionByVariousMethods)
{
  TestTypeWithReflection t;

  const auto &by_template =
      msce::get_reflection_of_type<TestTypeWithReflection>();
  const auto &by_string =
      msce::get_reflection_of_type("::TestTypeWithReflection");
  const auto &by_typeid =
      msce::get_reflection_of_type(typeid(TestTypeWithReflection));
  const auto &by_member = t.get_type_info();

  EXPECT_EQ(by_template, by_string);
  EXPECT_EQ(by_template, by_typeid);
  EXPECT_EQ(by_template, by_member);
}

/// @brief Test derived class reflection
TEST(ReflectionTests, DerivedClassReflection)
{
  TestTypeWithReflectionDerived td;
  const auto &type_info = td.get_type_info();

  EXPECT_TRUE(type_info.is_class());
  EXPECT_TRUE(type_info.has_member_named("test_int"));
  EXPECT_TRUE(type_info.has_member_named("test_bool"));
  EXPECT_TRUE(type_info.has_member_named("test_str"));
  EXPECT_TRUE(type_info.has_member_named("derived_int"));
}

/// @brief Test derived class member access
TEST(ReflectionTests, DerivedClassMemberAccess)
{
  TestTypeWithReflectionDerived td;
  td.test_int = 10;
  td.test_bool = false;
  td.test_str = "Derived";
  td.derived_int = 20;

  const auto &type_info = td.get_type_info();

  EXPECT_EQ(10, type_info.get_member_value<int>(td, "test_int"));
  EXPECT_EQ(false, type_info.get_member_value<bool>(td, "test_bool"));
  EXPECT_EQ("Derived", type_info.get_member_value<std::string>(td, "test_str"));
  EXPECT_EQ(20, type_info.get_member_value<int>(td, "derived_int"));
}

/// @brief Test derived class member modification
TEST(ReflectionTests, DerivedClassMemberModification)
{
  TestTypeWithReflectionDerived td;

  const auto &type_info = td.get_type_info();

  type_info.set_member_value(td, "test_int", 100);
  type_info.set_member_value(td, "test_bool", true);
  type_info.set_member_value(td, "test_str", std::string("Changed"));
  type_info.set_member_value(td, "derived_int", 200);

  EXPECT_EQ(100, td.test_int);
  EXPECT_TRUE(td.test_bool);
  EXPECT_EQ("Changed", td.test_str);
  EXPECT_EQ(200, td.derived_int);
}
