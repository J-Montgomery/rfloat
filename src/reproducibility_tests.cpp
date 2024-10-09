#include "rcmath_tests.hh"
#include <rfloat>

#include "reproducibility_tests.hh"

// Include the test cases
// Might be time to break this up into smaller files
#include "testdata/random_abs_testdata.hh"
#include "testdata/random_acos_testdata.hh"
#include "testdata/random_acosh_testdata.hh"
#include "testdata/random_asin_testdata.hh"
#include "testdata/random_asinh_testdata.hh"
#include "testdata/random_atan2_testdata.hh"
#include "testdata/random_atan_testdata.hh"
#include "testdata/random_atanh_testdata.hh"
#include "testdata/random_cbrt_testdata.hh"
#include "testdata/random_ceil_testdata.hh"
#include "testdata/random_cos_testdata.hh"
#include "testdata/random_cosh_testdata.hh"
#include "testdata/random_erf_testdata.hh"
#include "testdata/random_erfc_testdata.hh"
#include "testdata/random_exp2_testdata.hh"
#include "testdata/random_exp_testdata.hh"
#include "testdata/random_expm1_testdata.hh"
#include "testdata/random_fdim_testdata.hh"
#include "testdata/random_floor_testdata.hh"
#include "testdata/random_fma_testdata.hh"
#include "testdata/random_fmax_testdata.hh"
#include "testdata/random_fmin_testdata.hh"
#include "testdata/random_fmod_testdata.hh"
#include "testdata/random_hypot_testdata.hh"
#include "testdata/random_iostream_large_testdata.hh"
#include "testdata/random_iostream_medium_testdata.hh"
#include "testdata/random_iostream_small_testdata.hh"
#include "testdata/random_isgreater_testdata.hh"
#include "testdata/random_isgreaterequal_testdata.hh"
#include "testdata/random_isless_testdata.hh"
#include "testdata/random_islessequal_testdata.hh"
#include "testdata/random_islessgreater_testdata.hh"
#include "testdata/random_lerp_testdata.hh"
#include "testdata/random_lgamma_testdata.hh"
#include "testdata/random_log10_testdata.hh"
#include "testdata/random_log1p_testdata.hh"
#include "testdata/random_log2_testdata.hh"
#include "testdata/random_log_testdata.hh"
#include "testdata/random_logistic_map_testdata.hh"
#include "testdata/random_lorenz_testdata.hh"
#include "testdata/random_mandelbrot_testdata.hh"
#include "testdata/random_nearbyint_testdata.hh"
#include "testdata/random_nextafter_testdata.hh"
#include "testdata/random_nexttoward_testdata.hh"
#include "testdata/random_pow_testdata.hh"
#include "testdata/random_remainder_testdata.hh"
#include "testdata/random_rint_testdata.hh"
#include "testdata/random_round_testdata.hh"
#include "testdata/random_sin_testdata.hh"
#include "testdata/random_sinh_testdata.hh"
#include "testdata/random_sqrt_testdata.hh"
#include "testdata/random_tan_testdata.hh"
#include "testdata/random_tanh_testdata.hh"
#include "testdata/random_tgamma_testdata.hh"
#include "testdata/random_trunc_testdata.hh"

// Chaotic function tests
const std::size_t steps = 1000;

TEST(ChaoticFunctionTest, RandomInputsLorenz) {
    auto test_data =
        ParameterizedTest<rdouble, 3, 3>::LoadTestData(random_lorenz_testdata);
    for (const auto &param : test_data) {
        auto results = TestFunctions<rdouble>::lorenz(param.inputs, steps);
        EXPECT_EQ(results, param.expected_outputs);
    }
}

TEST(ChaoticFunctionTest, RandomInputsMandelbrot) {
    auto test_data = ParameterizedTest<rdouble, 2, 2>::LoadTestData(
        random_mandelbrot_testdata);
    for (const auto &param : test_data) {
        auto results = TestFunctions<rdouble>::mandelbrot(param.inputs, steps);
        EXPECT_EQ(results, param.expected_outputs);
    }
}

