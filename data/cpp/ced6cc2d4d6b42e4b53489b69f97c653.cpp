// Given a non-empty vector of non-negative integers and a positive integer `k`, write a C++ function `hasGoodSubarray` that returns `true` if there exists a contiguous subarray of length **at least two** whose sum is a multiple of `k` (i.e., sum % k == 0), and `false` otherwise. Note that `k` may be larger than any individual element, and the sum can exceed the range of `int`; handle this using appropriate types. You must implement an efficient algorithm that does not enumerate all subarrays. Example: for `nums = {23, 2, 4, 6, 7}` and `k = 6`, return `true` because subarray `[2, 4]` sums to 6; for `nums = {1, 2, 3}` and `k = 5`, return `false` because no contiguous subarray of length ≥2 has sum divisible by 5.

The solution uses the prefix sum modulo `k` technique. Compute `prefix[i]` = (sum of `nums[0..i]`) % `k`. A subarray from index `j+1` to `i` (inclusive) has sum divisible by `k` exactly when `prefix[i] == prefix[j]`. To ensure the subarray length is at least 2, we require `i - j >= 2`, i.e., `j <= i-2`. Therefore, when processing index `i`, we need to check if any `prefix[j]` for `j <= i-2` equals `prefix[i]`. We maintain a hash set `seen` that contains `prefix[j]` for all `j` up to `i-2`. Initially, before loop start, `seen` is empty. For `i=0`, no valid `j` exists (since `j <= -2` is impossible). For `i=1`, we add `prefix[0]` to `seen` before checking, so that later `i>=2` can compare against it. Then for each `i` from 2 onward, we first add `prefix[i-2]` to `seen` before the check (or equivalently, after the loop for `i` we add `prefix[i-1]` for the next iteration). Implementation: iterate `i` from 0 to n-1. For `i >= 2`, insert `prefix[i-2]` into `seen` before checking. Then if `seen` contains `prefix[i]`, return `true`. Also handle edge case: if `k == 1`, any subarray sums to 0 mod 1, so return `true` if `n >= 2`. Also note that zero values in `nums` work fine: a single zero alone does not count (length 1 problem), but two consecutive zeros give sum 0, which is divisible by any `k`. The algorithm runs in O(n) time and O(n) auxiliary space for the prefix array and the set (the set can be avoided by using a map from remainder to earliest index, but the set approach is simpler if we use the `prefix[i-2]` insertion trick). Space can be reduced to O(n) for prefix, or O(k) if we store only remainders in a boolean array, but `k` can be huge, so set is acceptable.

#include <vector>
#include <unordered_set>

// Returns true if there exists a contiguous subarray of length >= 2
// whose sum is divisible by k. The input vector nums contains non-negative integers.
bool hasGoodSubarray(const std::vector<int>& nums, int k) {
    const int n = static_cast<int>(nums.size());
    if (n < 2) {
        return false;
    }
    if (k == 1) {
        return true;
    }

    // Compute prefix sums modulo k
    std::vector<int> prefix(n);
    int running_sum = 0;
    for (int i = 0; i < n; ++i) {
        running_sum = (running_sum + nums[i]) % k;
        prefix[i] = running_sum;
    }

    // Store prefix remainders from indices up to i-2
    std::unordered_set<int> seen;
    for (int i = 0; i < n; ++i) {
        if (i >= 2) {
            seen.insert(prefix[i - 2]);
        }
        if (seen.find(prefix[i]) != seen.end()) {
            return true;
        }
    }
    return false;
}

#include <cassert>

int main() {
    // Basic example from problem
    std::vector<int> nums1 = {23, 2, 4, 6, 7};
    assert(hasGoodSubarray(nums1, 6) == true);

    // No subarray length >= 2 with sum divisible by 5
    std::vector<int> nums2 = {1, 2, 3};
    assert(hasGoodSubarray(nums2, 5) == false);

    // Two zeros always works
    std::vector<int> nums3 = {0, 0};
    assert(hasGoodSubarray(nums3, 10) == true);

    // k=1: any subarray of length >=2 works
    std::vector<int> nums4 = {5, 7};
    assert(hasGoodSubarray(nums4, 1) == true);

    // Single element: always false
    std::vector<int> nums5 = {6};
    assert(hasGoodSubarray(nums5, 6) == false);

    // Large k, but sum of two elements divisible by k
    std::vector<int> nums6 = {5, 5, 1};
    assert(hasGoodSubarray(nums6, 10) == true);

    // Requires subarray of length 3: [2,4,6] sum 12 divisible by 6
    std::vector<int> nums7 = {1, 2, 4, 6, 1};
    assert(hasGoodSubarray(nums7, 6) == true);

    // All ones with k=2: any even-length subarray works
    std::vector<int> nums8 = {1, 1, 1, 1};
    assert(hasGoodSubarray(nums8, 2) == true);

    // Subarray sum not divisible by k, but a longer one is
    std::vector<int> nums9 = {3, 1, 2, 4};
    assert(hasGoodSubarray(nums9, 7) == true); // [1,2,4] sums to 7

    // Empty? Not provided, but if empty, return false
    std::vector<int> nums10;
    assert(hasGoodSubarray(nums10, 5) == false);
}
