#pragma once

#include "rfloat.hh"
#include <cmath>

namespace rmath {

/* These functions are generally reproducible as implemented in
 * standard libraries
 */

template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
abs(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::abs(x.value));
}

template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
fmin(const ReproducibleWrapper<T, R> &x, const ReproducibleWrapper<T, R> &y) {
    return ReproducibleWrapper<T, R>(std::fmin(x.value, y.value));
}

template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
fmax(const ReproducibleWrapper<T, R> &x, const ReproducibleWrapper<T, R> &y) {
    return ReproducibleWrapper<T, R>(std::fmax(x.value, y.value));
}

/* It'd be better if we could ensure these were being generated as
 * the appropriate HW instructions and throw an error otherwise
 */
template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
sqrt(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::sqrt(x.value));
}

template <typename T, rounding_mode R>
constexpr inline ReproducibleWrapper<T, R>
fma(const ReproducibleWrapper<T, R> &x, const ReproducibleWrapper<T, R> &y,
    const ReproducibleWrapper<T, R> &z) {
    return ReproducibleWrapper<T, R>(std::fma(x.value, y.value, z.value));
}

} // namespace rmath
