Given a non-empty vector of integers that may contain duplicate values and unsorted elements, write a C++ function named `findMinimumValue` that takes a `const std::vector<int>&` and returns the smallest integer in the vector. The function must not modify the input and should handle vectors of any valid size, including size 1. If the vector contains negative numbers, zeros, or large positive values, the function must still return the correct minimum. The solution must be implemented from scratch without using any standard library algorithms like `std::min_element` or sorting.

The solution approach is a straightforward linear scan of the vector. We initialize a variable `minimum` with the first element of the vector (since the vector is guaranteed non-empty), then iterate through the remaining elements, updating `minimum` whenever a smaller value is found. This ensures we correctly handle duplicates (they simply don't change the minimum if equal or greater) and unsorted data. Edge cases include vectors with only one element (the initial value is already the minimum) and vectors containing `INT_MIN` or `INT_MAX`. The algorithm runs in O(n) time because it performs exactly n-1 comparisons, and uses O(1) auxiliary space since we only store a single integer variable. No extra data structures are required.

#include <vector>
#include <climits>

// Returns the smallest integer in the given non-empty vector.
// The input vector is not modified.
int findMinimumValue(const std::vector<int>& nums) {
    // Initialize with the first element (vector is guaranteed non-empty)
    int minimum = nums[0];
    
    // Scan the rest of the vector for smaller values
    for (std::size_t i = 1; i < nums.size(); ++i) {
        if (nums[i] < minimum) {
            minimum = nums[i];
        }
    }
    
    return minimum;
}

#include <cassert>
#include <vector>
#include <climits>

// Include the function declaration (in practice, this would be in the same file)
int findMinimumValue(const std::vector<int>& nums);

int main() {
    // Basic unsorted vector with positive numbers
    std::vector<int> v1 = {3, 7, 2, 9, 4};
    assert(findMinimumValue(v1) == 2);
    
    // Vector with negative numbers
    std::vector<int> v2 = {-5, -1, -10, -3};
    assert(findMinimumValue(v2) == -10);
    
    // Single-element vector
    std::vector<int> v3 = {42};
    assert(findMinimumValue(v3) == 42);
    
    // All duplicates
    std::vector<int> v4 = {7, 7, 7, 7};
    assert(findMinimumValue(v4) == 7);
    
    // Mixed values including zero
    std::vector<int> v5 = {10, -2, 0, 5, -2, 8};
    assert(findMinimumValue(v5) == -2);
    
    // Vector with extreme values
    std::vector<int> v6 = {INT_MAX, INT_MIN, 0, 100};
    assert(findMinimumValue(v6) == INT_MIN);
    
    // Vector in descending order
    std::vector<int> v7 = {9, 8, 7, 6, 5};
    assert(findMinimumValue(v7) == 5);
    
    // Vector in ascending order
    std::vector<int> v8 = {1, 2, 3, 4, 5};
    assert(findMinimumValue(v8) == 1);
    
    return 0;
}
