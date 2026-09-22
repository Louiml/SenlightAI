/*
Write a C++ function that takes a non-empty array of integers (passed as a pointer and a size) and returns a dynamically allocated array containing the same elements in reverse order. The function must not modify the original array, must be safe for any valid size ≥ 1, and must properly allocate the result with `new`. The caller is responsible for freeing the returned array. Do not include a `main` function; only the free function is required.
*/

#include <cstddef>  // for std::size_t

// Returns a newly allocated array that is the reverse of the input.
// The caller must free the returned array using delete[].
int* reverseArray(const int* original, std::size_t n) {
    int* reversed = new int[n];
    for (std::size_t i = 0; i < n; ++i) {
        reversed[i] = original[n - 1 - i];
    }
    return reversed;
}

#include <cassert>
#include <cstddef>

// Declaration of the function being tested.
int* reverseArray(const int* original, std::size_t n);

int main() {
    // Test 1: Standard reversal
    int a1[] = {1, 2, 3, 4};
    int* r1 = reverseArray(a1, 4);
    assert(r1[0] == 4 && r1[1] == 3 && r1[2] == 2 && r1[3] == 1);
    delete[] r1;

    // Test 2: Single element
    int a2[] = {42};
    int* r2 = reverseArray(a2, 1);
    assert(r2[0] == 42);
    delete[] r2;

    // Test 3: Negative numbers
    int a3[] = {-5, -10, -3};
    int* r3 = reverseArray(a3, 3);
    assert(r3[0] == -3 && r3[1] == -10 && r3[2] == -5);
    delete[] r3;

    // Test 4: Duplicate values
    int a4[] = {7, 7, 7};
    int* r4 = reverseArray(a4, 3);
    assert(r4[0] == 7 && r4[1] == 7 && r4[2] == 7);
    delete[] r4;

    // Test 5: Larger size, even count
    int a5[] = {0, 1, 2, 3, 4, 5};
    int* r5 = reverseArray(a5, 6);
    assert(r5[0] == 5 && r5[1] == 4 && r5[2] == 3);
    assert(r5[3] == 2 && r5[4] == 1 && r5[5] == 0);
    delete[] r5;

    // Test 6: Larger size, odd count
    int a6[] = {10, 20, 30, 40, 50};
    int* r6 = reverseArray(a6, 5);
    assert(r6[0] == 50 && r6[1] == 40 && r6[2] == 30);
    assert(r6[3] == 20 && r6[4] == 10);
    delete[] r6;

    // Test 7: All zeros
    int a7[] = {0, 0, 0, 0};
    int* r7 = reverseArray(a7, 4);
    assert(r7[0] == 0 && r7[1] == 0 && r7[2] == 0 && r7[3] == 0);
    delete[] r7;

    // Test 8: Two elements
    int a8[] = {100, -100};
    int* r8 = reverseArray(a8, 2);
    assert(r8[0] == -100 && r8[1] == 100);
    delete[] r8;

    // Test 9: Original unchanged
    int a9[] = {1, 2, 3};
    int* r9 = reverseArray(a9, 3);
    assert(a9[0] == 1 && a9[1] == 2 && a9[2] == 3);
    assert(r9[0] == 3 && r9[1] == 2 && r9[2] == 1);
    delete[] r9;

    // Test 10: Large size
    constexpr std::size_t N = 100;
    int big[N];
    for (std::size_t i = 0; i < N; ++i) big[i] = static_cast<int>(i);
    int* r10 = reverseArray(big, N);
    for (std::size_t i = 0; i < N; ++i) {
        assert(r10[i] == static_cast<int>(N - 1 - i));
    }
    delete[] r10;

    return 0;
}

// The solution creates a new array of the same size `n` using `new int[n]`. It then iterates `i` from 0 to `n-1`, assigning `result[i] = original[n - 1 - i]`. This directly mirrors the input into reverse order. Edge cases: size 1 works because `n-1-0 = 0`. Negative sizes are not possible for a valid array, so no check is needed (but the function can assume `n ≥ 1`). The original array is read-only and marked `const` for safety. Time complexity is O(n) because each element is copied once. Space complexity is O(n) for the new array, plus O(1) auxiliary (loop counter). Memory allocation with `new` is correct, and the caller must delete it with `delete[]` (not `delete`, per the original snippet's bug).
