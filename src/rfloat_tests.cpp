#include "gtest/gtest.h"

#include <rfloat/rfloat.hh>

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