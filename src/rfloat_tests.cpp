#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <unordered_map>

#include <rfloat>

#define CHECK_FLOAT_EQ(a, b) CHECK_LT(std::abs(a - b), 1e-6f)

const double d1 = 3.14159265358979323846;
const double d2 = 2.71828182845904523536;
const float f1 = 3.14159265358979323846f;
const float f2 = 2.71828182845904523536f;

// Tests for rdouble
TEST_CASE("InterfaceTest.rdouble_arithmetic") {
    rdouble rd1(d1);
    rdouble rd2(d2);

    CHECK_EQ(rd1 + rd2, d1 + d2);
    CHECK_EQ(rd1 - rd2, d1 - d2);
    CHECK_EQ(rd1 * rd2, d1 * d2);
    CHECK_EQ(rd1 / rd2, d1 / d2);
}

TEST_CASE("InterfaceTest.rfloat_assignment") {
    rfloat rf1(f1);
    rfloat rf2(f1);
    rf1 = f2;
    rf2 = rf1;

    CHECK_EQ(f2, rf1);
    CHECK_EQ(rf1, rf2);
}

TEST_CASE("InterfaceTest.rdouble_assignment") {
    rdouble rd1(d1);
    rdouble rd2(d1);
    rd1 = d2;
    rd2 = rd1;

    CHECK_EQ(d2, rd1);
    CHECK_EQ(rd1, rd2);
}

TEST_CASE("InterfaceTest.rfloat_relational") {
    rfloat rf1(f1);
    rfloat rf2(f2);

    CHECK_LT(rf2, rf1);
    CHECK_GT(rf1, rf2);
    CHECK_LE(rf1, rf1);
    CHECK_GE(rf2, rf2);
    CHECK_NE(rf1, rf2);

    CHECK_LT(rf2, f1);
    CHECK_GT(rf1, f2);
    CHECK_LE(rf1, f1);
    CHECK_GE(rf2, f2);
    CHECK_NE(rf1, f2);

    // Make sure the reverse expression also compiles
    CHECK_GT(f1, rf2);
    CHECK_LT(f2, rf1);
    CHECK_GE(f1, rf1);
    CHECK_LE(f2, f2);
    CHECK_NE(f1, rf2);
}

TEST_CASE("InterfaceTest.rdouble_relational") {
    rdouble rd1(d1);
    rdouble rd2(d2);

    CHECK_LT(rd2, rd1);
    CHECK_GT(rd1, rd2);
    CHECK_LE(rd1, rd1);
    CHECK_GE(rd2, rd2);
    CHECK_NE(rd1, rd2);

    CHECK_LT(rd2, d1);
    CHECK_GT(rd1, d2);
    CHECK_LE(rd1, d1);
    CHECK_GE(rd2, d2);
    CHECK_NE(rd1, d2);

    CHECK_GT(d1, rd2);
    CHECK_LT(d2, rd1);
    CHECK_GE(d1, rd1);
    CHECK_LE(d2, d2);
    CHECK_NE(d1, rd2);
}

TEST_CASE("InterfaceTest.rdouble_conversion") {
    rdouble rd1(d1);
    CHECK_EQ(d1, rd1.fp64());

    // This should generate a compile error,
    // but GTest has no way to verify that
    // CHECK_EQ(static_cast<float>(d1), rd1.fp32());
}

// Tests for rfloat
TEST_CASE("InterfaceTest.rfloat_arithmetic") {
    rfloat rf1(f1);
    rfloat rf2(f2);

    CHECK_EQ(rf1 + rf2, f1 + f2);
    CHECK_EQ(rf1 - rf2, f1 - f2);
    CHECK_EQ(rf1 * rf2, f1 * f2);
    CHECK_EQ(rf1 / rf2, f1 / f2);
}

