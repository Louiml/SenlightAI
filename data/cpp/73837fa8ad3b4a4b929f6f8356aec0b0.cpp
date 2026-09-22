// Write a C++ function named `maxSubarraySumWithAllNegativeFallback` that takes a non-empty vector of integers and returns (as an `int`) either the maximum sum of any non-empty contiguous subarray if at least one element is non-negative, or the maximum element value if all elements are negative. For example, given `{-2, 1, -3, 4, -1, 2, 1, -5, 4}`, the function should return `6` (from subarray `{4, -1, 2, 1}`), while given `{-5, -2, -9}`, it should return `-2`. Handle vectors that may contain zeros as non-negative values, and ensure the function works correctly for a single-element vector.
#include <cassert>
#include <vector>

int main() {
    // Basic mixed signs
    assert(maxSubarraySumWithAllNegativeFallback({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
    // All negative numbers
    assert(maxSubarraySumWithAllNegativeFallback({-5, -2, -9}) == -2);
    // Single negative element
    assert(maxSubarraySumWithAllNegativeFallback({-7}) == -7);
    // Single non-negative element
    assert(maxSubarraySumWithAllNegativeFallback({3}) == 3);
    // Zeros count as non-negative
    assert(maxSubarraySumWithAllNegativeFallback({-1, 0, -2}) == 0);
    // All zeros
    assert(maxSubarraySumWithAllNegativeFallback({0, 0, 0}) == 0);
    // Large positive subarray at the end
    assert(maxSubarraySumWithAllNegativeFallback({-3, -2, 5, 6}) == 11);
    // Mixed with largest sum being a single element (positive)
    assert(maxSubarraySumWithAllNegativeFallback({-1, -2, 10, -3, -4}) == 10);
    // Two-element vector with both negative
    assert(maxSubarraySumWithAllNegativeFallback({-4, -1}) == -1);
    // Two-element vector with one positive and one negative
    assert(maxSubarraySumWithAllNegativeFallback({5, -10}) == 5);
    return 0;
}
#include <vector>
#include <algorithm>

// Returns the maximum subarray sum when at least one element is non-negative,
// otherwise returns the maximum element (which will be negative).
int maxSubarraySumWithAllNegativeFallback(const std::vector<int>& nums) {
    // Check if any element is non-negative (>=0)
    bool hasNonNegative = false;
    for (int value : nums) {
        if (value >= 0) {
            hasNonNegative = true;
            break;
        }
    }

    if (hasNonNegative) {
        // Kadane's algorithm: currentSum resets to 0 if it drops below 0
        int currentSum = 0;
        int bestSum = 0; // Safe because at least one non-negative exists
        for (int value : nums) {
            currentSum += value;
            bestSum = std::max(bestSum, currentSum);
            if (currentSum < 0) {
                currentSum = 0;
            }
        }
        return bestSum;
    } else {
        // All negative: return the maximum (least negative) element
        int maxElement = nums[0];
        for (int value : nums) {
            maxElement = std::max(maxElement, value);
        }
        return maxElement;
    }
}
// The core problem is to compute the maximum subarray sum in the classic Kadane’s algorithm sense, but with a twist: if the entire array consists only of negative numbers, the standard Kadane’s algorithm would produce a negative maximum (which is actually the largest element), but a naive implementation that initializes the best sum to zero would incorrectly return zero. The given snippet uses a nested loop (O(n²)) to compute all subarray sums when at least one non-negative element exists, and a simple maximum scan otherwise. We can improve this to O(n) using Kadane’s algorithm: keep a running `currentSum` that is reset to zero whenever it becomes negative (but only if at least one non-negative number exists; otherwise, we need the largest element). A safe approach: first, scan the array to determine if any element is non-negative (>=0). If yes, apply Kadane’s algorithm with `currentSum` reset to zero when negative, and track the maximum. If no, then the answer is simply the maximum element in the array (which will be negative). Edge cases: all negative numbers, mixed numbers, zeros (which count as non-negative), and single-element vectors. Time complexity is O(n) for both scans (or O(n) total with a combined pass), and space complexity is O(1) auxiliary. The reference solution will use a single loop to both detect non-negativity and compute the answer, but for clarity we can do two passes.
