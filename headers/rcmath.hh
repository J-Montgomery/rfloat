#pragma once

#include "rfloat.hh"
#include <cmath>

namespace rmath {

template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
abs(const ReproducibleWrapper<T, R> &val) {
    return ReproducibleWrapper<T, R>(std::abs(val.value));
}

template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
sqrt(const ReproducibleWrapper<T, R> &val) {
    return ReproducibleWrapper<T, R>(std::sqrt(val.value));
}

template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
log(const ReproducibleWrapper<T, R> &val) {
    return ReproducibleWrapper<T, R>(std::log(val.value));
}

template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
log10(const ReproducibleWrapper<T, R> &val) {
    return ReproducibleWrapper<T, R>(std::log10(val.value));
}

template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
exp(const ReproducibleWrapper<T, R> &val) {
    return ReproducibleWrapper<T, R>(std::exp(val.value));
}

template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
sin(const ReproducibleWrapper<T, R> &val) {
    return ReproducibleWrapper<T, R>(std::sin(val.value));
}

template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
cos(const ReproducibleWrapper<T, R> &val) {
    return ReproducibleWrapper<T, R>(std::cos(val.value));
}

template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
tan(const ReproducibleWrapper<T, R> &val) {
    return ReproducibleWrapper<T, R>(std::tan(val.value));
}

template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
atan(const ReproducibleWrapper<T, R> &val) {
    return ReproducibleWrapper<T, R>(std::atan(val.value));
}

template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
atan2(const ReproducibleWrapper<T, R> &val1,
      const ReproducibleWrapper<T, R> &val2) {
    return ReproducibleWrapper<T, R>(std::atan2(val1.value, val2.value));
}

} // namespace rmath
