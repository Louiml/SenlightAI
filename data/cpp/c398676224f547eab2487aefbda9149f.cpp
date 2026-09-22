// Write a C++ function named `analyzeArray` that takes a pointer to a dynamically allocated array of `double` values and an integer representing its size, and returns a struct containing the average, a pointer to the maximum element, and a pointer to the minimum element. The function must not modify the original array, must handle the case where the array is non-empty (assume size ≥ 1), and should be `const`-correct. The returned pointers must point to elements within the original array, so that the caller can compute indices by subtracting the base pointer. The task is to implement the function, not the main program, but it must be self-contained with all necessary headers and type definitions.

#include <cassert>

int main() {
    // Test 1: Basic case
    double arr1[] = {1.0, 2.0, 3.0, 4.0};
    ArrayStats s1 = analyzeArray(arr1, 4);
    assert(s1.average == 2.5);
    assert(s1.maxPtr == &arr1[3]);
    assert(s1.minPtr == &arr1[0]);
    
    // Test 2: Negative numbers
    double arr2[] = {-5.5, -1.0, -10.0, -3.0};
    ArrayStats s2 = analyzeArray(arr2, 4);
    assert(s2.average == -4.875);
    assert(s2.maxPtr == &arr2[1]);
    assert(s2.minPtr == &arr2[2]);
    
    // Test 3: Single element
    double arr3[] = {7.25};
    ArrayStats s3 = analyzeArray(arr3, 1);
    assert(s3.average == 7.25);
    assert(s3.maxPtr == &arr3[0]);
    assert(s3.minPtr == &arr3[0]);
    
    // Test 4: All equal
    double arr4[] = {3.0, 3.0, 3.0};
    ArrayStats s4 = analyzeArray(arr4, 3);
    assert(s4.average == 3.0);
    assert(s4.maxPtr == &arr4[0]);
    assert(s4.minPtr == &arr4[0]);
    
    // Test 5: Max and min at boundaries
    double arr5[] = {10.0, -2.0, 8.0, 0.0}; // max first, min second
    ArrayStats s5 = analyzeArray(arr5, 4);
    assert(s5.average == 4.0);
    assert(s5.maxPtr == &arr5[0]);
    assert(s5.minPtr == &arr5[1]);
    
    // Test 6: Floating point duplicates
    double arr6[] = {1.5, 2.5, 2.5, 1.5};
    ArrayStats s6 = analyzeArray(arr6, 4);
    assert(s6.average == 2.0);
    assert(s6.maxPtr == &arr6[1]); // first occurrence
    assert(s6.minPtr == &arr6[0]); // first occurrence
    
    return 0;
}

#include <cstddef>

// Struct to hold analysis results
struct ArrayStats {
    double average;
    const double* maxPtr;
    const double* minPtr;
};

// Analyze a non-empty array of doubles.
// Returns average and pointers to max/min elements in the original array.
ArrayStats analyzeArray(const double* a, int size) {
    // Initialize with first element
    double sum = a[0];
    const double* maxPtr = &a[0];
    const double* minPtr = &a[0];
    
    // Iterate from second element
    for (int i = 1; i < size; ++i) {
        sum += a[i];
        if (a[i] > *maxPtr) {
            maxPtr = &a[i];
        }
        if (a[i] < *minPtr) {
            minPtr = &a[i];
        }
    }
    
    return {sum / size, maxPtr, minPtr};
}

// The solution defines a struct `ArrayStats` that holds three members: `double average`, `const double* maxPtr`, and `const double* minPtr`. Since the input is read-only, the function signature uses `const double* a` and `const int size` (or just `int size`). The algorithm first sums all elements to compute the average by dividing by size. Then it initializes `maxPtr` and `minPtr` to point to the first element. It iterates from index 1 to size-1, comparing each element with the current maximum and minimum, updating the pointers when a larger or smaller value is found. Edge cases include all elements being equal (pointers remain at first element), negative numbers (comparisons work normally), and size 1 (average is that element, both pointers point to it). Time complexity is O(n) for the single pass (sum and min/max in the same loop), and space complexity is O(1) auxiliary storage. The function is declared `const`-correct by using `const double*` for parameters and return pointers to indicate the original array is not modified.
