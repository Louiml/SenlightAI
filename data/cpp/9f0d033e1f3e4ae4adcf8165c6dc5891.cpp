/*
Write a C++ function named `findUniqueElement` that takes a non-empty integer array and its length, and returns the single element that appears exactly once, given that every other element appears exactly twice. The function must use the bitwise XOR operation to solve the problem efficiently without using extra data structures (like maps or sets). The array may contain negative numbers, zeros, and large values, but it is guaranteed that exactly one element is unique and all others are paired. Your function should be `const`-correct regarding the input array and must not modify it. The function signature should be `int findUniqueElement(const int nums[], int length)`. After implementing the function, validate it with a series of test cases in a `main` function using `assert` statements.
*/
// Returns the single element that appears once in an array where all others appear twice.
// Uses XOR cancellation: a ^ a = 0, so only the unique element remains.
int findUniqueElement(const int nums[], int length) {
    int result = 0;
    for (int i = 0; i < length; ++i) {
        result ^= nums[i];
    }
    return result;
}
#include <cassert>

int main() {
    // Test basic case
    int arr1[] = {1, 2, 2, 3, 3, 1, 5};
    assert(findUniqueElement(arr1, 7) == 5);

    // Test with negative numbers
    int arr2[] = {-1, -1, -2, -3, -3};
    assert(findUniqueElement(arr2, 5) == -2);

    // Test with zero as the unique element
    int arr3[] = {0, 4, 4, 7, 7};
    assert(findUniqueElement(arr3, 5) == 0);

    // Test with single element
    int arr4[] = {42};
    assert(findUniqueElement(arr4, 1) == 42);

    // Test with larger values and mixed order
    int arr5[] = {1000000, -5, 1000000, 3, 3, -5, 77};
    assert(findUniqueElement(arr5, 7) == 77);

    // Test with duplicate unique element at end (already covered, but adding more)
    int arr6[] = {9, 9, 8, 8, 7, 7, 6};
    assert(findUniqueElement(arr6, 7) == 6);

    // Test with all positive pairs and unique 1
    int arr7[] = {2, 2, 1, 3, 3, 4, 4};
    assert(findUniqueElement(arr7, 7) == 1);
}
// The core insight is that XORing a number with itself results in 0, and XORing any number with 0 results in the number itself. Since every element except one appears exactly twice, XORing all elements together will cancel out all paired numbers (each pair XORs to 0), leaving only the unique element. This works for negative numbers as well because bitwise XOR operates on the two's complement representation. Edge cases include an array of length 1 (the single element is unique), and arrays where the unique element is 0 or negative. The algorithm runs in O(n) time and uses O(1) auxiliary space, making it optimal. No extra storage is needed. The function must be `const`-correct by taking a constant pointer to the array and not modifying it.
