// Write a C++ function named `maxSubarrayProduct` that takes a non-empty vector of integers (which may contain zeros, negative numbers, and duplicates) and returns the maximum product that can be obtained by multiplying a contiguous subarray (i.e., a slice of consecutive elements). For example, for `[-2, 0, -1]`, the maximum product is `0` (not negative), and for `[2, 3, -2, 4]`, it is `6`. The function must handle arrays with a single element and arrays containing zeros correctly, and it should not use any extra arrays or dynamic allocation beyond constant auxiliary space. The solution must be efficient and based on scanning the array once. Assume the input vector is non-empty.
The classic approach uses a two-pass traversal with prefix and postfix products. The key insight is that the maximum product subarray either starts at the left side or ends at the right side of the array, or it lies in between, but by tracking products from both directions simultaneously we cover all possibilities. We maintain two running products: one multiplying forward (`prefix`) and one backward (`postfix`). When a zero is encountered, either running product becomes zero, so we reset it to 1 before multiplying the next element to effectively "start fresh" after the zero. At each step, we update both products with the current left and right elements, then update the global maximum with the larger of the two. This works because any contiguous subarray product can be expressed as some prefix product divided by an earlier prefix product (or equivalently for postfix), and the reset-on-zero ensures we never carry a zero through. Edge cases include arrays with all negative numbers (e.g., `[-1, -2]` yields `2`), arrays with zeros (e.g., `[0, -1]` yields `0`), and single-element arrays (e.g., `[5]` yields `5`). The algorithm runs in O(n) time and O(1) auxiliary space, where n is the length of the input vector. The function must be `const`-correct by accepting a `const std::vector<int>&`.
#include <vector>
#include <algorithm>
#include <climits>

// Returns the maximum product of any contiguous subarray within nums.
// Handles zeros, negatives, and single-element arrays.
int maxSubarrayProduct(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    int prefix = 1;
    int postfix = 1;
    int best = INT_MIN;

    for (int i = 0; i < n; ++i) {
        // Reset products when they become zero to start a new subarray.
        if (prefix == 0) prefix = 1;
        if (postfix == 0) postfix = 1;

        prefix *= nums[i];
        postfix *= nums[n - i - 1];
        best = std::max(best, std::max(prefix, postfix));
    }
    return best;
}
#include <cassert>
#include <vector>

// Forward declaration for testing (assumes solution is defined above).
int maxSubarrayProduct(const std::vector<int>&);

int main() {
    // Single element
    assert(maxSubarrayProduct({5}) == 5);
    assert(maxSubarrayProduct({-3}) == -3);

    // Basic cases
    assert(maxSubarrayProduct({2, 3, -2, 4}) == 6);
    assert(maxSubarrayProduct({-2, 0, -1}) == 0);
    assert(maxSubarrayProduct({-1, -2}) == 2);

    // Zeros and negatives
    assert(maxSubarrayProduct({0, 2}) == 2);
    assert(maxSubarrayProduct({-2, -3, 0, -4}) == 6); // max is 6 or 0? Wait: subarray {-2,-3}=6, {-4}= -4, {0}=0 -> 6
    assert(maxSubarrayProduct({-2, 0}) == 0);

    // All negative
    assert(maxSubarrayProduct({-1, -2, -3}) == 6); // {-1,-2}=2, {-2,-3}=6

    // Large array with many zeros
    assert(maxSubarrayProduct({1, 0, 2, 3, 0, -1, -2, 4}) == 8); // {-1,-2,4}=8

    return 0;
}
