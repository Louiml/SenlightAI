// Given an array of integers (which may include negative numbers, zeros, and positive numbers), write a C++ function named `maxProductSubarray` that takes a `const std::vector<int>&` as input and returns the maximum product that can be obtained by multiplying a contiguous subarray (at least one element). The function must handle large products correctly (use `long long` for the return and intermediate calculations) and must work efficiently for arrays up to length 10^5. The product of an empty subarray is not allowed; every result must come from at least one element. Handle all edge cases including arrays with all negative numbers, arrays with zeros, and arrays with a single element.
The standard efficient approach uses two simultaneous running products: one from the left (prefix product) and one from the right (suffix product). Initialize both to 1. Iterate through the array, updating the prefix product with the current element from the left and the suffix product with the current element from the right (using symmetric indexing). After each multiplication, update the global maximum answer. If either product becomes 0, reset it to 1 because a subsequent subarray can start fresh after a zero. This works because a zero breaks the product chain, and starting from the next element is optimal. The algorithm considers all possible contiguous subarrays implicitly, including those that start at the beginning (via prefix) and end at the end (via suffix), and any subarray in between is covered by one of the two directions when a zero resets occur. Key edge cases: an array with all zeros — the answer is 0 (since a single zero is a valid subarray); an array with one negative number — the answer is that negative number (since you must take at least one element); an array with zeros and negatives — the maximum will be positive if possible, otherwise zero or the largest negative. Time complexity: O(n) with a single pass. Space complexity: O(1) auxiliary, excluding the input vector.
#include <vector>
#include <algorithm>
#include <climits>

/**
 * Returns the maximum product of a contiguous subarray in the input vector.
 * Uses left-to-right and right-to-left running products, resetting on zeros.
 * Handles negative numbers and zeros correctly. O(n) time, O(1) space.
 */
long long maxProductSubarray(const std::vector<int>& nums) {
    long long ans = LLONG_MIN;
    long long prefix = 1;
    long long suffix = 1;
    int n = static_cast<int>(nums.size());
    
    for (int i = 0; i < n; ++i) {
        prefix *= nums[i];
        ans = std::max(ans, prefix);
        if (prefix == 0) prefix = 1;
        
        suffix *= nums[n - 1 - i];
        ans = std::max(ans, suffix);
        if (suffix == 0) suffix = 1;
    }
    
    return ans;
}
#include <cassert>
#include <vector>

// Forward declaration for testing (already defined above)
long long maxProductSubarray(const std::vector<int>& nums);

int main() {
    // Basic positive case
    assert(maxProductSubarray({2, 3, -2, 4}) == 6);
    
    // Negative and zero mixed
    assert(maxProductSubarray({-2, 0, -1}) == 0);
    
    // All negatives
    assert(maxProductSubarray({-1, -2, -3}) == 6); // -2 * -3 = 6, or -1 * -2 * -3 = -6
    
    // Single element negative
    assert(maxProductSubarray({-5}) == -5);
    
    // Single element zero
    assert(maxProductSubarray({0}) == 0);
    
    // All zeros
    assert(maxProductSubarray({0, 0, 0}) == 0);
    
    // Large product without overflow (within long long)
    assert(maxProductSubarray({100000, 100000, 100000}) == 1000000000000000LL);
    
    // Two negatives
    assert(maxProductSubarray({-2, -3}) == 6);
    
    // Mixed with zero in the middle
    assert(maxProductSubarray({-1, 2, 0, 3, 4}) == 12);
    
    // Standard maximum product subarray test
    assert(maxProductSubarray({6, -3, -10, 0, 2}) == 180); // 6 * -3 * -10 = 180
    
    return 0;
}
