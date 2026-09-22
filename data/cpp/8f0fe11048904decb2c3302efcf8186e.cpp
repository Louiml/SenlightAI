// Write a standalone C++ function that performs a linear search on an integer array for a given target value and returns the index of the first occurrence if found, or -1 if not found. The function must accept a vector of integers as the container, take the target as a second parameter, and be const-correct (i.e., it must not modify the input container). The function should handle empty arrays gracefully (returning -1) and work correctly even if the target appears multiple times (only the first index matters). Provide a free function with a descriptive name.
#include <cassert>
#include <vector>

// Declaration of the solution function (assumed to be defined above).
int findFirstIndex(const std::vector<int>& arr, int target);

int main() {
    // Basic search
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    assert(findFirstIndex(v1, 3) == 2);
    assert(findFirstIndex(v1, 5) == 4);

    // Target not present
    assert(findFirstIndex(v1, 9) == -1);

    // Multiple occurrences — first index returned
    std::vector<int> v2 = {7, 8, 7, 7};
    assert(findFirstIndex(v2, 7) == 0);

    // Empty vector
    std::vector<int> v3;
    assert(findFirstIndex(v3, 10) == -1);

    // Single-element vector
    std::vector<int> v4 = {42};
    assert(findFirstIndex(v4, 42) == 0);
    assert(findFirstIndex(v4, 1) == -1);

    // Negative numbers and zero
    std::vector<int> v5 = {-3, -1, 0, 2};
    assert(findFirstIndex(v5, -1) == 1);
    assert(findFirstIndex(v5, 0) == 2);
    assert(findFirstIndex(v5, -3) == 0);

    return 0;
}
#include <vector>

// Perform a linear search for target in arr.
// Returns the index of the first occurrence, or -1 if not found.
int findFirstIndex(const std::vector<int>& arr, int target) {
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] == target) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
// The solution uses the standard linear search algorithm: iterate through each element of the vector from index 0 upward, comparing each element to the target. As soon as a match is found, return the current index immediately (this ensures the first occurrence is returned, not a later one). If the loop completes without a match, return -1. Edge cases: an empty vector returns -1 (since no elements exist to compare); a vector with one element either returns 0 (if that element equals target) or -1; duplicate targets return the smallest index. The algorithm runs in O(n) time in the worst case (when target is absent or appears at the end) and O(1) auxiliary space, since only a loop index is needed. No sorting or extra storage is required.
