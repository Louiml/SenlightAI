/*
Write a C++ function named `calculateAggregateStats` that takes a vector of 32-bit signed integers and returns a `std::pair<long long, double>` where the first element is the sum of all elements and the second element is the arithmetic mean (average) of the elements as a double. The function must handle empty input by returning `{0, 0.0}`. The function must be `const`-correct, meaning it should accept the vector by `const std::vector<int>&` and not modify it. The mean must be computed using floating-point division to preserve fractional values. Edge cases include very large sums that may overflow a 32-bit integer (hence the use of `long long` for the sum) and negative numbers which affect both sum and mean normally.
*/

#include <vector>
#include <utility>

// Calculate the sum and mean of a vector of integers.
// Returns {sum, mean} where sum is long long and mean is double.
// For an empty vector, returns {0, 0.0}.
std::pair<long long, double> calculateAggregateStats(const std::vector<int>& values) {
    if (values.empty()) {
        return {0LL, 0.0};
    }

    long long sum = 0LL;
    double mean = 0.0;

    for (int value : values) {
        sum += value;
    }

    mean = static_cast<double>(sum) / values.size();
    return {sum, mean};
}

#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// The solution function is declared above; here we test it.
std::pair<long long, double> calculateAggregateStats(const std::vector<int>& values);

int main() {
    // Basic case
    auto result1 = calculateAggregateStats({1, 2, 3, 4});
    assert(result1.first == 10LL);
    assert(std::abs(result1.second - 2.5) < 1e-9);

    // Negative numbers
    auto result2 = calculateAggregateStats({-5, -1, -10});
    assert(result2.first == -16LL);
    assert(std::abs(result2.second - (-16.0 / 3.0)) < 1e-9);

    // Single element
    auto result3 = calculateAggregateStats({7});
    assert(result3.first == 7LL);
    assert(std::abs(result3.second - 7.0) < 1e-9);

    // Empty vector
    auto result4 = calculateAggregateStats({});
    assert(result4.first == 0LL);
    assert(result4.second == 0.0);

    // Large sum that overflows int
    std::vector<int> big(1000000, 100000);
    auto result5 = calculateAggregateStats(big);
    assert(result5.first == 100000000000LL);
    assert(std::abs(result5.second - 100000.0) < 1e-9);

    // Mixed signs and zeros
    auto result6 = calculateAggregateStats({0, 0, 0});
    assert(result6.first == 0LL);
    assert(result6.second == 0.0);

    auto result7 = calculateAggregateStats({-2, 0, 2});
    assert(result7.first == 0LL);
    assert(result7.second == 0.0);
}

// The solution is straightforward: iterate through each element in the vector, accumulate the sum into a `long long` variable to prevent integer overflow that would occur with `int` when summing many large values. Track the count of elements. If the vector is empty, return `{0, 0.0}` early. Otherwise, after the loop, compute the mean as `static_cast<double>(sum) / count`. The division must be performed in floating-point, so we cast the numerator to `double` (or the denominator to `double`) before dividing. This ensures fractional results are preserved. Edge cases: empty vector (returns zeros), single element (mean equals that element), negative numbers (handled naturally in summation and division), and large sums that exceed `int` range (handled by `long long` accumulator). Time complexity: O(n) where n is the number of elements, since we traverse the vector once. Space complexity: O(1) auxiliary, aside from the input vector itself and the returned pair.
