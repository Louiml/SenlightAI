Write a C++ function named `isNonDecreasing` that takes an array of integers and its size as parameters, and returns `true` if the array is sorted in non-decreasing order (each element is less than or equal to the next), and `false` otherwise. The function must handle arrays of size 0 or 1 correctly (trivially sorted), and must not modify the input array. After implementing the function, you will test it with various cases including empty arrays, single elements, strictly increasing, non-decreasing with duplicates, and unsorted arrays.

// The solution iterates through the array from index 0 to `size-2`, comparing each element with its immediate successor. If any element is strictly greater than the next, the array is not non-decreasing, so return `false` immediately. If the loop completes without finding such a violation, return `true`. Edge cases: for size 0 or 1, the loop condition `i < size - 1` is never satisfied, so the function directly returns `true` — this is correct because an empty or single-element array is trivially non-decreasing. The main algorithm runs in O(n) time, where n is the array size, and uses O(1) auxiliary space since only a loop counter is needed. The function should accept a `const int arr[]` parameter to guarantee the input array is not modified, and `size` as an `int` (though for larger arrays, `size_t` would be safer, the task uses `int` to match typical beginner exercises).

#include <cstddef>

// Returns true if the array is sorted in non-decreasing order.
// An array of size 0 or 1 is trivially non-decreasing.
bool isNonDecreasing(const int arr[], int size) {
    for (int i = 0; i < size - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

#include <cassert>

int main() {
    // Empty array (size 0) – trivially true
    int empty[] = {};
    assert(isNonDecreasing(empty, 0) == true);

    // Single element – trivially true
    int single[] = {42};
    assert(isNonDecreasing(single, 1) == true);

    // Strictly increasing
    int increasing[] = {1, 2, 3, 4, 5};
    assert(isNonDecreasing(increasing, 5) == true);

    // Non-decreasing with duplicates
    int duplicates[] = {1, 2, 2, 3, 3, 4};
    assert(isNonDecreasing(duplicates, 6) == true);

    // All equal
    int allEqual[] = {7, 7, 7};
    assert(isNonDecreasing(allEqual, 3) == true);

    // Unsorted – decreasing at the end
    int unsorted1[] = {1, 3, 2, 4};
    assert(isNonDecreasing(unsorted1, 4) == false);

    // Unsorted – decreasing at the beginning
    int unsorted2[] = {5, 4, 6};
    assert(isNonDecreasing(unsorted2, 3) == false);

    // Unsorted – mixed with duplicates and a violation
    int unsorted3[] = {1, 2, 2, 1, 3};
    assert(isNonDecreasing(unsorted3, 5) == false);

    // Large array with negative numbers – increasing
    int withNegatives[] = {-10, -5, -5, 0, 3};
    assert(isNonDecreasing(withNegatives, 5) == true);

    // Large array with negative and then a drop
    int withNegativesDrop[] = {-10, -5, -7, 0};
    assert(isNonDecreasing(withNegativesDrop, 4) == false);

    return 0;
}
