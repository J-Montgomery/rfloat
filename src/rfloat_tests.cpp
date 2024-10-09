#include "gtest/gtest.h"

#include <rfloat>

// Test fixture for NumericWrapper
class InterfaceTest : public ::testing::Test {
  protected:
    const double d1 = 3.14159265358979323846;
    const double d2 = 2.71828182845904523536;
    const float f1 = 3.14159265358979323846f;
    const float f2 = 2.71828182845904523536f;
};

// Tests for rdouble
TEST_F(InterfaceTest, rdouble_arithmetic) {
    rdouble rd1(d1);
    rdouble rd2(d2);

    EXPECT_EQ(rd1 + rd2, d1 + d2);
    EXPECT_EQ(rd1 - rd2, d1 - d2);
    EXPECT_EQ(rd1 * rd2, d1 * d2);
    EXPECT_EQ(rd1 / rd2, d1 / d2);
}

TEST_F(InterfaceTest, rfloat_assignment) {
    rfloat rf1(f1);
    rfloat rf2(f1);
    rf1 = f2;
    rf2 = rf1;

    EXPECT_EQ(f2, rf1);
    EXPECT_EQ(rf1, rf2);
}

TEST_F(InterfaceTest, rdouble_assignment) {
    rdouble rd1(d1);
    rdouble rd2(d1);
    rd1 = d2;
    rd2 = rd1;

    EXPECT_EQ(d2, rd1);
    EXPECT_EQ(rd1, rd2);
}

TEST_F(InterfaceTest, rfloat_relational) {
    rfloat rf1(f1);
    rfloat rf2(f2);

    EXPECT_LT(rf2, rf1);
    EXPECT_GT(rf1, rf2);
    EXPECT_LE(rf1, rf1);
    EXPECT_GE(rf2, rf2);
    EXPECT_NE(rf1, rf2);

    EXPECT_LT(rf2, f1);
    EXPECT_GT(rf1, f2);
    EXPECT_LE(rf1, f1);
    EXPECT_GE(rf2, f2);
    EXPECT_NE(rf1, f2);

    // Make sure the reverse expression also compiles
    EXPECT_GT(f1, rf2);
    EXPECT_LT(f2, rf1);
    EXPECT_GE(f1, rf1);
    EXPECT_LE(f2, f2);
    EXPECT_NE(f1, rf2);
}

TEST_F(InterfaceTest, rdouble_relational) {
    rdouble rd1(d1);
    rdouble rd2(d2);

    EXPECT_LT(rd2, rd1);
    EXPECT_GT(rd1, rd2);
    EXPECT_LE(rd1, rd1);
    EXPECT_GE(rd2, rd2);
    EXPECT_NE(rd1, rd2);

    EXPECT_LT(rd2, d1);
    EXPECT_GT(rd1, d2);
    EXPECT_LE(rd1, d1);
    EXPECT_GE(rd2, d2);
    EXPECT_NE(rd1, d2);

    EXPECT_GT(d1, rd2);
    EXPECT_LT(d2, rd1);
    EXPECT_GE(d1, rd1);
    EXPECT_LE(d2, d2);
    EXPECT_NE(d1, rd2);
}

TEST_F(InterfaceTest, rdouble_conversion) {
    rdouble rd1(d1);
    EXPECT_EQ(d1, rd1.fp64());

    // This should generate a compile error,
    // but GTest has no way to verify that
    // EXPECT_EQ(static_cast<float>(d1), rd1.fp32());
}

// Tests for rfloat
TEST_F(InterfaceTest, rfloat_arithmetic) {
    rfloat rf1(f1);
    rfloat rf2(f2);

    EXPECT_EQ(rf1 + rf2, f1 + f2);
    EXPECT_EQ(rf1 - rf2, f1 - f2);
    EXPECT_EQ(rf1 * rf2, f1 * f2);
    EXPECT_EQ(rf1 / rf2, f1 / f2);
}

TEST_F(InterfaceTest, rfloat_conversion) {
    rfloat fw(f1);
    EXPECT_EQ(static_cast<double>(f1), fw.fp64());
    EXPECT_EQ(f1, fw.fp32());
}

TEST_F(InterfaceTest, rdouble_move_semantics) {
    rdouble rd1(d1);
    rdouble rd2(d2);

    EXPECT_EQ(std::move(rd1) + rd2, d1 + d2);
    EXPECT_EQ(std::move(rd1) - rd2, d1 - d2);
    EXPECT_EQ(std::move(rd1) * rd2, d1 * d2);
    EXPECT_EQ(std::move(rd1) / rd2, d1 / d2);

    // Test reversed commutative operations
    EXPECT_EQ(rd1 + std::move(rd2), d1 + d2);
    EXPECT_EQ(rd1 * std::move(rd2), d1 * d2);

    // The inverted - and / operators should generate compile
    // errors, but GTest has no way to verify that
}

