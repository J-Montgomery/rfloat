

#include "gen_repro_tests.hh"
#include "rcmath_tests.hh"
#include <rfloat/rcmath.hh>
#include <rfloat/rfloat.hh>

constexpr std::size_t steps = 1000;
constexpr double pi = 3.14159265358979323846;

template <typename T>
static std::array<T, 1>
check_iostream_operators(const std::array<T, 1> &input) {
    std::stringstream ss;
    ss << std::setprecision(17) << input[0];
    T result;
    ss >> result;
    return {result};
}

template <typename T>
static std::array<T, 3> lorenz(const std::array<T, 3> &input) {
    TestFunctions<T> func;
    auto result = func.lorenz(input, steps);
    return result;
}

template <typename T>
static std::array<T, 2> mandelbrot(const std::array<T, 2> &input) {
    TestFunctions<T> func;
    auto result = func.mandelbrot(input, steps);
    return result;
}

template <typename T>
static std::array<T, 1> logistic_map(const std::array<T, 2> &input) {
    TestFunctions<T> func;
    auto result = func.logistic_map(input[0], input[1] / 4.0, steps);
    return {result};
}

template <typename T>
static std::array<T, 1> check_abs(const std::array<T, 1> &input) {
    return {rmath::abs(input[0])};
}

template <typename T>
static std::array<T, 1> check_fmod(const std::array<T, 2> &input) {
    return {rmath::fmod(input[0], input[1])};
}

template <typename T>
static std::array<T, 1> check_fmin(const std::array<T, 2> &input) {
    return {rmath::fmin(input[0], input[1])};
}

template <typename T>
static std::array<T, 1> check_fmax(const std::array<T, 2> &input) {
    return {rmath::fmax(input[0], input[1])};
}

template <typename T>
static std::array<T, 1> check_fdim(const std::array<T, 2> &input) {
    return {rmath::fdim(input[0], input[1])};
}

template <typename T>
static std::array<T, 1> check_sqrt(const std::array<T, 1> &input) {
    return {rmath::sqrt(input[0])};
}

template <typename T>
static std::array<T, 1> check_ceil(const std::array<T, 1> &input) {
    return {rmath::ceil(input[0])};
}

template <typename T>
static std::array<T, 1> check_floor(const std::array<T, 1> &input) {
    return {rmath::floor(input[0])};
}

template <typename T>
static std::array<T, 1> check_round(const std::array<T, 1> &input) {
    return {rmath::round(input[0])};
}

template <typename T>
static std::array<T, 1> check_trunc(const std::array<T, 1> &input) {
    return {rmath::trunc(input[0])};
}

template <typename T>
static std::array<T, 1> check_nextafter(const std::array<T, 2> &input) {
    return {rmath::nextafter(input[0], input[1])};
}

template <typename T>
static std::array<T, 1> check_nexttoward(const std::array<T, 2> &input) {
    return {rmath::nexttoward(input[0], input[1])};
}

template <typename T>
static std::array<T, 1> check_nearbyint(const std::array<T, 1> &input) {
    return {rmath::nearbyint(input[0])};
}

template <typename T>
static std::array<T, 1> check_rint(const std::array<T, 1> &input) {
    return {rmath::rint(input[0])};
}

template <typename T>
static std::array<T, 1> check_remainder(const std::array<T, 2> &input) {
    return {rmath::remainder(input[0], input[1])};
}

template <typename T>
static std::array<T, 1> check_fma(const std::array<T, 3> &input) {
    return {rmath::fma(input[0], input[1], input[2])};
}

template <typename T>
static std::array<T, 1> check_isgreater(const std::array<T, 2> &input) {
    return {rmath::isgreater(input[0], input[1]) ? 1.0 : 0.0};
}

template <typename T>
static std::array<T, 1> check_isless(const std::array<T, 2> &input) {
    return {rmath::isless(input[0], input[1]) ? 1.0 : 0.0};
}

template <typename T>
static std::array<T, 1> check_islessequal(const std::array<T, 2> &input) {
    return {rmath::islessequal(input[0], input[1]) ? 1.0 : 0.0};
}

template <typename T>
static std::array<T, 1> check_isgreaterequal(const std::array<T, 2> &input) {
    return {rmath::isgreaterequal(input[0], input[1]) ? 1.0 : 0.0};
}

template <typename T>
static std::array<T, 1> check_islessgreater(const std::array<T, 2> &input) {
    return {rmath::islessgreater(input[0], input[1]) ? 1.0 : 0.0};
}

