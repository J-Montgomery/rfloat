#pragma once

#include <fenv.h>
#include <limits>
#include <ostream>
#include <type_traits>

#if __cplusplus >= 202302L
/* Include the stdfloat header if it exists */
#include <stdfloat>
#endif /* __cplusplus >= 202302L */

#if (defined(__clang__) || defined(__GNUG__) || defined(__GNUC__))
// This macro adds a constraint on the specified parameter requiring
// it to be placed in a register, and also acts as a compiler barrier.
// This prevents compilers from doing unwanted optimizations
// that might combine, reorder, or remove instructions that
// are specified by the source code. Some compilers like GCC
// will even do this across lines in the absence of this macro.
#define OPT_BARRIER(param) __asm__ volatile("" ::"X"(param) :)
#else
// There's no standard way to define this on other compilers so
// you're on your own.
#pragma message("This compiler may not be supported. Proceed at your own risk.")
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

namespace rmath {

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
} // namespace rmath

template <typename T, rmath::RoundingMode R = rmath::RoundingMode::ToEven>
class ReproducibleWrapper {
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

    constexpr ReproducibleWrapper(const T &val) : value(val) {}
    constexpr ReproducibleWrapper() = default;

    template <rmath::RoundingMode R2>
    constexpr ReproducibleWrapper<T, R2> &operator=(T val) {
        value = val;
        return *this;
    }