TEST_CASE("InterfaceTest.rfloat_conversion") {
    rfloat fw(f1);
    CHECK_EQ(static_cast<double>(f1), fw.fp64());
    CHECK_EQ(f1, fw.fp32());
}

TEST_CASE("InterfaceTest.rdouble_move_semantics") {
    rdouble rd1(d1);
    rdouble rd2(d2);

    CHECK_EQ(std::move(rd1) + rd2, d1 + d2);
    CHECK_EQ(std::move(rd1) - rd2, d1 - d2);
    CHECK_EQ(std::move(rd1) * rd2, d1 * d2);
    CHECK_EQ(std::move(rd1) / rd2, d1 / d2);

    // Test reversed commutative operations
    CHECK_EQ(rd1 + std::move(rd2), d1 + d2);
    CHECK_EQ(rd1 * std::move(rd2), d1 * d2);

    // The inverted - and / operators should generate compile
    // errors, but GTest has no way to verify that
}

TEST_CASE("InterfaceTest.rfloat_move_semantics") {
    rfloat rf1(f1);
    rfloat rf2(f2);

    CHECK_EQ(std::move(rf1) - rf2, f1 - f2);
    CHECK_EQ(std::move(rf1) * rf2, f1 * f2);
    CHECK_EQ(std::move(rf1) + rf2, f1 + f2);
    CHECK_EQ(std::move(rf1) / rf2, f1 / f2);

    // Test reversed commutative operations
    CHECK_EQ(rf1 + std::move(rf2), f1 + f2);
    CHECK_EQ(rf1 * std::move(rf2), f1 * f2);

    // The inverted - and / operators should generate compile
    // errors, but GTest has no way to verify that
}

TEST_CASE("InterfaceTest.test_lhs_rhs_insensitivity") {
    rfloat rf1(f1);
    rdouble rd1(d1);

    CHECK_EQ(d1 + rd1, rd1 + d1);
    CHECK_EQ(f1 + rf1, rf1 + f1);
}

TEST_CASE("InterfaceTest.check_narrowing_detected") {
    rfloat rf1(f1);
    rdouble rd1(d1);

    // There's an implicit narrowing conversion that happens here
    // because we've defined conversions. Unfortunately,
    // there's no way to detect or warn about that narrowing
    // with library code, so be careful
    CHECK_EQ(rf1 + d1, f1 + d1);
    CHECK_EQ(rd1 + f1, f1 + d1);
}

TEST_CASE("InterfaceTest.check_mixed_types_allowed") {
    float a;
    rfloat b;

    auto c = a + b;
    static_assert(std::is_same<decltype(c), rfloat>::value);
}

TEST_CASE("InterfaceTest.check_explicit_upcasts_allowed") {
    rfloat a(f1);
    rdouble b(a);

    auto c = b + b;

    // A dummy test
    CHECK_EQ(c, 2 * b);
}

