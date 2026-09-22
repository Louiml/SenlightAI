// Write a C++ function that, given a sorted vector of integers in non-decreasing order and a target integer, returns the index of the target if it exists in the vector, otherwise returns -1. The function must operate on a `const std::vector<int>&` to avoid modifying the input and must use a binary search algorithm to achieve logarithmic time complexity. The vector may contain duplicate values, negative numbers, and may be empty. The function should handle all edge cases, including a single-element vector, target smaller than the first element, target larger than the last element, and target not present.
#include <cassert>
#include <vector>

// Function declaration from the solution above
int binarySearch(const std::vector<int>& nums, int target);

int main() {
    std::vector<int> nums1 = {1, 2, 3, 4, 5};
    assert(binarySearch(nums1, 3) == 2);
    assert(binarySearch(nums1, 6) == -1);
    assert(binarySearch(nums1, 0) == -1);

    std::vector<int> nums2 = {-10, -3, 0, 7, 9};
    assert(binarySearch(nums2, -3) == 1);
    assert(binarySearch(nums2, 9) == 4);
    assert(binarySearch(nums2, 5) == -1);

    std::vector<int> nums3 = {2};
    assert(binarySearch(nums3, 2) == 0);
    assert(binarySearch(nums3, 1) == -1);
    assert(binarySearch(nums3, 3) == -1);

    std::vector<int> nums4 = {};
    assert(binarySearch(nums4, 1) == -1);

    std::vector<int> nums5 = {1, 2, 2, 2, 3};
    int idx = binarySearch(nums5, 2);
    assert(idx >= 1 && idx <= 3); // any duplicate index is valid
    assert(binarySearch(nums5, 4) == -1);
}
#include <vector>

// Returns the index of target in sorted nums, or -1 if not found.
int binarySearch(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}
// The solution uses classic binary search on a sorted array. Initialize `left = 0` and `right = size - 1`. While `left <= right`, compute `mid` as `left + (right - left) / 2` to avoid integer overflow. Compare `nums[mid]` with the target: if equal, return `mid`; if `nums[mid] < target`, discard the left half by setting `left = mid + 1`; otherwise discard the right half by setting `right = mid - 1`. If the loop exits without finding the target, return -1. Edge cases: an empty vector immediately returns -1 because `right = -1` and the loop condition fails; a target not present results in the loop narrowing to a single element then exiting; duplicates don’t affect correctness because any matching index is acceptable. Time complexity is O(log n) and space complexity is O(1) auxiliary.
