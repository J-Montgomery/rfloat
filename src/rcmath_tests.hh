#pragma once

#include <array>
#include <vector>

template <typename T>
struct TestFunctions {
using Array2 = std::array<T, 2>;
using Array3 = std::array<T, 3>;
using Matrix = std::array<Array3, 3>;

T iir_filter(const std::vector<T>& ffw_coeff, const std::vector<T>& fb_coeff,
             std::vector<T>& input_hist, std::vector<T>& output_hist) const {
    // Constraint: fb_coeff[0] should be 1.0
    // Constraint: input_hist.size() == ffw_coeff.size(), output_hist.size() == fb_coeff.size() - 1

    T output = 0;

    for (size_t i = 0; i < ffw_coeff.size(); ++i) {
        output += ffw_coeff[i] * input_hist[i];
    }

    for (size_t i = 1; i < fb_coeff.size(); ++i) {
        output -= fb_coeff[i] * output_hist[i - 1];
    }

    output_hist.insert(output_hist.begin(), output);

    return output;
}

T fir_filter(const std::vector<T>& coeff, std::vector<T>& input_hist, std::vector<T>& output_hist) const {
    T output = 0;
    for (size_t i = 0; i < coeff.size(); ++i) {
        output += coeff[i] * input_hist[i];
    }
    output_hist.insert(output_hist.begin(), output);
    return output;
}

Matrix matrix_multiply(const Matrix& A,
                                                const Matrix& B) const {
    Matrix C{};

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

T naive_sum(const std::vector<T>& values) {
    T sum = 0;
    for (const auto& value : values) {
        sum += value;
    }
    return sum;
}

static Array3 lorenz(Array3 initial_state, std::size_t steps,
                     T sigma = 10.0, T rho = 28.0, T beta = 2.667, T dt = 0.01) {
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

    return Array3{x, y, z};
}

Array2 mandelbrot(const Array2& c, std::size_t steps) {
    T x = T{0};
    T y = T{0};
    for (std::size_t n = 0; n < steps; ++n) {
        if (x*x + y*y > T{4}) {
            return Array2{x, y};
        }
        T x_temp = x*x - y*y + c[0];
        y = T{2}*x*y + c[1];
        x = x_temp;
    }
    return Array2{x, y};
}

T logistic_map(T r, T x, std::size_t steps) {
    for (std::size_t i = 0; i < steps; ++i) {
        x = r * x * (T{1} - x);
    }
    return x;
}

};
