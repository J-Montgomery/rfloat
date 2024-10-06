#pragma once

#include <array>
#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include <vector>


// Infrastructure to read lines of test data from the headers
// included above and parsed into TestParam objects. Each line is a single test
// case.
template <typename T, std::size_t InputSize, std::size_t OutputSize>
struct TestParam {
    std::array<T, InputSize> inputs;
    std::array<T, OutputSize> expected_outputs;
};

template <typename T, std::size_t InputSize, std::size_t OutputSize>
class ParameterizedTest : public ::testing::Test {
  public:
    static std::vector<TestParam<T, InputSize, OutputSize>>
    LoadTestData(const std::string &data) {
        std::vector<TestParam<T, InputSize, OutputSize>> test_data;
        std::istringstream iss(data);
        std::string line;
        int line_number = 0;

        while (std::getline(iss, line)) {
            line_number++;

            if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
                continue;
            }

            std::istringstream line_stream(line);
            TestParam<T, InputSize, OutputSize> param;

            for (std::size_t i = 0; i < InputSize; ++i) {
                if (!(line_stream >> param.inputs[i])) {
                    throw std::runtime_error(
                        "Not enough valid input values on line " +
                        std::to_string(line_number));
                }
            }

            for (std::size_t i = 0; i < OutputSize; ++i) {
                if (!(line_stream >> param.expected_outputs[i])) {
                    throw std::runtime_error(
                        "Not enough valid output values on line " +
                        std::to_string(line_number));
                }
            }

            int extra;
            if (line_stream >> extra) {
                throw std::runtime_error("Too many values on line " +
                                         std::to_string(line_number));
            }

            test_data.push_back(param);
        }
        return test_data;
    }
};
