#include "gtest/gtest.h"
#include <rfloat>

#include <algorithm>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

class AlgorithmTest : public ::testing::Test {
  protected:
    std::vector<rfloat> numbers;
    const std::size_t size = 10;

    void SetUp() override {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1.0f, 100.0f);

        numbers.resize(size);
        std::generate(numbers.begin(), numbers.end(),
                      [&]() { return dis(gen); });
    }
};

TEST_F(AlgorithmTest, SortingWorks) {
    std::sort(numbers.begin(), numbers.end());
    EXPECT_TRUE(std::is_sorted(numbers.begin(), numbers.end()));
}

TEST_F(AlgorithmTest, AccumulateWorks) {
    rfloat sum = std::accumulate(numbers.begin(), numbers.end(), rfloat(0.0f));
    rfloat expected_sum = 0;
    for (const auto num : numbers) {
        expected_sum += num;
    }
    EXPECT_EQ(sum, expected_sum);
}

TEST_F(AlgorithmTest, FillNWorks) {
    std::vector<rfloat> filled(size);
    std::fill_n(filled.begin(), size, 42.0f);
    for (const auto num : filled) {
        EXPECT_EQ(num, 42.0f);
    }
}

TEST_F(AlgorithmTest, TransformWorks) {
    std::vector<rfloat> squared(size);
    std::transform(numbers.begin(), numbers.end(), squared.begin(),
                   [](rfloat x) { return x * x; });
    for (std::size_t i = 0; i < size; ++i) {
        EXPECT_EQ(squared[i], numbers[i] * numbers[i]);
    }
}

TEST_F(AlgorithmTest, MaxElementWorks) {
    auto max_it = std::max_element(numbers.begin(), numbers.end());
    EXPECT_NE(max_it, numbers.end());
    for (const auto num : numbers) {
        EXPECT_LE(num, *max_it);
    }
}

TEST_F(AlgorithmTest, CountIfWorks) {
    int threshold = 50;
    int count = std::count_if(numbers.begin(), numbers.end(),
                              [threshold](rfloat x) { return x > threshold; });
    int manual_count = 0;
    for (const auto num : numbers) {
        if (num > threshold) {
            manual_count++;
        }
    }
    EXPECT_EQ(count, manual_count);
}

#if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 202207L &&                \
    defined(ENABLE_RANGES)

// Ranges are currently unsupported,
TEST(RangesTransformTest, FloatRoundTransform) {
    std::vector<rfloat> input = {1.2f, 2.7f, 3.5f, 4.8f, 5.1f};
    auto rounded =
        input | std::views::transform([](rfloat x) { return std::round(x); });

    std::vector<rfloat> result(rounded.begin(), rounded.end());

    std::vector<rfloat> expected = {1.0f, 3.0f, 4.0f, 5.0f, 5.0f};

    ASSERT_EQ(result.size(), expected.size());

    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_FLOAT_EQ(result[i].underlying_value(),
                        expected[i].underlying_value());
    }
}
#endif /* ENABLE_RANGES */