TEST(ChaoticFunctionTest, RandomInputsLogisticMap) {
    auto test_data = ParameterizedTest<rdouble, 2, 1>::LoadTestData(
        random_logistic_map_testdata);
    for (const auto &param : test_data) {
        // We divide the x parameter by 4 on both sides because the random
        // numbers are generated with a distribution centered on 3.5 and x
        // should be in [0-1]
        auto results = TestFunctions<rdouble>::logistic_map(
            param.inputs[0], param.inputs[1] / 4.0, steps);
        EXPECT_EQ(results, param.expected_outputs[0]);
    }
}

// rcmath tests
TEST(IntegerTests, RandomInputsFloor) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_floor_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::floor(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::floor(param.inputs[0].underlying_value()));
    }
}

TEST(IntegerTests, RandomInputsCeil) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_ceil_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::ceil(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::ceil(param.inputs[0].underlying_value()));
    }
}

TEST(IntegerTests, RandomInputsRound) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_round_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::round(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::round(param.inputs[0].underlying_value()));
    }
}

TEST(IntegerTests, RandomInputsTrunc) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_trunc_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::trunc(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::trunc(param.inputs[0].underlying_value()));
    }
}

TEST(IntegerTests, RandomInputsNearbyInt) {
    auto test_data = ParameterizedTest<rdouble, 1, 1>::LoadTestData(
        random_nearbyint_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::nearbyint(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::nearbyint(param.inputs[0].underlying_value()));
    }
}

TEST(IntegerTests, RandomInputsRint) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_rint_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::rint(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::rint(param.inputs[0].underlying_value()));
    }
}

TEST(BasicTest, RandomInputsAbs) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_abs_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::abs(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::abs(param.inputs[0].underlying_value()));
    }
}

TEST(BasicTest, RandomInputsFMA) {
    auto test_data =
        ParameterizedTest<rdouble, 3, 1>::LoadTestData(random_fma_testdata);
    for (const auto &param : test_data) {
        auto results =
            rstd::fma(param.inputs[0], param.inputs[1], param.inputs[2]);
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
        auto results = rstd::sqrt(param.inputs[0]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::sqrt(param.inputs[0].underlying_value()));
    }
}

TEST(BasicTest, RandomInputsNextAfter) {
    auto test_data = ParameterizedTest<rdouble, 2, 1>::LoadTestData(
        random_nextafter_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::nextafter(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::nextafter(param.inputs[0].underlying_value(),
                                          param.inputs[1].underlying_value()));
    }
}

TEST(BasicTest, RandomInputsNextToward) {
    auto test_data = ParameterizedTest<rdouble, 2, 1>::LoadTestData(
        random_nexttoward_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::nexttoward(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::nexttoward(param.inputs[0].underlying_value(),
                                           param.inputs[1].underlying_value()));
    }
}

TEST(BasicTest, RandomInputsFmod) {
    auto test_data =
        ParameterizedTest<rdouble, 2, 1>::LoadTestData(random_fmod_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::fmod(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::fmod(param.inputs[0].underlying_value(),
                                     param.inputs[1].underlying_value()));
    }
}

TEST(BasicTest, RandomInputsRemainder) {
    auto test_data = ParameterizedTest<rdouble, 2, 1>::LoadTestData(
        random_remainder_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::remainder(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::remainder(param.inputs[0].underlying_value(),
                                          param.inputs[1].underlying_value()));
    }
}

TEST(BasicTest, RandomInputsFdim) {
    auto test_data =
        ParameterizedTest<rdouble, 2, 1>::LoadTestData(random_fdim_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::fdim(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::fdim(param.inputs[0].underlying_value(),
                                     param.inputs[1].underlying_value()));
    }
}

TEST(BasicTest, RandomInputsFmax) {
    auto test_data =
        ParameterizedTest<rdouble, 2, 1>::LoadTestData(random_fmax_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::fmax(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::fmax(param.inputs[0].underlying_value(),
                                     param.inputs[1].underlying_value()));
    }
}

