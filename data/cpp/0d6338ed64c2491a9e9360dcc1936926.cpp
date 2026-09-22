Write a C++ function `longestPositiveRun` that takes a non-empty vector of integers and returns the length of the longest contiguous subarray whose sum is strictly greater than zero. For example, given `{1,2,3,-5,1,2,5}`, the longest positive-sum contiguous subarray is the whole array with sum `9`, so the answer is `7`. If no subarray has a positive sum (e.g., all elements are zero or negative), return `0`. The function must be `const`-correct, and you must not modify the input vector.
The key is to consider all possible contiguous subarrays and check their sums. A brute‑force solution checks every start and end index, computing each sum in O(n²) time, which is too slow for large inputs. Instead, use a two‑pointer / sliding‑window approach: maintain a window `[left, right]` and its current sum. Expand the right pointer to extend the window. If the sum becomes positive, update the maximum length seen so far. If the sum becomes non‑positive, shrink the window from the left until the sum becomes positive again, or the left catches up to the right. This works because any subarray with a non‑positive sum cannot be part of a longer positive subarray—removing elements from the left can only help restore positivity. Edge cases: single positive element (length 1), all negative or zero (return 0), and the whole array having a positive sum (length equals vector size). Time complexity is O(n) because each index is visited at most twice (once by right, once by left). Space complexity is O(1) beyond the input.
#include <vector>
#include <algorithm>

// Returns the length of the longest contiguous subarray with a positive sum.
// If no such subarray exists, returns 0. Does not modify the input.
int longestPositiveRun(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    int left = 0;
    long long current_sum = 0;
    int max_len = 0;

    for (int right = 0; right < n; ++right) {
        current_sum += nums[right];

        // While the window sum is not positive, shrink from the left.
        while (current_sum <= 0 && left <= right) {
            current_sum -= nums[left];
            ++left;
        }

        // If the window is valid (sum > 0), update the best length.
        if (current_sum > 0) {
            max_len = std::max(max_len, right - left + 1);
        }
    }

    return max_len;
}
#include <cassert>
#include <vector>

int longestPositiveRun(const std::vector<int>& nums);

int main() {
    // Basic positive run covering whole array
    assert(longestPositiveRun({1, 2, 3, -5, 1, 2, 5}) == 7);
    // All negative -> no positive subarray
    assert(longestPositiveRun({-1, -2, -3}) == 0);
    // Single positive element
    assert(longestPositiveRun({5}) == 1);
    // Single zero -> no positive sum
    assert(longestPositiveRun({0}) == 0);
    // Mixed with only one positive subarray
    assert(longestPositiveRun({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
    // Large negative and small positive segments
    assert(longestPositiveRun({-10, 1, 2, -1, 1}) == 3);
    // All positive
    assert(longestPositiveRun({1, 2, 3, 4}) == 4);
    // Zero and positive mix
    assert(longestPositiveRun({0, 0, 3, -1, 2}) == 4);
    return 0;
}
