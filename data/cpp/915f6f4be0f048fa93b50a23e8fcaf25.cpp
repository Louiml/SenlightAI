Write a C++ function `int maxZeroSumSubarrayLength(const std::vector<int>& nums)` that returns the length of the longest contiguous subarray whose elements sum to zero. If no such subarray exists, return 0. The input vector may contain negative integers, zeros, and duplicates, and may be empty. The function must operate in linear time and use only a constant amount of extra space beyond an auxiliary hash map.
The solution uses a prefix-sum technique. Maintain a running sum as you iterate through the array. If the same prefix sum appears at two different indices `i` and `j` (with `i < j`), then the elements between `i+1` and `j` sum to zero. To compute the length efficiently, store the **first** occurrence index of each prefix sum in an unordered map (with the initial sum 0 mapped to index -1, representing the position before the first element). For each element, add it to the running sum. If the current sum has been seen before, compute the subarray length as `current_index - first_occurrence_index` and update the maximum. If not seen, record the current index as the first occurrence. Since we only need the earliest occurrence to maximize length, we never overwrite an existing entry. Edge cases include an empty array (return 0) and an array with a single zero (prefix sum 0 appears at index 0 and -1, giving length 1). The algorithm runs in O(n) time because each element is processed once, and O(n) auxiliary space for the hash map in the worst case.
#include <vector>
#include <unordered_map>
#include <algorithm>

// Return the length of the longest contiguous subarray with sum zero.
// Uses a prefix sum map storing the first occurrence of each sum.
int maxZeroSumSubarrayLength(const std::vector<int>& nums) {
    int sum = 0;
    int maxLength = 0;
    std::unordered_map<int, int> firstOccurrence; // prefix sum -> first index
    
    // Initialize to handle subarrays starting from index 0
    firstOccurrence[0] = -1;
    
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        sum += nums[i];
        
        if (firstOccurrence.find(sum) != firstOccurrence.end()) {
            // Found same prefix sum, subarray between first occurrence +1 and i sums to zero
            int length = i - firstOccurrence[sum];
            maxLength = std::max(maxLength, length);
        } else {
            // Store the first occurrence (do not overwrite existing entries)
            firstOccurrence[sum] = i;
        }
    }
    
    return maxLength;
}
#include <cassert>
#include <vector>

int maxZeroSumSubarrayLength(const std::vector<int>& nums);

int main() {
    // Basic cases
    assert(maxZeroSumSubarrayLength({}) == 0);
    assert(maxZeroSumSubarrayLength({0}) == 1);
    assert(maxZeroSumSubarrayLength({1, -1}) == 2);
    assert(maxZeroSumSubarrayLength({1, 2, -3, 1}) == 3); // [1,2,-3]
    assert(maxZeroSumSubarrayLength({1, 2, 3}) == 0);
    
    // Multiple zero-sum subarrays, longest should win
    assert(maxZeroSumSubarrayLength({1, -1, 2, -2, 3, -3, 4}) == 6); // entire array except last 4
    assert(maxZeroSumSubarrayLength({0, 0, 0}) == 3);
    assert(maxZeroSumSubarrayLength({5, -5, 0, 3, -3}) == 2); // first two or last two, length 2
    
    // Negative numbers and duplicates
    assert(maxZeroSumSubarrayLength({-2, 1, 1, 1}) == 2); // [1,1]? actually 1+1=2, not zero; check: -2+1+1=0 → indices 0..2 length 3
    assert(maxZeroSumSubarrayLength({-2, 1, 1}) == 3); // -2+1+1=0
    assert(maxZeroSumSubarrayLength({10, -10, 10, -10}) == 4); // whole array
    assert(maxZeroSumSubarrayLength({3, 0, -3}) == 3); // whole array
    assert(maxZeroSumSubarrayLength({1, 2, -2, -1}) == 4); // whole array
    
    // Longest at the end
    assert(maxZeroSumSubarrayLength({4, 1, -1, 2, -2}) == 4); // from index 1 to 4
    
    return 0;
}
