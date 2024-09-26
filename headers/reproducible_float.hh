#pragma once

#include <fenv.h>
#include <limits>
#include <type_traits>

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

#define SAFE_BINOP(result, a, b, op)                                           \
    typeof(a) result = (a)op(b);                                               \
    OPT_BARRIER(result);

enum class RoundingMode {
    RoundToEven,
    RoundToPositive,
    RoundToNegative,
    RoundToZero
};

template <typename T, RoundingMode R = RoundingMode::RoundToEven,
          bool init_rounding = false>
class NumericWrapper {
    static_assert(std::is_same<T, float>::value ||
                      std::is_same<T, double>::value,
                  "Unsupported floating point type");
    static_assert(std::is_floating_point<T>::value,
                  "Non-floating point types are not supported");
    static_assert(std::numeric_limits<T>::is_iec559,
                  "Type must be IEC 559 / IEEE 754 compliant");

  public:
    T value;

    explicit NumericWrapper(T val) : value(val) {
        if constexpr (init_rounding) {
            setround();
        }
    }

    NumericWrapper<T, R> &operator=(T val) {
        value = val;
        return *this;
    }

    template <RoundingMode R2>
    NumericWrapper<T, R> &operator=(const NumericWrapper<T, R2> &other) {
        value = other.value;
        return *this;
    }

    // Delete implicit conversion operators
    operator float() const = delete;
    operator double() const = delete;

    inline double fp64() const { return static_cast<double>(value); }

    template <typename T2 = T, typename = typename std::enable_if_t<
                                   std::is_same<T2, float>::value>>
    inline T2 fp32() const {
        return static_cast<float>(value);
    }

    void setround() {
        if (R == RoundingMode::RoundToEven) {
            fesetround(FE_TONEAREST);
        } else if constexpr (R == RoundingMode::RoundToPositive) {
            fesetround(FE_UPWARD);
        } else if constexpr (R == RoundingMode::RoundToNegative) {
            fesetround(FE_DOWNWARD);
        } else if constexpr (R == RoundingMode::RoundToZero) {
            fesetround(FE_TOWARDZERO);
        } else {
            static_assert(R == RoundingMode::RoundToEven,
                          "Rounding mode is not supported");
        }
    }
};

#if __cplusplus >= 202002
template <RoundingMode R = RoundingMode::RoundToEven,
          bool init_rounding = false>
using rfloat = NumericWrapper<float, R, init_rounding>;
template <RoundingMode R = RoundingMode::RoundToEven,
          bool init_rounding = false>
using rdouble = NumericWrapper<double, R, init_rounding>;
#else
using rfloat = NumericWrapper<float, RoundingMode::RoundToEven, false>;
using rdouble = NumericWrapper<double, RoundingMode::RoundToEven, false>;
#endif

template <typename T1, typename T2>
inline bool operator==(const NumericWrapper<T1> &lhs, const T2 &rhs) {
    if constexpr (std::is_same<T2, float>::value) {
        if constexpr (std::is_same<T1, T2>::value) {
            // If we're both floats,
            // then we can compare them without downcasting
            return lhs.value == rhs;
        } else {
            // Otherwise, we have to upcast both to doubles to
            // compare them
            return lhs.value == static_cast<decltype(lhs.fp64())>(rhs);
        }
    } else if constexpr (std::is_same<T2, double>::value) {
        return lhs.fp64() == rhs;
    }
}

/* Relational operators */

// Declare operator== as commutative to resolve the argument order ambiguity
// for the compiler
template <typename T1, typename T2>
inline bool operator==(const T2 &lhs, const NumericWrapper<T1> &rhs) {
    return rhs == lhs;
}

template <typename T1, typename T2>
inline bool operator==(const NumericWrapper<T1> &lhs,
                       const NumericWrapper<T2> &rhs) {
    if constexpr (std::is_same_v<T1, T2>) {
        return lhs.value == rhs.value;
    } else {
        static_assert(std::is_same_v<T1, double> && std::is_same_v<T2, double>);
        return lhs.fp64() == rhs.fp64();
    }
}

/* Arithmetic operators */
/* operator+() */
template <typename T, RoundingMode R>
static inline NumericWrapper<T, R> operator+(const NumericWrapper<T, R> &lhs,
                                             const T &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    SAFE_BINOP(result, lhs.value, rhs, +);
    return NumericWrapper(result);
}

template <typename T, RoundingMode R>
static inline NumericWrapper<T, R> operator+(const T &lhs,
                                             const NumericWrapper<T, R> &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    SAFE_BINOP(result, lhs, rhs.value, +);
    return NumericWrapper(result);
}

template <typename T, RoundingMode R>
static inline NumericWrapper<T, R> operator+(const NumericWrapper<T, R> &lhs,
                                             const NumericWrapper<T, R> &rhs) {
    SAFE_BINOP(result, lhs.value, rhs.value, +);
    return NumericWrapper(result);
}

/* operator-() */
template <typename T, RoundingMode R>
static inline NumericWrapper<T, R> operator-(const NumericWrapper<T, R> &lhs,
                                             const T &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    SAFE_BINOP(result, lhs.value, rhs, -);
    return NumericWrapper(result);
}

template <typename T, RoundingMode R>
static inline NumericWrapper<T, R> operator-(const T &lhs,
                                             const NumericWrapper<T, R> &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    SAFE_BINOP(result, lhs, rhs.value, -);
    return NumericWrapper(result);
}

template <typename T, RoundingMode R>
static inline NumericWrapper<T, R> operator-(const NumericWrapper<T, R> &lhs,
                                             const NumericWrapper<T, R> &rhs) {
    SAFE_BINOP(result, lhs.value, rhs.value, -);
    return NumericWrapper(result);
}

/* operator*() */
template <typename T, RoundingMode R>
static inline NumericWrapper<T, R> operator*(const NumericWrapper<T, R> &lhs,
                                             const T &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    SAFE_BINOP(result, lhs.value, rhs, *);
    return NumericWrapper(result);
}

template <typename T, RoundingMode R>
static inline NumericWrapper<T, R> operator*(const T &lhs,
                                             const NumericWrapper<T, R> &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    SAFE_BINOP(result, lhs, rhs.value, *);
    return NumericWrapper(result);
}

template <typename T, RoundingMode R>
static inline NumericWrapper<T, R> operator*(const NumericWrapper<T, R> &lhs,
                                             const NumericWrapper<T, R> &rhs) {
    SAFE_BINOP(result, lhs.value, rhs.value, *);
    return NumericWrapper(result);
}

/* operator/() */
template <typename T, RoundingMode R>
static inline NumericWrapper<T, R> operator/(const NumericWrapper<T, R> &lhs,
                                             const T &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    SAFE_BINOP(result, lhs.value, rhs, /);
    return NumericWrapper(result);
}

template <typename T, RoundingMode R>
static inline NumericWrapper<T, R> operator/(const T &lhs,
                                             const NumericWrapper<T, R> &rhs) {
    static_assert(std::numeric_limits<T>::is_iec559);
    SAFE_BINOP(result, lhs, rhs.value, /);
    return NumericWrapper(result);
}

template <typename T, RoundingMode R>
static inline NumericWrapper<T, R> operator/(const NumericWrapper<T, R> &lhs,
                                             const NumericWrapper<T, R> &rhs) {
    SAFE_BINOP(result, lhs.value, rhs.value, /);
    return NumericWrapper(result);
}

#undef OPT_BARRIER
#undef SAFE_BINOP
