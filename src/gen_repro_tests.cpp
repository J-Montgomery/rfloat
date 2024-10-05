

#include "gen_repro_tests.hh"
#include "rcmath_tests.hh"
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

int main() {
    using TestType = rdouble;

    auto random_lorenz_inputs =
        generate_random_args<TestType, 3>(0.0, 20.0, 100);
    generate_test_data<TestType, 3, 3>("random_lorenz", lorenz<TestType>,
                                       random_lorenz_inputs);

    auto random_mandelbrot_inputs =
        generate_random_args<TestType, 2>(0.0, 20.0, 100);
    generate_test_data<TestType, 2, 2>(
        "random_mandelbrot", mandelbrot<TestType>, random_mandelbrot_inputs);

    auto random_logistic_map_inputs =
        generate_random_args<TestType, 2>(3.5, 0.25, 100);
    generate_test_data<TestType, 2, 1>("random_logistic_map",
                                       logistic_map<TestType>,
                                       random_logistic_map_inputs);
    return 0;
}
