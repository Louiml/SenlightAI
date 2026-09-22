Write a C++ function that takes two parameters: an array of integers and its size. The function must compute and return the sum of all elements that appear strictly after the last zero in the array. If there is no zero in the array, the function must return 0. The array may contain negative numbers, zeros, and any size up to 100. The function should not modify the input array, and should handle edge cases such as a zero at the last position, multiple zeros, and an empty array (size 0, which returns 0). The function signature should be `int sumAfterLastZero(const int arr[], int size)`.

// The solution involves first scanning the array from left to right to find the index of the last zero. If no zero exists (i.e., the index remains -1), then return 0. Otherwise, initialize a result variable to 0, and iterate from `lastZeroIndex + 1` up to `size - 1`, adding each element to the sum. This algorithm runs in O(n) time with a single pass to find the zero and another pass (or combined in one loop if tracking the last zero dynamically) to sum the trailing elements; here we use two passes for clarity, giving O(n) time and O(1) auxiliary space. Edge cases include: (1) array with no zeros → return 0; (2) zero at the very last index → loop from `lastZeroIndex+1` to `size-1` is empty, so result remains 0; (3) multiple zeros → the last zero identified by updating the index whenever a zero is found; (4) empty array (size 0) → the loop over array doesn't execute, lastZero stays -1, return 0.

#include <cstddef> // for size_t (though we use int per spec)

// Sum all elements after the last zero in the array.
// If no zero exists, return 0.
int sumAfterLastZero(const int arr[], int size) {
    int lastZeroIndex = -1;
    for (int i = 0; i < size; ++i) {
        if (arr[i] == 0) {
            lastZeroIndex = i;
        }
    }
    
    if (lastZeroIndex == -1) {
        return 0;
    }
    
    int sum = 0;
    for (int i = lastZeroIndex + 1; i < size; ++i) {
        sum += arr[i];
    }
    return sum;
}

int main() {
    // No zero -> return 0
    int arr1[] = {1, 2, 3};
    assert(sumAfterLastZero(arr1, 3) == 0);

    // Single zero at start
    int arr2[] = {0, 5, 6, 7};
    assert(sumAfterLastZero(arr2, 4) == 18);

    // Zero at end -> sum of nothing = 0
    int arr3[] = {1, 2, 0};
    assert(sumAfterLastZero(arr3, 3) == 0);

    // Multiple zeros, last is in middle
    int arr4[] = {0, 1, 0, 2, 3};
    assert(sumAfterLastZero(arr4, 5) == 5);

    // Negative numbers after zero
    int arr5[] = {0, -2, -3, 4};
    assert(sumAfterLastZero(arr5, 4) == -1);

    // Empty array
    int arr6[] = {};
    assert(sumAfterLastZero(arr6, 0) == 0);

    // Single element zero
    int arr7[] = {0};
    assert(sumAfterLastZero(arr7, 1) == 0);

    // Single element non-zero
    int arr8[] = {42};
    assert(sumAfterLastZero(arr8, 1) == 0);

    // All zeros
    int arr9[] = {0, 0, 0};
    assert(sumAfterLastZero(arr9, 3) == 0);

    // Large values
    int arr10[] = {0, 100, -50, 25};
    assert(sumAfterLastZero(arr10, 4) == 75);
}
