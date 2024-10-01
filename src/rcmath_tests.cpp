#include <gtest/gtest.h>
#include <limits>
#include <random>
#include <vector>

#include "rcmath.hh"
#include "rfloat.hh"

#include "rcmath_tests.hh"

const std::vector<double> special_cases = {
    0.0,
    -0.0,
    std::numeric_limits<double>::infinity(),
    -std::numeric_limits<double>::infinity(),
    std::numeric_limits<double>::min(),
    -std::numeric_limits<double>::min(),
    std::numeric_limits<double>::denorm_min(),
    -std::numeric_limits<double>::denorm_min(),
    std::numeric_limits<double>::quiet_NaN(),
    std::numeric_limits<double>::signaling_NaN()};

template <typename T> std::vector<double> generate_random_doubles(int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(std::numeric_limits<T>::min(),
                                         std::numeric_limits<T>::max());

    std::vector<double> random_numbers;
    for (int i = 0; i < count; ++i) {
        random_numbers.push_back(dis(gen));
    }
    return random_numbers;
}

class RMathFunctionTest : public ::testing::TestWithParam<double> {};

TEST_P(RMathFunctionTest, AbsTest) {
    double a = GetParam();
    rdouble rd1(a);

    auto expected = std::abs(rd1.fp64());
    auto result = rmath::abs(rd1);

    EXPECT_EQ(result, expected);
}

TEST_P(RMathFunctionTest, SqrtTest) {
    double a = GetParam();
    rdouble rd1(a);

    auto expected = std::sqrt(rd1.fp64());
    auto result = rmath::sqrt(rd1);
    auto square = result * result;

    EXPECT_EQ(result, expected);
    EXPECT_FLOAT_EQ(square.fp64(), rd1.fp64());
}

TEST_P(RMathFunctionTest, FminTest) {
    double a = GetParam();
    double b = GetParam();
    rdouble rd1(a), rd2(b);

    auto expected = std::fmin(rd1.fp64(), rd2.fp64());
    auto result = rmath::fmin(rd1, rd2);

    EXPECT_EQ(result, expected);
}

TEST_P(RMathFunctionTest, FmaxTest) {
    double a = GetParam();
    double b = GetParam();
    rdouble rd1(a), rd2(b);

    auto expected = std::fmax(rd1.fp64(), rd2.fp64());
    auto result = rmath::fmax(rd1, rd2);

    EXPECT_EQ(result, expected);
}

TEST_P(RMathFunctionTest, FmaTest) {
    double a = GetParam();
    double b = GetParam();
    double c = GetParam();
    rdouble rd1(a), rd2(b), rd3(c);

    auto expected = std::fma(rd1.fp64(), rd2.fp64(), rd3.fp64());
    auto result = rmath::fma(rd1, rd2, rd3);

    EXPECT_EQ(result, expected);
}

INSTANTIATE_TEST_SUITE_P(
    RandomNumbers, RMathFunctionTest,
    ::testing::ValuesIn(generate_random_doubles<double>(100)));
class RFloatReproducibilityTest : public ::testing::Test {
  public:
    using TestType = float;
    TestFunctions<TestType> test;
};

TEST_F(RFloatReproducibilityTest, iir_filter) {
    std::vector<TestType> ffw_coeff = {1.0f, 2.0f, 3.0f};
    std::vector<TestType> fb_coeff = {1.0f, 2.0f, 3.0f};
    std::vector<TestType> history = {3.0f, 3.0f, 3.0f};
    std::vector<TestType> output = {0.0f, 0.0f};
    TestType expected = 18.0f;

    auto result = test.iir_filter(ffw_coeff, fb_coeff, history, output);
    EXPECT_EQ(result, expected);
}

TEST_F(RFloatReproducibilityTest, fir_filter) {
    std::vector<TestType> coeff = {1.0f, 2.0f, 3.0f};
    std::vector<TestType> history = {3.0f, 3.0f, 3.0f};
    std::vector<TestType> output = {0.0f, 0.0f};
    TestType expected = 18.0f;

    auto result = test.fir_filter(coeff, history, output);
    EXPECT_EQ(result, expected);
}

TEST_F(RFloatReproducibilityTest, MatrixMultiplication) {
    using Matrix = typename decltype(test)::Matrix;
    Matrix input = {
        {{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, {7.0f, 8.0f, 9.0f}}};

    Matrix expected = {{{30.0f, 36.0f, 42.0f},
                        {66.0f, 81.0f, 96.0f},
                        {102.0f, 126.0f, 150.0f}}};

    auto result = test.matrix_multiply(input, input);
    EXPECT_EQ(result, expected);
}

TEST_F(RFloatReproducibilityTest, LorenzAttractor) {
    using Array3 = typename decltype(test)::Array3;
    Array3 input = {0.0, 1.0, 0.0};
    Array3 expected = {-8.43770503997803f, -7.06214666366577f,
                       28.6556606292725f};

    auto result = test.lorenz(input, 1000);
    EXPECT_EQ(result, expected);
}