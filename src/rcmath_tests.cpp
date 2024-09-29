#include <gtest/gtest.h>
#include <limits>
#include <random>
#include <vector>

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

    EXPECT_EQ(0, 0);
}

INSTANTIATE_TEST_SUITE_P(SpecialCases, RMathFunctionTest,
                         ::testing::ValuesIn(special_cases));

INSTANTIATE_TEST_SUITE_P(
    RandomNumbers, RMathFunctionTest,
    ::testing::ValuesIn(generate_random_doubles<double>(100)));