template <typename T>
static std::array<T, 1> check_isunordered(const std::array<T, 2> &input) {
    return {rmath::isunordered(input[0], input[1]) ? 1.0 : 0.0};
}

#if defined(RMATH_NONDETERMINISTIC)

template <typename T>
static std::array<T, 1> check_sin(const std::array<T, 1> &input) {
    return {rmath::sin(input[0])};
}

template <typename T>
static std::array<T, 1> check_cos(const std::array<T, 1> &input) {
    return {rmath::cos(input[0])};
}

template <typename T>
static std::array<T, 1> check_tan(const std::array<T, 1> &input) {
    return {rmath::tan(input[0])};
}

template <typename T>
static std::array<T, 1> check_asin(const std::array<T, 1> &input) {
    return {rmath::asin(input[0])};
}

template <typename T>
static std::array<T, 1> check_acos(const std::array<T, 1> &input) {
    return {rmath::acos(input[0])};
}

template <typename T>
static std::array<T, 1> check_atan(const std::array<T, 1> &input) {
    return {rmath::atan(input[0])};
}

template <typename T>
static std::array<T, 1> check_atan2(const std::array<T, 2> &input) {
    return {rmath::atan2(input[0], input[1])};
}

template <typename T>
static std::array<T, 1> check_sinh(const std::array<T, 1> &input) {
    return {rmath::sinh(input[0])};
}

template <typename T>
static std::array<T, 1> check_cosh(const std::array<T, 1> &input) {
    return {rmath::cosh(input[0])};
}

template <typename T>
static std::array<T, 1> check_tanh(const std::array<T, 1> &input) {
    return {rmath::tanh(input[0])};
}

template <typename T>
static std::array<T, 1> check_asinh(const std::array<T, 1> &input) {
    return {rmath::asinh(input[0])};
}

template <typename T>
static std::array<T, 1> check_acosh(const std::array<T, 1> &input) {
    return {rmath::acosh(input[0])};
}

template <typename T>
static std::array<T, 1> check_atanh(const std::array<T, 1> &input) {
    return {rmath::atanh(input[0])};
}

template <typename T>
static std::array<T, 1> check_hypot(const std::array<T, 2> &input) {
    return {rmath::hypot(input[0], input[1])};
}

template <typename T>
static std::array<T, 1> check_log(const std::array<T, 1> &input) {
    return {rmath::log(input[0])};
}

template <typename T>
static std::array<T, 1> check_log2(const std::array<T, 1> &input) {
    return {rmath::log2(input[0])};
}

template <typename T>
static std::array<T, 1> check_log10(const std::array<T, 1> &input) {
    return {rmath::log10(input[0])};
}

template <typename T>
static std::array<T, 1> check_log1p(const std::array<T, 1> &input) {
    return {rmath::log1p(input[0])};
}

template <typename T>
static std::array<T, 1> check_exp(const std::array<T, 1> &input) {
    return {rmath::exp(input[0])};
}

template <typename T>
static std::array<T, 1> check_exp2(const std::array<T, 1> &input) {
    return {rmath::exp2(input[0])};
}

template <typename T>
static std::array<T, 1> check_expm1(const std::array<T, 1> &input) {
    return {rmath::expm1(input[0])};
}

template <typename T>
static std::array<T, 1> check_pow(const std::array<T, 2> &input) {
    return {rmath::pow(input[0], input[1])};
}

template <typename T>
static std::array<T, 1> check_cbrt(const std::array<T, 1> &input) {
    return {rmath::cbrt(input[0])};
}

template <typename T>
static std::array<T, 1> check_erf(const std::array<T, 1> &input) {
    return {rmath::erf(input[0])};
}

template <typename T>
static std::array<T, 1> check_erfc(const std::array<T, 1> &input) {
    return {rmath::erfc(input[0])};
}

template <typename T>
static std::array<T, 1> check_lgamma(const std::array<T, 1> &input) {
    return {rmath::lgamma(input[0])};
}

template <typename T>
static std::array<T, 1> check_tgamma(const std::array<T, 1> &input) {
    return {rmath::tgamma(input[0])};
}

#if __cpp_lib_interpolate >= 201902L
template <typename T>
static std::array<T, 1> check_lerp(const std::array<T, 3> &input) {
    return {rmath::lerp(input[0], input[1], input[2])};
}
#endif /* __cpp_lib_interpolate >= 201902L */

#endif /* defined(RMATH_NONDETERMINISTIC)*/

