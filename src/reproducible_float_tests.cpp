#include "gtest/gtest.h"

#include "../headers/reproducible_float.hh"

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
    rdouble dw1(d1);
    rdouble dw2(d2);

    EXPECT_EQ(dw1 + dw2, d1 + d2);
    EXPECT_EQ(dw1 - dw2, d1 - d2);
    EXPECT_EQ(dw1 * dw2, d1 * d2);
    EXPECT_EQ(dw1 / dw2, d1 / d2);
}

TEST_F(InterfaceTest, rfloat_pod_assignment) {
    rdouble fw1(f1);
    rdouble fw2(f1);
    fw1 = f2;
    fw2 = fw1;

    EXPECT_EQ(f2, fw1);
    EXPECT_EQ(fw1, fw2);
}

TEST_F(InterfaceTest, rdouble_assignment) {
    rdouble dw1(d1);
    rdouble dw2(d1);
    dw1 = d2;
    dw2 = dw1;

    EXPECT_EQ(d2, dw1);
    EXPECT_EQ(dw1, dw2);
}

TEST_F(InterfaceTest, rdouble_conversion) {
    rdouble dw(d1);
    EXPECT_EQ(d1, dw.fp64());

    // This should generate a compile error,
    // but GTest has no way to verify that
    // EXPECT_EQ(static_cast<float>(d1), dw.fp32());
}

// Tests for rfloat
TEST_F(InterfaceTest, rfloat_arithmetic) {
    rfloat fw1(f1);
    rfloat fw2(f2);

    EXPECT_EQ(fw1 + fw2, f1 + f2);
    EXPECT_EQ(fw1 - fw2, f1 - f2);
    EXPECT_EQ(fw1 * fw2, f1 * f2);
    EXPECT_EQ(fw1 / fw2, f1 / f2);
}

TEST_F(InterfaceTest, rfloat_conversion) {
    rfloat fw(f1);
    EXPECT_EQ(static_cast<double>(f1), fw.fp64());
    EXPECT_EQ(f1, fw.fp32());
}

TEST_F(InterfaceTest, rdouble_move_semantics) {
    rdouble dw1(d1);
    rdouble dw2(d2);

    EXPECT_EQ(std::move(dw1) + dw2, d1 + d2);
    EXPECT_EQ(std::move(dw1) - dw2, d1 - d2);
    EXPECT_EQ(std::move(dw1) * dw2, d1 * d2);
    EXPECT_EQ(std::move(dw1) / dw2, d1 / d2);

    // Test reversed commutative operations
    EXPECT_EQ(dw1 + std::move(dw2), d1 + d2);
    EXPECT_EQ(dw1 * std::move(dw2), d1 * d2);

    // The inverted - and / operators should generate compile
    // errors, but GTest has no way to verify that
}

TEST_F(InterfaceTest, rfloat_move_semantics) {
    rfloat fw1(f1);
    rfloat fw2(f2);

    EXPECT_EQ(std::move(fw1) - fw2, f1 - f2);
    EXPECT_EQ(std::move(fw1) * fw2, f1 * f2);
    EXPECT_EQ(std::move(fw1) + fw2, f1 + f2);
    EXPECT_EQ(std::move(fw1) / fw2, f1 / f2);

    // Test reversed commutative operations
    EXPECT_EQ(fw1 + std::move(fw2), f1 + f2);
    EXPECT_EQ(fw1 * std::move(fw2), f1 * f2);

    // The inverted - and / operators should generate compile
    // errors, but GTest has no way to verify that
}

TEST_F(InterfaceTest, test_lhs_rhs_insensitivity) {
    rfloat fw1(f1);
    rdouble dw1(d1);

    EXPECT_EQ(d1 + dw1, dw1 + d1);
    EXPECT_EQ(f1 + fw1, fw1 + f1);
}

// This should generate compile errors
// TEST_F(InterfaceTest, validate_non_iec559) {
//     NumericWrapper<long double> dw1(f1);

//     EXPECT_EQ(dw1 + d1, dw1 + d1);
// }
