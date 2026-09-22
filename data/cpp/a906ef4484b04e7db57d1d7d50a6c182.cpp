Write a C++ function that takes an array of integers and its length as parameters, and returns a new dynamically allocated array containing only the nonzero elements from the input, preserving their original relative order. The returned array must be exactly the size of the count of nonzero elements, and the original array must not be modified. If the input is empty or contains no nonzero elements, return a pointer to a valid heap-allocated array of size 0 (i.e., `new int[0]`). The function should use `const` correctly for the input array, and the caller is responsible for deleting the returned array with `delete[]`.

// The algorithm first counts the nonzero elements in the input array by iterating through all elements once, incrementing a counter whenever the current element is not equal to 0. Then, allocate a new dynamic array of that exact size using `new int[nonzeroCount]`. Next, iterate through the original array again, copying each nonzero element into the next available position of the new array. This second pass ensures the relative order is preserved. Edge cases include an empty array (length 0) or all zeros – in both cases the count is 0, and `new int[0]` returns a valid non-null pointer (though it cannot be dereferenced). No extra memory is used beyond the output array and a few scalar variables. Time complexity is O(n) for two passes (counting and copying), and space complexity is O(k) where k is the number of nonzero elements, which is optimal for the output. The function should not modify the input; hence, the input pointer is declared as `const int*`.

#include <cstddef>

// Returns a new heap-allocated array containing only the nonzero elements of `arr`.
// The caller must delete the returned pointer with delete[].
// If `len` is 0 or all elements are zero, returns a valid array of size 0.
int* extractNonZero(const int* arr, std::size_t len) {
    // Count nonzero elements
    std::size_t count = 0;
    for (std::size_t i = 0; i < len; ++i) {
        if (arr[i] != 0) {
            ++count;
        }
    }

    // Allocate exactly as many elements as needed
    int* result = new int[count];

    // Fill result with nonzero values preserving order
    std::size_t pos = 0;
    for (std::size_t i = 0; i < len; ++i) {
        if (arr[i] != 0) {
            result[pos++] = arr[i];
        }
    }

    return result;
}

#include <cassert>
#include <cstddef>

// Include the solution function here

int main() {
    // Test 1: Mixed values
    int arr1[] = {2, 0, 4, 0, 6, 0, 8, 9};
    int* r1 = extractNonZero(arr1, 8);
    assert(r1[0] == 2 && r1[1] == 4 && r1[2] == 6 && r1[3] == 8 && r1[4] == 9);
    delete[] r1;

    // Test 2: No zeros
    int arr2[] = {1, 2, 3};
    int* r2 = extractNonZero(arr2, 3);
    assert(r2[0] == 1 && r2[1] == 2 && r2[2] == 3);
    delete[] r2;

    // Test 3: All zeros
    int arr3[] = {0, 0, 0};
    int* r3 = extractNonZero(arr3, 3);
    // Cannot dereference size-0 array; just check non-null (new int[0] is valid)
    assert(r3 != nullptr);
    delete[] r3;

    // Test 4: Empty array
    int* r4 = extractNonZero(nullptr, 0);
    assert(r4 != nullptr);
    delete[] r4;

    // Test 5: Single nonzero
    int arr5[] = {0, 7, 0};
    int* r5 = extractNonZero(arr5, 3);
    assert(r5[0] == 7);
    delete[] r5;

    // Test 6: All zeros after nonzero
    int arr6[] = {5, 0, 0, 0};
    int* r6 = extractNonZero(arr6, 4);
    assert(r6[0] == 5);
    delete[] r6;

    // Test 7: Negative and positive nonzero values
    int arr7[] = {-3, 0, 0, 5, 0, -1};
    int* r7 = extractNonZero(arr7, 6);
    assert(r7[0] == -3 && r7[1] == 5 && r7[2] == -1);
    delete[] r7;

    // Test 8: Large array with some zeros (basic sanity)
    int arr8[100] = {};
    arr8[0] = 4;
    arr8[99] = -2;
    int* r8 = extractNonZero(arr8, 100);
    assert(r8[0] == 4 && r8[1] == -2);
    delete[] r8;

    return 0;
}
