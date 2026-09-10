#pragma once

#include <cstdint>
#include <type_traits>

#define ITERATE_TEST()                                                         \
  for (msce::test_iterations_type i = 0; i < msce::TEST_ITERATIONS; ++i)

namespace msce
{

/// @brief The number of iterations in test using it.
constexpr const uint64_t TEST_ITERATIONS = 10000;

using test_iterations_type = std::conditional_t<
    (TEST_ITERATIONS < UINT8_MAX), uint8_t,
    std::conditional_t<(TEST_ITERATIONS < UINT16_MAX), uint16_t,
                       std::conditional_t<(TEST_ITERATIONS < UINT32_MAX),
                                          uint32_t, uint64_t>>>;

/// @brief Precision used for checking floating-point types with some tolerance.
constexpr const long double TEST_FLOATING_POINT_TOLERANCE = 1.0E-14;

/// @brief Precision used for checking floating-point types with some
/// tolerance specificali in trigonometry enviroments.
constexpr const long double TEST_FLOATING_POINT_TRIGONOMETRY_TOLERANCE = 1.0E-9;

/// @brief Seed used for srand function.
constexpr const int RAND_FUNCTION_SEED = 0x81049854;
} // namespace msce
