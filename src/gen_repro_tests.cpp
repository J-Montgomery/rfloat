#include <string>
#include <sstream>
#include <fstream>
#include <functional>
#include <array>
#include <iostream>
#include <vector>
#include <cmath>

#include <iomanip>

template<typename T, size_t InputSize, size_t OutputSize>
class TestDataGenerator {
public:
    using InputType = std::array<T, InputSize>;
    using OutputType = std::array<T, OutputSize>;
    using FunctionType = std::function<OutputType(const InputType&)>;

    TestDataGenerator(FunctionType func, const std::string& name)
        : m_func(func), m_name(name) {}

    std::string generate(const std::vector<InputType>& inputs) {
        std::ostringstream oss;
        oss << "#pragma once\n#include <string>\n\n";
        oss << "std::string " << m_name << "_testdata = R\"(\n";

        for (const auto& input : inputs) {
            for (const auto& val : input) {
                oss << std::setprecision(17) << val << " ";
            }

            OutputType output = m_func(input);
            for (const auto& val : output) {
                oss << std::setprecision(17) << val << " ";
            }
            oss << "\n";
        }

        oss << ")\";\n";
        return oss.str();
    }

private:
    FunctionType m_func;
    std::string m_name;
};

template<typename T, size_t InputSize, size_t OutputSize>
void generate_test_data(
    const std::string& funcname,
    std::function<std::array<T, OutputSize>(const std::array<T, InputSize>&)> func,
    const std::vector<std::array<T, InputSize>>& inputs
) {
    TestDataGenerator<T, InputSize, OutputSize> generator(func, funcname);
    std::string testData = generator.generate(inputs);

    std::string filename = funcname + "_testdata.hh";
    std::ofstream outfile(filename);
    if (outfile) {
        outfile << testData;
        outfile.close();
        std::cout << "Test data for " << funcname << " saved to " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file " << filename << " for writing" << std::endl;
    }
}

std::array<double, 1> double_num(const std::array<double, 1>& input) {
    return {input[0] * 2};
}

std::array<double, 3> complex_func(const std::array<double, 2>& input) {
    return {input[0] / input[1], input[0] - input[1], input[0] * input[1]};
}

int main() {
    std::vector<std::array<double, 1>> double_num_inputs = {{1}, {2}, {3}, {4}};
    generate_test_data<double, 1, 1>("double_num", double_num, double_num_inputs);

    std::vector<std::array<double, 2>> complex_func_inputs = {{1, 2}, {3, 4}, {5, 6}};
    generate_test_data<double, 2, 3>("complex_func", complex_func, complex_func_inputs);

    return 0;
}
