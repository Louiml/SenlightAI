// Write a C++ function named `minimumSubarrayLength` that takes an integer `target` and a vector of non-negative integers `nums`, and returns the minimal length of a contiguous subarray whose sum is at least `target`. If no such subarray exists, return `0`. The function should be efficient for inputs up to 100,000 elements, and must handle cases where the target is zero, the array is empty, or all elements together are smaller than the target. The sum of any subarray may exceed the range of a 32-bit integer, so use a 64-bit type for the running sum internally. The relative order of input must remain unchanged, and the vector should not be modified (use const reference).
The standard efficient approach is the sliding window (two-pointer) technique. We maintain two indices: a `left` pointer marking the start of the current window and a `right` pointer that expands the window to the right. We keep a running sum of elements from `left` to `right`. Whenever the running sum becomes at least `target`, we record the current window length (`right - left + 1`) as a candidate answer, then shrink the window from the left by subtracting `nums[left]` and incrementing `left`, repeating this shrinking step as long as the sum still meets the target. This way, each element is added once and removed at most once, giving a linear scan. We must be careful to use a 64-bit integer (`long long`) for the running sum to avoid overflow when summing many large values. Edge cases: if `target <= 0`, the minimum length is `1` (any single element qualifies, and if the array is empty, return `0`); if the total sum is less than `target`, return `0` (our loop will naturally never update the answer, so we initialize the answer to a large value and convert to `0` at the end). Time complexity is O(n) and space complexity is O(1) auxiliary.
#include <vector>
#include <algorithm>
#include <climits>

// Returns the minimal length of a contiguous subarray with sum >= target.
// Returns 0 if no such subarray exists.
int minimumSubarrayLength(int target, const std::vector<int>& nums) {
    if (target <= 0) {
        return nums.empty() ? 0 : 1;
    }

    long long current_sum = 0;
    int left = 0;
    int min_length = INT_MAX;

    for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
        current_sum += nums[right];

        while (current_sum >= target) {
            min_length = std::min(min_length, right - left + 1);
            current_sum -= nums[left];
            ++left;
        }
    }

    return (min_length == INT_MAX) ? 0 : min_length;
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(minimumSubarrayLength(7, {2, 3, 1, 2, 4, 3}) == 2); // [4,3]
    assert(minimumSubarrayLength(4, {1, 4, 4}) == 1);           // [4]
    assert(minimumSubarrayLength(11, {1, 1, 1}) == 0);          // sum < target

    // Zero target
    assert(minimumSubarrayLength(0, {1, 2, 3}) == 1);
    assert(minimumSubarrayLength(-5, {10, 20}) == 1);

    // Empty array
    assert(minimumSubarrayLength(5, {}) == 0);

    // Large values to test overflow handling (sum exceeds 32-bit)
    std::vector<int> large = {1000000000, 1000000000, 1000000000};
    assert(minimumSubarrayLength(2000000000, large) == 2);

    // Single element exactly equal
    assert(minimumSubarrayLength(5, {5}) == 1);

    // Whole array needed
    assert(minimumSubarrayLength(6, {1, 2, 3}) == 3);

    // Window shrink after finding a hit
    assert(minimumSubarrayLength(8, {5, 1, 3, 2, 4}) == 3); // [3,2,4] or [5,1,3]

    // All zeros with positive target
    assert(minimumSubarrayLength(1, {0, 0, 0}) == 0);
}
