#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <limits>
#include <random>
#include <vector>

#include <rfloat>

#include "rcmath_tests.hh"

namespace rfloat_tests {

using TestType = rfloat;
TestFunctions<TestType> test;
std::size_t steps = 1000;

TEST_CASE("RFloatReproducibilityTest.iir_filter") {
    std::vector<rfloat> ffw_coeff = {1.0f, 2.0f, 3.0f};
    std::vector<rfloat> fb_coeff = {1.0f, 2.0f, 3.0f};
    std::vector<rfloat> history = {3.0f, 3.0f, 3.0f};
    std::vector<rfloat> output = {0.0f, 2.0f};
    rfloat expected = 12.0f;

    auto result = test.iir_filter(ffw_coeff, fb_coeff, history, output);
    CHECK_EQ(result, expected);
}

TEST_CASE("RFloatReproducibilityTest.fir_filter") {
    std::vector<rfloat> coeff = {1.0f, 2.0f, 3.0f};
    std::vector<rfloat> history = {3.0f, 3.0f, 3.0f};
    std::vector<rfloat> output = {0.0f, 0.0f};
    rfloat expected = 18.0f;

    auto result = test.fir_filter(coeff, history, output);
    CHECK_EQ(result, expected);
}

TEST_CASE("RFloatReproducibilityTest.RandomMatrixMultiplication") {
    using Matrix = typename decltype(test)::Matrix;
    Matrix lhs = {
        {{1.234567890123456f, -0.000000001234567f, 11.123456f},
         {2.718281828459045f, 3.141592653589793f, -1.414213562373095f},
         {0.000000000000001f, -0.999999f, 0.618033988749895f}}};
    Matrix rhs = {
        {{0.123456791043282f, -0.987654328346252f, 0.555555582046509f},
         {0.111111111938953f, 0.222222223877907f, -0.333333343267441f},
         {0.444444447755814f, -0.666666686534882f, 0.888888895511627f}}};

    Matrix expected = {
        {{5.0961742401123f, -8.63496398925781f, 10.5733880996704f},
         {0.0561168193817139f, -1.04378175735474f, -0.794119715690613f},
         {0.163570776581764f, -0.634244680404663f, 0.882696568965912f}}};

    auto result = test.matrix_multiply(lhs, rhs);

    CHECK_EQ(result, expected);
}

TEST_CASE("RFloatReproducibilityTest.LorenzAttractor") {
    using Array3 = typename decltype(test)::Array3;
    Array3 input = {0.0, 1.0, 0.0};
    Array3 expected = {-8.43770503997803f, -7.06214666366577f,
                       28.6556606292725f};

    auto result = test.lorenz(input, steps);
    std::cout << std::setprecision(17) << result[0] << " " << result[1] << " "
              << result[2] << std::endl;
    std::cout << std::setprecision(17) << expected[0] << " " << expected[1]
              << " " << expected[2] << std::endl;
    CHECK_EQ(result, expected);
}

TEST_CASE("RFloatReproducibilityTest.Mandelbrot") {
    using Array2 = typename decltype(test)::Array2;
    Array2 coordinate = {0.001643721971153f, 0.822467633298876f};
    Array2 expected = {-3.53889393806458f, 0.821848750114441f};

    auto result = test.mandelbrot(coordinate, steps);
    CHECK_EQ(result, expected);
}

TEST_CASE("RFloatReproducibilityTest.LogisticMap") {
    rfloat r = 3.99f, x = 0.7f;
    rfloat expected = 0.062762513756752f;

    auto result = test.logistic_map(r, x, steps);
    CHECK_EQ(result, expected);
}

} // namespace rfloat_tests

namespace rdouble_tests {

using TestType = rdouble;
TestFunctions<TestType> test;
std::size_t steps = 1000;

TEST_CASE("RDoubleReproducibilityTest.RandomMatrixMultiplication") {
    using Matrix = typename decltype(test)::Matrix;
    Matrix lhs = {{{1.234567890123456, -0.000000001234567, 11.123456},
                   {2.718281828459045, 3.141592653589793, -1.414213562373095},
                   {0.000000000000001, -0.999999, 0.618033988749895}}};
    Matrix rhs = {{{0.123456791043282, -0.987654328346252, 0.555555582046509},
                   {0.111111111938953, 0.222222223877907, -0.333333343267441},
                   {0.444444447755814, -0.666666686534882, 0.888888895511627}}};

    Matrix expected = {
        {{5.0961740489586385, -8.6349638749286317, 10.573387601297169},
         {0.056116838954734183, -1.043782037863358, -0.79411967050488763},
         {0.16357077399642911, -0.63424467310151322, 0.88269655958263782}}};

    auto result = test.matrix_multiply(lhs, rhs);
    CHECK_EQ(result, expected);
}

TEST_CASE("RDoubleReproducibilityTest.LorenzAttractor") {
    using Array3 = typename decltype(test)::Array3;
    Array3 input = {0.0, 1.0, 0.0};
    Array3 expected = {-8.4283517044458289, -7.0491893984733949,
                       28.649356114765581};

    auto result = test.lorenz(input, steps);
    std::cout << std::setprecision(17) << result[0] << " " << result[1] << " "
              << result[2] << std::endl;
    std::cout << std::setprecision(17) << expected[0] << " " << expected[1]
              << " " << expected[2] << std::endl;
    CHECK_EQ(result, expected);
}

TEST_CASE("RDoubleReproducibilityTest.Mandelbrot") {
    using Array2 = typename decltype(test)::Array2;
    Array2 coordinate = {0.001643721971153, 0.822467633298876};
    Array2 expected = {-0.24380113611053808, 0.55291831058262619};

    auto result = test.mandelbrot(coordinate, steps);
    CHECK_EQ(result, expected);
}

TEST_CASE("RDoubleReproducibilityTest.LogisticMap") {
    rdouble r = 3.99, x = 0.7;
    rdouble expected = 0.92639622053510662;

    auto result = test.logistic_map(r, x, steps);
    CHECK_EQ(result, expected);
}

} // namespace rdouble_tests