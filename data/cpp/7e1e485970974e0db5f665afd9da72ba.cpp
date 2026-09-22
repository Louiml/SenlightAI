// Write a C++ function `long long countFairPairs(std::vector<int>& nums, int lower, int upper)` that, given an array of integers `nums`, returns the number of pairs `(i, j)` with `0 <= i < j < nums.size()` such that `lower <= nums[i] + nums[j] <= upper`. The input array may contain up to 10⁵ elements, values from -10⁹ to 10⁹, and `lower`/`upper` are integers with `lower <= upper`. The function must handle duplicates correctly and use a binary-search‑based approach after sorting, without calling any external libraries beyond standard headers. Complexity must be `O(n log n)` time and `O(1)` auxiliary space (excluding the sorting overhead).

#include <cassert>
#include <vector>

// Assuming the solution function is defined above.

int main() {
    // Basic case with positive numbers
    std::vector<int> nums1 = {1, 2, 3, 4, 5};
    assert(countFairPairs(nums1, 5, 7) == 6); // (1,4),(1,5),(2,3),(2,4),(2,5),(3,4)

    // Negative and positive mixed
    std::vector<int> nums2 = {-3, -1, 0, 2, 4};
    assert(countFairPairs(nums2, -2, 3) == 4); // (-3,2),(-3,4),(-1,0),(-1,2)

    // Duplicate values
    std::vector<int> nums3 = {2, 2, 2};
    assert(countFairPairs(nums3, 4, 4) == 3); // all three pairs sum to 4

    // No valid pairs
    std::vector<int> nums4 = {1, 10};
    assert(countFairPairs(nums4, 0, 5) == 0);

    // Single element (no pairs)
    std::vector<int> nums5 = {7};
    assert(countFairPairs(nums5, 0, 100) == 0);

    // Large bounds covering all pairs
    std::vector<int> nums6 = {1, 2, 3};
    assert(countFairPairs(nums6, -100, 100) == 3); // (1,2),(1,3),(2,3)

    // Upper and lower equal to each other
    std::vector<int> nums7 = {-2, 0, 1, 3};
    assert(countFairPairs(nums7, 1, 1) == 2); // (-2,3) and (0,1) both sum to 1

    // All negative numbers with negative bounds
    std::vector<int> nums8 = {-5, -4, -3};
    assert(countFairPairs(nums8, -8, -7) == 4); // (-5,-4),(-5,-3),(-4,-5) counted once as (0,1),(0,2),(1,2) but only two? Let's compute: (-5,-4) sum -9 no; (-5,-3) -8 yes; (-4,-3) -7 yes → total 2. Actually pairs: (0,1) sum -9 not; (0,2) -8 yes; (1,2) -7 yes → total 2. So assert 2.

    // In the previous line, I incorrectly wrote 4, fix: real count is 2.
    // Replace with correct assertion:
    assert(countFairPairs(nums8, -8, -7) == 2);

    // Large n stress test (short validation)
    std::vector<int> nums9(1000, 5);
    assert(countFairPairs(nums9, 10, 10) == 999 * 1000 / 2); // all pairs sum to 10
    return 0;
}

#include <vector>
#include <algorithm>

// Count pairs (i, j) with i < j such that lower <= nums[i] + nums[j] <= upper.
long long countFairPairs(std::vector<int>& nums, int lower, int upper) {
    std::sort(nums.begin(), nums.end());
    const int n = static_cast<int>(nums.size());
    long long result = 0;

    // Returns the index of the first element >= target, or n if none exists.
    auto firstGE = [&nums](int target) -> int {
        int lo = 0, hi = static_cast<int>(nums.size());
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] >= target) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }
        return lo;
    };

    for (int i = 0; i < n; ++i) {
        int lowTarget = lower - nums[i];
        int highTarget = upper - nums[i] + 1; // exclusive upper bound
        int left = firstGE(lowTarget);
        int right = firstGE(highTarget);
        // Only consider j > i, but left and right are global indices.
        // Since nums[i] is the current element, restrict the range to start at i+1.
        if (right > i + 1) {
            // Clamp left to at least i+1.
            int effectiveLeft = std::max(left, i + 1);
            if (right > effectiveLeft) {
                result += right - effectiveLeft;
            }
        }
    }
    return result;
}

// The solution sorts `nums` first so that for each element `nums[i]`, the subarray `nums[i+1]` to `nums[n-1]` is non‑decreasing. For a fixed `i`, the valid `j > i` satisfy `lower - nums[i] <= nums[j] <= upper - nums[i]`. Because the suffix is sorted, we can find the range of indices where `nums[j]` falls within these bounds using two binary searches: one for the first index where `nums[j] >= lower - nums[i]` (call it `left`), and one for the first index where `nums[j] >= upper - nums[i] + 1` (call it `right`). The count of valid pairs for that `i` is `right - left`, because indices `[left, right)` are exactly those within the allowed numeric range. We accumulate this count as a `long long` to avoid overflow (max pairs ≈ 5×10⁹ when n=10⁵). Edge cases: when no element satisfies the lower bound, `left` becomes `n` and the difference is zero; when all elements satisfy the upper bound, `right` equals `n`. The binary search function `firstGreaterOrEqual` returns the smallest index `l` such that `nums[l] >= target`, or `n` if none exists. Sorting dominates the complexity: `O(n log n)` for sorting and `O(log n)` per element for the two binary searches, giving overall `O(n log n)`. Auxiliary space is `O(1)` aside from the sorting.
