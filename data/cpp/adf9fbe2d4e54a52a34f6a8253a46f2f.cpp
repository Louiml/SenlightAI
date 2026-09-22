// Write a C++ function named `pointerArithmeticDemo` that takes an integer array and its size as parameters, along with an integer `initialValue`. The function should dynamically allocate an integer array of the given size using `new`, initialize the first element to `initialValue`, and then use pointer arithmetic (not array indexing) to fill the remaining elements such that each subsequent element is the square of the previous element's index plus the previous element itself (i.e., `arr[i] = arr[i-1] + i*i`). The function should return the sum of all elements in the array, and it must also ensure the dynamically allocated memory is properly freed before returning. Handle the edge case where the size is 0 or negative by returning 0 immediately without allocating memory. Apply `const` correctness wherever appropriate for parameters that should not be modified.

The solution approach involves allocating an array of the given size using `new int[]`. For the base case when size is 1, initialize `arr[0]` to `initialValue` and return that value (sum). For larger sizes, use a pointer `ptr` that points to the current element. Set the first element via `*ptr = initialValue`, then for each subsequent index `i` from 1 to size-1, move the pointer forward by one position (using `++` or `ptr += 1`) and assign `*ptr = *(ptr-1) + i*i`. The sum can be accumulated during the loop. Memory management requires a `try-catch` block to ensure that if any exception occurs during allocation or assignment, the allocated memory is freed. Edge cases: if `size <= 0`, return 0 without allocating; if `size == 0` or negative, no memory allocation. For `size == 1`, simply allocate one element, assign, sum, free, and return. Time complexity is O(n) because we iterate through each element once. Space complexity is O(1) auxiliary (excluding the dynamically allocated array itself, which is O(n) but is freed before return). The function should use `const` for the size parameter (since it's not modified) and possibly for the `initialValue` if it's not changed.

#include <cstddef>
#include <new>

// Fills a dynamically allocated array using pointer arithmetic and returns the sum.
// If size <= 0, returns 0. The array is freed before returning.
int pointerArithmeticDemo(const std::size_t size, const int initialValue) {
    if (size == 0) {
        return 0;
    }

    int* arr = nullptr;
    try {
        arr = new int[size];
        int* ptr = arr;
        *ptr = initialValue;

        std::size_t sum = initialValue;
        for (std::size_t i = 1; i < size; ++i) {
            ++ptr; // move to next element
            *ptr = *(ptr - 1) + static_cast<int>(i * i);
            sum += *ptr;
        }

        delete[] arr;
        return static_cast<int>(sum);
    } catch (...) {
        delete[] arr;
        throw; // rethrow after cleanup
    }
}

#include <cassert>

int main() {
    // Edge case: size 0
    assert(pointerArithmeticDemo(0, 5) == 0);
    
    // Single element
    assert(pointerArithmeticDemo(1, 10) == 10);
    
    // Two elements: [3, 3 + 1*1 = 4] sum = 7
    assert(pointerArithmeticDemo(2, 3) == 7);
    
    // Three elements: [2, 2+1=3, 3+4=7] sum = 12
    assert(pointerArithmeticDemo(3, 2) == 12);
    
    // Four elements: [1, 1+1=2, 2+4=6, 6+9=15] sum = 24
    assert(pointerArithmeticDemo(4, 1) == 24);
    
    // Larger test: size 5, initial 0 -> [0,1,5,14,30] sum = 50
    assert(pointerArithmeticDemo(5, 0) == 50);
    
    // Negative initial value: size 3, initial -2 -> [-2,-1,3] sum = 0
    assert(pointerArithmeticDemo(3, -2) == 0);
    
    // Large size to verify no overflow for sum (use small initial to avoid overflow)
    // size 100, initial 1 -> sum will be large but fits in int for these values
    // However, we just check a known small case since sum may overflow for large.
    // Here we use a moderate size.
    assert(pointerArithmeticDemo(10, 1) == 1 + 2 + 6 + 15 + 31 + 56 + 92 + 141 + 205 + 286); // sum = 835
}
