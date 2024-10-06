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

// The functions exposed by default are usually deterministic
// in real implementations

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
                                      const ReproducibleWrapper<T, R> &y) {
    return ReproducibleWrapper<T, R>(std::fdim(x.value, y.value));
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

// Manipulation functions

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
ReproducibleWrapper<T, R> frexp(const ReproducibleWrapper<T, R> &x, int *exp) {
    return ReproducibleWrapper<T, R>(std::frexp(x.value, exp));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
ReproducibleWrapper<T, R> ldexp(const ReproducibleWrapper<T, R> &x, int exp) {
    return ReproducibleWrapper<T, R>(std::ldexp(x.value, exp));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
ReproducibleWrapper<T, R> modf(const ReproducibleWrapper<T, R> &x,
                               ReproducibleWrapper<T, R> *exp) {
    return ReproducibleWrapper<T, R>(std::modf(x.value, &exp->value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
ReproducibleWrapper<T, R> scalbn(const ReproducibleWrapper<T, R> &x, int exp) {
    return ReproducibleWrapper<T, R>(std::scalbn(x.value, exp));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
int ilogb(const ReproducibleWrapper<T, R> &x) {
    return std::ilogb(x.value);
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
ReproducibleWrapper<T, R> logb(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::logb(x.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
ReproducibleWrapper<T, R> nextafter(const ReproducibleWrapper<T, R> &from,
                                    const ReproducibleWrapper<T, R> &to) {
    return ReproducibleWrapper<T, R>(std::nextafter(from.value, to.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
ReproducibleWrapper<T, R> nexttoward(const ReproducibleWrapper<T, R> &from,
                                     const ReproducibleWrapper<T, R> &to) {
    return ReproducibleWrapper<T, R>(std::nexttoward(from.value, to.value));
}

template <typename T, rounding_mode R>
FEATURE_CXX23(constexpr)
ReproducibleWrapper<T, R> copysign(const ReproducibleWrapper<T, R> &mag,
                                   const ReproducibleWrapper<T, R> &sign) {
    return ReproducibleWrapper<T, R>(std::copysign(mag.value, sign.value));
}

#if defined(RMATH_NONDETERMINISTIC)
// These functions are generally non-deterministic, and more importantly
// IEEE-754 either doesn't acknowledge or have specific precision
// requirements for them.
// Nevertheless, they're useful for interoperability in existing programs

#if __cpp_lib_interpolate >= 201902L
template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> lerp(const ReproducibleWrapper<T, R> &a,
                               const ReproducibleWrapper<T, R> &b,
                               const ReproducibleWrapper<T, R> &t) {
    return ReproducibleWrapper<T, R>(std::lerp(a.value, b.value, t.value));
}
#endif /* __cpp_lib_interpolate >= 201902L */

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

#if __STDCPP_MATH_SPEC_FUNCS__ >= 201003L &&                                   \
    defined(__STDCPP_WANT_MATH_SPEC_FUNCS__)
template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> assoc_laguerre(const unsigned int n,
                                         const unsigned int m,
                                         const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::assoc_laguerre(n, m, x.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> assoc_legendre(const unsigned int n,
                                         const unsigned int m,
                                         const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::assoc_legendre(n, m, x.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> beta(const ReproducibleWrapper<T, R> &x,
                               const ReproducibleWrapper<T, R> &y) {
    return ReproducibleWrapper<T, R>(std::beta(x.value, y.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> comp_ellint_1(const ReproducibleWrapper<T, R> &k) {
    return ReproducibleWrapper<T, R>(std::comp_ellint_1(k.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> comp_ellint_2(const ReproducibleWrapper<T, R> &k) {
    return ReproducibleWrapper<T, R>(std::comp_ellint_2(k.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> comp_ellint_3(const ReproducibleWrapper<T, R> &k,
                                        const ReproducibleWrapper<T, R> &nu) {
    return ReproducibleWrapper<T, R>(std::comp_ellint_3(k.value, nu.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> cyl_bessel_i(const ReproducibleWrapper<T, R> &nu,
                                       const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::cyl_bessel_i(nu.value, x.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> cyl_bessel_j(const ReproducibleWrapper<T, R> &nu,
                                       const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::cyl_bessel_j(nu.value, x.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> cyl_bessel_k(const ReproducibleWrapper<T, R> &nu,
                                       const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::cyl_bessel_k(nu.value, x.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> cyl_neumann(const ReproducibleWrapper<T, R> &nu,
                                      const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::cyl_neumann(nu.value, x.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> ellint_1(const ReproducibleWrapper<T, R> &k,
                                   const ReproducibleWrapper<T, R> &phi) {
    return ReproducibleWrapper<T, R>(std::ellint_1(k.value, phi.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> ellint_2(const ReproducibleWrapper<T, R> &k,
                                   const ReproducibleWrapper<T, R> &phi) {
    return ReproducibleWrapper<T, R>(std::ellint_2(k.value, phi.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> ellint_3(const ReproducibleWrapper<T, R> &k,
                                   const ReproducibleWrapper<T, R> &nu,
                                   const ReproducibleWrapper<T, R> &phi) {
    return ReproducibleWrapper<T, R>(
        std::ellint_3(k.value, nu.value, phi.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> expint(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::expint(x.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> hermite(const unsigned int n,
                                  const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::hermite(n, x.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> legendre(const unsigned int n,
                                   const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::legendre(n, x.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> laguerre(const unsigned int n,
                                   const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::laguerre(n, x.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> riemann_zeta(const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::riemann_zeta(x.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> sph_bessel(const unsigned int n,
                                     const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::sph_bessel(n, x.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> sph_legendre(const unsigned int n,
                                       const unsigned int m,
                                       const ReproducibleWrapper<T, R> &theta) {
    return ReproducibleWrapper<T, R>(std::sph_legendre(n, m, theta.value));
}

template <typename T, rounding_mode R>
ReproducibleWrapper<T, R> sph_neumann(const unsigned int n,
                                      const ReproducibleWrapper<T, R> &x) {
    return ReproducibleWrapper<T, R>(std::sph_neumann(n, x.value));
}
#endif /* defined(__STDCPP_WANT_MATH_SPEC_FUNCS__) */
#endif /* defined(RMATH_NONDETERMINISTIC) */

} // namespace rmath
#undef FEATURE_CXX23
#undef FEATURE_CXX26
