Write a C++ function named `maximumSubarraySum` that takes a non-empty vector of integers (which may contain negative numbers) and returns the maximum possible sum of a contiguous subarray using Kadane's algorithm. The function must handle cases where all elements are negative (in which case the maximum subarray is the single largest element) and correctly process a mix of positive and negative values. The function should return an `int` and operate on a `const std::vector<int>&` without modifying the input.

// The core algorithm is Kadane's algorithm: we traverse the array once, maintaining a running `current_sum` of the best contiguous subarray ending at the current position. At each step, if `current_sum` becomes negative, it is reset to zero because a negative prefix would only reduce any future subarray sum; instead, starting fresh from the next element gives a better or equal result. Then we add the current element to `current_sum` and update the global maximum (`best_sum`) with `max(best_sum, current_sum)`.  
//
// Edge cases:  
// - If the array contains all negative numbers, every `current_sum` after adding a negative element will be negative, but we never reset before adding because we check the reset condition before adding, not after. In that scenario, `best_sum` is correctly updated to the largest (least negative) element because `current_sum` starts at 0, we add the first negative (e.g., -2) → `current_sum = -2`, `best = max(initial, -2)`. For subsequent negatives, `current_sum` becomes more negative, but `best` keeps the largest seen so far.  
// - For a single-element array, the function returns that element itself.  
// - Empty input is not allowed per the task specification, but we can defensively handle it by returning 0, though not required.  
//
// Time complexity: O(n) where n is the number of elements, because we traverse the vector once.  
// Space complexity: O(1) auxiliary space, since we only use two integer variables.

#include <vector>
#include <algorithm>

/**
 * Computes the maximum sum of a contiguous subarray within the given vector.
 * Uses Kadane's algorithm.
 * 
 * @param arr A non-empty vector of integers.
 * @return The maximum subarray sum.
 */
int maximumSubarraySum(const std::vector<int>& arr) {
    if (arr.empty()) {
        return 0; // Defensive handling; task guarantees non-empty input.
    }

    int current_sum = 0;
    int best_sum = arr[0]; // Initialize to first element to handle all-negative case.

    for (int value : arr) {
        // If current_sum has become negative, reset it to 0 because
        // a negative prefix does not help in maximizing subarray sum.
        if (current_sum < 0) {
            current_sum = 0;
        }
        current_sum += value;
        best_sum = std::max(best_sum, current_sum);
    }

    return best_sum;
}

#include <cassert>
#include <vector>

// Forward declaration for testing
int maximumSubarraySum(const std::vector<int>& arr);

int main() {
    // Basic case with mixed positive and negative numbers
    assert(maximumSubarraySum({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
    
    // All negative numbers: maximum is the single largest (least negative)
    assert(maximumSubarraySum({-1, -2, -3}) == -1);
    
    // All positive numbers: sum of all elements
    assert(maximumSubarraySum({1, 2, 3, 4}) == 10);
    
    // Single element
    assert(maximumSubarraySum({5}) == 5);
    assert(maximumSubarraySum({-7}) == -7);
    
    // Alternating signs
    assert(maximumSubarraySum({1, -2, 3, -4, 5}) == 5);
    
    // Leading and trailing negative numbers
    assert(maximumSubarraySum({-3, -1, 2, 3, -1}) == 5);
    
    // Multiple zeros
    assert(maximumSubarraySum({0, 0, 0}) == 0);
    
    // Large positive and negative cancellation
    assert(maximumSubarraySum({10, -2, -3, 5}) == 10);
    
    // Checking input is not modified (const reference)
    std::vector<int> input = {2, -1, 2};
    int result = maximumSubarraySum(input);
    assert(input[0] == 2 && input[1] == -1 && input[2] == 2);
    assert(result == 3);
    
    return 0;
}
