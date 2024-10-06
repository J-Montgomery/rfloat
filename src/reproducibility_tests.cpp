#include "rcmath_tests.hh"
#include <rfloat/rcmath.hh>
#include <rfloat/rfloat.hh>

#include "reproducibility_tests.hh"

#include "testdata/random_logistic_map_testdata.hh"
#include "testdata/random_lorenz_testdata.hh"
#include "testdata/random_mandelbrot_testdata.hh"

#include "testdata/random_abs_testdata.hh"
#include "testdata/random_ceil_testdata.hh"
#include "testdata/random_floor_testdata.hh"
#include "testdata/random_fma_testdata.hh"
#include "testdata/random_round_testdata.hh"
#include "testdata/random_sqrt_testdata.hh"

#include "testdata/random_isgreater_testdata.hh"
#include "testdata/random_isgreaterequal_testdata.hh"
#include "testdata/random_isless_testdata.hh"
#include "testdata/random_islessequal_testdata.hh"
#include "testdata/random_islessgreater_testdata.hh"

/* Chaotic function tests*/
const std::size_t steps = 1000;

TEST(ChaoticFunctionTest, RandomInputsLorenz) {
    auto test_data =
        ParameterizedTest<rdouble, 3, 3>::LoadTestData(random_lorenz_testdata);
    using tf = TestFunctions<rdouble>;
    for (const auto &param : test_data) {
        auto results = TestFunctions<rdouble>::lorenz(param.inputs, steps);
        EXPECT_EQ(results, param.expected_outputs);
    }
}

TEST(ChaoticFunctionTest, RandomInputsMandelbrot) {
    auto test_data = ParameterizedTest<rdouble, 2, 2>::LoadTestData(
        random_mandelbrot_testdata);
    using tf = TestFunctions<rdouble>;
    for (const auto &param : test_data) {
        auto results = TestFunctions<rdouble>::mandelbrot(param.inputs, steps);
        EXPECT_EQ(results, param.expected_outputs);
    }
}

TEST(ChaoticFunctionTest, RandomInputsLogisticMap) {
    auto test_data = ParameterizedTest<rdouble, 2, 1>::LoadTestData(
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
TEST(IntegerTests, RandomInputsFloor) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_floor_testdata);
    for (const auto &param : test_data) {
        auto results = rmath::floor(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::floor(param.inputs[0].underlying_value()));
    }
}

TEST(IntegerTests, RandomInputsCeil) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_ceil_testdata);
    for (const auto &param : test_data) {
        auto results = rmath::ceil(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::ceil(param.inputs[0].underlying_value()));
    }
}

TEST(IntegerTests, RandomInputsRound) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_round_testdata);
    for (const auto &param : test_data) {
        auto results = rmath::round(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::round(param.inputs[0].underlying_value()));
    }
}

// Basic functions
TEST(BasicTest, RandomInputsAbs) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_abs_testdata);
    for (const auto &param : test_data) {
        auto results = rmath::abs(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::abs(param.inputs[0].underlying_value()));
    }
}

TEST(BasicTest, RandomInputsFMA) {
    auto test_data =
        ParameterizedTest<rdouble, 3, 1>::LoadTestData(random_fma_testdata);
    for (const auto &param : test_data) {
        auto results =
            rmath::fma(param.inputs[0], param.inputs[1], param.inputs[2]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::fma(param.inputs[0].underlying_value(),
                                    param.inputs[1].underlying_value(),
                                    param.inputs[2].underlying_value()));
    }
}

TEST(BasicTest, RandomInputsSqrt) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_sqrt_testdata);
    for (const auto &param : test_data) {
        auto results = rmath::sqrt(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::sqrt(param.inputs[0].underlying_value()));
    }
}

// Comparison tests
TEST(ComparisonTestsTest, RandomIsGreater) {
    auto test_data = ParameterizedTest<rdouble, 2, 1>::LoadTestData(
        random_isgreater_testdata);
    for (const auto &param : test_data) {
        auto results = rmath::isgreater(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ((results == 1.0),
                  std::isgreater(param.inputs[0].underlying_value(),
                                 param.inputs[1].underlying_value()));
    }
}

TEST(ComparisonTestsTest, RandomIsGreaterEqual) {
    auto test_data = ParameterizedTest<rdouble, 2, 1>::LoadTestData(
        random_isgreaterequal_testdata);
    for (const auto &param : test_data) {
        auto results = rmath::isgreaterequal(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ((results == 1.0),
                  std::isgreaterequal(param.inputs[0].underlying_value(),
                                      param.inputs[1].underlying_value()));
    }
}

TEST(ComparisonTestsTest, RandomIsLess) {
    auto test_data =
        ParameterizedTest<rdouble, 2, 1>::LoadTestData(random_isless_testdata);
    for (const auto &param : test_data) {
        auto results = rmath::isless(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ((results == 1.0),
                  std::isless(param.inputs[0].underlying_value(),
                              param.inputs[1].underlying_value()));
    }
}

TEST(ComparisonTestsTest, RandomIsLessEqual) {
    auto test_data = ParameterizedTest<rdouble, 2, 1>::LoadTestData(
        random_islessequal_testdata);
    for (const auto &param : test_data) {
        auto results = rmath::islessequal(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ((results == 1.0),
                  std::islessequal(param.inputs[0].underlying_value(),
                                   param.inputs[1].underlying_value()));
    }
}

TEST(ComparisonTestsTest, RandomIsLessGreater) {
    auto test_data = ParameterizedTest<rdouble, 2, 1>::LoadTestData(
        random_islessgreater_testdata);
    for (const auto &param : test_data) {
        auto results = rmath::islessgreater(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ((results == 1.0),
                  std::islessgreater(param.inputs[0].underlying_value(),
                                     param.inputs[1].underlying_value()));
    }
}