TEST_F(InterfaceTest, rfloat_move_semantics) {
    rfloat rf1(f1);
    rfloat rf2(f2);

    EXPECT_EQ(std::move(rf1) - rf2, f1 - f2);
    EXPECT_EQ(std::move(rf1) * rf2, f1 * f2);
    EXPECT_EQ(std::move(rf1) + rf2, f1 + f2);
    EXPECT_EQ(std::move(rf1) / rf2, f1 / f2);

    // Test reversed commutative operations
    EXPECT_EQ(rf1 + std::move(rf2), f1 + f2);
    EXPECT_EQ(rf1 * std::move(rf2), f1 * f2);

    // The inverted - and / operators should generate compile
    // errors, but GTest has no way to verify that
}

TEST_F(InterfaceTest, test_lhs_rhs_insensitivity) {
    rfloat rf1(f1);
    rdouble rd1(d1);

    EXPECT_EQ(d1 + rd1, rd1 + d1);
    EXPECT_EQ(f1 + rf1, rf1 + f1);
}

TEST_F(InterfaceTest, check_narrowing_detected) {
    rfloat rf1(f1);
    rdouble rd1(d1);

    // There's an implicit narrowing conversion that happens here
    // because we've defined conversions. Unfortunately,
    // there's no way to detect or warn about that narrowing
    // with library code, so be careful
    EXPECT_EQ(rf1 + d1, f1 + d1);
    EXPECT_EQ(rd1 + f1, f1 + d1);
}

TEST_F(InterfaceTest, check_mixed_types_allowed) {
    float a;
    rfloat b;

    auto c = a + b;
    static_assert(std::is_same<decltype(c), rfloat>::value);
}

TEST_F(InterfaceTest, check_explicit_upcasts_allowed) {
    rfloat a(f1);
    rdouble b(a);

    auto c = b + b;

    // A dummy test
    EXPECT_EQ(c, 2 * b);
}

TEST_F(InterfaceTest, check_numeric_limits) {
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

TEST_F(InterfaceTest, check_float_iostream_interfaces) {
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
        EXPECT_EQ(x, y);
    }
}

TEST_F(InterfaceTest, check_double_iostream_interfaces) {
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
        EXPECT_EQ(x, y);
    }
}

TEST_F(InterfaceTest, check_float_unary_operations) {
    rfloat a(f1);

    auto b = -a;
    static_assert(std::is_same<decltype(b), rfloat>::value);

    auto c = +a;
    static_assert(std::is_same<decltype(c), rfloat>::value);

    EXPECT_EQ(a, c);
    EXPECT_EQ(-b, c);
}

TEST_F(InterfaceTest, check_underlying_value) {
    rfloat rf1(f1);
    rdouble rd1(d1);

    static_assert(std::is_same<decltype(rf1.underlying_value()), float>::value);
    static_assert(
        std::is_same<decltype(rd1.underlying_value()), double>::value);

    EXPECT_EQ(rf1.underlying_value(), f1);
    EXPECT_EQ(rd1.underlying_value(), d1);
}

TEST_F(InterfaceTest, check_conversion_docs) {
    rfloat a(f1);
    float b = a.underlying_value();
    EXPECT_EQ(b, f1);

    rfloat c(f1);
    rdouble d =
        c.fp64(); // Casts are allowed as long as they don't lose precision
    EXPECT_EQ(d, f1);

    rdouble e(f1);
    float f = e.underlying_value();
    EXPECT_EQ(f, f1);
}

TEST_F(InterfaceTest, check_docs_interest_example) {
    rdouble principal{1000.0};
    rdouble interest_rate{5.0};
    constexpr rdouble term = 5;

    rdouble rate = interest_rate / 100.0;
    auto total = principal * rstd::pow(rate + 1.0, term);
    EXPECT_FLOAT_EQ(total.underlying_value(), 1276.2816);
}

// This test shouldn't compile
// TEST_F(InterfaceTest, check_downcasts_prohibited) {
//     rdouble a(d1);
//     rfloat b = a;

//     auto c = b + b;
//     EXPECT_EQ(c, 2 * b);
// }

// This should generate compile errors
// TEST_F(InterfaceTest, check_implicit_upcasts_prohibited) {
//     rfloat a(f1);
//     rdouble b = a; // Prohibit implicit upcasts

//     auto c = b + b;
//     EXPECT_EQ(c, 2 * b);
// }

// This should generate compile errors
// TEST_F(InterfaceTest, check_mixed_types_prohibited) {
//     rfloat rf1(f1);
//     rdouble rd1(d1);

//     EXPECT_EQ(rd1 + rf1, d1 + f1);
// }

// This should generate compile errors
// TEST_F(InterfaceTest, test_equality_static_failure) {
//     rfloat rf1(f1);
//     int i = 0;

//     EXPECT_TRUE(rf1 == i);
// }

// This should generate compile errors
// TEST_F(InterfaceTest, validate_non_iec559) {
//     ReproducibleWrapper<long double> rd1(f1);

//     EXPECT_EQ(rd1 + d1, rd1 + d1);
// }

// This should generate compile errors because the cast functions are deleted
// TEST_F(InterfaceTest, c_cast_fails) {
//     rfloat rf1(f1);
//     rdouble rd1(d1);

//     EXPECT_EQ((float)rf1, f1);
//     EXPECT_EQ((double)rd1, d1);
// }