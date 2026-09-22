/*
Write a C++ function named `sumArray` that takes an integer count `n` and an array of integers, and returns the sum of all elements in the array. The array is guaranteed to contain at least one element, and the sum may exceed the range of a 32-bit integer, so the return type must be `long long`. Additionally, the function must not modify the array, and the solution should be self-contained (only the function, no `main`) with appropriate `const` correctness.
*/

#include <cstddef>

// Compute the sum of the first n integers in the array v.
// The function does not modify the array and returns a long long to avoid overflow.
long long sumArray(std::size_t n, const int v[]) {
    long long total = 0;
    for (std::size_t i = 0; i < n; ++i) {
        total += v[i];
    }
    return total;
}

int main() {
    int a1[] = {1, 2, 3, 4};
    assert(sumArray(4, a1) == 10);

    int a2[] = {-5, 10, -3};
    assert(sumArray(3, a2) == 2);

    int a3[] = {7};
    assert(sumArray(1, a3) == 7);

    int a4[] = {0, 0, 0};
    assert(sumArray(3, a4) == 0);

    int a5[] = {1000000000, 1000000000};
    assert(sumArray(2, a5) == 2000000000LL);

    int a6[] = {2147483647, 2147483647};
    assert(sumArray(2, a6) == 4294967294LL);

    int a7[] = {-2147483648, -2147483648};
    assert(sumArray(2, a7) == -4294967296LL);

    int a8[] = {1, -1, 2, -2, 3};
    assert(sumArray(5, a8) == 3);

    int a9[] = {100, -100};
    assert(sumArray(2, a9) == 0);

    int a10[] = {12345};
    assert(sumArray(1, a10) == 12345);
}

// The approach is a straightforward linear summation. Initialize a `long long` accumulator to 0, then iterate through each element of the array from index 0 to `n-1`, adding each value to the accumulator. The function takes the array as a const pointer to express that it does not modify the input, and the count `n` is used to bound the loop. Edge cases: the minimum case `n == 1` works naturally; negative integers are handled by normal addition; and the use of `long long` prevents overflow for large sums (e.g., up to 10^5 elements each around 10^9). Time complexity is O(n) because each element is visited once. Space complexity is O(1) because only a single accumulator variable is used, not counting the input array itself.
