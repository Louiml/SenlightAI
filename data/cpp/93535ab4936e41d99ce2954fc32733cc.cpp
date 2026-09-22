/*
Write a C++ function named `averageUsingPointer` that takes a `const double*` array and its size as parameters, and returns the arithmetic mean of the elements as a `double`. The function must access the array elements exclusively through pointer arithmetic (using `*(arr + i)` or `arr[i]`) and must not use any array indexing syntax beyond what is equivalent to pointer dereferencing. The function should handle an empty array by returning `0.0`, and it must be `const`-correct: the pointer parameter should be a pointer to `const double` so that the function does not modify the data. Ensure the function works for arrays of any positive size, including arrays with all identical values, negative values, or zero.
*/

#include <cstddef>

// Computes the arithmetic mean of a double array using pointer arithmetic.
// Returns 0.0 for an empty array (size == 0).
double averageUsingPointer(const double* arr, std::size_t size) {
    if (size == 0) {
        return 0.0;
    }
    double sum = 0.0;
    const double* ptr = arr; // pointer to first element
    for (std::size_t i = 0; i < size; ++i) {
        sum += *ptr; // dereference current element
        ++ptr;       // move to next element
    }
    return sum / static_cast<double>(size);
}

#include <cassert>
#include <cmath>

int main() {
    // Test 1: typical array
    double a1[5] = {1000.0, 2.0, 3.4, 17.0, 50.0};
    assert(std::abs(averageUsingPointer(a1, 5) - 214.48) < 1e-9); // (1000+2+3.4+17+50)/5 = 1072.4/5 = 214.48

    // Test 2: single element
    double a2[1] = {42.0};
    assert(averageUsingPointer(a2, 1) == 42.0);

    // Test 3: empty array
    double* a3 = nullptr;
    assert(averageUsingPointer(a3, 0) == 0.0);

    // Test 4: all negative values
    double a4[3] = {-1.0, -2.0, -3.0};
    assert(std::abs(averageUsingPointer(a4, 3) - (-2.0)) < 1e-9);

    // Test 5: all identical values
    double a5[4] = {7.5, 7.5, 7.5, 7.5};
    assert(averageUsingPointer(a5, 4) == 7.5);

    // Test 6: includes zero and positive/negative mix
    double a6[4] = {-5.0, 0.0, 5.0, 10.0};
    assert(std::abs(averageUsingPointer(a6, 4) - 2.5) < 1e-9);

    // Test 7: large array (compile-time known size)
    double a7[1000];
    for (int i = 0; i < 1000; ++i) a7[i] = i;
    assert(std::abs(averageUsingPointer(a7, 1000) - 499.5) < 1e-6); // sum = 499500, /1000 = 499.5

    // Test 8: array of size 2 with a zero
    double a8[2] = {0.0, 8.0};
    assert(averageUsingPointer(a8, 2) == 4.0);

    return 0;
}

// The solution initializes a running sum to `0.0`. If the size is `0`, return `0.0` immediately (the sum of zero elements is conventionally `0.0`, and the average is not well-defined, so we choose a safe fallback). Otherwise, iterate from index `0` to `size - 1`, adding `*ptr` (where `ptr` is advanced via `++ptr`) to the sum. After the loop, divide the sum by `size` (cast to `double` to avoid integer division if `size` were an `int`; but here `size` is likely `size_t`, so we explicitly cast to `double`). Edge cases: single-element array returns that element; negative values sum correctly; large sizes could cause overflow but that is outside typical exercise scope. Time complexity is O(n) for a single pass. Space complexity is O(1), using only a few local variables.
