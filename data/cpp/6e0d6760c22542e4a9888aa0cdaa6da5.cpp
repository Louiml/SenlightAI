// Write a C++ function `minimumSwapsToGroupSmallValues` that takes a vector of integers `arr`, its size `n`, and a threshold `k`, and returns the minimum number of swaps needed to make all elements less than or equal to `k` contiguous in the array. A swap exchanges any two elements in the array. The function must handle edge cases where no such elements exist or all elements already satisfy the condition. The input vector may contain duplicates, and values can be negative or positive.
The key insight is that if there are `m` elements with value ≤ k, then the final contiguous block must have length `m`. We can slide a window of exactly `m` positions across the array. For each window, count how many elements inside it are > k (these must be swapped out). The number of swaps needed for that window equals that count, because we only need to replace those "bad" elements with "good" ones from outside the window. The answer is the minimum over all window positions of this count. If `m == 0`, no swaps are needed (return 0). If `m == n`, all elements are already good, so return 0. We initialize the first window by counting its bad elements, then slide the window: when moving right, add the new element's contribution and remove the old element's contribution. Time complexity is O(n) since each element is processed at most twice (once for initial count, once during sliding). Space complexity is O(1) beyond the input. Edge cases include duplicates, negative values, and when `m == n` (no swaps needed) or `m == 0` (return 0, because the empty block is trivially contiguous).
#include <vector>
#include <algorithm>

// Returns the minimum number of swaps to make all elements ≤ k contiguous.
int minimumSwapsToGroupSmallValues(const std::vector<int>& arr, int n, int k) {
    // Count elements ≤ k
    int goodCount = 0;
    for (int value : arr) {
        if (value <= k) ++goodCount;
    }
    
    // If no good elements or all are good, no swaps needed
    if (goodCount == 0 || goodCount == n) return 0;
    
    // Count "bad" elements (> k) in the first window of size goodCount
    int badInWindow = 0;
    for (int i = 0; i < goodCount; ++i) {
        if (arr[i] > k) ++badInWindow;
    }
    
    int minSwaps = badInWindow;
    
    // Slide the window
    for (int i = goodCount; i < n; ++i) {
        // Add new element entering the window
        if (arr[i] > k) ++badInWindow;
        // Remove old element leaving the window
        if (arr[i - goodCount] > k) --badInWindow;
        
        minSwaps = std::min(minSwaps, badInWindow);
    }
    
    return minSwaps;
}
#include <vector>
#include <cassert>

// Declaration for testing
int minimumSwapsToGroupSmallValues(const std::vector<int>& arr, int n, int k);

int main() {
    // Example from snippet
    std::vector<int> arr1 = {1, 1, 3, 3, 5, 7, 7};
    assert(minimumSwapsToGroupSmallValues(arr1, 7, 4) == 2);
    
    // All elements already ≤ k
    std::vector<int> arr2 = {2, 1, 3, 4};
    assert(minimumSwapsToGroupSmallValues(arr2, 4, 5) == 0);
    
    // No elements ≤ k
    std::vector<int> arr3 = {6, 7, 8};
    assert(minimumSwapsToGroupSmallValues(arr3, 3, 5) == 0);
    
    // Single good element in middle
    std::vector<int> arr4 = {5, 3, 5};
    assert(minimumSwapsToGroupSmallValues(arr4, 3, 3) == 0);
    
    // Two good elements not contiguous
    std::vector<int> arr5 = {2, 9, 2, 9, 2};
    assert(minimumSwapsToGroupSmallValues(arr5, 5, 3) == 1);
    
    // Negative values
    std::vector<int> arr6 = {-1, 0, -2, 3, 4};
    assert(minimumSwapsToGroupSmallValues(arr6, 5, 0) == 1);
    
    // Duplicates and all good
    std::vector<int> arr7 = {1, 1, 1};
    assert(minimumSwapsToGroupSmallValues(arr7, 3, 1) == 0);
    
    // Large array (no assert, just ensure it runs)
    std::vector<int> arr8(1000, 1);
    arr8[500] = 100;
    assert(minimumSwapsToGroupSmallValues(arr8, 1000, 1) == 0);
    
    return 0;
}
