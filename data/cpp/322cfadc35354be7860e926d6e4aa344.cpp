/*
Write a C++ function `long long countTripletsFromRuns(const std::vector<int>& values)` that, given a non-empty vector of integers, returns the total number of triplets (i, j, k) with i < j < k such that values[i] == values[j] == values[k]. In other words, for each maximal contiguous run of identical numbers, if the run has length L, it contributes L choose 3 (i.e., L*(L-1)*(L-2)/6) to the total. Your function must handle large vectors (up to 10^6 elements) and values that may be negative or large, but the total will fit in a `long long`. The input vector is not modified; pass it by `const` reference. Do not include any `main` function; only provide the function implementation with necessary headers.
*/
#include <vector>

// Given a non-empty vector of integers, return the total number of triplets (i, j, k)
// with i < j < k such that values[i] == values[j] == values[k], which equals sum over runs
// of binomial(runLength, 3).
long long countTripletsFromRuns(const std::vector<int>& values) {
    const std::size_t n = values.size();
    if (n < 3) {
        return 0;
    }

    long long total = 0;
    std::size_t runLength = 1;

    for (std::size_t i = 1; i < n; ++i) {
        if (values[i] == values[i - 1]) {
            ++runLength;
        } else {
            if (runLength >= 3) {
                total += static_cast<long long>(runLength) * (runLength - 1) * (runLength - 2) / 6;
            }
            runLength = 1;
        }
    }

    // Handle the last run.
    if (runLength >= 3) {
        total += static_cast<long long>(runLength) * (runLength - 1) * (runLength - 2) / 6;
    }

    return total;
}
#include <cassert>
#include <vector>

// The solution function is already included via the header or definition above.

int main() {
    // Basic runs
    assert(countTripletsFromRuns({1, 1, 1}) == 1);
    assert(countTripletsFromRuns({1, 1, 1, 1}) == 4);  // C(4,3)=4
    assert(countTripletsFromRuns({1, 2, 3}) == 0);     // no runs of length >=3
    assert(countTripletsFromRuns({5, 5, 5, 5, 5}) == 10); // C(5,3)=10

    // Mixed runs
    assert(countTripletsFromRuns({1, 1, 1, 2, 2, 2, 2}) == 1 + 4); // 1 from run1, 4 from run2
    assert(countTripletsFromRuns({1, 2, 2, 3, 3, 3}) == 1);

    // Single element or two elements
    assert(countTripletsFromRuns({7}) == 0);
    assert(countTripletsFromRuns({7, 7}) == 0);

    // Larger run with alternating values
    std::vector<int> v = {1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1};
    // run lengths: 4 -> C=4, 5 -> C=10, 3 -> C=1 => total 15
    assert(countTripletsFromRuns(v) == 15);

    // Empty? Not required, but ensure non-empty only. Test with negative numbers
    assert(countTripletsFromRuns({-1, -1, -1, -1}) == 4);
}
// The key is to recognize that any triplet of equal values must lie entirely within a single contiguous run of equal elements. For a run of length L, the number of ways to choose 3 distinct indices is binomial(L, 3) = L*(L-1)*(L-2)/6. Therefore, we scan the vector once, identifying consecutive runs of identical values. We maintain a running `runLength` counter; whenever we encounter a new value different from the previous, we add binomial(runLength, 3) to a total and reset the counter to 1. At the end, add the last run’s contribution. Edge cases: if the vector length is less than 3, the total is 0; if a run’s length is 1 or 2, binomial is 0, so no special handling needed. Time complexity is O(n) with a single pass, using O(1) auxiliary space (excluding the vector itself). The formula uses division by 6, which is exact for integer L because L*(L-1)*(L-2) is always divisible by 6.
