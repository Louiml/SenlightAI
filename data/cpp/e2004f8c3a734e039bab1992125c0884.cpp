// Write a C++ function that, given a vector of positive integers and a target sum, determines whether there exists a contiguous subarray whose elements sum exactly to the target. The function should return a boolean indicating existence. If multiple such subarrays exist, it is enough to return whether at least one exists. Handle edge cases such as empty input vectors, target equal to zero (where an empty subarray is not allowed), and subarrays of length one. The function should use only O(1) additional space beyond the input and should not modify the input vector.

#include <cassert>
#include <vector>

// ... solution function here ...

int main() {
    // Basic case with a subarray in the middle.
    assert(hasSubarraySum({1, 2, 3, 4, 5}, 9) == true); // [4,5]

    // Single element equals target.
    assert(hasSubarraySum({10}, 10) == true);

    // Single element does not equal target.
    assert(hasSubarraySum({10}, 5) == false);

    // Entire vector sums to target.
    assert(hasSubarraySum({2, 4, 6}, 12) == true);

    // Empty vector.
    assert(hasSubarraySum({}, 5) == false);

    // Target zero with positive elements.
    assert(hasSubarraySum({1, 2, 3}, 0) == false);

    // No subarray exists.
    assert(hasSubarraySum({1, 3, 5, 7}, 12) == false);

    // Subarray at the beginning.
    assert(hasSubarraySum({5, 2, 1, 8}, 7) == true); // [5,2]

    // Long run with repeated values.
    std::vector<int> big(1000, 1);
    assert(hasSubarraySum(big, 500) == true); // 500 ones

    // Negative target should be false.
    assert(hasSubarraySum({1, 2, 3}, -1) == false);

    return 0;
}

#include <vector>

// Check if any contiguous subarray of positive integers sums to the target.
bool hasSubarraySum(const std::vector<int>& nums, int target) {
    if (nums.empty()) return false;
    if (target <= 0) return false; // all positive elements, so no positive-length subarray can sum to zero or negative.

    int left = 0;
    int current_sum = 0;

    for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
        current_sum += nums[right];

        // Shrink window while sum exceeds target.
        while (current_sum > target && left <= right) {
            current_sum -= nums[left];
            ++left;
        }

        if (current_sum == target) {
            return true;
        }
    }

    return false;
}

// The problem is a classic sliding-window or two-pointer technique for positive numbers. Since all elements are positive integers, we can maintain a window defined by two indices `left` and `right`. We expand the window to the right by adding `nums[right]` to a running `current_sum`. While `current_sum` exceeds the target, we shrink the window from the left by subtracting `nums[left]` and incrementing `left`. At each step, if `current_sum` equals the target, we return `true`. Because all numbers are positive, this monotonic behavior guarantees we never miss a subarray summing to the target. The main loop runs in O(n) time since each index is visited at most twice (once added, once removed). Space complexity is O(1) beyond the input. Edge cases: an empty vector cannot contain a subarray, so return false (unless target is zero, but empty subarray is disallowed). A single element equal to the target is handled naturally. If the target is zero, no positive-length subarray of positive integers sums to zero, so return false.
