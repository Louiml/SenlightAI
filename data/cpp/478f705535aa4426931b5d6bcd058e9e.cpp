Write a C++ function that takes an integer array and its size as parameters, along with a non-negative rotation count `d`. The function should return a `std::vector<int>` containing the array elements after rotating them to the left by `d` positions. A left rotation shifts each element `d` positions to the left, with elements that wrap around the beginning appearing at the end. The original array must not be modified, and if `d` is greater than the array size, it should be treated modulo the size. Handle empty arrays gracefully by returning an empty vector.
#include <cassert>
#include <vector>
#include <cstddef>

// Declaration of the function under test (must match the solution)
std::vector<int> rotateLeft(const int arr[], size_t size, size_t d);

int main() {
    // Example from the prompt: {1,2,3,4,5,6,7}, d=2 => {3,4,5,6,7,1,2}
    int arr1[] = {1,2,3,4,5,6,7};
    std::vector<int> res1 = rotateLeft(arr1, 7, 2);
    assert(res1 == std::vector<int>({3,4,5,6,7,1,2}));

    // d = 0 returns original order
    int arr2[] = {10,20,30};
    std::vector<int> res2 = rotateLeft(arr2, 3, 0);
    assert(res2 == std::vector<int>({10,20,30}));

    // d > size (modulo behavior)
    int arr3[] = {5,6,7};
    std::vector<int> res3 = rotateLeft(arr3, 3, 5); // 5 % 3 = 2
    assert(res3 == std::vector<int>({7,5,6}));

    // Single element
    int arr4[] = {42};
    std::vector<int> res4 = rotateLeft(arr4, 1, 100);
    assert(res4 == std::vector<int>({42}));

    // Empty array
    int* arr5 = nullptr;
    std::vector<int> res5 = rotateLeft(arr5, 0, 3);
    assert(res5.empty());

    // d = size (no change)
    int arr6[] = {1,2,3,4};
    std::vector<int> res6 = rotateLeft(arr6, 4, 4);
    assert(res6 == std::vector<int>({1,2,3,4}));

    // Larger rotation with wrap (d = 1)
    int arr7[] = {-1,0,1};
    std::vector<int> res7 = rotateLeft(arr7, 3, 1);
    assert(res7 == std::vector<int>({0,1,-1}));

    return 0;
}
#include <vector>
#include <cstddef>

// Rotate a given array to the left by 'd' positions.
// Returns a new vector containing the rotated elements; the original array is unchanged.
// 'd' is treated modulo the array size, so large or zero values are handled naturally.
// If the array is empty, an empty vector is returned.
std::vector<int> rotateLeft(const int arr[], size_t size, size_t d) {
    if (size == 0) {
        return {};
    }

    // Normalize rotation count modulo size (handles d >= size; d=0 gives no rotation)
    d = d % size;

    std::vector<int> result(size);

    for (size_t i = 0; i < size; ++i) {
        // New index after left rotation: (i - d + size) % size
        result[i] = arr[(i + d) % size];
    }

    return result;
}
Note: The above code uses `(i + d) % size` which is equivalent to `(i - (size - d)) % size`; it computes the original index for the new position `i`. This is a standard way to implement left rotation by picking the element at distance `d` ahead.
// The core algorithm is to simulate a left rotation. For each element at index `i` in the original array, its new index in the rotated result is `(i - d + n) % n`, where `n` is the size. This formula correctly handles wrap-around and also works for `d` ≥ `n` by using modular arithmetic. Alternatively, we could rotate in three steps (reverse parts), but the direct index mapping is simpler and more readable. Key edge cases: empty array (size 0) returns empty vector; `d = 0` returns a copy of the original; `d` being a multiple of array size returns the original order; negative `d` is not allowed per specification, but we can handle it by converting to positive modulo if desired (though not required). Time complexity is O(n) because we iterate over all elements once and perform constant-time operations per element. Space complexity is O(n) for the returned vector, ignoring the input array itself.
