// Write a C++ function `sumOfLeadingIntegers` that takes a positive integer `n` and a vector of integers `values` (where `values.size() >= n`) and returns the sum of the first `n` integers in the vector as an `int`. The function must handle cases where `n` is zero (return 0) and where the vector contains negative numbers or large values that may cause overflow; in that case, the sum is computed using `long long` internally but the function still returns `int` (assume the result fits in an `int`). The function must not modify the input vector, and it should be const-correct.
The solution is straightforward: iterate through the vector for `i` from 0 to `n-1`, accumulating each element into a sum. Use a `long long` accumulator to avoid intermediate overflow when adding many large integers. The function signature takes `const std::vector<int>&` to prevent modification and improve efficiency. Edge cases: if `n` is 0, the loop runs zero times and returns 0. If `n` is greater than `values.size()`, the behavior is undefined per the task specification (we assume the caller ensures `values.size() >= n`). The time complexity is O(n) because we visit exactly `n` elements; space complexity is O(1) beyond the input vector. The function returns an `int` per the requirement, although the internal computation uses `long long` to avoid overflow during addition; it assumes the final result fits in an `int`.
#include <vector>
#include <cstddef> // for size_t

// Sum the first n integers of the given vector.
// Precondition: values.size() >= n.
// Returns 0 if n == 0.
int sumOfLeadingIntegers(int n, const std::vector<int>& values) {
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        total += values[static_cast<size_t>(i)];
    }
    // The task guarantees the result fits in an int.
    return static_cast<int>(total);
}
#include <cassert>
#include <vector>

int main() {
    // Basic sum
    assert(sumOfLeadingIntegers(3, {1, 2, 3, 4}) == 6);
    // Sum all elements when n equals size
    assert(sumOfLeadingIntegers(4, {10, -20, 30, -40}) == -20);
    // n = 0 returns 0
    assert(sumOfLeadingIntegers(0, {5, 7}) == 0);
    // Single element
    assert(sumOfLeadingIntegers(1, {42}) == 42);
    // Negative values
    assert(sumOfLeadingIntegers(2, {-5, -7, 10}) == -12);
    // Large numbers that would overflow int during intermediate addition
    assert(sumOfLeadingIntegers(3, {2000000000, 2000000000, -1000000000}) == 3000000000);
    // All zeros
    assert(sumOfLeadingIntegers(5, {0, 0, 0, 0, 0}) == 0);
    // Mixed large and small
    assert(sumOfLeadingIntegers(4, {100, 200, 300, -600}) == 0);
    return 0;
}
