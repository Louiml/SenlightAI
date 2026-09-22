/*
Write a C++ function `doubleEveryElementWithOffset(int* data, size_t N, size_t offset)` that processes an array of `N` integers, multiplying each element by 2 and then adding `offset` to the result. The function must be written without using any parallel programming constructs (no SYCL, threads, or OpenMP). It should modify the array in-place and be `const`-correct where applicable. The function must handle edge cases: an empty array (`N == 0`) should do nothing, and large arrays (up to 256 million elements) must work efficiently. The input array is assumed to be valid and non-null when `N > 0`. For example, if the array initially contains `[0, 1, 2, 3]` and `offset = 5`, after the call it should be `[5, 7, 9, 11]`.
*/

#include <cstddef>   // for size_t

// Multiply each element of the array by 2 and add the given offset.
// Modifies the array in-place. Does nothing if N == 0.
void doubleEveryElementWithOffset(int* data, size_t N, size_t offset) {
    if (data == nullptr || N == 0) {
        return;
    }
    for (size_t i = 0; i < N; ++i) {
        data[i] = data[i] * 2 + static_cast<int>(offset);
    }
}

#include <cassert>

int main() {
    // Test 1: Basic case with positive values
    int arr1[] = {0, 1, 2, 3};
    doubleEveryElementWithOffset(arr1, 4, 5);
    assert(arr1[0] == 5);
    assert(arr1[1] == 7);
    assert(arr1[2] == 9);
    assert(arr1[3] == 11);

    // Test 2: Empty array should not crash
    int* empty = nullptr;
    doubleEveryElementWithOffset(empty, 0, 10);
    // No assertion needed, just no crash

    // Test 3: Negative values and zero offset
    int arr2[] = {-4, -2, 0, 2};
    doubleEveryElementWithOffset(arr2, 4, 0);
    assert(arr2[0] == -8);
    assert(arr2[1] == -4);
    assert(arr2[2] == 0);
    assert(arr2[3] == 4);

    // Test 4: Single element
    int arr3[] = {7};
    doubleEveryElementWithOffset(arr3, 1, 3);
    assert(arr3[0] == 17);  // 7*2 + 3 = 17

    // Test 5: Large offset
    int arr4[] = {1, 100, 1000};
    doubleEveryElementWithOffset(arr4, 3, 1000000);
    assert(arr4[0] == 1000002);
    assert(arr4[1] == 1000200);
    assert(arr4[2] == 1002000);

    // Test 6: Repeated values
    int arr5[] = {5, 5, 5};
    doubleEveryElementWithOffset(arr5, 3, 1);
    assert(arr5[0] == 11);
    assert(arr5[1] == 11);
    assert(arr5[2] == 11);

    return 0;
}

// The solution is straightforward: iterate over the array with an index from 0 to N-1, and for each element compute `data[i] = data[i] * 2 + offset`. Since the operation is element-wise and independent, no special ordering is needed. Edge cases: if `N == 0`, the loop body never executes, so no special guard is required, but it's safe to add an early return for clarity. Also, given that the array elements can be large (up to 256 million), the multiplication by 2 could overflow `int` if the original values exceed ~1 billion; the problem does not specify bounds, so we assume the input values are chosen to avoid overflow (or we note this as a limitation). The time complexity is O(N) because each element is visited exactly once. The space complexity is O(1) because we modify the array in-place and only use a loop counter. No auxiliary data structures are needed.
