Write a C++ function `long long maxSubarrayProduct(const std::vector<int>& arr)` that returns the maximum product of any contiguous subarray within the given non-empty array of integers. The array may contain positive numbers, negative numbers, and zeros. The result may exceed the range of `int`, so return a `long long`. For example, for input `{2, 3, -2, 4}`, the maximum product is `6` (from subarray `{2, 3}`), and for `{-2, 0, -1}` the maximum is `0`. Handle edge cases such as all-negative arrays (e.g., `{-2, -3, -4}` → `24` from the whole array) and arrays with a single element. Do not use brute force; design an efficient linear-time algorithm.
// The key observation is that the maximum product subarray can be found by scanning the array from left to right while maintaining the maximum and minimum product ending at the current position. Since multiplying by a negative number flips the role of the maximum and minimum (the smallest negative product becomes the largest positive when multiplied by another negative), we need to track both. A cleaner approach uses prefix and suffix products: compute the product of all elements from the left (prefix) and from the right (suffix) up to each index, and track the maximum of these products. When a zero is encountered, the product resets (because any subarray crossing a zero has product zero), so we reset prefix and suffix to 1 after processing the zero. This captures all possible subarrays because any subarray is either a prefix of some suffix or a suffix of some prefix, and resetting at zero ensures we never include a zero in the product unless it is the subarray itself. Edge cases: if all numbers are zero, the maximum is 0; if there is a single element, return that element. The algorithm runs in O(n) time and O(1) extra space, where n is the array size.
#include <vector>
#include <algorithm>
#include <climits>

// Returns the maximum product of any contiguous subarray in arr.
// Handles positive, negative, and zero values. Assumes arr is non-empty.
long long maxSubarrayProduct(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    long long ans = LLONG_MIN;
    long long pre = 1;   // prefix product
    long long post = 1;  // suffix product (from the right)
    
    for (int i = 0; i < n; ++i) {
        pre *= arr[i];
        post *= arr[n - i - 1];
        ans = std::max(ans, std::max(pre, post));
        
        // If we hit zero, the product resets for the next segment.
        if (pre == 0) pre = 1;
        if (post == 0) post = 1;
    }
    
    return ans;
}
#include <cassert>

int main() {
    // Basic cases
    assert(maxSubarrayProduct({2, 3, -2, 4}) == 6);
    assert(maxSubarrayProduct({-2, 0, -1}) == 0);
    assert(maxSubarrayProduct({-2, -3, -4}) == 24);
    
    // Single element
    assert(maxSubarrayProduct({5}) == 5);
    assert(maxSubarrayProduct({-5}) == -5);
    assert(maxSubarrayProduct({0}) == 0);
    
    // All zeros
    assert(maxSubarrayProduct({0, 0, 0}) == 0);
    
    // Mixed with large product
    assert(maxSubarrayProduct({1, 2, 3, 4}) == 24);
    assert(maxSubarrayProduct({-1, -2, -3, 0, 5, 6}) == 30);
    
    // Negative single element at end
    assert(maxSubarrayProduct({1, -2, 3}) == 3);
    
    // Product can exceed int range
    std::vector<int> large = {100000, 100000, 100000};
    assert(maxSubarrayProduct(large) == 1000000000000LL);
    
    return 0;
}
