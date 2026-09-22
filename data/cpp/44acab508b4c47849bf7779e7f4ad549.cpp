/*
Write a C++ function `int recursiveProduct(const int array[], int n)` that computes the product of the first `n` elements of an integer array using recursion. The function must handle the base case when `n == 0` by returning 1 (the multiplicative identity), and for `n > 0`, it must recursively compute the product of the first `n-1` elements and multiply it by `array[n-1]`. The input array may contain any integers (including negatives and zeros), and the function must not modify the original array. Assume `n` is non-negative and the array has at least `n` elements. Ensure the function works correctly for `n = 0` (returning 1) and that overflow behavior is not a concern (i.e., you may ignore integer overflow for this exercise). The function should use proper `const` correctness for the array parameter.
*/
// Compute the product of the first n elements of array recursively.
// Returns 1 when n == 0 (multiplicative identity).
int recursiveProduct(const int array[], int n) {
    // Base case: product of zero elements is 1.
    if (n == 0) {
        return 1;
    }
    // Recursive case: multiply the product of the first n-1 elements by the nth element.
    return recursiveProduct(array, n - 1) * array[n - 1];
}
#include <cassert>

int main() {
    // Test with positive numbers.
    int arr1[] = {1, 2, 3, 4};
    assert(recursiveProduct(arr1, 4) == 24);

    // Test with negative numbers.
    int arr2[] = {-2, -3, 4};
    assert(recursiveProduct(arr2, 3) == 24);

    // Test with zero in the array.
    int arr3[] = {5, 0, 7};
    assert(recursiveProduct(arr3, 3) == 0);

    // Test with n == 0 (empty subarray).
    int arr4[] = {10, 20};
    assert(recursiveProduct(arr4, 0) == 1);

    // Test with n == 1.
    int arr5[] = {-7};
    assert(recursiveProduct(arr5, 1) == -7);

    // Test with a mix of values, including a single-element result.
    int arr6[] = {3, 3, 3};
    assert(recursiveProduct(arr6, 3) == 27);

    // Test with a larger array.
    int arr7[] = {1, -1, 1, -1, 1};
    assert(recursiveProduct(arr7, 5) == 1);

    // Test with multiple large positives (no overflow check).
    int arr8[] = {10, 10, 10};
    assert(recursiveProduct(arr8, 3) == 1000);

    return 0;
}
// The solution uses a straightforward recursive decomposition. For a given `n`, the product of the first `n` elements is `array[n-1] * (product of the first n-1 elements)`. The recursion terminates when `n == 0`, where the product of zero elements is defined as 1 (the multiplicative identity). This definition is consistent because multiplying any product by 1 leaves it unchanged, and it also naturally handles the case of an empty subarray. For each recursive call, exactly one multiplication is performed, and the recursion depth is `n`. The time complexity is O(n) because there is one recursive call per element, and the auxiliary space complexity is O(n) due to the call stack (since each recursive call holds a stack frame). Edge cases include `n = 0` (return 1), `n = 1` (return `array[0]`), and arrays containing zeros or negatives—these are handled naturally by multiplication. No input validation is needed because the problem statement guarantees `n` is non-negative and the array is large enough.
