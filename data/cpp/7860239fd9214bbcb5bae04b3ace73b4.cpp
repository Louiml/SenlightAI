/*
Write a C++ function `long long countFairPairs(std::vector<int>& nums, int lower, int upper)` that counts the number of pairs `(i, j)` with `i < j` such that `lower <= nums[i] + nums[j] <= upper`. The input vector is not necessarily sorted, may contain duplicate values, and its size `n` satisfies `1 <= n <= 10^5`. The values of `nums[i]`, `lower`, and `upper` can be negative and range in absolute value up to 10^9. The function must return the total count as a 64-bit integer, since the result may exceed the range of a 32-bit integer. You may modify the input vector (e.g., sort it). Do not include a main function.
*/
#include <vector>
#include <algorithm>

// Count pairs (i,j) with i<j and lower <= nums[i]+nums[j] <= upper.
// The input vector is sorted in-place; duplicates are handled.
long long countFairPairs(std::vector<int>& nums, int lower, int upper) {
    std::sort(nums.begin(), nums.end());
    long long count = 0;
    const int n = static_cast<int>(nums.size());
    
    for (int i = 0; i < n - 1; ++i) {
        const int target_low = lower - nums[i];
        const int target_high = upper - nums[i];
        
        // First index j > i with nums[j] >= target_low
        auto it_start = std::lower_bound(nums.begin() + i + 1, nums.end(), target_low);
        // First index j > i with nums[j] > target_high
        auto it_end = std::upper_bound(nums.begin() + i + 1, nums.end(), target_high);
        
        count += static_cast<long long>(it_end - it_start);
    }
    
    return count;
}
#include <cassert>
#include <vector>

// The solution function is declared above; this main provides tests.
// Include the function definition here or link appropriately.

int main() {
    // Basic cases
    std::vector<int> v1 = {0, 1, 2, 3};
    assert(countFairPairs(v1, 3, 3) == 2); // (0,3), (1,2)

    // Negative numbers
    std::vector<int> v2 = {-3, -1, 1, 2};
    assert(countFairPairs(v2, -4, -2) == 2); // (-3,-1): -4, (-3,1): -2? wait check: pairs: (-3,-1)=-4 ok, (-3,1)=-2 ok, (-3,2)=-1 no, (-1,1)=0 no, etc. Actually count = 2

    // Single element
    std::vector<int> v3 = {5};
    assert(countFairPairs(v3, 0, 10) == 0);

    // Duplicates
    std::vector<int> v4 = {1, 1, 1, 1};
    assert(countFairPairs(v4, 2, 2) == 6); // all C(4,2)=6 pairs sum to 2

    // Large range, all pairs valid
    std::vector<int> v5 = {1, 2, 3};
    assert(countFairPairs(v5, 0, 100) == 3); // (0,1),(0,2),(1,2)

    // No valid pairs
    std::vector<int> v6 = {10, 20, 30};
    assert(countFairPairs(v6, 5, 15) == 0);

    // Negative and positive mix
    std::vector<int> v7 = {-5, 0, 5};
    assert(countFairPairs(v7, 0, 0) == 1); // (-5,5)

    // Edge with lower > upper? Assume lower <= upper, but test anyway
    std::vector<int> v8 = {1, 2};
    assert(countFairPairs(v8, 5, 3) == 0); // unsorted? Actually valid range empty

    // Large count exceeding int
    std::vector<int> v9(100000, 1);
    assert(countFairPairs(v9, 2, 2) == 4999950000LL); // C(100000,2)

    return 0;
}
// The solution sorts the input vector in non-decreasing order, which allows using binary search to count valid partners for each element without nested loops. For each index `i` (from 0 to n-2), we need to find all indices `j > i` such that `lower - nums[i] <= nums[j] <= upper - nums[i]`. After sorting, the valid range for `nums[j]` is contiguous. We use `std::lower_bound` on the subarray starting at `i+1` to find the first index where `nums[j] >= lower - nums[i]`, and `std::upper_bound` to find the first index where `nums[j] > upper - nums[i]`. The number of valid `j` values for this `i` is the difference of these two indices. We accumulate this difference. Edge cases include duplicate values (they are handled naturally because binary search works with values, not positions) and negative bounds (the subtraction works fine with signed integers). Time complexity is O(n log n) due to the initial sort and O(log n) per binary search, so total O(n log n). Space complexity is O(1) auxiliary (ignoring the sort's internal stack), since we only use a few variables.
