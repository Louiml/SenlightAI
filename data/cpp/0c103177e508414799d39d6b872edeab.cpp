Write a C++ function that takes a sorted vector of integers (which may contain duplicates) and a target value, and returns a vector of two integers: the first element is the starting index of the target's first occurrence, and the second is the ending index of its last occurrence. If the target is not present in the array, return `{-1, -1}`. The function must run in `O(log n)` time using binary search. You may assume the input vector is sorted in non-decreasing order and is non-empty (but the target may be outside the range of values). The solution must demonstrate correct handling of duplicate values and boundary conditions.
#include <cassert>
#include <vector>

int main() {
    // Basic case with duplicates
    std::vector<int> v1 = {1, 2, 3, 3, 3, 4, 5};
    assert(findFirstLastOccurrence(v1, 3) == std::vector<int>({2, 4}));

    // Single occurrence
    assert(findFirstLastOccurrence(v1, 2) == std::vector<int>({1, 1}));

    // Target not present
    assert(findFirstLastOccurrence(v1, 6) == std::vector<int>({-1, -1}));

    // All elements equal to target
    std::vector<int> v2 = {7, 7, 7, 7};
    assert(findFirstLastOccurrence(v2, 7) == std::vector<int>({0, 3}));

    // Target smaller than all elements
    assert(findFirstLastOccurrence(v2, 0) == std::vector<int>({-1, -1}));

    // Target larger than all elements
    assert(findFirstLastOccurrence(v2, 10) == std::vector<int>({-1, -1}));

    // Single element vector, target equal
    std::vector<int> v3 = {5};
    assert(findFirstLastOccurrence(v3, 5) == std::vector<int>({0, 0}));

    // Single element vector, target not equal
    assert(findFirstLastOccurrence(v3, 4) == std::vector<int>({-1, -1}));

    // Large range with target at boundaries
    std::vector<int> v4 = {1, 1, 2, 3, 4, 4, 4, 5};
    assert(findFirstLastOccurrence(v4, 1) == std::vector<int>({0, 1}));
    assert(findFirstLastOccurrence(v4, 5) == std::vector<int>({7, 7}));

    return 0;
}
#include <vector>

// Returns {first occurrence index, last occurrence index} of target in a sorted vector.
// If target is absent, returns {-1, -1}.
std::vector<int> findFirstLastOccurrence(const std::vector<int>& nums, int target) {
    int n = static_cast<int>(nums.size());
    int left = -1;
    int low = 0, high = n - 1;

    // Search for leftmost occurrence
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (nums[mid] == target) {
            left = mid;
            high = mid - 1;  // keep searching left
        } else if (nums[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    int right = -1;
    low = 0;
    high = n - 1;

    // Search for rightmost occurrence
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (nums[mid] == target) {
            right = mid;
            low = mid + 1;  // keep searching right
        } else if (nums[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return {left, right};
}
// The core idea is to perform two separate binary searches: one to find the leftmost (first) occurrence of the target, and another to find the rightmost (last) occurrence. For the leftmost search, when the middle element equals the target, we do not stop immediately; instead we store the middle index as a potential answer and continue searching in the left half (`r = mid - 1`) to see if an earlier occurrence exists. For the rightmost search, when the middle element equals the target, we store the index and continue searching in the right half (`l = mid + 1`) to find a later occurrence. If the target is absent, both searches return `-1`. This works even with duplicates because each binary search narrows down to the boundary. Edge cases include: target less than the smallest element (left search returns `-1`), target greater than the largest element (right search returns `-1`), all elements equal to target (leftmost returns index 0, rightmost returns last index), and a single element equal to target (both return that index). Time complexity is `O(log n)` per search, so `O(log n)` total. Space complexity is `O(1)` auxiliary, plus the output vector.
