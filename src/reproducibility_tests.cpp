#include <array>
#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "rcmath_tests.hh"
#include <rfloat/rcmath.hh>
#include <rfloat/rfloat.hh>

#include "testdata/random_logistic_map_testdata.hh"
#include "testdata/random_lorenz_testdata.hh"
#include "testdata/random_mandelbrot_testdata.hh"

#include "testdata/random_abs_testdata.hh"
#include "testdata/random_ceil_testdata.hh"
#include "testdata/random_floor_testdata.hh"
#include "testdata/random_fma_testdata.hh"

// Infrastructure to read lines of test data from the headers
// included above and parsed into TestParam objects. Each line is a single test
// case.
template <typename T, std::size_t InputSize, std::size_t OutputSize>
struct TestParam {
    std::array<T, InputSize> inputs;
    std::array<T, OutputSize> expected_outputs;
};

template <typename T, std::size_t InputSize, std::size_t OutputSize>
class HeaderParameterizedTest : public ::testing::Test {
  public:
    static std::vector<TestParam<T, InputSize, OutputSize>>
    LoadTestData(std::string &data) {
        std::vector<TestParam<T, InputSize, OutputSize>> test_data;
        std::istringstream iss(data);
        std::string line;
        int line_number = 0;

        while (std::getline(iss, line)) {
            line_number++;

            if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
                continue;
            }

            std::istringstream line_stream(line);
            TestParam<T, InputSize, OutputSize> param;

            for (std::size_t i = 0; i < InputSize; ++i) {
                if (!(line_stream >> param.inputs[i])) {
                    throw std::runtime_error(
                        "Not enough valid input values on line " +
                        std::to_string(line_number));
                }
            }

            for (std::size_t i = 0; i < OutputSize; ++i) {
                if (!(line_stream >> param.expected_outputs[i])) {
                    throw std::runtime_error(
                        "Not enough valid output values on line " +
                        std::to_string(line_number));
                }
            }

            int extra;
            if (line_stream >> extra) {
                throw std::runtime_error("Too many values on line " +
                                         std::to_string(line_number));
            }

            test_data.push_back(param);
        }
        return test_data;
    }
};

/* Chaotic function tests*/
const std::size_t steps = 1000;

TEST(LorenzTest, RandomInputs) {
    auto test_data = HeaderParameterizedTest<rdouble, 3, 3>::LoadTestData(
        random_lorenz_testdata);
    using tf = TestFunctions<rdouble>;
    for (const auto &param : test_data) {
        auto results = TestFunctions<rdouble>::lorenz(param.inputs, steps);
        EXPECT_EQ(results, param.expected_outputs);
    }
}

TEST(MandelbrotTest, RandomInputs) {
    auto test_data = HeaderParameterizedTest<rdouble, 2, 2>::LoadTestData(
        random_mandelbrot_testdata);
    using tf = TestFunctions<rdouble>;
    for (const auto &param : test_data) {
        auto results = TestFunctions<rdouble>::mandelbrot(param.inputs, steps);
        EXPECT_EQ(results, param.expected_outputs);
    }
}

TEST(LogisticMapTest, RandomInputs) {
    auto test_data = HeaderParameterizedTest<rdouble, 2, 1>::LoadTestData(
        random_logistic_map_testdata);
    using tf = TestFunctions<rdouble>;
    for (const auto &param : test_data) {
        // We divide the x parameter by 4 on both sides because the random
        // numbers are generated with a distribution centered on 3.5 and x
        // should be in [0-1]
        auto results = TestFunctions<rdouble>::logistic_map(
            param.inputs[0], param.inputs[1] / 4.0, steps);
        EXPECT_EQ(results, param.expected_outputs[0]);
    }
}

/* rcmath tests */
TEST(AbsTest, RandomInputs) {
    auto test_data = HeaderParameterizedTest<rdouble, 1, 1>::LoadTestData(
        random_abs_testdata);
    for (const auto &param : test_data) {
        auto results = rmath::abs(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::abs(param.inputs[0].underlying_value()));
    }
}

TEST(FloorTest, RandomInputs) {
    auto test_data = HeaderParameterizedTest<rdouble, 1, 1>::LoadTestData(
        random_floor_testdata);
    for (const auto &param : test_data) {
        auto results = rmath::floor(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::floor(param.inputs[0].underlying_value()));
    }
}

TEST(CeilTest, RandomInputs) {
    auto test_data = HeaderParameterizedTest<rdouble, 1, 1>::LoadTestData(
        random_ceil_testdata);
    for (const auto &param : test_data) {
        auto results = rmath::ceil(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::ceil(param.inputs[0].underlying_value()));
    }
}

TEST(FmaTest, RandomInputs) {
    auto test_data = HeaderParameterizedTest<rdouble, 3, 1>::LoadTestData(
        random_fma_testdata);
    for (const auto &param : test_data) {
        auto results =
            rmath::fma(param.inputs[0], param.inputs[1], param.inputs[2]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::fma(param.inputs[0].underlying_value(),
                                    param.inputs[1].underlying_value(),
                                    param.inputs[2].underlying_value()));
    }
}