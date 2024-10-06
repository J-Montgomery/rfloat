#pragma once

#include <array>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include <iomanip>

template <typename T, std::size_t InputSize, std::size_t OutputSize>
class TestDataGenerator {
  public:
    using InputType = std::array<T, InputSize>;
    using OutputType = std::array<T, OutputSize>;
    using FunctionType = std::function<OutputType(const InputType &)>;

    TestDataGenerator(FunctionType func, const std::string &name)
        : m_func(func), m_name(name) {}

    std::string generate(const std::vector<InputType> &inputs) {
        std::ostringstream oss;
        oss << "#pragma once\n#include <string>\n\n";
        oss << "std::string " << m_name << "_testdata = R\"(\n";

        for (const auto &input : inputs) {
            std::ostringstream tmp;
            try {
                
                for (const auto &val : input) {
                    if(std::isnan(val.fp64()) || std::isinf(val.fp64())) {
                        throw std::runtime_error("Unsupported special value");
                    }
                    tmp << std::setprecision(17) << val << " ";
                }

                OutputType output = m_func(input);
                for (const auto &val : output) {
                    if(std::isnan(val.fp64()) || std::isinf(val.fp64())) {
                        throw std::runtime_error("Unsupported special value");
                    }
                    tmp << std::setprecision(17) << val << " ";
                }

                oss << tmp.str() << "\n";
            } catch (std::runtime_error &e) {
                // Silently drop records that contain special values like
                // NaN or Inf. std::istream doesn't support them and I don't
                // want to write a custom parser.
                continue;
            }
        }

        oss << ")\";\n";
        return oss.str();
    }

  private:
    FunctionType m_func;
    std::string m_name;
};

template <typename T, std::size_t InputSize, std::size_t OutputSize>
void generate_test_data(
    const std::string &funcname,
    std::function<std::array<T, OutputSize>(const std::array<T, InputSize> &)>
        func,
    const std::vector<std::array<T, InputSize>> &inputs) {
    TestDataGenerator<T, InputSize, OutputSize> generator(func, funcname);
    std::string testData = generator.generate(inputs);

    std::string filename = funcname + "_testdata.hh";
    std::ofstream outfile(filename);
    if (outfile) {
        outfile << testData;
        outfile.close();
        std::cout << "Test data for " << funcname << " saved to " << filename
                  << std::endl;
    } else {
        std::cerr << "Unable to open file " << filename << " for writing"
                  << std::endl;
    }
}

template <typename T> std::vector<T> generate_uniform_random_list(typename T::underlying_type min, typename T::underlying_type max, std::size_t count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);

    std::vector<T> random_numbers;
    for (std::size_t i = 0; i < count; ++i) {
        random_numbers.push_back(static_cast<T>(dis(gen)));
    }
    return random_numbers;
}

template <typename T> std::vector<T> generate_normal_random_list(typename T::underlying_type mean, typename T::underlying_type stddev, std::size_t count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dis(mean, stddev);

    std::vector<T> random_numbers;
    for (std::size_t i = 0; i < count; ++i) {
        random_numbers.push_back(static_cast<T>(dis(gen)));
    }
    return random_numbers;
}

template<typename T>
std::vector<T> generate_selected_random_list(const std::vector<T>& collection, std::size_t count) {
    if (collection.size() < count) {
        throw std::invalid_argument("Not enough special values provided");
    }

    std::vector<T> result;
    result.reserve(count);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, collection.size() - 1);

    for (std::size_t i = 0; i < count; ++i) {
        std::size_t idx = dis(gen);
        result.push_back(static_cast<T>(collection[idx]));
    }

    return result;
}

template <typename T, std::size_t InputSize> 
std::vector<std::array<T, InputSize>> uniform_random_args(std::size_t count, typename T::underlying_type min = -1e9, typename T::underlying_type max = 1e9) {
    std::vector<std::array<T, InputSize>> inputs;
    inputs.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        std::array<T, InputSize> input;
        auto result = generate_uniform_random_list<T>(min, max, InputSize);
        std::copy(result.begin(), result.end(), input.begin());
        inputs.push_back(input);
    }
    return inputs;
}

template <typename T, std::size_t InputSize>
std::vector<std::array<T, InputSize>> normal_random_args(std::size_t count, typename T::underlying_type mean, typename T::underlying_type stddev) {
    std::vector<std::array<T, InputSize>> inputs;
    inputs.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        std::array<T, InputSize> input;
        auto result = generate_normal_random_list<T>(mean, stddev, InputSize);
        std::copy(result.begin(), result.end(), input.begin());
        inputs.push_back(input);
    }
    return inputs;
}

template <typename T, std::size_t InputSize> 
std::vector<std::array<T, InputSize>> selected_random_args(const std::vector<T>& collection, std::size_t count) {
    std::vector<std::array<T, InputSize>> inputs;
    inputs.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        std::array<T, InputSize> input;
        auto result = generate_selected_random_list<T>(collection, InputSize);
        std::copy(result.begin(), result.end(), input.begin());
        inputs.push_back(input);
    }
    return inputs;
}