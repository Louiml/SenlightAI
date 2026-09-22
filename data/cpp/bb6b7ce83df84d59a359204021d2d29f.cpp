Given an array of `n` integers and a positive integer `k`, write a C++ function `constructArray` that returns a vector containing a valid sequence of length `n * k` such that every contiguous subarray of length `k` in that sequence contains all distinct elements (i.e., no duplicate within any window of size `k`). If no such sequence exists (because the number of distinct values in the input array exceeds `k`), return an empty vector. If the original array already has length `k`, you may return the original array as is (length `n`). Otherwise, the output sequence must repeat a fixed permutation of all distinct values from the input array, cycling through them in a consistent order. For example, if input array is `[1, 2, 3]` and `k = 2`, then `[1, 2, 1, 2, 1, 2]` is valid. The function must not rely on any global state and must work for any `n ≥ 1`, `k ≥ 1`, and array elements fitting within `int`.
The core idea is that for every length-`k` window to have all distinct elements, the sequence must be periodic with period equal to the number of distinct values in the array, but since `k` can be larger than the number of distinct values, we must repeat each distinct value over and over in a fixed cyclic order. If the number of distinct values `d` is greater than `k`, it is impossible to fit all distinct values into a window of size `k` without duplicates, so return an empty vector. If `n == k`, the original array can be returned directly because the whole array is one window. Otherwise, we collect all distinct values from the input array into a sorted set (or sorted vector) to ensure a deterministic order, then build a sequence of length `n * k` by cycling through the distinct values repeatedly, resetting the index to 0 after reaching `d`. This guarantees that within any window of size `k`, since `d ≤ k`, the maximum number of consecutive distinct values in the cyclic pattern is exactly `d`, and because `d ≤ k`, no window of length `k` can contain more than `d` distinct positions before repeating, so all windows have distinct elements. Time complexity is O(n log n + n*k) due to sorting and output generation, and space complexity is O(n + k) for storage.
#include <vector>
#include <set>
#include <algorithm>

// Construct a valid sequence of length n*k such that every length-k subarray has distinct elements.
// Returns an empty vector if impossible (distinct count > k). If n == k, returns the original array.
std::vector<int> constructArray(const std::vector<int>& arr, int k) {
    int n = static_cast<int>(arr.size());
    if (n == 0 || k <= 0) return {};

    // Collect distinct values in sorted order for deterministic output.
    std::set<int> distinctSet(arr.begin(), arr.end());
    std::vector<int> distinct(distinctSet.begin(), distinctSet.end());
    int d = static_cast<int>(distinct.size());

    if (d > k) return {}; // Impossible: window of size k cannot hold all distinct values.

    if (n == k) return arr; // Whole array is exactly one window.

    // Build output sequence by cycling through distinct values.
    std::vector<int> result;
    result.reserve(n * k);
    int idx = 0;
    for (int i = 0; i < n * k; ++i) {
        result.push_back(distinct[idx]);
        idx = (idx + 1) % d;
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Basic test: n=3, k=2, distinct count = 2 ≤ k, output length = 6.
    std::vector<int> input1 = {1, 2, 3};
    std::vector<int> out1 = constructArray(input1, 2);
    assert(out1.size() == 6);
    assert(out1 == std::vector<int>({1, 2, 1, 2, 1, 2}));

    // Impossible case: distinct count > k.
    std::vector<int> input2 = {1, 2, 3, 4};
    assert(constructArray(input2, 2).empty());

    // n == k: return original.
    std::vector<int> input3 = {5, 1, 3};
    assert(constructArray(input3, 3) == input3);

    // Single distinct value, any k.
    std::vector<int> input4 = {7, 7, 7};
    std::vector<int> out4 = constructArray(input4, 1);
    assert(out4.size() == 3);
    assert(out4 == std::vector<int>({7, 7, 7}));

    // k larger than distinct count, n small.
    std::vector<int> input5 = {2, 2, 1};
    std::vector<int> out5 = constructArray(input5, 5);
    // n*k = 15, pattern cycles {1,2}.
    assert(out5.size() == 15);
    for (int i = 0; i < 15; ++i) {
        int expected = (i % 2 == 0) ? 1 : 2;
        assert(out5[i] == expected);
    }

    // Duplicate values in input, distinct count smaller than n.
    std::vector<int> input6 = {10, 20, 10, 20, 10};
    std::vector<int> out6 = constructArray(input6, 2);
    assert(out6.size() == 10);
    assert(out6 == std::vector<int>({10, 20, 10, 20, 10, 20, 10, 20, 10, 20}));

    // Edge case: k=1, any array works because every window of length 1 has distinct.
    std::vector<int> input7 = {1, 2, 2};
    std::vector<int> out7 = constructArray(input7, 1);
    assert(out7.size() == 3);
    assert(out7 == std::vector<int>({1, 2, 1})); // cycles through distinct values in sorted order {1,2}.

    // Large k with few distinct values.
    std::vector<int> input8 = {3, 3};
    std::vector<int> out8 = constructArray(input8, 4);
    assert(out8.size() == 8);
    assert(out8 == std::vector<int>({3, 3, 3, 3, 3, 3, 3, 3}));

    // Input with negative numbers.
    std::vector<int> input9 = {-1, -3, -3, -1};
    std::vector<int> out9 = constructArray(input9, 2);
    assert(out9.size() == 8);
    assert(out9 == std::vector<int>({-3, -1, -3, -1, -3, -1, -3, -1}));

    return 0;
}
