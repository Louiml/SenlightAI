// Write a C++ function named `computeQuarterlySalesStats` that takes a `std::vector<double>` of quarterly sales figures (which may have fewer than 4 elements), fills the missing quarters with 0.0 to always produce exactly 4 quarters, and returns a `SalesStats` struct containing the average, maximum, and minimum of those 4 values. The function must handle empty input by treating all quarters as 0.0, and must handle negative values correctly. Do not use classes or global constants; just define the struct and function. The function should be `const`-correct for its inputs and should not perform any I/O.
#include <cassert>
#include <cmath>

int main() {
    // Normal case with exactly 4 values.
    SalesStats s1 = computeQuarterlySalesStats({100.0, 200.0, 300.0, 400.0});
    assert(s1.average == 250.0);
    assert(s1.max == 400.0);
    assert(s1.min == 100.0);

    // Fewer than 4 values pads with zeros.
    SalesStats s2 = computeQuarterlySalesStats({10.0, -20.0});
    // quarters: 10, -20, 0, 0 -> sum = -10, avg = -2.5
    assert(std::fabs(s2.average - (-2.5)) < 1e-9);
    assert(s2.max == 10.0);
    assert(s2.min == -20.0);

    // Empty input -> all zeros.
    SalesStats s3 = computeQuarterlySalesStats({});
    assert(s3.average == 0.0);
    assert(s3.max == 0.0);
    assert(s3.min == 0.0);

    // More than 4 values truncates to first 4.
    SalesStats s4 = computeQuarterlySalesStats({5.0, 1.0, 9.0, 2.0, 100.0, -50.0});
    // quarters: 5, 1, 9, 2 -> sum=17, avg=4.25
    assert(std::fabs(s4.average - 4.25) < 1e-9);
    assert(s4.max == 9.0);
    assert(s4.min == 1.0);

    // All negative values.
    SalesStats s5 = computeQuarterlySalesStats({-3.0, -7.0, -1.0, -9.0});
    assert(s5.average == -5.0);
    assert(s5.max == -1.0);
    assert(s5.min == -9.0);
}
#include <vector>
#include <algorithm>

// Structure to hold computed quarterly statistics.
struct SalesStats {
    double average;
    double max;
    double min;
};

// Compute average, max, min for exactly 4 quarters.
// If input has fewer than 4 elements, missing quarters are treated as 0.0.
// If input has more than 4 elements, only the first 4 are considered.
SalesStats computeQuarterlySalesStats(const std::vector<double>& sales) {
    const int QUARTERS = 4;
    double arr[QUARTERS] = {0.0, 0.0, 0.0, 0.0};

    // Copy up to 4 values, ignoring any extras.
    size_t count = std::min(sales.size(), static_cast<size_t>(QUARTERS));
    for (size_t i = 0; i < count; ++i) {
        arr[i] = sales[i];
    }

    // Compute sum, min, max in one pass.
    double sum = 0.0;
    double minVal = arr[0];
    double maxVal = arr[0];
    for (int i = 0; i < QUARTERS; ++i) {
        sum += arr[i];
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
    }

    SalesStats result;
    result.average = sum / QUARTERS;
    result.max = maxVal;
    result.min = minVal;
    return result;
}
// The solution needs to expand the input vector to exactly 4 elements. If the input has 4 or more elements, only the first 4 are used; if fewer, the remaining slots are filled with 0.0. After constructing the 4-element array, compute the sum, min, and max in a single pass. Initialize min and max with the first element (which is always defined because we always have exactly 4 values after padding). Edge cases: empty input → all zeros, so min=max=0.0, average=0.0; input with negatives works naturally; input with more than 4 elements ignores extras (truncation). Time complexity is O(1) because we only process at most 4 elements (even if the input vector is larger, we only look at the first 4). Space complexity is O(1) for the fixed local array, plus O(n) for the input vector passed by const reference (not copied).