int main() {
    using TestType = rdouble;

    // basic deterministic tests
    auto random_abs_inputs = uniform_random_args<TestType, 1>(100);
    generate_test_data<TestType, 1, 1>("random_abs", check_abs<TestType>,
                                       random_abs_inputs);

    auto random_sqrt_inputs = uniform_random_args<TestType, 1>(100, 0.0);
    generate_test_data<TestType, 1, 1>("random_sqrt", check_sqrt<TestType>,
                                       random_sqrt_inputs);

    auto random_ceil_inputs = uniform_random_args<TestType, 1>(100);
    generate_test_data<TestType, 1, 1>("random_ceil", check_ceil<TestType>,
                                       random_ceil_inputs);

    auto random_floor_inputs = uniform_random_args<TestType, 1>(100);
    generate_test_data<TestType, 1, 1>("random_floor", check_floor<TestType>,
                                       random_floor_inputs);

    auto random_round_inputs = uniform_random_args<TestType, 1>(100);
    generate_test_data<TestType, 1, 1>("random_round", check_round<TestType>,
                                       random_round_inputs);

    auto random_trunc_inputs = uniform_random_args<TestType, 1>(100);
    generate_test_data<TestType, 1, 1>("random_trunc", check_trunc<TestType>,
                                       random_trunc_inputs);

    auto random_rint_inputs = uniform_random_args<TestType, 1>(100);
    generate_test_data<TestType, 1, 1>("random_rint", check_rint<TestType>,
                                       random_rint_inputs);

    auto random_nearbyint_inputs = uniform_random_args<TestType, 1>(100);
    generate_test_data<TestType, 1, 1>(
        "random_nearbyint", check_nearbyint<TestType>, random_nearbyint_inputs);

    auto random_nextafter_inputs = uniform_random_args<TestType, 2>(
        100, std::numeric_limits<TestType>::min(), 0.000001);
    generate_test_data<TestType, 2, 1>(
        "random_nextafter", check_nextafter<TestType>, random_nextafter_inputs);

    auto random_nexttoward_inputs = uniform_random_args<TestType, 2>(
        100, std::numeric_limits<TestType>::min(), 0.000001);
    generate_test_data<TestType, 2, 1>("random_nexttoward",
                                       check_nexttoward<TestType>,
                                       random_nexttoward_inputs);

    auto random_fmod_inputs = uniform_random_args<TestType, 2>(100);
    generate_test_data<TestType, 2, 1>("random_fmod", check_fmod<TestType>,
                                       random_fmod_inputs);

    auto random_fmax_inputs = uniform_random_args<TestType, 2>(100);
    generate_test_data<TestType, 2, 1>("random_fmax", check_fmax<TestType>,
                                       random_fmax_inputs);

    auto random_fmin_inputs = uniform_random_args<TestType, 2>(100);
    generate_test_data<TestType, 2, 1>("random_fmin", check_fmin<TestType>,
                                       random_fmin_inputs);

    auto random_fdim_inputs = uniform_random_args<TestType, 2>(100);
    generate_test_data<TestType, 2, 1>("random_fdim", check_fdim<TestType>,
                                       random_fdim_inputs);

    auto random_remainder_inputs = uniform_random_args<TestType, 2>(100);
    generate_test_data<TestType, 2, 1>(
        "random_remainder", check_remainder<TestType>, random_remainder_inputs);

    auto random_fma_inputs = normal_random_args<TestType, 3>(100, 0.0, 3000.0);
    generate_test_data<TestType, 3, 1>("random_fma", check_fma<TestType>,
                                       random_fma_inputs);

    auto random_isgreater_inputs = uniform_random_args<TestType, 2>(100);
    generate_test_data<TestType, 2, 1>(
        "random_isgreater", check_isgreater<TestType>, random_isgreater_inputs);

    auto random_isless_inputs = uniform_random_args<TestType, 2>(100);
    generate_test_data<TestType, 2, 1>("random_isless", check_isless<TestType>,
                                       random_isless_inputs);

    auto random_islessequal_inputs = uniform_random_args<TestType, 2>(100);
    generate_test_data<TestType, 2, 1>("random_islessequal",
                                       check_islessequal<TestType>,
                                       random_islessequal_inputs);

    auto random_isgreaterequal_inputs = uniform_random_args<TestType, 2>(100);
    generate_test_data<TestType, 2, 1>("random_isgreaterequal",
                                       check_isgreaterequal<TestType>,
                                       random_isgreaterequal_inputs);

    auto random_islessgreater_inputs = uniform_random_args<TestType, 2>(100);
    generate_test_data<TestType, 2, 1>("random_islessgreater",
                                       check_islessgreater<TestType>,
                                       random_islessgreater_inputs);
#if defined(RMATH_NONDETERMINISTIC)
    // exp/log
    auto random_exp_inputs = uniform_random_args<TestType, 1>(100, -10.0, 10.0);
    generate_test_data<TestType, 1, 1>("random_exp", check_exp<TestType>,
                                       random_exp_inputs);

    auto random_exp2_inputs =
        uniform_random_args<TestType, 1>(100, -10.0, 10.0);
    generate_test_data<TestType, 1, 1>("random_exp2", check_exp2<TestType>,
                                       random_exp2_inputs);

    auto random_expm1_inputs =
        uniform_random_args<TestType, 1>(100, -10.0, 10.0);
    generate_test_data<TestType, 1, 1>("random_expm1", check_expm1<TestType>,
                                       random_expm1_inputs);

    auto random_log_inputs = uniform_random_args<TestType, 1>(100, 0.0);
    generate_test_data<TestType, 1, 1>("random_log", check_log<TestType>,
                                       random_log_inputs);

    auto random_log2_inputs = uniform_random_args<TestType, 1>(100);
    generate_test_data<TestType, 1, 1>("random_log2", check_log2<TestType>,
                                       random_log2_inputs);

    auto random_log10_inputs = uniform_random_args<TestType, 1>(100);
    generate_test_data<TestType, 1, 1>("random_log10", check_log10<TestType>,
                                       random_log10_inputs);

    auto random_log1p_inputs = uniform_random_args<TestType, 1>(100);
    generate_test_data<TestType, 1, 1>("random_log1p", check_log1p<TestType>,
                                       random_log1p_inputs);

    // trig functions
    auto random_sin_inputs = uniform_random_args<TestType, 1>(100, 0.0, pi);
    generate_test_data<TestType, 1, 1>("random_sin", check_sin<TestType>,
                                       random_sin_inputs);

    auto random_cos_inputs = uniform_random_args<TestType, 1>(100, 0.0, pi);
    generate_test_data<TestType, 1, 1>("random_cos", check_cos<TestType>,
                                       random_cos_inputs);

    auto random_tan_inputs =
        uniform_random_args<TestType, 1>(100, -pi / 2.0, pi / 2.0);
    generate_test_data<TestType, 1, 1>("random_tan", check_tan<TestType>,
                                       random_tan_inputs);

    auto random_asin_inputs = uniform_random_args<TestType, 1>(100, -1.0, 1.0);
    generate_test_data<TestType, 1, 1>("random_asin", check_asin<TestType>,
                                       random_asin_inputs);

    auto random_acos_inputs = uniform_random_args<TestType, 1>(100, -1.0, 1.0);
    generate_test_data<TestType, 1, 1>("random_acos", check_acos<TestType>,
                                       random_acos_inputs);

    auto random_atan_inputs =
        uniform_random_args<TestType, 1>(100, -pi / 2.0, pi / 2.0);
    generate_test_data<TestType, 1, 1>("random_atan", check_atan<TestType>,
                                       random_atan_inputs);

    auto random_atan2_inputs =
        uniform_random_args<TestType, 2>(100, -pi / 2.0, pi / 2.0);
    generate_test_data<TestType, 2, 1>("random_atan2", check_atan2<TestType>,
                                       random_atan2_inputs);

    auto random_hypot_inputs =
        uniform_random_args<TestType, 2>(100, -pi / 2.0, pi / 2.0);
    generate_test_data<TestType, 2, 1>("random_hypot", check_hypot<TestType>,
                                       random_hypot_inputs);

    // hyperbolic functions
    auto random_sinh_inputs =
        uniform_random_args<TestType, 1>(100, -pi * 2.0, pi * 2.0);
    generate_test_data<TestType, 1, 1>("random_sinh", check_sinh<TestType>,
                                       random_sinh_inputs);

    auto random_cosh_inputs =
        uniform_random_args<TestType, 1>(100, -pi * 2.0, pi * 2.0);
    generate_test_data<TestType, 1, 1>("random_cosh", check_cosh<TestType>,
                                       random_cosh_inputs);

    auto random_tanh_inputs =
        uniform_random_args<TestType, 1>(100, -pi / 2.0, pi / 2.0);
    generate_test_data<TestType, 1, 1>("random_tanh", check_tanh<TestType>,
                                       random_tanh_inputs);

    auto random_asinh_inputs =
        uniform_random_args<TestType, 1>(100, -pi * 2.0, pi * 2.0);
    generate_test_data<TestType, 1, 1>("random_asinh", check_asinh<TestType>,
                                       random_asinh_inputs);

    auto random_acosh_inputs =
        uniform_random_args<TestType, 1>(100, -pi * 2.0, pi * 2.0);
    generate_test_data<TestType, 1, 1>("random_acosh", check_acosh<TestType>,
                                       random_acosh_inputs);

    auto random_atanh_inputs =
        uniform_random_args<TestType, 1>(100, -pi / 2.0, pi / 2.0);
    generate_test_data<TestType, 1, 1>("random_atanh", check_atanh<TestType>,
                                       random_atanh_inputs);
    // powers
    auto random_cbrt_inputs = uniform_random_args<TestType, 1>(100, 0.0);
    generate_test_data<TestType, 1, 1>("random_cbrt", check_cbrt<TestType>,
                                       random_cbrt_inputs);
    auto random_pow_inputs = uniform_random_args<TestType, 2>(100, -10.0, 10.0);
    generate_test_data<TestType, 2, 1>("random_pow", check_pow<TestType>,
                                       random_pow_inputs);

    // advanced functions
    auto random_erf_inputs = uniform_random_args<TestType, 1>(100, -5.0, 5.0);
    generate_test_data<TestType, 1, 1>("random_erf", check_erf<TestType>,
                                       random_erf_inputs);

    auto random_erfc_inputs = uniform_random_args<TestType, 1>(100, -5.0, 5.0);
    generate_test_data<TestType, 1, 1>("random_erfc", check_erfc<TestType>,
                                       random_erfc_inputs);

    auto random_lgamma_inputs =
        uniform_random_args<TestType, 1>(100, 0.0, 10.0);
    generate_test_data<TestType, 1, 1>("random_lgamma", check_lgamma<TestType>,
                                       random_lgamma_inputs);

    auto random_tgamma_inputs =
        uniform_random_args<TestType, 1>(100, 0.0, 10.0);
    generate_test_data<TestType, 1, 1>("random_tgamma", check_tgamma<TestType>,
                                       random_tgamma_inputs);
#if __cpp_lib_interpolate >= 201902L
    auto random_lerp_inputs = uniform_random_args<TestType, 3>(100);
    generate_test_data<TestType, 3, 1>("random_lerp", check_lerp<TestType>,
                                       random_lerp_inputs);
#endif /* __cpp_lib_interpolate >= 201902L */
#endif /* defined(RMATH_NONDETERMINISTIC) */

    // Chaotic function tests
    auto random_lorenz_inputs = normal_random_args<TestType, 3>(100, 0.0, 20.0);
    generate_test_data<TestType, 3, 3>("random_lorenz", lorenz<TestType>,
                                       random_lorenz_inputs);
    auto random_mandelbrot_inputs =
        normal_random_args<TestType, 2>(100, 0.0, 20.0);
    generate_test_data<TestType, 2, 2>(
        "random_mandelbrot", mandelbrot<TestType>, random_mandelbrot_inputs);

    auto random_logistic_map_inputs =
        normal_random_args<TestType, 2>(100, 3.5, 0.25);
    generate_test_data<TestType, 2, 1>("random_logistic_map",
                                       logistic_map<TestType>,
                                       random_logistic_map_inputs);

    // Implementation validation tests
    auto random_iostream_large_inputs = uniform_random_args<TestType, 1>(
        100, std::numeric_limits<typename TestType::underlying_type>::min(),
        std::numeric_limits<typename TestType::underlying_type>::max());
    auto random_iostream_medium_inputs = uniform_random_args<TestType, 1>(100);
    auto random_iostream_small_inputs = uniform_random_args<TestType, 1>(
        100, std::numeric_limits<typename TestType::underlying_type>::min(),
        0.00000001);
    generate_test_data<TestType, 1, 1>("random_iostream_large",
                                       check_iostream_operators<TestType>,
                                       random_iostream_large_inputs);
    generate_test_data<TestType, 1, 1>("random_iostream_medium",
                                       check_iostream_operators<TestType>,
                                       random_iostream_medium_inputs);

    generate_test_data<TestType, 1, 1>("random_iostream_small",
                                       check_iostream_operators<TestType>,
                                       random_iostream_small_inputs);

    return 0;
}
