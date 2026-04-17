#ifndef MSCE_TEST_CONFIGS_H
#define MSCE_TEST_CONFIGS_H
#include <gtest/gtest.h>
#include <cstddef>

#include <MSCE/msce_macros.h>

#include <MSCE/ECS/system.h>
#include <MSCE/ECS/component.h>
#include <MSCE/ECS/entity.h>

#include <MSCE/Prototypes/entityPrototype.hpp>
#include <MSCE/Prototypes/prototype.hpp>

#include <MSCE/Managers/systemManager.h>
#include <MSCE/Managers/componentManager.h>
#include <MSCE/Managers/entityManager.h>
#include <MSCE/Managers/prototypeManager.h>

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_ARCHIVE(cereal::JSONInputArchive);
CEREAL_REGISTER_ARCHIVE(cereal::JSONOutputArchive);

/// @brief The number of iterations in test using it.
const size_t TEST_ITERATIONS = 10;

/// @brief Precision used for checking floating-point types with some tolerance.
const double TEST_FLOATING_POINT_TOLERANCE = 1e-10;

/// @brief Seed used for srand function.
const unsigned int RAND_FUNCTION_SEED = 0x81049854;

class TestSystem1 : public msce::System
{
private:
    void SwitchValue()
    {
        value = !value;
    }

public:
    bool value = false;
    void init() override
    {
        System::init();
        SwitchValue();
    }
};

struct TestPrototype1 : public msce::IPrototype
{
    int test_int = 15556;
    bool test_bool = true;
    std::string test_str = "Hello World!";

    MSCE_DEFINE_PROTOTYPE(TestPrototype1, test_int, test_bool, test_str)
};
MSCE_REGISTER_PROTOTYPE(TestPrototype1, TestPrototype1)

struct TestTypeWithReflection
{
    int test_int = 15556;
    bool test_bool = true;
    std::string test_str = "Hello World!";

    MSCE_GENERATE_REFLECTION_METHODS(TestTypeWithReflection, test_int, test_bool, test_str)
};
struct TestTypeWithReflectionDerived : public TestTypeWithReflection
{
    int derived_test_int = 22222;
    bool derived_test_bool = false;
    std::string derived_test_str = "Bye Bye, Earth!";

    MSCE_GENERATE_REFLECTION_METHODS_DERIVED(TestTypeWithReflectionDerived, TestTypeWithReflection, derived_test_int, derived_test_bool, derived_test_str)
};

#endif // MSCE_TEST_CONFIGS_H
