// Write a C++ function named `sumArray` that accepts a pointer to a constant integer array (`const int* arr`) and an integer representing the number of elements (`int size`), and returns the sum of all elements as an `int`. The function must iterate through the array using a `for` loop, accumulating the total. The function should correctly handle arrays containing negative numbers, zeros, positive numbers, and the special case where `size` is 0 (returning 0). The solution must be fully self-contained with appropriate headers, and the function must be declared with `const` correctness for the array parameter. Do not include a `main` function in the solution; test code will be provided separately.

The main algorithm is straightforward: initialize an integer accumulator to 0, then use a `for` loop from index 0 to `size-1`, adding each element `arr[i]` to the accumulator. After the loop completes, return the accumulator. Edge cases include an empty array (`size == 0`), where the loop does not execute and the function returns 0; arrays with negative numbers, where the accumulator correctly sums negative values; and arrays with all zeros, where the sum remains 0. The time complexity is O(n) because we visit each element exactly once. The space complexity is O(1) because we only use a single accumulator variable, regardless of the array size. The function must be declared as `int sumArray(const int* arr, int size)` to signal that the array is not modified, enabling the caller to pass constant arrays safely.

#include <cstddef>  // for std::size_t (optional but good practice)

// Returns the sum of all elements in the given integer array.
// The array is passed as a constant pointer to indicate it is not modified.
// If size is 0, the sum is defined as 0.
int sumArray(const int* arr, int size) {
    int total = 0;
    for (int i = 0; i < size; ++i) {
        total += arr[i];
    }
    return total;
}

#include <cassert>

// Forward declaration to match the solution function signature.
int sumArray(const int* arr, int size);

int main() {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {10, 20, 30};
    int arr3[] = {-1, -2, -3, -4, -5};
    int arr4[] = {0, 0, 0, 0, 0};
    int arr5[] = {7};
    int arr6[] = {100, -100, 50, -50};

    assert(sumArray(arr1, 5) == 15);   // 1+2+3+4+5 = 15
    assert(sumArray(arr2, 3) == 60);   // 10+20+30 = 60
    assert(sumArray(arr3, 5) == -15);  // -1-2-3-4-5 = -15
    assert(sumArray(arr4, 5) == 0);    // all zeros
    assert(sumArray(arr1, 0) == 0);    // empty array
    assert(sumArray(arr5, 1) == 7);    // single positive
    assert(sumArray(arr3, 1) == -1);   // single negative
    assert(sumArray(arr6, 4) == 0);    // mixed positive and negative cancel

    return 0;
}
