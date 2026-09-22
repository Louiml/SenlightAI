// Write a C++ function that takes a non-empty array of integers and its length, and rotates the array one position to the left, shifting every element one index toward the front and moving the first element to the last position. The function must modify the array in-place and return nothing (void). The array length is guaranteed to be at least 1. The function should handle large arrays (up to 100,000 elements) and values that fit in a signed 64-bit integer. After the rotation, the resulting array should be such that for every valid index `i`, `arr[i]` equals the original `arr[(i + 1) % n]`.

#include <cassert>
#include <cstddef>

// Declaration of the tested function (assumed to be in the same translation unit).
void rotateLeftOne(long long arr[], std::size_t n);

int main() {
    // Test 1: Basic rotation of 5 elements.
    long long arr1[] = {1, 2, 3, 4, 5};
    rotateLeftOne(arr1, 5);
    long long expected1[] = {2, 3, 4, 5, 1};
    for (std::size_t i = 0; i < 5; ++i) assert(arr1[i] == expected1[i]);

    // Test 2: Single element array.
    long long arr2[] = {42};
    rotateLeftOne(arr2, 1);
    assert(arr2[0] == 42);

    // Test 3: Two elements.
    long long arr3[] = {7, 8};
    rotateLeftOne(arr3, 2);
    assert(arr3[0] == 8 && arr3[1] == 7);

    // Test 4: All identical elements.
    long long arr4[] = {3, 3, 3, 3};
    rotateLeftOne(arr4, 4);
    for (std::size_t i = 0; i < 4; ++i) assert(arr4[i] == 3);

    // Test 5: Negative and large values.
    long long arr5[] = {-5, -1000, 9999999999LL, 0};
    rotateLeftOne(arr5, 4);
    long long expected5[] = {-1000, 9999999999LL, 0, -5};
    for (std::size_t i = 0; i < 4; ++i) assert(arr5[i] == expected5[i]);

    // Test 6: Large array; check first, middle, and last positions.
    const std::size_t n = 100000;
    static long long arr6[n];
    for (std::size_t i = 0; i < n; ++i) arr6[i] = static_cast<long long>(i);
    rotateLeftOne(arr6, n);
    assert(arr6[0] == 1);
    assert(arr6[n - 2] == n - 1);
    assert(arr6[n - 1] == 0);

    return 0;
}

#include <cstddef>

// Rotate the array left by one position in-place.
// The first element moves to the end, and all other elements shift left by one.
// The array must be non-empty.
void rotateLeftOne(long long arr[], std::size_t n) {
    if (n <= 1) {
        return; // No change needed for empty or single-element arrays.
    }

    long long first = arr[0];
    for (std::size_t i = 0; i < n - 1; ++i) {
        arr[i] = arr[i + 1];
    }
    arr[n - 1] = first;
}

// The solution is a direct left rotation by one position. The main idea is to save the first element in a temporary variable, then shift all remaining elements one position to the left using a loop from index 0 to `n - 2`, assigning `arr[i] = arr[i + 1]`. After the loop, assign the saved first element to the last position `arr[n - 1]`. This is an in-place algorithm that does not require extra space beyond one temporary variable. Edge cases: when `n == 1`, the loop does not execute and the function simply assigns the first element back to itself, which is correct. When `n == 0` (not allowed by the task spec but handled defensively by returning immediately if desired), the function should avoid accessing `arr[0]`. Time complexity is `O(n)` because we iterate through `n - 1` elements once. Space complexity is `O(1)` since only a constant amount of auxiliary memory is used. The function must use `long long` or `int64_t` to handle large values, and the array size is at most 100,000, so indexing with `size_t` or `long long` is safe.