TEST(BasicTest, RandomInputsFmin) {
    auto test_data =
        ParameterizedTest<rdouble, 2, 1>::LoadTestData(random_fmin_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::fmin(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ(results, std::fmin(param.inputs[0].underlying_value(),
                                     param.inputs[1].underlying_value()));
    }
}

TEST(ComparisonTestsTest, RandomIsGreater) {
    auto test_data = ParameterizedTest<rdouble, 2, 1>::LoadTestData(
        random_isgreater_testdata);
    for (const auto &param : test_data) {
        auto results = rstd::isgreater(param.inputs[0], param.inputs[1]);
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
        auto results = rstd::isgreaterequal(param.inputs[0], param.inputs[1]);
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
        auto results = rstd::isless(param.inputs[0], param.inputs[1]);
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
        auto results = rstd::islessequal(param.inputs[0], param.inputs[1]);
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
        auto results = rstd::islessgreater(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(results, param.expected_outputs[0]);
        EXPECT_EQ((results == 1.0),
                  std::islessgreater(param.inputs[0].underlying_value(),
                                     param.inputs[1].underlying_value()));
    }
}

TEST(IOStreamTests, RandomLargeInputs) {
    auto test_data = ParameterizedTest<rdouble, 1, 1>::LoadTestData(
        random_iostream_large_testdata);
    for (const auto &param : test_data) {
        std::stringstream ss;
        ss << std::setprecision(17) << param.inputs[0];
        rdouble result;
        ss >> result;
        EXPECT_EQ(result, param.expected_outputs[0]);
    }
}

TEST(IOStreamTests, RandomMediumInputs) {
    auto test_data = ParameterizedTest<rdouble, 1, 1>::LoadTestData(
        random_iostream_medium_testdata);
    for (const auto &param : test_data) {
        std::stringstream ss;
        ss << std::setprecision(17) << param.inputs[0];
        rdouble result;
        ss >> result;
        EXPECT_EQ(result, param.expected_outputs[0]);
    }
}

TEST(IOStreamTests, RandomSmallInputs) {
    auto test_data = ParameterizedTest<rdouble, 1, 1>::LoadTestData(
        random_iostream_small_testdata);
    for (const auto &param : test_data) {
        std::stringstream ss;
        ss << std::setprecision(17) << param.inputs[0];
        rdouble result;
        ss >> result;
        EXPECT_EQ(result, param.expected_outputs[0]);
    }
}

#if defined(RSTD_NONDETERMINISM) && defined(ENABLE_NONDETERMINISTIC_TESTS)
TEST(TrigTests, RandomSin) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_sin_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::sin(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::sin(param.inputs[0].underlying_value()));
    }
}

TEST(TrigTests, RandomCos) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_cos_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::cos(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::cos(param.inputs[0].underlying_value()));
    }
}

TEST(TrigTests, RandomTan) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_tan_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::tan(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::tan(param.inputs[0].underlying_value()));
    }
}

TEST(TrigTests, RandomAsin) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_asin_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::asin(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::asin(param.inputs[0].underlying_value()));
    }
}

TEST(TrigTests, RandomAcos) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_acos_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::acos(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::acos(param.inputs[0].underlying_value()));
    }
}

TEST(TrigTests, RandomAtan) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_atan_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::atan(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::atan(param.inputs[0].underlying_value()));
    }
}

TEST(TrigTests, RandomAtan2) {
    auto test_data =
        ParameterizedTest<rdouble, 2, 1>::LoadTestData(random_atan2_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::atan2(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::atan2(param.inputs[0].underlying_value(),
                                     param.inputs[1].underlying_value()));
    }
}

TEST(TrigTests, RandomHypot) {
    auto test_data =
        ParameterizedTest<rdouble, 2, 1>::LoadTestData(random_hypot_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::hypot(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::hypot(param.inputs[0].underlying_value(),
                                     param.inputs[1].underlying_value()));
    }
}

