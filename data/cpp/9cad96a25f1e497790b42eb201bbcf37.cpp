/*
Write a C++ function named `countOccurrencesSorted` that takes a sorted array of integers (non-decreasing order) along with its size and a target integer, and returns the number of times that target appears in the array. The function must be efficient enough to handle large arrays by exploiting the sorted property — a simple linear scan is not acceptable. The input array is guaranteed to be sorted, but may contain negative numbers, duplicate values, and the target may or may not be present. The function should work correctly even if the target is absent (return 0) or if the array is empty. Ensure your implementation uses `const` appropriately for parameters that should not be modified.
*/
#include <vector>

// Count occurrences of target in a sorted array (non-decreasing order).
// Uses two binary searches for O(log n) time.
int countOccurrencesSorted(const std::vector<int>& arr, int target) {
    int n = static_cast<int>(arr.size());
    
    // Find first occurrence (lowest index where arr[i] == target)
    int low = 0, high = n - 1;
    int first = n;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] >= target) {
            first = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    
    // Check if target exists
    if (first >= n || arr[first] != target) {
        return 0;
    }
    
    // Find last occurrence (highest index where arr[i] == target)
    low = 0;
    high = n - 1;
    int last = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] <= target) {
            last = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    
    return last - first + 1;
}
#include <cassert>
#include <vector>

// Function declaration (provided by solution)
int countOccurrencesSorted(const std::vector<int>& arr, int target);

int main() {
    // Basic cases
    std::vector<int> arr1 = {1, 2, 2, 2, 3, 4};
    assert(countOccurrencesSorted(arr1, 2) == 3);
    
    // Target absent
    assert(countOccurrencesSorted(arr1, 5) == 0);
    
    // Single occurrence
    assert(countOccurrencesSorted(arr1, 4) == 1);
    
    // All elements same
    std::vector<int> arr2 = {7, 7, 7, 7};
    assert(countOccurrencesSorted(arr2, 7) == 4);
    
    // Empty array
    std::vector<int> arr3;
    assert(countOccurrencesSorted(arr3, 10) == 0);
    
    // Negative numbers and duplicates
    std::vector<int> arr4 = {-5, -5, -3, 0, 0, 0, 2};
    assert(countOccurrencesSorted(arr4, -5) == 2);
    assert(countOccurrencesSorted(arr4, 0) == 3);
    assert(countOccurrencesSorted(arr4, -1) == 0);
    
    // Target smaller than all and larger than all
    std::vector<int> arr5 = {10, 20, 30};
    assert(countOccurrencesSorted(arr5, 5) == 0);
    assert(countOccurrencesSorted(arr5, 40) == 0);
    
    // Large array with target at boundaries
    std::vector<int> arr6(100000, 5);
    assert(countOccurrencesSorted(arr6, 5) == 100000);
    
    return 0;
}
// The required approach is to use binary search to locate the first and last occurrence of the target. Since the array is sorted, all occurrences of the target will be contiguous. We can implement a helper function `findFirst` that performs binary search to find the lowest index where the target appears, and another `findLast` that finds the highest index. For `findFirst`, we standard binary search but when we encounter an element equal to the target, we move the high pointer to `mid - 1` to continue searching left; for `findLast`, we move the low pointer to `mid + 1` to continue right. After computing both indices, if the target is not present (first index is `n` or the value at that index is not the target), return 0; otherwise return `last - first + 1`. Edge cases: empty array (return 0), target smaller than all or larger than all (binary search naturally returns `n` for first or `-1` for last), and target present only once (first == last). Time complexity is O(log n) for each binary search, so O(log n) total, and O(1) auxiliary space.