TEST_CASE("InterfaceTest.check_numeric_limits") {
    static_assert(std::numeric_limits<rfloat>::digits ==
                  std::numeric_limits<float>::digits);
    static_assert(std::numeric_limits<rfloat>::digits10 ==
                  std::numeric_limits<float>::digits10);

    static_assert(std::numeric_limits<rdouble>::digits ==
                  std::numeric_limits<double>::digits);
    static_assert(std::numeric_limits<rdouble>::digits10 ==
                  std::numeric_limits<double>::digits10);

    static_assert(std::numeric_limits<rfloat>::min() ==
                  std::numeric_limits<float>::min());
    static_assert(std::numeric_limits<rfloat>::max() ==
                  std::numeric_limits<float>::max());

    static_assert(std::numeric_limits<rdouble>::min() ==
                  std::numeric_limits<double>::min());
    static_assert(std::numeric_limits<rdouble>::max() ==
                  std::numeric_limits<double>::max());

    static_assert(std::numeric_limits<rfloat>::lowest() ==
                  std::numeric_limits<float>::lowest());
    static_assert(std::numeric_limits<rfloat>::denorm_min() ==
                  std::numeric_limits<float>::denorm_min());
    static_assert(std::numeric_limits<rfloat>::infinity() ==
                  std::numeric_limits<float>::infinity());
    static_assert(std::numeric_limits<rfloat>::epsilon() ==
                  std::numeric_limits<float>::epsilon());

    static_assert(std::numeric_limits<rdouble>::lowest() ==
                  std::numeric_limits<double>::lowest());
    static_assert(std::numeric_limits<rdouble>::denorm_min() ==
                  std::numeric_limits<double>::denorm_min());
    static_assert(std::numeric_limits<rdouble>::infinity() ==
                  std::numeric_limits<double>::infinity());
    static_assert(std::numeric_limits<rdouble>::epsilon() ==
                  std::numeric_limits<double>::epsilon());

    static_assert(std::numeric_limits<rfloat>::is_iec559 ==
                  std::numeric_limits<float>::is_iec559);
    static_assert(std::numeric_limits<rdouble>::is_iec559 ==
                  std::numeric_limits<double>::is_iec559);

    static_assert(std::numeric_limits<rfloat>::has_infinity ==
                  std::numeric_limits<float>::has_infinity);
    static_assert(std::numeric_limits<rdouble>::has_infinity ==
                  std::numeric_limits<double>::has_infinity);

    static_assert(std::numeric_limits<rfloat>::has_quiet_NaN ==
                  std::numeric_limits<float>::has_quiet_NaN);
    static_assert(std::numeric_limits<rdouble>::has_quiet_NaN ==
                  std::numeric_limits<double>::has_quiet_NaN);

    static_assert(std::numeric_limits<rfloat>::has_signaling_NaN ==
                  std::numeric_limits<float>::has_signaling_NaN);
    static_assert(std::numeric_limits<rdouble>::has_signaling_NaN ==
                  std::numeric_limits<double>::has_signaling_NaN);

    static_assert(std::is_same<rfloat::underlying_type, float>::value);
    static_assert(std::is_same<rdouble::underlying_type, double>::value);
}

TEST_CASE("InterfaceTest.check_float_iostream_interfaces") {
    std::vector<rfloat> special_values = {
        1.0f,
        1.0f / 3.0f,
        3.14159265358979323846f,
        0.0,
        -0.0,
        std::numeric_limits<rfloat>::min(),
        -std::numeric_limits<rfloat>::min(),
        std::numeric_limits<rfloat>::max(),
        -std::numeric_limits<rfloat>::max(),
        std::numeric_limits<rfloat>::denorm_min()};

    std::stringstream ss;

    for (const auto &x : special_values) {
        ss << std::setprecision(17) << x << " ";
        rfloat y = 0.0;
        ss >> y;
        CHECK_EQ(x, y);
    }
}

TEST_CASE("InterfaceTest.check_double_iostream_interfaces") {
    std::vector<rdouble> special_values = {
        1.0,
        1.0 / 3.0,
        3.14159265358979323846,
        0.0,
        -0.0,
        std::numeric_limits<rdouble>::min(),
        -std::numeric_limits<rdouble>::min(),
        std::numeric_limits<rdouble>::max(),
        -std::numeric_limits<rdouble>::max(),
        std::numeric_limits<rdouble>::denorm_min()};

    for (const auto &x : special_values) {
        std::stringstream ss("");
        ss << std::setprecision(17) << x << " ";
        rdouble y = 0.0;
        ss >> y;
        CHECK_EQ(x, y);
    }
}

TEST_CASE("InterfaceTest.check_float_unary_operations") {
    rfloat a(f1);

    auto b = -a;
    static_assert(std::is_same<decltype(b), rfloat>::value);

    auto c = +a;
    static_assert(std::is_same<decltype(c), rfloat>::value);

    CHECK_EQ(a, c);
    CHECK_EQ(-b, c);
}

