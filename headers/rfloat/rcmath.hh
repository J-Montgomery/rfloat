#pragma once

#include "rfloat.hh"
#include <cmath>

#if __cpp_lib_constexpr_cmath >= 202202L
#define FEATURE_CXX23(expr) expr
#else
#define FEATURE_CXX23(expr)
#endif /* __cpp_lib_constexpr_cmath >= 202202L */

#if __cpp_lib_constexpr_cmath >= 202306L
#define FEATURE_CXX26(expr) expr
#else
#define FEATURE_CXX26(expr)
#endif /* __cpp_lib_constexpr_cmath >= 202202L */

namespace rmath {

/* These functions are generally reproducible as implemented in
 * standard libraries
 */

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
inline ReproducibleWrapper<T, R> abs(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::abs(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
inline ReproducibleWrapper<T, R> fmin(const ReproducibleWrapper<T, R> &x,
                                      const ReproducibleWrapper<T, R> &y) {
    return ReproducibleWrapper<T, R>(std::fmin(x.value, y.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
inline ReproducibleWrapper<T, R> fmax(const ReproducibleWrapper<T, R> &x,
                                      const ReproducibleWrapper<T, R> &y) {
    return ReproducibleWrapper<T, R>(std::fmax(x.value, y.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
inline ReproducibleWrapper<T, R> fdim(const ReproducibleWrapper<T, R> &x,
                                      const ReproducibleWrapper<T, R> &y,
                                      const ReproducibleWrapper<T, R> &z) {
    return ReproducibleWrapper<T, R>(std::fdim(x.value, y.value, z.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
inline ReproducibleWrapper<T, R> fmod(const ReproducibleWrapper<T, R> &x,
                                      const ReproducibleWrapper<T, R> &y) {
    return ReproducibleWrapper<T, R>(std::fmod(x.value, y.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
inline ReproducibleWrapper<T, R> remainder(const ReproducibleWrapper<T, R> &x,
                                           const ReproducibleWrapper<T, R> &y) {
    return ReproducibleWrapper<T, R>(std::remainder(x.value, y.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
inline ReproducibleWrapper<T, R> remquo(const ReproducibleWrapper<T, R> &x,
                                        const ReproducibleWrapper<T, R> &y) {
    return ReproducibleWrapper<T, R>(std::remquo(x.value, y.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
inline ReproducibleWrapper<T, R> ceil(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::ceil(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
inline ReproducibleWrapper<T, R> floor(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::floor(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
inline ReproducibleWrapper<T, R> trunc(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::trunc(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
inline ReproducibleWrapper<T, R> round(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::round(x.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> nearbyint(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::nearbyint(x.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> rint(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::rint(x.value));
}

/* It'd be better if we could ensure these were being generated as
 * the appropriate HW instructions and throw an error otherwise
 */
template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
inline ReproducibleWrapper<T, R> sqrt(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::sqrt(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
inline ReproducibleWrapper<T, R> fma(const ReproducibleWrapper<T, R> &x,
                                     const ReproducibleWrapper<T, R> &y,
                                     const ReproducibleWrapper<T, R> &z) {
    return ReproducibleWrapper<T, R>(std::fma(x.value, y.value, z.value));
}

// Classification functions

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
int fpclassify(const ReproducibleWrapper<T, R> &x) {
    return std::fpclassify(x.value);
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
bool isfinite(const ReproducibleWrapper<T, R> &x) {
    return std::isfinite(x.value);
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
bool isinf(const ReproducibleWrapper<T, R> &x) {
    return std::isinf(x.value);
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
bool isnan(const ReproducibleWrapper<T, R> &x) {
    return std::isnan(x.value);
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
bool isnormal(const ReproducibleWrapper<T, R> &x) {
    return std::isnormal(x.value);
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
bool signbit(const ReproducibleWrapper<T, R> &x) {
    return std::signbit(x.value);
}

// Comparisons

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
bool isgreater(const ReproducibleWrapper<T, R> &x,
               const ReproducibleWrapper<T, R> &y) {
    return std::isgreater(x.value, y.value);
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
bool isgreaterequal(const ReproducibleWrapper<T, R> &x,
                    const ReproducibleWrapper<T, R> &y) {
    return std::isgreaterequal(x.value, y.value);
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
bool isless(const ReproducibleWrapper<T, R> &x,
            const ReproducibleWrapper<T, R> &y) {
    return std::isless(x.value, y.value);
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
bool islessequal(const ReproducibleWrapper<T, R> &x,
                 const ReproducibleWrapper<T, R> &y) {
    return std::islessequal(x.value, y.value);
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
bool islessgreater(const ReproducibleWrapper<T, R> &x,
                   const ReproducibleWrapper<T, R> &y) {
    return std::islessgreater(x.value, y.value);
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
bool isunordered(const ReproducibleWrapper<T, R> &x,
                 const ReproducibleWrapper<T, R> &y) {
    return std::isunordered(x.value, y.value);
}

#if defined(RMATH_NONDETERMINISTIC)
// These functions are generally non-deterministic, and more importantly
// IEEE-754 either doesn't acknowledge or have specific precision
// requirements for them.
// Nevertheless, they're useful for interoperability in existing programs

// Exponentials
template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> log(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::log(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> log10(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::log10(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> log2(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::log2(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> log1p(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::log1p(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> exp(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::exp(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> exp2(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::exp2(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> expm1(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::expm1(x.value));
}

// Power functions
template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> pow(const ReproducibleWrapper<T, R> &base,
                                     const ReproducibleWrapper<T, R> &exp) {
    return ReproducibleWrapper<T, R>(std::pow(base.value, exp.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> cbrt(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::cbrt(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> hypot(const ReproducibleWrapper<T, R> &x,
                                       const ReproducibleWrapper<T, R> &y) {
    return ReproducibleWrapper<T, R>(std::hypot(x.value, y.value));
}

// Trigonometric functions

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> sin(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::sin(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> cos(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::cos(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> tan(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::tan(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> asin(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::asin(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> acos(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::acos(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> atan(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::atan(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> atan2(const ReproducibleWrapper<T, R> &y,
                                       const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::atan2(y.value, x.value));
}

// Hyperbolic functions

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> sinh(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::sinh(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> cosh(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::cosh(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> tanh(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::tanh(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> asinh(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::asinh(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> acosh(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::acosh(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> atanh(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::atanh(x.value));
}

// Error and gamma functions

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> erf(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::erf(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> erfc(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::erfc(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> tgamma(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::tgamma(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX26(constexpr)
inline ReproducibleWrapper<T, R> lgamma(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::lgamma(x.value));
}
#endif

} // namespace rmath
#undef FEATURE_CXX23
#undef FEATURE_CXX26
