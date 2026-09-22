Write a C++ function that takes an array of integers and its size, and returns the sum of all even numbers in the array. The function must correctly handle arrays containing no even numbers and arrays with only one element. Additionally, the function should be robust when called with a size of zero. The signature should be `int sumEven(const int arr[], int size)`.
#include <cassert>

int main() {
    // Test 1: Normal case with even numbers
    int arr1[] = {1, 2, 3, 4, 5};
    assert(sumEven(arr1, 5) == 6); // 2 + 4 = 6

    // Test 2: No even numbers
    int arr2[] = {1, 3, 5, 7};
    assert(sumEven(arr2, 4) == 0);

    // Test 3: Single even number
    int arr3[] = {8};
    assert(sumEven(arr3, 1) == 8);

    // Test 4: Single odd number
    int arr4[] = {9};
    assert(sumEven(arr4, 1) == 0);

    // Test 5: Empty array (size 0)
    int arr5[] = {};
    assert(sumEven(arr5, 0) == 0);

    // Test 6: Negative even numbers included
    int arr6[] = {-2, 3, -4, 5};
    assert(sumEven(arr6, 4) == -6); // -2 + -4 = -6

    // Test 7: All even numbers
    int arr7[] = {2, 4, 6, 8};
    assert(sumEven(arr7, 4) == 20);

    // Test 8: Large array with zeros
    int arr8[] = {0, 0, 0};
    assert(sumEven(arr8, 3) == 0); // 0 is even

    return 0;
}
#include <cstddef> // for size_t

// Returns the sum of all even numbers in the array.
// Returns 0 if the array is empty or contains no even numbers.
int sumEven(const int arr[], int size) {
    // Base case: empty array contributes 0
    if (size == 0) {
        return 0;
    }
    // Recursively sum even numbers in the first size-1 elements
    int partial_sum = sumEven(arr, size - 1);
    // If the last element is even, add it to the partial sum
    if (arr[size - 1] % 2 == 0) {
        partial_sum += arr[size - 1];
    }
    return partial_sum;
}
// The core problem is summing even elements in an integer array. A recursive approach mirrors common academic exercises and provides a clean demonstration of recursion, but an iterative approach is equally valid and more efficient in practice. The key edge cases are: (1) an empty array (size 0) where the sum should be 0, (2) arrays with no even numbers where the result must be 0 (this requires careful handling in recursion to avoid undefined behavior when a call does not return a value), and (3) negative even numbers, which should be added normally since the sum is algebraic. In the recursive solution, the base case returns 0 when the size reaches 0. For each recursive step, we compute the sum of the first `n-1` elements, then check if the last element (`arr[n-1]`) is even; if so, add it to that sum, else return the sum unchanged. This avoids the missing-return bug in the original snippet. Time complexity is O(n) because we visit each element once, and space complexity is O(n) due to the call stack if recursion is used, or O(1) if iterative. The solution must use `const` for the array pointer to indicate read-only access.
