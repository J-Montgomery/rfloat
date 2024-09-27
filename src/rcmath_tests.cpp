#include <gtest/gtest.h>
#include <limits>
#include <random>
#include <vector>

#include "float_math.h" // rlibm
#include <cmath>

#include "rcmath.hh"

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

TEST_P(RMathFunctionTest, ExpTest) {
    double input = GetParam();

    rdouble rmath_result = rmath::exp(rdouble(input));
    double cmath_result = std::exp(input);
    double rlibm_result = rlibm_exp(input);

    if (std::isnan(input)) {
        EXPECT_TRUE(std::isnan(rmath_result.fp64()));
        EXPECT_TRUE(std::isnan(cmath_result));
        EXPECT_TRUE(std::isnan(rlibm_result));
    } else {
        EXPECT_EQ(rmath_result, cmath_result);
        EXPECT_EQ(rmath_result, rlibm_result);
    }
}

INSTANTIATE_TEST_SUITE_P(SpecialCases, RMathFunctionTest,
                         ::testing::ValuesIn(special_cases));

INSTANTIATE_TEST_SUITE_P(
    RandomNumbers, RMathFunctionTest,
    ::testing::ValuesIn(generate_random_doubles<double>(100)));
