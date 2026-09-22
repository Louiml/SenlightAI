// Write a C++ function `countDistinctTriples` that takes a vector of integers and returns the number of distinct triplets `(i, j, k)` with `i < j < k` such that the three elements at those indices sum to zero. The vector may contain duplicate values, negative numbers, and any length from 0 upwards. A "distinct triplet" means a unique combination of *values* chosen from the array, not unique indices; for example, if the array is `{0, 0, 0}`, there is exactly one distinct triplet `(0,0,0)` even though there are multiple index combinations. If the array has fewer than 3 elements, return 0.
// The standard approach for counting distinct triplets that sum to zero is to sort the array and then use two-pointer technique. After sorting, we iterate over each possible first element. To ensure distinct triplets, we skip duplicate first elements. For each first element at index `i`, we set a left pointer `lo = i+1` and right pointer `hi = n-1`. If the sum `arr[i]+arr[lo]+arr[hi] == 0`, we count it. Then we move both pointers inward, but crucially we must skip duplicate values for both `lo` and `hi` to avoid counting the same triplet again. If the sum is negative, we increment `lo`; if positive, decrement `hi`. This avoids the O(n³) brute force approach. Edge cases include arrays with fewer than 3 elements (return 0), all zeros (count exactly 1), and arrays with many duplicates (e.g., `{-1,0,1,1,-1}` should count 1 for `(-1,0,1)` and not double-count due to multiple index pairs). Time complexity is O(n²) after sorting O(n log n). Space complexity is O(1) auxiliary if we sort in-place (assuming the input vector is passed by value or we copy it).
#include <vector>
#include <algorithm>

int countDistinctTriples(std::vector<int> nums) {
    std::sort(nums.begin(), nums.end());
    int n = nums.size();
    if (n < 3) return 0;
    int count = 0;
    for (int i = 0; i < n - 2; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) continue; // skip duplicate first elements
        int lo = i + 1;
        int hi = n - 1;
        while (lo < hi) {
            int sum = nums[i] + nums[lo] + nums[hi];
            if (sum == 0) {
                ++count;
                // skip duplicates for lo
                while (lo < hi && nums[lo] == nums[lo + 1]) ++lo;
                // skip duplicates for hi
                while (lo < hi && nums[hi] == nums[hi - 1]) --hi;
                ++lo;
                --hi;
            } else if (sum < 0) {
                ++lo;
            } else {
                --hi;
            }
        }
    }
    return count;
}
#include <cassert>
#include <vector>

// Solution function declaration (assume it is defined above)
int countDistinctTriples(std::vector<int> nums);

int main() {
    // Basic cases
    assert(countDistinctTriples({}) == 0);
    assert(countDistinctTriples({1,2}) == 0);
    assert(countDistinctTriples({0,0,0}) == 1);
    assert(countDistinctTriples({1,2,-3}) == 1);
    assert(countDistinctTriples({-1,0,1}) == 1);
    assert(countDistinctTriples({-1,0,1,2,-1,-4}) == 2); // {-1,-1,2} and {-1,0,1}
    // Duplicates handled
    assert(countDistinctTriples({-1,0,1,1,-1}) == 1); // only one distinct value triplet
    assert(countDistinctTriples({0,0,0,0}) == 1);
    // No triplets
    assert(countDistinctTriples({1,2,3}) == 0);
    assert(countDistinctTriples({5,5,5}) == 0);
    // Large set with negative and positive
    assert(countDistinctTriples({-2,0,1,1,2}) == 2); // (-2,0,2) and (-2,1,1)
    return 0;
}