TEST(LogExpTests, RandomLog) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_log_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::log(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::log(param.inputs[0].underlying_value()));
    }
}

TEST(LogExpTests, RandomExp) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_exp_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::exp(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::exp(param.inputs[0].underlying_value()));
    }
}

TEST(LogExpTests, RandomLog2) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_log2_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::log2(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::log2(param.inputs[0].underlying_value()));
    }
}

TEST(LogExpTests, RandomLog10) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_log10_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::log10(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::log10(param.inputs[0].underlying_value()));
    }
}

TEST(LogExpTests, RandomExp2) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_exp2_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::exp2(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::exp2(param.inputs[0].underlying_value()));
    }
}

TEST(LogExpTests, RandomLog1p) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_log1p_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::log1p(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::log1p(param.inputs[0].underlying_value()));
    }
}

TEST(LogExpTests, RandomExpm1) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_expm1_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::expm1(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::expm1(param.inputs[0].underlying_value()));
    }
}

TEST(PowTests, RandomPow) {
    auto test_data =
        ParameterizedTest<rdouble, 2, 1>::LoadTestData(random_pow_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::pow(param.inputs[0], param.inputs[1]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::pow(param.inputs[0].underlying_value(),
                                   param.inputs[1].underlying_value()));
    }
}

TEST(PowTests, RandomCbrt) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_cbrt_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::cbrt(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::cbrt(param.inputs[0].underlying_value()));
    }
}

TEST(HyperbolicTests, RandomSinh) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_sinh_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::sinh(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::sinh(param.inputs[0].underlying_value()));
    }
}

TEST(HyperbolicTests, RandomCosh) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_cosh_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::cosh(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::cosh(param.inputs[0].underlying_value()));
    }
}

TEST(HyperbolicTests, RandomTanh) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_tanh_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::tanh(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::tanh(param.inputs[0].underlying_value()));
    }
}

TEST(HyperbolicTests, RandomAsinh) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_asinh_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::asinh(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::asinh(param.inputs[0].underlying_value()));
    }
}

TEST(HyperbolicTests, RandomAcosh) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_acosh_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::acosh(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::acosh(param.inputs[0].underlying_value()));
    }
}

TEST(HyperbolicTests, RandomAtanh) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_atanh_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::atanh(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::atanh(param.inputs[0].underlying_value()));
    }
}

TEST(SpecialTests, RandomErf) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_erf_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::erf(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::erf(param.inputs[0].underlying_value()));
    }
}

TEST(SpecialTests, RandomErfc) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_erfc_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::erfc(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::erfc(param.inputs[0].underlying_value()));
    }
}

TEST(SpecialTests, RandomTgamma) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_tgamma_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::tgamma(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::tgamma(param.inputs[0].underlying_value()));
    }
}

TEST(SpecialTests, RandomLgamma) {
    auto test_data =
        ParameterizedTest<rdouble, 1, 1>::LoadTestData(random_lgamma_testdata);
    for (const auto &param : test_data) {
        auto result = rstd::lgamma(param.inputs[0]);
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::lgamma(param.inputs[0].underlying_value()));
    }
}

#if __cpp_lib_interpolate >= 201902L
TEST(SpecialTests, DISABLED_RandomLerp) {
    auto test_data =
        ParameterizedTest<rdouble, 3, 1>::LoadTestData(random_lerp_testdata);
    for (const auto &param : test_data) {
        auto result =
            rstd::lerp(param.inputs[0], param.inputs[1], param.inputs[2]);
        std::cout << std::setprecision(17) << result << "\n";
        std::cout << std::setprecision(17) << param.expected_outputs[0]
                  << "\n\n";
        EXPECT_EQ(result, param.expected_outputs[0]);
        EXPECT_EQ(result, std::lerp(param.inputs[0].underlying_value(),
                                    param.inputs[1].underlying_value(),
                                    param.inputs[2].underlying_value()));
    }
}
#endif /* __cpp_lib_interpolate >= 201902L */

#endif /* defined(RSTD_NONDETERMINISM) */
