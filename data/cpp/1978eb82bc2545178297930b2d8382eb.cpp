Write a C++ function `long long rangeAfterSort(const std::vector<long long>& values)` that takes a vector of integers (which may contain negative numbers, duplicates, and up to \(N \le 10^5\) elements), sorts it in non-decreasing order, and returns the difference between the largest and smallest elements (i.e., `max - min`). If the vector has fewer than two elements, the function should return 0 because the range is undefined (or trivially zero). The function must not modify the input vector; it should work on a copy or use a non-destructive method. The input vector will always contain at least one element.
#include <cassert>
#include <vector>

// Function under test (include the above solution here)
long long rangeAfterSort(const std::vector<long long>& values);

int main() {
    // Basic cases
    assert(rangeAfterSort({3, 1, 2}) == 2);           // 3-1
    assert(rangeAfterSort({5, 5, 5}) == 0);           // duplicates
    assert(rangeAfterSort({7}) == 0);                 // single element
    assert(rangeAfterSort({}) == 0);                  // empty (edge case)
    
    // Negative numbers
    assert(rangeAfterSort({-10, -5, -1}) == 9);       // -1 - (-10)
    assert(rangeAfterSort({-3, 3}) == 6);
    
    // Large values to check long long
    assert(rangeAfterSort({1000000000LL, -1000000000LL}) == 2000000000LL);
    
    // Unsorted input, duplicates, and mixed signs
    assert(rangeAfterSort({0, 5, -2, 5, 10, -2}) == 12); // 10 - (-2)
    
    // Random larger vector
    std::vector<long long> big = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    assert(rangeAfterSort(big) == 8); // 9-1
    
    return 0;
}
#include <vector>
#include <algorithm>
#include <cstddef>

// Returns the range (max - min) of the vector after sorting.
// If the vector has fewer than 2 elements, returns 0.
long long rangeAfterSort(const std::vector<long long>& values) {
    if (values.size() < 2) {
        return 0;
    }
    std::vector<long long> sorted = values; // copy to avoid modifying input
    std::sort(sorted.begin(), sorted.end());
    return sorted.back() - sorted.front();
}
// The task reduces to finding the minimum and maximum of the vector, because after sorting, the smallest element is at index 0 and the largest at index N-1. A naive sort gives O(N log N) time and O(N) space if we copy, but we can also scan to find min and max in O(N) time and O(1) auxiliary space, which handles edge cases with fewer than 2 elements by returning 0. However, the original snippet sorts, so to stay faithful we can copy the vector, sort the copy, and compute `copy.back() - copy.front()`. Note that `long long` is used because the difference can overflow `int`. Edge cases: empty vector (though problem says non-empty, we can return 0 safely), single element (returns 0), duplicates (min and max may be equal, result 0), all negative numbers (difference positive). Time complexity O(N log N) due to sorting; if we instead use min/max scan, O(N) time and O(1) extra space. Since the task specification does not forbid sorting, we can implement either, but the simplest is to sort a copy.
