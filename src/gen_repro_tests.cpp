

#include "gen_repro_tests.hh"
#include "rcmath_tests.hh"
#include <rfloat/rfloat.hh>

constexpr std::size_t steps = 1000;

template <typename T> std::array<T, 3> lorenz(const std::array<T, 3> &input) {
    TestFunctions<T> func;
    auto result = func.lorenz(input, steps);
    return result;
}

int main() {
    using TestType = rdouble;

    auto random_lorenz_inputs =
        generate_random_args<TestType, 3>(0.0, 20.0, 1000);
    generate_test_data<TestType, 3, 3>("random_lorenz", lorenz<TestType>,
                                       random_lorenz_inputs);

    return 0;
}
