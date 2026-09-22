Write a C++ function that takes a positive integer `target` and a vector of positive integers `nums`, and returns the length of the smallest contiguous subarray whose sum is greater than or equal to `target`. If no such subarray exists, return `0`. The function must run efficiently for large inputs (up to 10^5 elements). Examples: `minSubArrayLen(7, {2,3,1,2,4,3})` returns `2` (subarray `[4,3]`), and `minSubArrayLen(11, {1,1,1})` returns `0`.

// The problem is a classic sliding-window (two-pointer) minimum-length subarray sum. Maintain a window defined by indices `left` and `right` (both starting at 0). As we expand `right` to the right, we add `nums[right]` to the running sum `current_sum`. While `current_sum >= target`, we record the current window length `right - left + 1` as a candidate for the minimum, then shrink the window by subtracting `nums[left]` and incrementing `left`. This continues until the sum drops below `target`. The greedy nature is valid because once a window satisfies the condition, any larger window containing it cannot be a better (shorter) answer, so we shrink as much as possible. Edge cases: empty vector (return 0), no subarray reaches target (return 0), single element exactly equal to target (return 1), and all elements sum below target. Complexity: each element is added once and subtracted at most once, so O(n) time and O(1) auxiliary space (excluding input storage).

#include <vector>
#include <algorithm>

// Returns the minimal length of a contiguous subarray whose sum >= target.
// Returns 0 if no such subarray exists.
int minSubArrayLen(int target, const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    int left = 0;
    long long current_sum = 0;
    int min_length = n + 1; // initial value larger than any possible length

    for (int right = 0; right < n; ++right) {
        current_sum += nums[right];
        while (left <= right && current_sum >= target) {
            min_length = std::min(min_length, right - left + 1);
            current_sum -= nums[left];
            ++left;
        }
    }
    return (min_length == n + 1) ? 0 : min_length;
}

#include <cassert>
#include <vector>

// Declare the function (or include the solution header)
int minSubArrayLen(int target, const std::vector<int>& nums);

int main() {
    assert(minSubArrayLen(7, {2,3,1,2,4,3}) == 2);
    assert(minSubArrayLen(4, {1,4,4}) == 1);
    assert(minSubArrayLen(11, {1,1,1}) == 0);
    assert(minSubArrayLen(1, {1}) == 1);
    assert(minSubArrayLen(5, {2,2,2,2}) == 0);
    assert(minSubArrayLen(6, {1,2,3,4}) == 2); // [3,4] or [2,3,4]? Actually [3,4] sum=7 >=6, length 2
    assert(minSubArrayLen(10, {1,2,3,4,5}) == 3); // [3,4,5] sum=12
    assert(minSubArrayLen(15, {5,1,3,5,10,7,4,9,2,8}) == 2); // [10,7] or [7,4,9]? Actually [10,7] sum=17 >=15
    assert(minSubArrayLen(100, {}) == 0);
    assert(minSubArrayLen(2, {1,1}) == 2);
}