    template <rmath::RoundingMode R2>
    constexpr ReproducibleWrapper<T, R> &
    operator=(const ReproducibleWrapper<T, R2> &other) {
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
    constexpr inline auto
    operator<=>(const ReproducibleWrapper<T, R> &rhs) const {
        return value <=> rhs.value;
    };
#endif /* __cplusplus >= 202002L */

    /* Comparison operators */
    constexpr inline auto
    operator<(const ReproducibleWrapper<T, R> &rhs) const {
        return value < rhs.value;
    };

    constexpr inline auto
    operator>(const ReproducibleWrapper<T, R> &rhs) const {
        return value > rhs.value;
    };

    constexpr inline auto
    operator<=(const ReproducibleWrapper<T, R> &rhs) const {
        return value <= rhs.value;
    };

    constexpr inline auto
    operator>=(const ReproducibleWrapper<T, R> &rhs) const {
        return value >= rhs.value;
    };

    constexpr inline auto
    operator==(const ReproducibleWrapper<T, R> &rhs) const {
        return value == rhs.value;
    };

    constexpr inline auto
    operator!=(const ReproducibleWrapper<T, R> &rhs) const {
        return value != rhs.value;
    };

    /* Arithmetic operators */
    FEATURE_CXX20(constexpr)
    inline ReproducibleWrapper<T, R>
    operator+(const ReproducibleWrapper<T, R> &rhs) const {
        SAFE_BINOP(result, value, rhs.value, +);
        return ReproducibleWrapper(result);
    }

    FEATURE_CXX20(constexpr)
    inline ReproducibleWrapper<T, R>
    operator-(const ReproducibleWrapper<T, R> &rhs) const {
        SAFE_BINOP(result, value, rhs.value, -);
        return ReproducibleWrapper(result);
    }

    FEATURE_CXX20(constexpr)
    inline ReproducibleWrapper<T, R>
    operator*(const ReproducibleWrapper<T, R> &rhs) const {
        SAFE_BINOP(result, value, rhs.value, *);
        return ReproducibleWrapper(result);
    }

    FEATURE_CXX20(constexpr)
    inline ReproducibleWrapper<T, R>
    operator/(const ReproducibleWrapper<T, R> &rhs) const {
        SAFE_BINOP(result, value, rhs.value, /);
        return ReproducibleWrapper(result);
    }

    /* Arithmetic assignment operators */
    FEATURE_CXX20(constexpr)
    inline ReproducibleWrapper<T, R> &
    operator+=(const ReproducibleWrapper<T, R> &rhs) {
        SAFE_BINOP(result, value, rhs.value, +);
        value = result;
        return *this;
    }

    FEATURE_CXX20(constexpr)
    inline ReproducibleWrapper<T, R> &
    operator-=(const ReproducibleWrapper<T, R> &rhs) {
        SAFE_BINOP(result, value, rhs.value, -);
        value = result;
        return *this;
    }

    FEATURE_CXX20(constexpr)
    inline ReproducibleWrapper<T, R> &
    operator*=(const ReproducibleWrapper<T, R> &rhs) {
        SAFE_BINOP(result, value, rhs.value, *);
        value = result;
        return *this;
    }

    FEATURE_CXX20(constexpr)
    inline ReproducibleWrapper<T, R> &
    operator/=(const ReproducibleWrapper<T, R> &rhs) {
        SAFE_BINOP(result, value, rhs.value, /);
        value = result;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &stream,
                                    const ReproducibleWrapper<T, R> &x) {
        return stream << x.value;
    }

    // Support expressions with non-reproducible types
    // on the left hand side by converting them to reproducible types
    // with the same type and rounding mode as the right hand side
    // first. This should be essentially free, and the type overloads
    // will ensure the operation is done safely.
    FEATURE_CXX20(constexpr)
    friend inline ReproducibleWrapper<T, R>
    operator+(const T &lhs, const ReproducibleWrapper<T, R> &rhs) {
        return ReproducibleWrapper(lhs) + rhs;
    }

    FEATURE_CXX20(constexpr)
    friend inline ReproducibleWrapper<T, R>
    operator-(const T &lhs, const ReproducibleWrapper<T, R> &rhs) {
        return ReproducibleWrapper(lhs) - rhs;
    }

    FEATURE_CXX20(constexpr)
    friend inline ReproducibleWrapper<T, R>
    operator*(const T &lhs, const ReproducibleWrapper<T, R> &rhs) {
        return ReproducibleWrapper(lhs) * rhs;
    }

    FEATURE_CXX20(constexpr)
    friend inline ReproducibleWrapper<T, R>
    operator/(const T &lhs, const ReproducibleWrapper<T, R> &rhs) {
        return ReproducibleWrapper(lhs) / rhs;
    }

#if __cplusplus >= 202002L
    constexpr friend inline auto
    operator<=>(const T &lhs, const ReproducibleWrapper<T, R> &rhs) {
        return ReproducibleWrapper(lhs) <=> rhs;
    };
#else

    // Relational operators
    FEATURE_CXX20(constexpr)
    friend inline auto operator==(const T &lhs,
                                  const ReproducibleWrapper<T, R> &rhs) {
        return ReproducibleWrapper(lhs) == rhs;
    }

    FEATURE_CXX20(constexpr)
    friend inline auto operator!=(const T &lhs,
                                  const ReproducibleWrapper<T, R> &rhs) {
        return ReproducibleWrapper(lhs) != rhs;
    }

    FEATURE_CXX20(constexpr)
    friend inline auto operator<=(const T &lhs,
                                  const ReproducibleWrapper<T, R> &rhs) {
        return ReproducibleWrapper(lhs) <= rhs;
    }

    FEATURE_CXX20(constexpr)
    friend inline auto operator>=(const T &lhs,
                                  const ReproducibleWrapper<T, R> &rhs) {
        return ReproducibleWrapper(lhs) >= rhs;
    }

    FEATURE_CXX20(constexpr)
    friend inline auto operator>(const T &lhs,
                                 const ReproducibleWrapper<T, R> &rhs) {
        return ReproducibleWrapper(lhs) > rhs;
    }

    FEATURE_CXX20(constexpr)
    friend inline auto operator<(const T &lhs,
                                 const ReproducibleWrapper<T, R> &rhs) {
        return ReproducibleWrapper(lhs) < rhs;
    }
#endif /* __cplusplus >= 202002L */
};

// It'd be nice to use C++20 template aliases here,
// but dependent types resolve after normal declarations,
// so doing it this way would make statements like
// `using f = rfloat;` invalid.
// Instead, pick reasonable defaults
using rfloat = ReproducibleWrapper<float, rmath::RoundingMode::ToEven>;
using rdouble = ReproducibleWrapper<double, rmath::RoundingMode::ToEven>;
using rounding_mode = rmath::RoundingMode;

// Sanity checks
static_assert(std::is_trivial<rfloat>::value &&
                  std::is_default_constructible<rfloat>::value,
              "something is wrong");
static_assert(std::is_trivial<rdouble>::value &&
                  std::is_default_constructible<rdouble>::value,
              "something is wrong");

#undef OPT_BARRIER
#undef SAFE_BINOP
