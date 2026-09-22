// Write a C++ function that takes a vector of integers and an integer `k`, and returns the number of index pairs `(i, j)` with `0 <= i < j < nums.size()` such that `nums[i] == nums[j]` and the product `i * j` is divisible by `k`. The function must handle vectors with duplicate values, `k` equal to 1, `k` equal to 0 (then no pair qualifies because `i*j` will never be divisible by 0, since division by zero is undefined), empty vectors, and vectors of size 1. The function should be efficient and avoid unnecessary recomputation.
// The problem is a straightforward double-loop over all index pairs. For each pair `(i, j)` with `i < j`, we check two conditions: (1) the values at those indices are equal, and (2) `(i * j) % k == 0`. When `k == 0`, the modulo operation is undefined (division by zero), so we must return 0 immediately as no pair can satisfy the condition. For `k > 0`, the modulo check is valid. Edge cases: empty vector or vector of size 1 yields 0 pairs. Duplicate values are counted multiple times because each distinct index pair is considered separately. The algorithm is O(n^2) time and O(1) auxiliary space. We also note that `i * j` can overflow `int` for large indices, so we cast to `long long` before multiplication to avoid undefined behavior.
#include <vector>

// Count pairs (i, j) with i < j such that nums[i] == nums[j] and (i*j) % k == 0.
int countEqualIndexDivisiblePairs(const std::vector<int>& nums, int k) {
    // If k is zero, modulo is undefined; no pair can satisfy the condition.
    if (k == 0) return 0;

    const int n = static_cast<int>(nums.size());
    int result = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            // Use long long to avoid overflow of i*j for large n.
            if (nums[i] == nums[j] && (static_cast<long long>(i) * j) % k == 0) {
                ++result;
            }
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared above (or included via header).

int main() {
    // Basic test with k=2, equal values at indices where product is even.
    std::vector<int> v1 = {3, 1, 3};
    assert(countEqualIndexDivisiblePairs(v1, 2) == 1); // (0,2) product=0 divisible

    // k=1: any product divisible by 1, so count all equal pairs.
    std::vector<int> v2 = {1, 2, 1, 1};
    // Equal pairs: (0,2), (0,3), (2,3) => total 3
    assert(countEqualIndexDivisiblePairs(v2, 1) == 3);

    // k=0: no valid pairs.
    std::vector<int> v3 = {5, 5};
    assert(countEqualIndexDivisiblePairs(v3, 0) == 0);

    // Empty vector.
    std::vector<int> v4;
    assert(countEqualIndexDivisiblePairs(v4, 5) == 0);

    // Single element.
    std::vector<int> v5 = {7};
    assert(countEqualIndexDivisiblePairs(v5, 2) == 0);

    // No equal values.
    std::vector<int> v6 = {1, 2, 3};
    assert(countEqualIndexDivisiblePairs(v6, 3) == 0);

    // Large indices to verify no overflow: vector size 1000, k=1, all equal.
    std::vector<int> v7(1000, 0);
    int expected = 1000 * 999 / 2; // all pairs (i<j) count
    assert(countEqualIndexDivisiblePairs(v7, 1) == expected);

    // k=3, product divisible, but values differ.
    std::vector<int> v8 = {2, 2, 3, 2};
    // Equal pairs: (0,1), (0,3), (1,3)
    // Products: (0*1)=0%3==0 -> count, (0*3)=0%3==0 -> count, (1*3)=3%3==0 -> count => 3
    assert(countEqualIndexDivisiblePairs(v8, 3) == 3);
}
