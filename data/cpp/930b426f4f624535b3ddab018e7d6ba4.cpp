/*
Write a C++ function `int secondSmallestAfterSort(std::vector<int> nums)` that takes a non-empty vector of integers, sorts it in non-decreasing order, and returns the first element that is strictly greater than the smallest element. If no such element exists (i.e., all elements are equal), return -1. The function must not modify the input vector (use a copy), and it must handle vectors with negative numbers and duplicates. You may assume the vector contains at least one integer.
*/

#include <vector>
#include <algorithm>

// Return the first element strictly greater than the smallest after sorting.
// If none exists, return -1.
int secondSmallestAfterSort(std::vector<int> nums) {
    std::sort(nums.begin(), nums.end());
    
    if (nums.empty()) {
        return -1; // Not expected per spec but safe.
    }
    
    int smallest = nums[0];
    for (std::size_t i = 1; i < nums.size(); ++i) {
        if (nums[i] > smallest) {
            return nums[i];
        }
    }
    return -1;
}

#include <cassert>
#include <vector>

int secondSmallestAfterSort(std::vector<int> nums); // Declaration for test

int main() {
    assert(secondSmallestAfterSort({5, 2, 8, 2}) == 5);
    assert(secondSmallestAfterSort({1, 1, 1, 1}) == -1);
    assert(secondSmallestAfterSort({7}) == -1);
    assert(secondSmallestAfterSort({-3, -1, -3, 0}) == -1);
    assert(secondSmallestAfterSort({10, 9, 9, 8}) == 9);
    assert(secondSmallestAfterSort({0, -5, 12, -5}) == 0);
    assert(secondSmallestAfterSort({3, 3, 2, 2}) == 3);
    assert(secondSmallestAfterSort({-10, -10, -9}) == -9);
    assert(secondSmallestAfterSort({42}) == -1);
    assert(secondSmallestAfterSort({100, 100, 101, 100}) == 101);
    return 0;
}

// The task is straightforward: copy the input vector to avoid mutating the original, sort the copy in ascending order using `std::sort`. The smallest element is now at index 0. Then iterate from index 1 onward to find the first element `a[i]` such that `a[i] > a[0]`. If found, return that value immediately; otherwise, after the loop, return -1. Edge cases: (1) If the vector has only one element, the loop does not execute, and we correctly return -1. (2) If all elements are equal, the loop never finds a strictly greater value, so we return -1. (3) Negative numbers and duplicates do not require special handling because sorting and the strict comparison handle them. Time complexity is O(n log n) due to sorting, and space complexity is O(n) for the copy. The function is `const`-correct by taking the vector by value (copy) and not modifying it further.
