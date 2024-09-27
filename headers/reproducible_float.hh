#pragma once

#include <fenv.h>
#include <limits>
#include <type_traits>

#if __cplusplus >= 202302L
/* Include the stdfloat header if it exists */
#include <stdfloat>
#endif /* __cplusplus >= 202302L */

// Most compilers don't support this pragma and ignore it, but
// we're still technically required to set it
#pragma STDC FENV_ACCESS ON

#if (defined(__clang__) || defined(__GNUG__) || defined(__GNUC__))
// This macro adds a constraint on the specified parameter requiring
// it to exist in a register. This prevents most (and hopefully all)
// compilers from doing things unwanted optimizations like FMA when
// they see subsequent lines that can be fused together.
#define OPT_BARRIER(param) __asm__ volatile("" ::"X"(param) :)
#else
// There's no standard way to define this on other compilers, it
// becomes a no-op.
// You're on your own.
#warning "This compiler may not be supported. Proceed at your own risk."
#define OPT_BARRIER(param)
#endif

#if __cplusplus >= 202002L
#define FEATURE_CXX20(expr) expr
#else
#define FEATURE_CXX20(expr)
#endif /* __cplusplus >= 202002L */

// Our safety checks are taken care of at the usage site
#define SAFE_BINOP(result, a, b, op)                                           \
    T result = (a)op(b);                                                       \
    OPT_BARRIER(result);

namespace rfp {

enum class RoundingMode { ToEven, ToPositive, ToNegative, ToZero };

template <RoundingMode R> void SetRoundingMode() {
    if (R == RoundingMode::ToEven) {
        fesetround(FE_TONEAREST);
    } else if constexpr (R == RoundingMode::ToPositive) {
        fesetround(FE_UPWARD);
    } else if constexpr (R == RoundingMode::ToNegative) {
        fesetround(FE_DOWNWARD);
    } else if constexpr (R == RoundingMode::ToZero) {
        fesetround(FE_TOWARDZERO);
    } else {
        static_assert(R == RoundingMode::ToEven,
                      "Rounding mode is not supported");
    }
}
} // namespace rfp

template <typename T, rfp::RoundingMode R = rfp::RoundingMode::ToEven>
class NumericWrapper {
#if __cplusplus >= 202302L
    static_assert(std::is_same<T, float>::value ||
                      std::is_same<T, double>::value ||
                      std::is_same<T, std::float16_t>::value ||
                      std::is_same<T, std::float32_t>::value ||
                      std::is_same<T, std::float64_t>::value,
                  "Unsupported floating point type");
#else  /* Pre-C++23 */
    static_assert(std::is_same<T, float>::value ||
                      std::is_same<T, double>::value,
                  "Unsupported floating point type");
#endif /* __cplusplus >= 202302L */
    static_assert(std::is_floating_point<T>::value,
                  "Non-floating point types are not supported");
    static_assert(std::numeric_limits<T>::is_iec559,
                  "Type must be IEC 559 / IEEE 754 compliant");

  public:
    T value;

    constexpr NumericWrapper(const T &val) : value(val) {}

    template <rfp::RoundingMode R2>
    constexpr NumericWrapper<T, R2> &operator=(T val) {
        value = val;
        return *this;
    }

    template <rfp::RoundingMode R2>
    constexpr NumericWrapper<T, R> &
    operator=(const NumericWrapper<T, R2> &other) {
        value = other.value;
        return *this;
    }

    // Normally we'd want to delete the implicit conversion operators
    // However, GCC incorrectly assumes that doing so means that
    // the types are convertible and generates confusing errors about
    // ambiguous overloads with floats and doubles.
    // Therefore, we avoid explicitly deleting them.
    // operator float() const = delete;
    // operator double() const = delete;

    constexpr inline double fp64() const { return static_cast<double>(value); }

    template <typename T2 = T, typename = typename std::enable_if_t<
                                   std::is_same<T2, float>::value>>
    constexpr inline T2 fp32() const {
        return value;
    }

#if __cplusplus >= 202302L
    template <typename T2 = T, typename = typename std::enable_if_t<
                                   std::is_same<T2, std::float16_t>::value>>
    constexpr inline T2 fp16() const {
        return value;
    }
#endif /* __cplusplus >= 202302L */

#if __cplusplus >= 202002L
    constexpr inline auto operator<=>(const NumericWrapper<T, R> &rhs) const {
        return value <=> rhs.value;
    };
#endif /* __cplusplus >= 202002L */

    constexpr inline auto operator<(const NumericWrapper<T, R> &rhs) const {
        return value < rhs.value;
    };

    constexpr inline auto operator>(const NumericWrapper<T, R> &rhs) const {
        return value > rhs.value;
    };

    constexpr inline auto operator<=(const NumericWrapper<T, R> &rhs) const {
        return value <= rhs.value;
    };

    constexpr inline auto operator>=(const NumericWrapper<T, R> &rhs) const {
        return value >= rhs.value;
    };

