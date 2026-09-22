/*
Write a C++ function `bool hasCloseDuplicate(const std::vector<int>& nums, int indexDiff, int valueDiff)` that, given an integer array `nums` and two non-negative integers `indexDiff` and `valueDiff`, determines whether there exists a pair of distinct indices `(i, j)` such that `abs(i - j) <= indexDiff` and `abs(nums[i] - nums[j]) <= valueDiff`. The function should return `true` if such a pair exists, and `false` otherwise. The array length is between 2 and 105, values are in the range [-10^9, 10^9], `indexDiff` is between 1 and the array length, and `valueDiff` is between 0 and 10^9. Handle potential integer overflow when computing differences.
*/
#include <set>
#include <vector>

// Checks if there exists a pair (i, j) with abs(i - j) <= indexDiff and abs(nums[i] - nums[j]) <= valueDiff.
bool hasCloseDuplicate(const std::vector<int>& nums, int indexDiff, int valueDiff) {
    std::set<long long> window;
    int n = static_cast<int>(nums.size());
    
    for (int i = 0; i < n; ++i) {
        // Remove the element that falls out of the window
        if (i > indexDiff) {
            window.erase(static_cast<long long>(nums[i - indexDiff - 1]));
        }
        
        // Convert to long long to avoid overflow when adding/subtracting valueDiff
        long long current = static_cast<long long>(nums[i]);
        long long lowerBound = current - static_cast<long long>(valueDiff);
        long long upperBound = current + static_cast<long long>(valueDiff);
        
        // Check if any existing element is within [lowerBound, upperBound]
        auto it = window.lower_bound(lowerBound);
        if (it != window.end() && *it <= upperBound) {
            return true;
        }
        
        window.insert(current);
    }
    
    return false;
}
#include <cassert>
#include <vector>

// Function declaration for testing (assuming the solution is in the same translation unit)
bool hasCloseDuplicate(const std::vector<int>& nums, int indexDiff, int valueDiff);

int main() {
    // Example from the problem statement
    std::vector<int> nums1 = {1, 2, 3, 1};
    assert(hasCloseDuplicate(nums1, 3, 0) == true);
    
    std::vector<int> nums2 = {1, 5, 9, 1, 5, 9};
    assert(hasCloseDuplicate(nums2, 2, 3) == false);
    
    // Simple cases
    std::vector<int> nums3 = {1, 10, 2, 5};
    assert(hasCloseDuplicate(nums3, 1, 3) == false);
    
    std::vector<int> nums4 = {1, 4, 2, 0};
    assert(hasCloseDuplicate(nums4, 2, 2) == true); // (1,3) difference 2
    
    // valueDiff = 0, requires exact duplicates within range
    std::vector<int> nums5 = {5, 5, 5};
    assert(hasCloseDuplicate(nums5, 1, 0) == true);
    
    std::vector<int> nums6 = {5, 1, 5};
    assert(hasCloseDuplicate(nums6, 1, 0) == false);
    
    // Large values to check overflow handling
    std::vector<int> nums7 = {1000000000, -1000000000};
    assert(hasCloseDuplicate(nums7, 1, 2000000000) == true);
    
    // Edge: indexDiff = 1, adjacent elements
    std::vector<int> nums8 = {3, 6, 9};
    assert(hasCloseDuplicate(nums8, 1, 3) == true); // (0,1)
    assert(hasCloseDuplicate(nums8, 1, 2) == false);
    
    // Edge: indexDiff equals array length, check all pairs
    std::vector<int> nums9 = {1, 7, 3};
    assert(hasCloseDuplicate(nums9, 3, 4) == true); // (0,2) diff 2
    assert(hasCloseDuplicate(nums9, 3, 1) == false);
    
    // Negative numbers and duplicates
    std::vector<int> nums10 = {-1, -1, -2};
    assert(hasCloseDuplicate(nums10, 2, 0) == true); // (0,1)
    
    return 0;
}
// The problem requires checking a sliding window of size `indexDiff` over the array. For each position `i`, we need to know if there is any previous element within the window (indices from `i - indexDiff` to `i-1`) whose value is within `valueDiff` of `nums[i]`. A brute-force O(n * indexDiff) approach is too slow for large inputs. Instead, we can maintain a sorted structure of the values currently in the window. The C++ standard library's `std::set` provides ordered storage with efficient insertion, deletion, and lower-bound lookup.
//
// For each index `i`, we first remove the element that is about to slide out of the window (if `i > indexDiff`). Then we use `set.lower_bound(nums[i] - valueDiff)` to find the smallest element in the set that is at least `nums[i] - valueDiff`. If this iterator is not `end()` and its value is less than or equal to `nums[i] + valueDiff`, then we have found a valid pair, because that element is within `valueDiff` of `nums[i]` and was inserted at an index within the allowed range. After the check, we insert the current value into the set. The algorithm runs in O(n log k) time where k is the window size (at most `indexDiff`), and O(k) space. Edge cases include `valueDiff = 0` (requires exact duplicates) and when `indexDiff` is large (the window extends to the beginning of the array). The use of `long long` for intermediate calculations avoids overflow from `nums[i] ± valueDiff` since values and diffs can be up to 10^9 and the sum can be up to 2*10^9, which exceeds 32-bit signed integer range (2,147,483,647). Using `long long` handles this safely.
