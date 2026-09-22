/*
Write a C++ function `longestIncreasingSubsequenceLength` that takes an array of integers and its size as input, and returns the length of the longest strictly increasing subsequence (LIS) in the array. The subsequence must preserve the original order of elements but may skip elements; strictly increasing means each element must be greater than the previous, with no equal values allowed. The array may contain up to 100,000 elements, and the function must handle edge cases such as an empty array (return 0), an array with all identical values (return 1), and an array that is already sorted in descending order (where LIS length is 1). The solution should be efficient for large inputs, avoiding exponential-time recursion.
*/
#include <vector>
#include <algorithm>

// Returns the length of the longest strictly increasing subsequence.
int longestIncreasingSubsequenceLength(const std::vector<int>& arr) {
    if (arr.empty()) {
        return 0;
    }
    
    std::vector<int> tails;
    tails.push_back(arr[0]);
    
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > tails.back()) {
            tails.push_back(arr[i]);
        } else {
            auto it = std::lower_bound(tails.begin(), tails.end(), arr[i]);
            *it = arr[i];
        }
    }
    
    return static_cast<int>(tails.size());
}
#include <cassert>

int main() {
    // Empty array
    std::vector<int> empty;
    assert(longestIncreasingSubsequenceLength(empty) == 0);
    
    // Single element
    std::vector<int> single = {5};
    assert(longestIncreasingSubsequenceLength(single) == 1);
    
    // All identical elements
    std::vector<int> identical = {3, 3, 3, 3};
    assert(longestIncreasingSubsequenceLength(identical) == 1);
    
    // Strictly increasing array
    std::vector<int> increasing = {1, 2, 3, 4, 5};
    assert(longestIncreasingSubsequenceLength(increasing) == 5);
    
    // Strictly decreasing array
    std::vector<int> decreasing = {5, 4, 3, 2, 1};
    assert(longestIncreasingSubsequenceLength(decreasing) == 1);
    
    // Mixed array with known LIS
    std::vector<int> mixed = {10, 22, 9, 33, 21, 50, 41, 60};
    assert(longestIncreasingSubsequenceLength(mixed) == 5); // 10, 22, 33, 50, 60
    
    // Array with negative numbers
    std::vector<int> negative = {-5, -1, -10, -2, 0};
    assert(longestIncreasingSubsequenceLength(negative) == 3); // -5, -1, 0 or -10, -2, 0
    
    // Duplicate values requiring strict inequality
    std::vector<int> duplicates = {2, 2, 3, 1, 4, 4, 5};
    assert(longestIncreasingSubsequenceLength(duplicates) == 4); // 1, 3, 4, 5 or 2, 3, 4, 5
    
    // Large but simple case
    std::vector<int> large;
    for (int i = 0; i < 1000; ++i) {
        large.push_back(i);
    }
    assert(longestIncreasingSubsequenceLength(large) == 1000);
    
    return 0;
}
// The most efficient approach for this problem uses a greedy algorithm with binary search, often called the "patience sorting" method. We maintain a vector `tails` where `tails[i]` stores the smallest possible tail value of any increasing subsequence of length `i+1` seen so far. As we iterate through each element `x` in the input array, we find the position `pos` where `x` should be inserted into `tails` to maintain sorted order. If `x` is greater than all current tails, we append it to `tails` (increasing the LIS length). Otherwise, we replace the element at position `pos` with `x`, which updates the smallest tail for a subsequence of that length without changing the overall LIS length. The `lower_bound` function performs binary search in O(log n) time, so the total time complexity is O(n log n) and auxiliary space is O(n). Edge cases: empty array returns 0; a single-element array returns 1; arrays with all equal elements will result in `tails` always being replaced at position 0, so the length remains 1. The algorithm correctly handles negative numbers and negative values in the array since it only compares values.
