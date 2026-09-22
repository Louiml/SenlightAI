// Write a C++ function `int findDuplicateNumber(const std::vector<int>& nums)` that accepts a non-empty vector of integers where exactly one integer appears more than once, and all other integers appear at most once. The vector contains `n` elements, each integer in the range `[1, n-1]`. Your function must return the integer that appears more than once. The input vector must not be modified. Your solution must work correctly for any such vector, including cases where the duplicate appears many times and where the duplicate is the smallest or largest value in the range.
// The key observation is that we are not allowed to modify the input vector, and we need an efficient approach. The snippet's approach subtracts `n` from each element, sorts the modified values, and then finds a pair of equal modified values. Since each original integer is in `[1, n-1]`, subtracting `n` yields values in `[1-n, -1]`, which are all negative and distinct except for duplicates. Sorting these negative values groups equal elements together. When two consecutive equal values are found, adding back `n` recovers the original duplicate.
//
// However, the snippet’s use of a variable-length array (`int diff[n]`) is not standard C++ and may fail on some compilers; we should use `std::vector<int>` instead. Also, there is a subtle edge case: the problem guarantees a duplicate exists, so the loop will find one, but if no duplicate existed (which won't happen per the specification), the function should still return a sensible value (e.g., `0`). 
//
// Time complexity: Sorting takes `O(n log n)`, and scanning takes `O(n)`, so overall `O(n log n)`. Space complexity: `O(n)` for the auxiliary vector. An alternative hash-set approach would be `O(n)` time and `O(n)` space, but the sorting approach is straightforward and uses less auxiliary memory than a hash set in practice.
//
// Edge cases: The duplicate could appear exactly twice or many times; sorting still groups them. The vector size `n` is at least 2, and the range `[1, n-1]` ensures that subtracting `n` produces negative values, so the sorted order is well-defined. No modulo or overflow issues are present because values fit in `int`.
#include <vector>
#include <algorithm>

// Given a vector where exactly one integer appears more than once and all others
// appear at most once, and each integer is in [1, n-1] with n = nums.size(),
// return the duplicate integer.
int findDuplicateNumber(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    std::vector<int> diff(n);
    
    // Transform each value by subtracting n to make all values negative.
    // After sorting, equal transformed values will be adjacent.
    for (int i = 0; i < n; ++i) {
        diff[i] = nums[i] - n;
    }
    
    std::sort(diff.begin(), diff.end());
    
    // Scan for two equal consecutive transformed values.
    for (int i = 1; i < n; ++i) {
        if (diff[i] == diff[i - 1]) {
            return diff[i] + n; // Recover the original duplicate value.
        }
    }
    
    // The problem guarantees a duplicate exists, but return 0 as a fallback.
    return 0;
}
#include <cassert>
#include <vector>

int main() {
    // Simple case: duplicate is 2
    std::vector<int> v1 = {1, 3, 4, 2, 2};
    assert(findDuplicateNumber(v1) == 2);
    
    // Duplicate is the largest value in range
    std::vector<int> v2 = {3, 1, 3, 4, 2};
    assert(findDuplicateNumber(v2) == 3);
    
    // Duplicate is the smallest value in range
    std::vector<int> v3 = {1, 2, 1, 3};
    assert(findDuplicateNumber(v3) == 1);
    
    // Duplicate appears many times
    std::vector<int> v4 = {5, 1, 5, 2, 5, 3, 4};
    assert(findDuplicateNumber(v4) == 5);
    
    // Only two elements, duplicate is 1
    std::vector<int> v5 = {1, 1};
    assert(findDuplicateNumber(v5) == 1);
    
    // Larger case, duplicate is 7
    std::vector<int> v6 = {7, 9, 7, 4, 2, 8, 3, 5, 6, 1};
    assert(findDuplicateNumber(v6) == 7);
    
    // Duplicate appears exactly twice
    std::vector<int> v7 = {2, 3, 4, 2, 1};
    assert(findDuplicateNumber(v7) == 2);
    
    // Destructive check: the input vector should not be modified
    std::vector<int> v8 = {4, 3, 1, 1, 2};
    std::vector<int> original = v8;
    findDuplicateNumber(v8);
    assert(v8 == original);
}
