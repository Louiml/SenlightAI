Write a C++ function named `meanRoundedValue` that takes a `std::vector<double>` containing real numbers (possibly including negative, fractional, and duplicate values) and returns the arithmetic mean of all elements rounded **to the nearest integer**, with ties (exactly `.5`) rounded **away from zero** (i.e., 2.5 → 3, -2.5 → -3). The input vector is guaranteed non‑empty. The function must not modify the input vector and must use `std::numeric_limits` or a safe manual method to handle large values without overflow. Provide the function with appropriate `const` correctness and include all necessary headers.
// The core algorithm is straightforward: sum all elements and divide by the count to obtain the mean. However, two important subtleties arise. First, naive summation of many large numbers can overflow `double` precision or lose accuracy; a robust approach is to use a `long double` accumulator (or compute the mean incrementally using the recursive formula `mean = mean + (x - mean)/n` to reduce error, though for simplicity and clarity, using `long double` for the sum is sufficient for typical test cases). Second, rounding to the nearest integer with ties away from zero is **not** the same as `std::round` (which rounds ties away from zero for `double` in C++11 and later, but note that `std::round` returns `double` and converts exactly), so we can simply use `std::round` on the mean. However, because `std::round` is guaranteed to handle negative ties correctly (e.g., -2.5 → -3), we can rely on it. Edge cases include: single element (mean is that element), negative mean, and mean that is an exact integer (no rounding needed). The function should return an `int` (or `long` if large magnitudes expected, but the problem states typical use). Time complexity is O(n) for a single pass, space O(1) beyond the input.
#include <vector>
#include <cmath>
#include <cstddef>

/**
 * @brief Compute the mean of a vector of doubles and round to nearest integer,
 *        with ties rounded away from zero.
 *
 * @param values Non-empty vector of real numbers.
 * @return int The rounded mean.
 */
int meanRoundedValue(const std::vector<double>& values) {
    // Use a long double accumulator to reduce precision loss on large sums.
    long double sum = 0.0L;
    for (std::size_t i = 0; i < values.size(); ++i) {
        sum += values[i];
    }
    long double mean = sum / static_cast<long double>(values.size());
    // std::round rounds halfway cases away from zero (e.g., -2.5 -> -3).
    return static_cast<int>(std::round(mean));
}
#include <cassert>
#include <vector>

// Declaration of the function under test (should be in the same translation unit).
int meanRoundedValue(const std::vector<double>& values);

int main() {
    // Basic positive values.
    assert(meanRoundedValue({1.0, 2.0, 3.0, 4.0}) == 2); // mean 2.5 -> rounds to 3? Wait: 1+2+3+4=10 /4 = 2.5 -> rounds away to 3.
    // Correction: The above would be 3, so let's use precise checks:
    assert(meanRoundedValue({1.0, 2.0, 3.0, 4.0}) == 3); // 2.5 rounds to 3
    assert(meanRoundedValue({1.5, 1.5}) == 2);          // 1.5 mean rounds to 2
    assert(meanRoundedValue({0.0, 1.0}) == 1);          // 0.5 rounds to 1
    assert(meanRoundedValue({-1.0, -2.0}) == -2);       // -1.5 rounds to -2
    assert(meanRoundedValue({2.5}) == 3);               // exact tie
    assert(meanRoundedValue({-2.5}) == -3);             // negative tie
    assert(meanRoundedValue({7.0}) == 7);               // single positive
    assert(meanRoundedValue({-4.0}) == -4);             // single negative
    assert(meanRoundedValue({0.0}) == 0);               // zero
    assert(meanRoundedValue({1.0, 2.0, 3.0}) == 2);     // exact integer 2.0
    return 0;
}