    constexpr inline auto operator==(const NumericWrapper<T, R> &rhs) const {
        return value == rhs.value;
    };

    constexpr inline auto operator!=(const NumericWrapper<T, R> &rhs) const {
        return value != rhs.value;
    };

    FEATURE_CXX20(constexpr)
    inline NumericWrapper<T, R>
    operator+(const NumericWrapper<T, R> &rhs) const {
        SAFE_BINOP(result, value, rhs.value, +);
        return NumericWrapper(result);
    }
    FEATURE_CXX20(constexpr)
    inline NumericWrapper<T, R>
    operator-(const NumericWrapper<T, R> &rhs) const {
        SAFE_BINOP(result, value, rhs.value, -);
        return NumericWrapper(result);
    }

    FEATURE_CXX20(constexpr)
    inline NumericWrapper<T, R>
    operator*(const NumericWrapper<T, R> &rhs) const {
        SAFE_BINOP(result, value, rhs.value, *);
        return NumericWrapper(result);
    }

    FEATURE_CXX20(constexpr)
    inline NumericWrapper<T, R>
    operator/(const NumericWrapper<T, R> &rhs) const {
        SAFE_BINOP(result, value, rhs.value, /);
        return NumericWrapper(result);
    }
};

#if __cplusplus >= 202002L
// Use C++20 alias template type deduction if it's available
template <rfp::RoundingMode R = rfp::RoundingMode::ToEven>
using rfloat = NumericWrapper<float, R>;
template <rfp::RoundingMode R = rfp::RoundingMode::ToEven>
using rdouble = NumericWrapper<double, R>;
#else
// otherwise pick reasonable defaults
using rfloat = NumericWrapper<float, rfp::RoundingMode::ToEven>;
using rdouble = NumericWrapper<double, rfp::RoundingMode::ToEven>;
#endif /* __cplusplus >= 202002L */

// Support expressions with non-reproducible types
// on the left hand side by converting them to reproducible types
// with the same type and rounding mode as the right hand side
// first. This should be essentially free, and the type overloads
// will ensure the operation is done safely.

#if __cplusplus >= 202002L
template <typename T, rfp::RoundingMode R>
constexpr inline auto operator<=>(const T &lhs,
                                  const NumericWrapper<T, R> &rhs) {
    return NumericWrapper(lhs) <=> rhs;
};
#else

// Relational operators
template <typename T, rfp::RoundingMode R>
FEATURE_CXX20(constexpr)
static inline auto operator==(const T &lhs, const NumericWrapper<T, R> &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    return NumericWrapper(lhs) == rhs;
}

template <typename T, rfp::RoundingMode R>
FEATURE_CXX20(constexpr)
static inline auto operator!=(const T &lhs, const NumericWrapper<T, R> &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    return NumericWrapper(lhs) != rhs;
}

template <typename T, rfp::RoundingMode R>
FEATURE_CXX20(constexpr)
static inline auto operator<=(const T &lhs, const NumericWrapper<T, R> &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    return NumericWrapper(lhs) <= rhs;
}

template <typename T, rfp::RoundingMode R>
FEATURE_CXX20(constexpr)
static inline auto operator>=(const T &lhs, const NumericWrapper<T, R> &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    return NumericWrapper(lhs) >= rhs;
}

template <typename T, rfp::RoundingMode R>
FEATURE_CXX20(constexpr)
static inline auto operator>(const T &lhs, const NumericWrapper<T, R> &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    return NumericWrapper(lhs) > rhs;
}

template <typename T, rfp::RoundingMode R>
FEATURE_CXX20(constexpr)
static inline auto operator<(const T &lhs, const NumericWrapper<T, R> &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    return NumericWrapper(lhs) < rhs;
}
#endif /* __cplusplus >= 202002L */

// Arithmetic operators
template <typename T, rfp::RoundingMode R>
FEATURE_CXX20(constexpr)
static inline NumericWrapper<T, R> operator+(const T &lhs,
                                             const NumericWrapper<T, R> &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    return NumericWrapper(lhs) + rhs;
}

template <typename T, rfp::RoundingMode R>
FEATURE_CXX20(constexpr)
static inline NumericWrapper<T, R> operator-(const T &lhs,
                                             const NumericWrapper<T, R> &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    return NumericWrapper(lhs) - rhs;
}

template <typename T, rfp::RoundingMode R>
FEATURE_CXX20(constexpr)
static inline NumericWrapper<T, R> operator*(const T &lhs,
                                             const NumericWrapper<T, R> &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    return NumericWrapper(lhs) * rhs;
}

template <typename T, rfp::RoundingMode R>
FEATURE_CXX20(constexpr)
static inline NumericWrapper<T, R> operator/(const T &lhs,
                                             const NumericWrapper<T, R> &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    return NumericWrapper(lhs) / rhs;
}

#undef OPT_BARRIER
#undef SAFE_BINOP
