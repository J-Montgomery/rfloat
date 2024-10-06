

#include "gen_repro_tests.hh"
#include "rcmath_tests.hh"
#include <rfloat/rcmath.hh>
#include <rfloat/rfloat.hh>

constexpr std::size_t steps = 1000;

template <typename T> std::array<T, 3> lorenz(const std::array<T, 3> &input) {
    TestFunctions<T> func;
    auto result = func.lorenz(input, steps);
    return result;
}

template <typename T>
std::array<T, 2> mandelbrot(const std::array<T, 2> &input) {
    TestFunctions<T> func;
    auto result = func.mandelbrot(input, steps);
    return result;
}

template <typename T>
std::array<T, 1> logistic_map(const std::array<T, 2> &input) {
    TestFunctions<T> func;
    auto result = func.logistic_map(input[0], input[1] / 4.0, steps);
    return {result};
}

template <typename T>
std::array<T, 1> check_abs(const std::array<T, 1> &input) {
    return {rmath::abs(input[0])};
}

template <typename T>
std::array<T, 1> check_sqrt(const std::array<T, 1> &input) {
    return {rmath::sqrt(input[0])};
}

template <typename T>
std::array<T, 1> check_ceil(const std::array<T, 1> &input) {
    return {rmath::ceil(input[0])};
}

template <typename T>
std::array<T, 1> check_floor(const std::array<T, 1> &input) {
    return {rmath::floor(input[0])};
}

template <typename T>
std::array<T, 1> check_round(const std::array<T, 1> &input) {
    return {rmath::round(input[0])};
}

template <typename T>
std::array<T, 1> check_fma(const std::array<T, 3> &input) {
    return {rmath::fma(input[0], input[1], input[2])};
}

int main() {
    using TestType = rdouble;

    auto random_abs_inputs = uniform_random_args<TestType, 1>(100);
    generate_test_data<TestType, 1, 1>("random_abs", check_abs<TestType>,
                                       random_abs_inputs);

    auto random_sqrt_inputs = uniform_random_args<TestType, 1>(100);
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

    auto random_fma_inputs = normal_random_args<TestType, 3>(100, 0.0, 3000.0);
    generate_test_data<TestType, 3, 1>("random_fma", check_fma<TestType>,
                                       random_fma_inputs);

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
    return 0;
}
