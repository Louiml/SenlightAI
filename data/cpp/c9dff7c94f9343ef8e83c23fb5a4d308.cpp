// Write a C++ function `findTwoSumIndices` that takes a non-empty vector of integers `nums` and an integer `target`, and returns a vector of two integers representing the **1-based indices** (as in the original snippet) of the two numbers that sum to `target`. It is guaranteed that exactly one such pair exists. The function must work even if the vector contains duplicate values, and the two indices must correspond to two distinct elements (i.e., you cannot use the same element twice). The order of the returned indices does not matter, but the smaller index must appear first. The solution should handle negative numbers and zero. The function should not modify the original input vector (i.e., it must work on a copy or use `const` reference). Return the indices as a `std::vector<int>` of size 2.
// The core requirement is to find two distinct indices whose values sum to `target`. A direct brute-force double loop would be \(O(n^2)\), which is acceptable for small inputs, but the task requires a more efficient approach reminiscent of two-pointer search on a sorted array. The algorithm works as follows:  
// 1. Make a sorted copy of the input array.  
// 2. Use a two-pointer technique on the sorted array: place a left pointer at index 0 and a right pointer at index `size-1`. While `left < right`, compute the sum of the two values. If the sum equals `target`, record the two values. If the sum is less than `target`, increment `left`; if greater, decrement `right`. This finds the pair in \(O(n \log n)\) due to sorting, plus \(O(n)\) for the scan.  
// 3. After finding the two values, map them back to original indices using `std::find` on the original vector. Since duplicate values exist, we must handle the case where both values are equal: after finding the first index via `find`, search for the second value starting from `firstIndex + 1` to ensure distinct indices. If the values are distinct, we can simply find each value's index independently. Finally, sort the two indices so the smaller comes first, and return them as 1-based (add 1 to each).  
// Edge cases:  
// - Duplicate values (e.g., `[3, 3]` with target 6) require careful second search.  
// - Negative numbers work naturally.  
// - The input size is at least 2, and exactly one pair exists.  
// Time complexity: \(O(n \log n)\) for sorting, plus \(O(n)\) for scanning and \(O(n)\) for finding indices (linear due to `find`). Space complexity: \(O(n)\) for the sorted copy and the result.
#include <vector>
#include <algorithm>
#include <stdexcept>

// Returns 1-based indices of two distinct numbers in nums that sum to target.
// Assumes exactly one valid pair exists.
std::vector<int> findTwoSumIndices(const std::vector<int>& nums, int target) {
    std::vector<int> sorted = nums;
    std::sort(sorted.begin(), sorted.end());

    int left = 0;
    int right = sorted.size() - 1;
    int value1 = 0, value2 = 0;
    bool found = false;

    while (left < right) {
        int sum = sorted[left] + sorted[right];
        if (sum == target) {
            value1 = sorted[left];
            value2 = sorted[right];
            found = true;
            break;
        } else if (sum < target) {
            ++left;
        } else {
            --right;
        }
    }

    if (!found) {
        throw std::invalid_argument("No valid pair exists in input");
    }

    // Find original indices in the original vector
    auto it1 = std::find(nums.begin(), nums.end(), value1);
    int index1 = static_cast<int>(it1 - nums.begin());

    auto it2 = std::find(nums.begin(), nums.end(), value2);
    int index2 = static_cast<int>(it2 - nums.begin());

    if (index1 == index2) {
        // Both values are the same (duplicate). Search for the second occurrence.
        it2 = std::find(nums.begin() + index1 + 1, nums.end(), value2);
        index2 = static_cast<int>(it2 - nums.begin());
    }

    if (index1 > index2) {
        std::swap(index1, index2);
    }

    // Return 1-based indices
    return {index1 + 1, index2 + 1};
}
#include <cassert>
#include <vector>

// The solution function is defined here (as above)

int main() {
    // Basic case
    std::vector<int> nums1 = {2, 7, 11, 15};
    std::vector<int> result1 = findTwoSumIndices(nums1, 9);
    assert(result1 == std::vector<int>({1, 2}));

    // Negative numbers
    std::vector<int> nums2 = {-3, 4, 3, 90};
    std::vector<int> result2 = findTwoSumIndices(nums2, 0);
    assert(result2 == std::vector<int>({1, 3}));

    // Duplicate values
    std::vector<int> nums3 = {3, 2, 4, 3};
    std::vector<int> result3 = findTwoSumIndices(nums3, 6);
    assert(result3 == std::vector<int>({1, 4})); // 3+3=6, indices 1 and 4 (1-based)

    // Two identical elements at start and end
    std::vector<int> nums4 = {1, 2, 3, 1};
    std::vector<int> result4 = findTwoSumIndices(nums4, 2);
    assert(result4 == std::vector<int>({1, 4}));

    // Order of returned indices is always ascending, even if found in reverse
    std::vector<int> nums5 = {10, 5, 2, 7, 8};
    std::vector<int> result5 = findTwoSumIndices(nums5, 15);
    assert(result5 == std::vector<int>({2, 5})); // 5+10=15, indices 2 and 5

    // Check original vector unchanged (not modified by function)
    assert(nums1 == std::vector<int>({2, 7, 11, 15}));
    assert(nums3 == std::vector<int>({3, 2, 4, 3}));

    // Edge case: exactly two elements
    std::vector<int> nums6 = {5, -1};
    std::vector<int> result6 = findTwoSumIndices(nums6, 4);
    assert(result6 == std::vector<int>({1, 2}));

    // All tests passed
    return 0;
}
