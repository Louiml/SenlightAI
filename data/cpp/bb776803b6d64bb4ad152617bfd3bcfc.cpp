Write a standalone C++ function named `strided_sum` that takes a raw integer array and its total length as parameters, and returns the sum of the elements at even indices (0, 2, 4, ...) only. The function must not use any external libraries beyond the standard headers, and must handle empty arrays (return 0) as well as arrays with only one element. The function should be `const`-correct: accept the array as a pointer to const integers and the length as a `size_t`. Do not modify the input array.
#include <cassert>

int main() {
    int arr1[] = {1, 2, 3, 4, 5, 6}; // even indices: 1+3+5 = 9
    assert(strided_sum(arr1, 6) == 9);

    int arr2[] = {10}; // only index 0
    assert(strided_sum(arr2, 1) == 10);

    int arr3[] = {7, 8, 9, 10}; // even indices: 7+9 = 16
    assert(strided_sum(arr3, 4) == 16);

    // Empty array – must not crash
    assert(strided_sum(nullptr, 0) == 0);

    int arr4[] = {-1, -2, -3, -4, -5}; // even indices: -1 + -3 + -5 = -9
    assert(strided_sum(arr4, 5) == -9);

    int arr5[] = {100, 200, 300, 400, 500, 600, 700}; // 100+300+500+700 = 1600
    assert(strided_sum(arr5, 7) == 1600);

    // Large negative/positive mix
    int arr6[] = {5, -5, 10, -10, 15}; // 5+10+15 = 30
    assert(strided_sum(arr6, 5) == 30);
    return 0;
}
#include <cstddef> // for size_t

// Returns the sum of elements at even indices (0, 2, 4, ...) of the given array.
// An empty array (length == 0) yields a sum of 0.
long long strided_sum(const int* arr, size_t length) {
    long long sum = 0;
    for (size_t i = 0; i < length; i += 2) {
        sum += arr[i];
    }
    return sum;
}
// The core algorithm is straightforward: iterate over the array with a loop that starts at index 0 and increments by 2 each step, accumulating the values into a `long long` sum to avoid integer overflow for larger arrays. Edge cases include: (a) an empty array (length 0) → immediately return 0 without dereferencing any pointer; (b) an array of length 1 → the loop runs once for index 0 and sums that single element; (c) arrays with odd or even lengths → the loop condition `i < length` naturally stops at the last valid even index. Time complexity is O(n/2) = O(n), and space complexity is O(1) aside from the input array itself.
