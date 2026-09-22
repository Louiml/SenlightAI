Write a C++ function `int minRangeAfterRemovals(std::vector<int> nums, int k)` that takes an array of integers and an integer `k` (where `0 <= k < nums.size()`), and returns the minimum possible difference between the maximum and minimum element of the remaining array after removing exactly `k` elements. You may reorder the remaining elements arbitrarily, but you cannot add or modify any element values. For example, given `nums = [5, 1, 8, 3, 9]` and `k = 2`, removing `8` and `9` leaves `[5, 1, 3]`, whose range is `4`, and that is the minimum possible.
The key observation is that after removing `k` elements, we keep exactly `n - k` elements. To minimize the range (max - min) of the kept elements, we should keep a contiguous block of the sorted array, because any non-contiguous selection would have a range at least as large as a contiguous block that spans the same min and max. So sort the array ascending. Then consider all windows of length `m = n - k` in the sorted array. For each window from index `i` to `i + m - 1`, the range is `sorted[i + m - 1] - sorted[i]`. Take the minimum over all valid `i` from `0` to `k` (since `i + m - 1 <= n - 1`). Edge case: `k = 0` means we keep all elements, so only one window exists. If `k = n - 1`, we keep exactly one element, so the range is `0`, and the loop handles this because `m = 1` and the difference is always `0`. Complexity: sorting takes `O(n log n)`, the loop takes `O(n - k)`, and we use `O(1)` extra space beyond the input.
#include <vector>
#include <algorithm>
#include <climits>

// Returns the minimum possible difference between max and min
// after removing exactly k elements from nums.
int minRangeAfterRemovals(std::vector<int> nums, int k) {
    const int n = static_cast<int>(nums.size());
    if (k >= n) {
        return 0; // remove all → empty or one element range 0 (handled by caller constraints)
    }
    std::sort(nums.begin(), nums.end());
    const int keep = n - k;
    int ans = INT_MAX;
    for (int i = 0; i + keep - 1 < n; ++i) {
        ans = std::min(ans, nums[i + keep - 1] - nums[i]);
    }
    return ans;
}
#include <cassert>
#include <vector>

// Solution function is declared here (assume included from above)
int minRangeAfterRemovals(std::vector<int> nums, int k);

int main() {
    assert(minRangeAfterRemovals({5, 1, 8, 3, 9}, 2) == 4);
    assert(minRangeAfterRemovals({1, 2, 3, 4, 5}, 0) == 4);
    assert(minRangeAfterRemovals({1, 2, 3, 4, 5}, 4) == 0);
    assert(minRangeAfterRemovals({10, 20, 30}, 1) == 10);
    assert(minRangeAfterRemovals({7, 7, 7, 7}, 2) == 0);
    assert(minRangeAfterRemovals({1, 100, 2, 99, 3, 98}, 3) == 1);
    assert(minRangeAfterRemovals({-5, -1, -10, 0, 3}, 2) == 3);
    assert(minRangeAfterRemovals({42}, 0) == 0);
    assert(minRangeAfterRemovals({1, 2, 2, 3}, 1) == 1);
    assert(minRangeAfterRemovals({5, 6, 1, 2, 9, 10, 3, 4}, 4) == 1);
    return 0;
}
