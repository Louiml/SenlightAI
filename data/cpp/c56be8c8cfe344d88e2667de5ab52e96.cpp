/*
Write a C++ function `findSmallestMissing` that takes a sorted array of distinct non-negative integers (with no duplicates) and its size, and returns the smallest non-negative integer that is not present in the array. The array is guaranteed to be sorted in ascending order, and the integers are non-negative and distinct. The function should handle the case where no element is missing within the range `[0, n-1]` (i.e., the array is exactly `{0,1,2,...,n-1}`) by returning `n`. The function should not modify the input array and must work for arrays of any valid size (including size 0, in which case the smallest missing is `0`). Implement the function with proper `const` correctness and test it with several cases including empty arrays, arrays with missing elements at the beginning, middle, end, and arrays with no missing elements.
*/
#include <vector>
#include <cstddef>

// Returns the smallest non-negative integer not present in the sorted array.
// The array contains distinct non-negative integers in ascending order.
int findSmallestMissing(const int arr[], std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
        if (arr[i] != static_cast<int>(i)) {
            return static_cast<int>(i);
        }
    }
    return static_cast<int>(n);
}
#include <cassert>
#include <cstddef>

// Function under test (declared here for the test)
int findSmallestMissing(const int arr[], std::size_t n);

int main() {
    // Empty array
    int empty[] = {};
    assert(findSmallestMissing(empty, 0) == 0);

    // Missing at the beginning
    int start[] = {5, 6, 7};
    assert(findSmallestMissing(start, 3) == 0);

    // Missing in the middle
    int mid[] = {0, 1, 3, 4};
    assert(findSmallestMissing(mid, 4) == 2);

    // Missing at the end
    int end[] = {0, 1, 2};
    assert(findSmallestMissing(end, 3) == 3);

    // No missing (perfect sequence)
    int perfect[] = {0, 1, 2, 3, 4};
    assert(findSmallestMissing(perfect, 5) == 5);

    // Larger case with multiple gaps
    int gaps[] = {0, 2, 3, 5, 6};
    assert(findSmallestMissing(gaps, 5) == 1);

    // Single element missing at position 0
    int single[] = {1};
    assert(findSmallestMissing(single, 1) == 0);

    // Single element perfect
    int singlePerfect[] = {0};
    assert(findSmallestMissing(singlePerfect, 1) == 1);

    return 0;
}
// The problem is a classic "find smallest missing positive" variant restricted to non-negative integers in a sorted array. Since the array is sorted and contains distinct non-negative integers, we can compare each element with its index. The smallest missing integer is the first index `i` where `arr[i] != i`. If no such mismatch occurs, then all elements `0` through `n-1` are present, so the missing integer is `n`. For an empty array (`n == 0`), the loop does not run, the mismatch flag stays false, and we return `0`, which is correct. The algorithm runs in O(n) time because it scans the array once, and uses O(1) auxiliary space since only a loop variable and a return value are needed. Edge cases include: empty array, array starting with a number > 0 (e.g., `{5,6,7}` → missing `0`), missing in the middle (e.g., `{0,1,3}` → missing `2`), missing at the end (e.g., `{0,1,2}` → missing `3`), and the case where all numbers are present (e.g., `{0,1,2,3}` → missing `4`). The solution is straightforward and robust.
