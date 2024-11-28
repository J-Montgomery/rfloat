#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <rfloat>

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

#define CHECK_FLOAT_EQ(a, b) CHECK_LT(std::abs(a - b), 1e-6f)
#if __cpp_lib_ranges >= 202110L && defined(ENABLE_RANGES)
#include <ranges>

TEST_CASE("RangesTransformTest.FloatRoundTransform") {
    std::vector<rfloat> input = {1.2f, 2.7f, 3.5f, 4.8f, 5.1f};
    auto rounded =
        input | std::views::transform([](rfloat x) { return rstd::round(x); });

    std::vector<rfloat> result(rounded.begin(), rounded.end());

    std::vector<rfloat> expected = {1.0f, 3.0f, 4.0f, 5.0f, 5.0f};

    REQUIRE_EQ(result.size(), expected.size());

    for (size_t i = 0; i < result.size(); ++i) {
        CHECK_FLOAT_EQ(result[i].underlying_value(),
                       expected[i].underlying_value());
    }
}
#endif /* ENABLE_RANGES */

std::vector<rfloat> numbers;
const std::size_t size = 10;

void SetUp() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1.0f, 100.0f);

    numbers.resize(size);
    std::generate(numbers.begin(), numbers.end(), [&]() { return dis(gen); });
}

TEST_CASE("AlgorithmTest.SortingWorks") {
    SetUp();

    std::sort(numbers.begin(), numbers.end());
    CHECK(std::is_sorted(numbers.begin(), numbers.end()));
}

TEST_CASE("AlgorithmTest.AccumulateWorks") {
    SetUp();
    rfloat sum = std::accumulate(numbers.begin(), numbers.end(), rfloat(0.0f));
    rfloat expected_sum = 0;
    for (const auto num : numbers) {
        expected_sum += num;
    }
    CHECK_EQ(sum, expected_sum);
}

TEST_CASE("AlgorithmTest.FillNWorks") {
    SetUp();
    std::vector<rfloat> filled(size);
    std::fill_n(filled.begin(), size, 42.0f);
    for (const auto num : filled) {
        CHECK_EQ(num, 42.0f);
    }
}

TEST_CASE("AlgorithmTest.TransformWorks") {
    SetUp();
    std::vector<rfloat> squared(size);
    std::transform(numbers.begin(), numbers.end(), squared.begin(),
                   [](rfloat x) { return x * x; });
    for (std::size_t i = 0; i < size; ++i) {
        CHECK_EQ(squared[i], numbers[i] * numbers[i]);
    }
}

TEST_CASE("AlgorithmTest.MaxElementWorks") {
    SetUp();
    auto max_it = std::max_element(numbers.begin(), numbers.end());
    CHECK_NE(max_it, numbers.end());
    for (const auto num : numbers) {
        CHECK_LE(num, *max_it);
    }
}

TEST_CASE("AlgorithmTest.CountIfWorks") {
    SetUp();
    long threshold = 50;
    long count =
        std::count_if(numbers.begin(), numbers.end(), [threshold](rfloat x) {
            return rstd::lround(x) > threshold;
        });
    long manual_count = 0;
    for (const auto num : numbers) {
        if (rstd::lround(num) > threshold) {
            manual_count++;
        }
    }
    CHECK_EQ(count, manual_count);
}
