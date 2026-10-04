#pragma once
#include <cstdint>
#include <cmath>
#include <type_traits>

namespace msce
{
struct EasingLinear
{
  float operator()(float t) { return t; }
};

template <typename T, typename Easing = EasingLinear> struct Interpolate
{
  static_assert(false,
                "lerp structure-function wasn't specialized for given type.");
};

template <typename T, typename Easing>
  requires std::is_arithmetic_v<T>
struct Interpolate<T, Easing>
{
  T operator()(const T a, const T b, const float t)
  {
    return a + (b - a) * Easing{}(t);
  }
};

template <float amount> struct EasingIn
{
  float operator()(float t) { return std::pow(t, amount); }
};

template <float amount> struct EasingOut
{
  static_assert(amount != 0, "Easing amount for EasingOut can't be zero");
  float operator()(float t)
  {
    if (amount == 0) return 0;
    return std::pow(t, 1 / amount);
  }
};

template <float amountIn, float amountOut = amountIn> struct EasingInOut
{
  float operator()(float t)
  {
    return Interpolate<float, EasingLinear>{}(EasingIn<amountIn>{}(t),
                                              EasingOut<amountOut>{}(t), t);
  }
};

} // namespace msce