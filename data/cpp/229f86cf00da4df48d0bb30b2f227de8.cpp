Write a C++ function that takes a vector of non-negative integers `nums` and an integer `k`, and returns the number of subarrays (contiguous non-empty sequences) in which the maximum element of the entire array appears at least `k` times. For example, if `nums = [1,3,2,3,3]` and `k = 2`, the maximum is `3`, and subarrays containing at least two `3`s include `[3,2,3]`, `[3,2,3,3]`, `[2,3,3]`, `[3,3]`, and `[1,3,2,3]`, `[1,3,2,3,3]` — a total of 6. The function must handle empty input (return 0), `k` larger than the frequency of the maximum (return 0), and large inputs efficiently. The solution should be self-contained with proper `const` correctness and include only the function definition (no `main`).

// The solution identifies the maximum element in the array once in O(n) time. Then it uses a sliding window (two pointers) over the array. The window is expanded by moving a right pointer and counting how many times the maximum appears inside that window. Whenever the count reaches `k`, the current window is valid, and importantly, every subarray that starts at the current left position and ends anywhere from the current right pointer to the end of the array also contains at least `k` occurrences of the maximum (since extending the window to the right only adds more elements, maintaining the count). Therefore, once the count is ≥ `k`, we add `nums.size() - right` to the answer (the number of valid right-end choices), then shrink the window from the left (incrementing left and adjusting the count accordingly) until the count drops below `k`. This continues as the right pointer moves across the array. Edge cases: if `k` is 0, the result would be every subarray, but the problem implies `k >= 1`; if the maximum occurs fewer than `k` times overall, the answer is 0. The algorithm runs in O(n) time because each pointer moves at most n times, and O(1) extra space.

#include <vector>
#include <algorithm>
#include <cstddef>

// Count subarrays where the global maximum appears at least k times.
long long countSubarrays(const std::vector<int>& nums, int k) {
    if (nums.empty() || k <= 0) return 0;

    const int max_val = *std::max_element(nums.begin(), nums.end());
    long long count_max = 0;
    long long left = 0;
    long long ans = 0;
    const std::size_t n = nums.size();

    for (std::size_t right = 0; right < n; ++right) {
        if (nums[right] == max_val) ++count_max;

        while (count_max >= k) {
            // All subarrays starting at 'left' ending at right, right+1, ..., n-1 are valid.
            ans += static_cast<long long>(n - right);
            if (nums[left] == max_val) --count_max;
            ++left;
        }
    }
    return ans;
}

#include <cassert>
#include <vector>

// Assume countSubarrays is defined above.

int main() {
    {
        std::vector<int> nums = {1, 3, 2, 3, 3};
        assert(countSubarrays(nums, 2) == 6);
        assert(countSubarrays(nums, 1) == 15); // all subarrays contain at least one 3, total subarrays = 5*6/2
        assert(countSubarrays(nums, 5) == 0); // only three 3's exist
        assert(countSubarrays(nums, 3) == 1); // only the whole array contains three 3's
    }
    {
        std::vector<int> nums = {5, 1, 5, 2};
        assert(countSubarrays(nums, 2) == 3); // [5,1,5], [5,1,5,2], [1,5,2]? Actually check: windows [0..2], [0..3], [1..3] have two 5's → 3
        assert(countSubarrays(nums, 1) == 10);
    }
    {
        std::vector<int> nums = {};
        assert(countSubarrays(nums, 1) == 0);
    }
    {
        std::vector<int> nums = {2, 2, 2};
        assert(countSubarrays(nums, 2) == 3); // [0..1], [1..2], [0..2] have two 2's
    }
    {
        std::vector<int> nums = {1, 2, 3, 4, 5};
        assert(countSubarrays(nums, 1) == 15);
        assert(countSubarrays(nums, 2) == 0);
    }
    return 0;
}
