/*
Write a C++ function `long long maxProductSubarray(const std::vector<int>& arr)` that returns the maximum product that can be obtained from any contiguous subarray (i.e., a non‑empty slice) of the given array of integers. The input array may contain positive numbers, negative numbers, and zeros. The result may be very large, so use a `long long` return type. The function must handle arrays of length 1, arrays with all negative numbers, arrays with zeros, and arrays where the maximum product is achieved by a subarray not including both endpoints. The solution must run in linear time and use constant extra space.
*/

#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the maximum product of any contiguous subarray of `arr`.
long long maxProductSubarray(const std::vector<int>& arr) {
    if (arr.empty()) return 0; // Not expected per spec, but safe.
    
    long long maxProd = arr[0];
    long long minProd = arr[0];
    long long result = arr[0];
    
    for (size_t i = 1; i < arr.size(); ++i) {
        long long x = arr[i];
        
        // If x is negative, swapping max and min is equivalent to considering products.
        if (x < 0) std::swap(maxProd, minProd);
        
        maxProd = std::max(x, maxProd * x);
        minProd = std::min(x, minProd * x);
        
        result = std::max(result, maxProd);
    }
    
    return result;
}

#include <cassert>
#include <vector>

// The solution function is defined above. Here we test it.
int main() {
    // Basic positive and negative cases
    assert(maxProductSubarray({2, 3, -2, 4}) == 6);
    assert(maxProductSubarray({-2, 0, -1}) == 0);
    
    // All negative numbers: maximum product is the single largest (closest to zero)
    assert(maxProductSubarray({-2, -3, -1}) == -1);
    
    // Single element
    assert(maxProductSubarray({-5}) == -5);
    assert(maxProductSubarray({7}) == 7);
    
    // Zeros break the product, but subarray without zero can be large
    assert(maxProductSubarray({0, 1, 2, 0, 3}) == 3);
    
    // Large product test
    assert(maxProductSubarray({-1, -2, -3, -4}) == 24);
    
    // Mixed with zeros and negatives
    assert(maxProductSubarray({1, 0, -1, 5, -2}) == 10);
    
    // Classic case from LeetCode
    assert(maxProductSubarray({2, -5, -2, -4, 3}) == 24);
    
    return 0;
}

// The key insight is that when scanning left‑to‑right, the maximum product ending at each position can be computed from the previous maximum or minimum product (since a negative number can flip a large negative into a large positive). Maintain two running values: `maxProd` (maximum product of a subarray ending at the current index) and `minProd` (minimum product ending at the current index). For each new element `x`, the new candidates are: `x` itself (fresh start), `maxProd * x`, and `minProd * x`. Update `maxProd` and `minProd` accordingly, and track the global maximum answer. This is essentially a Kadane‑style approach adapted for multiplication. Edge cases: if the element is zero, both `maxProd` and `minProd` reset to zero (or effectively to 1 for the next iteration, but the algorithm naturally handles it if we include `x` as a candidate). Arrays of length 1: the answer is simply that element. Arrays with only negative numbers: the maximum product is the negative number closest to zero (e.g., `-1` in `[-2,-3,-1]` gives `-1`). Time complexity is `O(n)`, space complexity is `O(1)`.
