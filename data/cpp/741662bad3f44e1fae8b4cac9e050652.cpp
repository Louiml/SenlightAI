Write a C++ function `long long weightedSubarraySum(const std::vector<int>& arr)` that, given a non-empty array of integers, computes the sum of all suffix sums for every starting index. Specifically, for each starting index `i` from 0 to n-1, take the sum of elements from `i` to the end, then add those suffix sums together across all `i`. The result may be large, so return it as a `long long`. For example, for `arr = [1, 2, 3]`, the suffix sums are `1+2+3 = 6`, `2+3 = 5`, `3 = 3`, and the total is `6+5+3 = 14`. The input array may contain negative numbers, zeros, and duplicate values, and it may be very large. The function must not modify the input array.
The naive double-loop from the snippet sums each element as many times as the number of suffix sums it appears in. For an element at index `i`, it appears in suffix sums starting at indices `0` through `i` (inclusive), so it is counted exactly `i+1` times. Therefore, the total result is `sum_{i=0}^{n-1} arr[i] * (i+1)`. This avoids the O(n²) nested loop. Edge cases: a single element returns that element (since it appears once), negative numbers are handled naturally by multiplication, and large arrays may overflow a 32-bit integer, so a `long long` accumulator and multiplication with a type cast are necessary. Complexity is O(n) time and O(1) auxiliary space (excluding the input vector storage).
#include <vector>

// Compute sum over all suffix sums of the input array.
// Each arr[i] contributes arr[i] * (i + 1) because it appears in suffix sums starting at indices 0..i.
long long weightedSubarraySum(const std::vector<int>& arr) {
    long long total = 0;
    const long long n = static_cast<long long>(arr.size());
    for (long long i = 0; i < n; ++i) {
        total += static_cast<long long>(arr[i]) * (i + 1);
    }
    return total;
}
#include <cassert>
#include <vector>

int main() {
    // Single element
    assert(weightedSubarraySum({5}) == 5);
    // Positive example from description
    assert(weightedSubarraySum({1, 2, 3}) == 14);
    // Negative and zero values
    assert(weightedSubarraySum({-1, 0, 2}) == -1*1 + 0*2 + 2*3 == 5);
    // All negative
    assert(weightedSubarraySum({-2, -3}) == (-2*1) + (-3*2) == -8);
    // Duplicate values
    assert(weightedSubarraySum({4, 4}) == 4*1 + 4*2 == 12);
    // Large values to check long long overflow
    assert(weightedSubarraySum({1000000, 1000000, 1000000}) == 1000000LL * 1 + 1000000LL * 2 + 1000000LL * 3);
    // Larger array with mixed signs
    assert(weightedSubarraySum({1, -2, 3, -4}) == (1*1) + (-2*2) + (3*3) + (-4*4) == 1 - 4 + 9 - 16 == -10);
    // Empty array? Not allowed by spec, but function doesn't handle empty; test a known valid case.
    // Additional sanity with 5 elements
    assert(weightedSubarraySum({1, 1, 1, 1, 1}) == 1 + 2 + 3 + 4 + 5 == 15);
    return 0;
}
