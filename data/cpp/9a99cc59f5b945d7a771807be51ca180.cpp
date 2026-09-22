Write a C++ function `long long maxSubarraySumOfFixedSize(const std::vector<int>& arr, int k)` that, given a non-empty vector of integers and a positive size `k` not exceeding the vector length, returns the maximum sum of any contiguous subarray of exactly `k` elements. The function must work correctly for arrays containing negative numbers, zeros, and very large values near the limits of `int`, and must return the result as a `long long` to avoid overflow during summation. For example, for `arr = {1, 2, 3, 4, 5}` and `k = 2`, the maximum sum is `9` (from subarray `{4,5}`).
#include <cassert>
#include <vector>
#include <climits>

int main() {
    // Basic cases
    std::vector<int> arr1 = {1, 2, 3, 4, 5};
    assert(maxSubarraySumOfFixedSize(arr1, 2) == 9);
    
    // All negative numbers
    std::vector<int> arr2 = {-5, -1, -3, -2};
    assert(maxSubarraySumOfFixedSize(arr2, 3) == -6); // subarray {-1, -3, -2}
    
    // Mixed positive and negative
    std::vector<int> arr3 = {2, -1, 3, -4, 5};
    assert(maxSubarraySumOfFixedSize(arr3, 3) == 4); // subarrays: 2-1+3=4, -1+3-4=-2, 3-4+5=4 => max 4
    
    // k equals entire array length
    std::vector<int> arr4 = {7, 8, 9};
    assert(maxSubarraySumOfFixedSize(arr4, 3) == 24);
    
    // Single-element array and k=1
    std::vector<int> arr5 = {42};
    assert(maxSubarraySumOfFixedSize(arr5, 1) == 42);
    
    // Duplicates and zeros
    std::vector<int> arr6 = {0, 0, -1, 0, 0};
    assert(maxSubarraySumOfFixedSize(arr6, 2) == 0);
    
    // Large values to verify long long return type
    std::vector<int> arr7 = {100000, 200000, 300000, 400000};
    assert(maxSubarraySumOfFixedSize(arr7, 2) == 700000);
    
    // Large negatives to verify no overflow in sum
    std::vector<int> arr8 = {INT_MIN, 1, 2, 3};
    assert(maxSubarraySumOfFixedSize(arr8, 2) == static_cast<long long>(INT_MIN) + 1);
    
    // Alternating high and low values
    std::vector<int> arr9 = {10, -20, 30, -40, 50};
    assert(maxSubarraySumOfFixedSize(arr9, 2) == 10); // max is 10 from {30,-20}? Actually 10+(-20)=-10, -20+30=10, 30+(-40)=-10, -40+50=10 => max 10
    
    return 0;
}
#include <vector>
#include <algorithm>
#include <cstddef>

// Returns the maximum sum of any contiguous subarray of exactly k elements.
// Assumes arr is non-empty, k >= 1, and k <= arr.size().
long long maxSubarraySumOfFixedSize(const std::vector<int>& arr, int k) {
    const std::size_t n = arr.size();
    long long window_sum = 0;
    
    // Compute sum of the first k elements
    for (std::size_t i = 0; i < static_cast<std::size_t>(k); ++i) {
        window_sum += arr[i];
    }
    
    long long max_sum = window_sum;
    
    // Slide the window across the rest of the array
    for (std::size_t i = static_cast<std::size_t>(k); i < n; ++i) {
        window_sum -= arr[i - k];       // remove element leaving the window
        window_sum += arr[i];           // add new element entering the window
        max_sum = std::max(max_sum, window_sum);
    }
    
    return max_sum;
}
// The main algorithm is the sliding window technique. First, compute the sum of the first `k` elements and initialize the maximum with that sum. Then, for each subsequent position `i` from `k` to `n-1`, update the current window sum by subtracting the element that leaves the window (`arr[i-k]`) and adding the new element (`arr[i]`). After each update, compare the new sum with the current maximum and keep the larger one. Edge cases: if `k == n`, the algorithm only computes the sum of the whole array once and returns it; if all numbers are negative, the algorithm correctly returns the least negative sum because it always considers exactly `k` elements; if `k` is 0, the problem definition excludes it (we assume `k >= 1`). The time complexity is `O(n)` because we traverse the array once for the initial sum and once for the sliding window, and the space complexity is `O(1)` auxiliary, not counting the input vector.
