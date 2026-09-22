Write a C++ function that performs a recursive linear search on an integer array. The function should take an array, its size, and a key value to search for. It must return `true` if the key is found in the array, and `false` otherwise. The function must be implemented recursively, without using any loops, and must handle an empty array (size 0) gracefully by returning `false`. Test the function on arrays of varying lengths, including cases where the key is present at the first, middle, or last position, as well as cases where the key is absent, and an empty array.

// The solution recursively checks the first element of the array against the key. If the array size is 0, the base case returns `false`. If the first element matches the key, return `true`. Otherwise, recurse on the remaining sub-array by passing `arr+1` and `size-1`. This effectively reduces the problem size by one each recursion. Edge cases include: empty array (size == 0) → false; key at first position → immediately true; key missing → recurses until size becomes 0, then returns false. Time complexity is O(n) worst-case (must scan entire array), and O(1) auxiliary space for the recursion stack depth is O(n) in the worst case, but no extra data structures are used. The function is `const`-correct because it does not modify the array.

#include <cstddef>

// Recursively search for key in arr[0..size-1].
// Returns true if key is present, false otherwise.
bool recursiveLinearSearch(const int arr[], std::size_t size, int key) {
    // Base case: empty sub-array, key not found.
    if (size == 0) {
        return false;
    }
    // Check current first element.
    if (arr[0] == key) {
        return true;
    }
    // Recursively search the rest of the array.
    return recursiveLinearSearch(arr + 1, size - 1, key);
}

#include <cassert>

int main() {
    int arr1[] = {1, 2, 3, 4, 5, 6};
    std::size_t size1 = sizeof(arr1) / sizeof(arr1[0]);

    // Key present at various positions
    assert(recursiveLinearSearch(arr1, size1, 1) == true);   // first
    assert(recursiveLinearSearch(arr1, size1, 4) == true);   // middle
    assert(recursiveLinearSearch(arr1, size1, 6) == true);   // last

    // Key absent
    assert(recursiveLinearSearch(arr1, size1, 10) == false);

    // Empty array
    int arr2[] = {};
    assert(recursiveLinearSearch(arr2, 0, 5) == false);

    // Single-element array with and without key
    int arr3[] = {42};
    assert(recursiveLinearSearch(arr3, 1, 42) == true);
    assert(recursiveLinearSearch(arr3, 1, 0) == false);

    // Duplicate values
    int arr4[] = {7, 3, 7, 9};
    std::size_t size4 = sizeof(arr4) / sizeof(arr4[0]);
    assert(recursiveLinearSearch(arr4, size4, 7) == true);
    assert(recursiveLinearSearch(arr4, size4, 3) == true);
    assert(recursiveLinearSearch(arr4, size4, 9) == true);
    assert(recursiveLinearSearch(arr4, size4, 8) == false);

    // Negative numbers
    int arr5[] = {-5, -2, 0, 3};
    std::size_t size5 = sizeof(arr5) / sizeof(arr5[0]);
    assert(recursiveLinearSearch(arr5, size5, -2) == true);
    assert(recursiveLinearSearch(arr5, size5, 0) == true);
    assert(recursiveLinearSearch(arr5, size5, -10) == false);
}
