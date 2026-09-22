Write a C++ function named `maximumOrderedTripletValue` that takes a non-empty vector of 32-bit integers (`std::vector<int>`) and returns the maximum value of `(nums[i] - nums[j]) * nums[k]` for all valid triplets of indices `(i, j, k)` satisfying `0 <= i < j < k < n`. If the computed value could be negative, the function must return 0 (since the problem states that the maximum value is at least 0 given the default initialization). The input vector may contain negative numbers, positive numbers, and duplicates. The result must be returned as a 64-bit integer (`long long`) to avoid overflow during multiplication. The function must be `const`-correct (i.e., the input vector is passed by `const` reference) and should work for any vector size `n >= 3` (though your implementation should handle smaller sizes gracefully by returning 0, but the test cases will ensure `n >= 3`). Do not modify the input vector. Provide a clear, self-contained implementation with appropriate headers and comments.

The problem is a straightforward enumeration over all possible triplets of indices, ensuring that the index order `i < j < k` is strictly respected. The naive approach uses three nested loops: the outer loop selects `i` from `0` to `n-3`, the middle loop selects `j` from `i+1` to `n-2`, and the inner loop selects `k` from `j+1` to `n-1`. For each triplet, compute `(nums[i] - nums[j])` as a 64-bit integer (cast to avoid potential int overflow), then multiply by `nums[k]` (also cast to 64-bit) and update the maximum. Since we initialize the maximum to 0, any negative result will be ignored, matching the problem's requirement that if the maximum is negative, we return 0. No special handling of duplicates is needed because they are valid triplets. The time complexity is `O(n^3)` due to the three nested loops, and space complexity is `O(1)` auxiliary space (excluding the input vector). For `n` up to say 100, this is acceptable; for larger inputs a more optimal `O(n)` or `O(n^2)` solution exists, but this task focuses on correctness with a simple approach. Edge cases include: all negative values (e.g., `[-1, -2, -3]` gives `(-1 - -2)*(-3) = (1)*(-3) = -3`, so max is 0), large values near `2^31` requiring 64-bit arithmetic, and vectors with duplicate values.

#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the maximum value of (nums[i] - nums[j]) * nums[k] for i < j < k.
// If the result would be negative, returns 0.
long long maximumOrderedTripletValue(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    long long best = 0; // Since we want max(0, any positive value)
    
    for (int i = 0; i < n - 2; ++i) {
        for (int j = i + 1; j < n - 1; ++j) {
            // Compute difference as 64-bit to avoid overflow before multiplication.
            long long diff = static_cast<long long>(nums[i]) - nums[j];
            for (int k = j + 1; k < n; ++k) {
                long long value = diff * static_cast<long long>(nums[k]);
                if (value > best) {
                    best = value;
                }
            }
        }
    }
    return best;
}

#include <cassert>
#include <vector>

// The solution function is declared above; here we test it.

int main() {
    // Basic case with positive numbers
    std::vector<int> v1 = {12, 6, 1, 2, 7};
    assert(maximumOrderedTripletValue(v1) == 77); // (12 - 1) * 7 = 77

    // All negative numbers -> maximum value is 0
    std::vector<int> v2 = {-1, -2, -3, -4};
    assert(maximumOrderedTripletValue(v2) == 0);

    // Example where best is 0 due to negative product
    std::vector<int> v3 = {1, 2, 3};
    assert(maximumOrderedTripletValue(v3) == 0); // (1-2)*3 = -3

    // Duplicate values allowed
    std::vector<int> v4 = {5, 5, 5, 5};
    assert(maximumOrderedTripletValue(v4) == 0); // (5-5)*5 = 0

    // Large values to test 64-bit arithmetic
    std::vector<int> v5 = {1000000000, 1, 999999999};
    assert(maximumOrderedTripletValue(v5) == 999999999000000000LL); // (1e9 - 1) * 999999999

    // Mixed positive and negative: (i=0, j=1, k=2) gives (10 - (-5)) * 2 = 30
    std::vector<int> v6 = {10, -5, 2, 3};
    assert(maximumOrderedTripletValue(v6) == 30);

    // Minimal size n=3
    std::vector<int> v7 = {3, 1, 10};
    assert(maximumOrderedTripletValue(v7) == 20); // (3-1)*10 = 20

    // Ascending order forces negative differences but k positive -> negative product
    std::vector<int> v8 = {1, 2, 3, 4};
    assert(maximumOrderedTripletValue(v8) == 0); // all (negative)*positive = negative

    // Descending order gives positive differences
    std::vector<int> v9 = {4, 3, 2, 1};
    assert(maximumOrderedTripletValue(v9) == 2); // (4-3)*2 = 2, (3-2)*1 = 1, etc.

    // More complex: best at i=0, j=1, k=3 => (8-2)*5 = 30
    std::vector<int> v10 = {8, 2, 1, 5};
    assert(maximumOrderedTripletValue(v10) == 30);

    return 0;
}
