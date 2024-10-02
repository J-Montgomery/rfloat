#include <iomanip>
#include <iostream>

#include <array>
#include <rfloat/rfloat.hh>

/* This file exists as an easy template to diagnose issues detected by the
 * normal test cases. All the abstractions and namespaces used by GTest make
 * disassembly difficult, so this removes all of them. The Lorenz test is used
 * by default as it's the most sensitive of the test cases.
 */

template <typename T>
std::array<T, 3> lorenz(std::array<T, 3> initial_state, std::size_t steps,
                        T sigma = 10.0, T rho = 28.0, T beta = 2.667,
                        T dt = 0.01) {
    T x = initial_state[0];
    T y = initial_state[1];
    T z = initial_state[2];

    for (std::size_t i = 0; i < steps; ++i) {
        T dx = sigma * (y - x);
        T dy = x * (rho - z) - y;
        T dz = x * y - beta * z;

        x += dx * dt;
        y += dy * dt;
        z += dz * dt;
    }

    return std::array<T, 3>{x, y, z};
}

extern "C" int lorenz_test() {
    using T = rfloat;
    bool errorflag = false;
    std::array<T, 3> input = {0.0, 1.0, 0.0};
    std::array<T, 3> expected = {T{-8.4283517044458289}, T{-7.0491893984733949},
                                 T{28.649356114765581}};

    auto result = lorenz<T>(input, 1000);

    std::cout << std::setprecision(17) << result[0] << " " << result[1] << " "
              << result[2] << std::endl;

    if (result[0] != expected[0]) {
        std::cout << std::setprecision(17) << "Error: " << result[0]
                  << " != " << expected[0] << std::endl;
        errorflag |= true;
    }

    if (result[1] != expected[1]) {
        std::cout << std::setprecision(17) << "Error: " << result[1]
                  << " != " << expected[1] << std::endl;
        errorflag |= true;
    }

    if (result[2] != expected[2]) {
        std::cout << std::setprecision(17) << "Error: " << result[2]
                  << " != " << expected[2] << std::endl;
        errorflag |= true;
    }

    return 0;
}

int main() { return lorenz_test(); }