TEST_CASE("InterfaceTest.check_underlying_value") {
    rfloat rf1(f1);
    rdouble rd1(d1);

    static_assert(std::is_same<decltype(rf1.underlying_value()), float>::value);
    static_assert(
        std::is_same<decltype(rd1.underlying_value()), double>::value);

    CHECK_EQ(rf1.underlying_value(), f1);
    CHECK_EQ(rd1.underlying_value(), d1);
}

TEST_CASE("InterfaceTest.check_conversion_docs") {
    rfloat a(f1);
    float b = a.underlying_value();
    CHECK_EQ(b, f1);

    rfloat c(f1);
    rdouble d =
        c.fp64(); // Casts are allowed as long as they don't lose precision
    CHECK_EQ(d, f1);

    rdouble e(f1);
    float f = e.underlying_value();
    CHECK_EQ(f, f1);
}

#if defined(RSTD_NONDETERMINISM) && defined(ENABLE_NONDETERMINISTIC_TESTS)
TEST_CASE("InterfaceTest.check_docs_interest_example") {
    rdouble principal{1000.0};
    rdouble interest_rate{5.0};
    constexpr rdouble term = 5;

    rdouble rate = interest_rate / 100.0;
    auto total = principal * rstd::pow(rate + 1.0, term);
    CHECK_FLOAT_EQ(total.underlying_value(), 1276.2815625);
}
#endif /* defined(RSTD_NONDETERMINISM) &&                                      \
          defined(ENABLE_NONDETERMINISTIC_TESTS) */

TEST_CASE("InterfaceTest.unordered_map_basic") {
    std::unordered_map<rfloat, rfloat> m;
    m[f1] = f1;
    m[f2] = f2;

    CHECK_EQ(m[f1], f1);
    CHECK_EQ(m[f2], f2);

    m[f1] = f2;
    CHECK_EQ(m[f1], f2);
    CHECK_EQ(m[f2], f2);

    m.erase(f1);
    try {
        auto x = m.at(f1);
        CHECK(false); // The previous statement should have thrown
    } catch (const std::out_of_range &) {
        CHECK(true);
    }
}

// This test shouldn't compile
// TEST_CASE("InterfaceTest.check_downcasts_prohibited") {
//     rdouble a(d1);
//     rfloat b = a;

//     auto c = b + b;
//     CHECK_EQ(c, 2 * b);
// }

// This should generate compile errors
// TEST_CASE("InterfaceTest.check_implicit_upcasts_prohibited") {
//     rfloat a(f1);
//     rdouble b = a; // Prohibit implicit upcasts

//     auto c = b + b;
//     CHECK_EQ(c, 2 * b);
// }

// This should generate compile errors
// TEST_CASE("InterfaceTest.check_mixed_types_prohibited") {
//     rfloat rf1(f1);
//     rdouble rd1(d1);

//     CHECK_EQ(rd1 + rf1, d1 + f1);
// }

// This should generate compile errors
// TEST_CASE("InterfaceTest.test_equality_static_failure") {
//     rfloat rf1(f1);
//     int i = 0;

//     CHECK_TRUE(rf1 == i);
// }

// This should generate compile errors. Unfortunately,
// the compile error it generates is "unsupported type"
// rather than "not iec559" because some toolchains/targets
// consider extended types to be IEC 559 compliant and
// return true, while others don't implement them.
// TEST_CASE("InterfaceTest.validate_non_iec559") {
//     rstd::ReproducibleWrapper<long double> rd1(f1);

//     CHECK_EQ(rd1 + d1, rd1 + d1);
// }

// This should generate compile errors because the cast functions are deleted
// TEST_CASE("InterfaceTest.c_cast_fails") {
//     rfloat rf1(f1);
//     rdouble rd1(d1);

//     CHECK_EQ((float)rf1, f1);
//     CHECK_EQ((double)rd1, d1);
// }