Write a C++ function named `computeAverage` that accepts a non-empty array of integers and its size, and returns the integer average (truncated toward zero) as an `int`. The function must not modify the array. Additionally, implement a helper logic that includes a placeholder behavior for a separate function `Add` that takes an integer by value and does nothing (to mirror the original snippet’s structure, but the solution must focus on the average computation). The input array will contain at least one element, may include negative numbers, and may contain duplicate values. The average should be computed using integer division (truncation toward zero), which matches C++ semantics for `int / int`. Ensure the function signature is `int computeAverage(const int arr[], int size)`.

#include <cassert>

int main() {
    int a1[] = {1, 2, 3, 4};
    assert(computeAverage(a1, 4) == 2);  // 10/4 = 2 (truncated)

    int a2[] = {5};
    assert(computeAverage(a2, 1) == 5);

    int a3[] = {-5, -1, -10};
    assert(computeAverage(a3, 3) == -5);  // -16/3 = -5 (truncated)

    int a4[] = {10, 10, 10};
    assert(computeAverage(a4, 3) == 10);

    int a5[] = {-7, -8};
    assert(computeAverage(a5, 2) == -7);  // -15/2 = -7 (truncated)

    int a6[] = {0, 100, -50, 30};
    assert(computeAverage(a6, 4) == 20);  // 80/4 = 20
}

#include <cstddef>  // for size_t if needed, but we use int size

// Compute the integer average (truncated toward zero) of a non-empty array.
// The array is not modified; size must be greater than zero.
int computeAverage(const int arr[], int size) {
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += arr[i];
    }
    return sum / size;
}

// The main algorithm is straightforward: iterate through the array once, accumulate the sum into an `int` (being cautious about potential overflow — for the task scope, assume values fit within `int` and the sum does not overflow `int`). Then compute the integer average as `sum / size`. Because C++ integer division truncates toward zero for positive and negative operands, this naturally satisfies the truncation requirement. Edge cases: an array with a single element returns that element itself; all negative values yield a negative average truncated toward zero (e.g., `[-5, -5]` gives `-5`, `[-7, -8]` gives `-7`). No special handling for duplicates is needed. Time complexity is O(n) for n elements, and space complexity is O(1) auxiliary (only a sum variable). The solution must not modify the input, so `const int arr[]` is used, and the function is free-standing without a `main`.
