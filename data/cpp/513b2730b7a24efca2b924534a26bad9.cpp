// Write a C++ function named `calculateAverage` that accepts an array of integers and its size as parameters, and returns the average of all elements as a `float`. The function must handle an empty array by returning `0.0f` (since dividing by zero is undefined), and must correctly compute the average for arrays containing negative numbers, zeros, and duplicate values. The function should be const-correct, meaning it should not modify the input array. After implementing the function, write a test program that demonstrates its correctness using several test cases, including edge cases like an empty array, a single-element array, an array with all zeros, and an array with mixed positive and negative numbers.

The solution computes the sum of all elements in the array using a simple loop, then divides the sum by the size to get the average. Since the size is an integer and we want a floating-point result, we cast either the sum or the size to `float` before division to avoid integer truncation. The main edge case is an empty array (size 0): we must check for this and return `0.0f` to avoid division by zero. Negative numbers and duplicates require no special handling—they simply contribute to the sum. The time complexity is O(n), where n is the array size, because we traverse the array once. The space complexity is O(1), as we only use a single accumulator variable.

#include <cstddef>

// Calculate and return the average of the integers in the array.
// Returns 0.0f if the array is empty (size == 0).
float calculateAverage(const int arr[], std::size_t size) {
    if (size == 0) {
        return 0.0f;
    }

    int sum = 0;
    for (std::size_t i = 0; i < size; ++i) {
        sum += arr[i];
    }

    return static_cast<float>(sum) / static_cast<float>(size);
}

#include <cassert>
#include <cstddef>

// Declare the function (or include the header where it's defined).
float calculateAverage(const int arr[], std::size_t size);

int main() {
    // Test 1: Empty array
    const int emptyArr[] = {};
    assert(calculateAverage(emptyArr, 0) == 0.0f);

    // Test 2: Single element
    const int singleArr[] = {42};
    assert(calculateAverage(singleArr, 1) == 42.0f);

    // Test 3: All negative numbers
    const int negArr[] = {-10, -20, -30};
    assert(calculateAverage(negArr, 3) == -20.0f);

    // Test 4: Mixed positive and negative
    const int mixedArr[] = {10, -5, 0, 5};
    assert(calculateAverage(mixedArr, 4) == 2.5f); // sum=10, avg=10/4=2.5

    // Test 5: All zeros
    const int zeroArr[] = {0, 0, 0, 0};
    assert(calculateAverage(zeroArr, 4) == 0.0f);

    // Test 6: Duplicate values
    const int dupArr[] = {7, 7, 7};
    assert(calculateAverage(dupArr, 3) == 7.0f);

    // Test 7: Fractional average that would truncate if not using float
    const int fractionArr[] = {1, 2};
    assert(calculateAverage(fractionArr, 2) == 1.5f);

    // Test 8: Larger array with mixed values
    const int largeArr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(calculateAverage(largeArr, 10) == 5.5f); // sum=55, avg=5.5

    // Test 9: Array with only one negative number
    const int singleNegArr[] = {-1};
    assert(calculateAverage(singleNegArr, 1) == -1.0f);

    // Test 10: Array with large values to ensure no overflow in sum
    const int largeValArr[] = {1000000, 2000000, -1500000};
    assert(calculateAverage(largeValArr, 3) == 500000.0f); // sum=1500000, avg=500000

    return 0;
}
