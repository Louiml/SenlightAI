Write a C++ function that takes an array of integers and its size, and returns the sum of all elements in the array. The array may contain positive and negative integers, and its size `n` will be at least 1. The function must not modify the input array. Your implementation should use a `while` loop (not a `for` loop) as the primary iteration structure, and must be robust for any valid integer values.

// The solution simply accumulates the sum by iterating through each element of the array exactly once. We initialize a `sum` variable to 0, then use an index `i` starting at 0, and increment it inside a `while` loop while `i < n`. For each iteration, we add `arr[i]` to the running total. This approach works for arrays of any length and handles negative numbers, zero, or large values naturally since we use standard integer addition. Edge cases include arrays with a single element (returns that element) and arrays with all zeros (returns 0). Since the array is passed by pointer without `const`, but we do not modify it, the function is safe. Time complexity is O(n) because we visit each element once. Space complexity is O(1) since we only use a few integer variables regardless of input size.

#include <cstddef>  // for size_t (optional, can use int)

// Compute the sum of all elements in an integer array of given size.
int sumArrayElements(int arr[], int n) {
    int total = 0;
    int index = 0;
    while (index < n) {
        total += arr[index];
        ++index;
    }
    return total;
}

#include <cassert>

int main() {
    // Test 1: Basic positive array
    int arr1[] = {1, 2, 3, 4};
    assert(sumArrayElements(arr1, 4) == 10);

    // Test 2: Array with negative numbers
    int arr2[] = {-5, 10, -3};
    assert(sumArrayElements(arr2, 3) == 2);

    // Test 3: Single element
    int arr3[] = {42};
    assert(sumArrayElements(arr3, 1) == 42);

    // Test 4: All zeros
    int arr4[] = {0, 0, 0, 0};
    assert(sumArrayElements(arr4, 4) == 0);

    // Test 5: Mixed large values and negatives
    int arr5[] = {100, -200, 300, -50, 0};
    assert(sumArrayElements(arr5, 5) == 150);

    // Test 6: Array with zeros and positives
    int arr6[] = {0, 0, 7, 0};
    assert(sumArrayElements(arr6, 4) == 7);

    // Test 7: Larger array
    int arr7[] = {1, 1, 1, 1, 1, 1, 1, 1};
    assert(sumArrayElements(arr7, 8) == 8);

    return 0;
}
