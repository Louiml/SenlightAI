// Write a C++ function named `SumDigit` that takes a pointer to an array of integers and its length as parameters. The function should compute and return the sum of all digits of every integer in the array (ignoring any negative sign). For example, the array `{12, -34, 5}` has digit sum `1+2+3+4+5 = 15`. The function must not modify the array, must handle an empty array (length 0) by returning 0, and must assume the input is valid (non-null pointer when length > 0). Provide a standalone implementation without a `main` function.

The main algorithm iterates over each integer in the array, and for each integer, we extract its absolute value (using `std::abs`) and repeatedly take the last digit via modulo 10, adding it to a running total, then dividing by 10 to remove that digit until the number becomes 0. The negative sign is ignored by using the absolute value. Edge cases include the integer 0 (digit sum is 0), negative integers (use absolute value), and an empty array (loop doesn't execute, returns 0). No overflow is a concern because digits are small, but the total sum could become large if many large numbers are given; however, using `long long` is safe. Time complexity is O(n * d) where n is array length and d is the average number of digits per integer, and space complexity is O(1).

#include <cstdlib>   // for std::abs (for int)
#include <cstddef>   // for std::size_t

// Computes the sum of all digits of every integer in the array.
// Uses absolute values to ignore negative signs. Returns 0 for empty array.
long long SumDigit(const int* arr, std::size_t length) {
    long long total = 0;
    for (std::size_t i = 0; i < length; ++i) {
        int num = std::abs(arr[i]);
        while (num > 0) {
            total += num % 10;
            num /= 10;
        }
        // Note: if arr[i] == 0, the loop does nothing, adding 0.
    }
    return total;
}

#include <cassert>
#include <cstddef>

// Function declaration (as per task)
long long SumDigit(const int* arr, std::size_t length);

int main() {
    // Test 1: Mixed positive and negative
    int arr1[] = {12, -34, 5};
    assert(SumDigit(arr1, 3) == 15);

    // Test 2: Single zero
    int arr2[] = {0};
    assert(SumDigit(arr2, 1) == 0);

    // Test 3: Large numbers with many digits
    int arr3[] = {123456, 7890};
    assert(SumDigit(arr3, 2) == (1+2+3+4+5+6 + 7+8+9+0));

    // Test 4: Empty array
    assert(SumDigit(nullptr, 0) == 0);

    // Test 5: All negative numbers
    int arr5[] = {-10, -20, -30};
    assert(SumDigit(arr5, 3) == (1+0+2+0+3+0));

    // Test 6: Single digit positive and negative
    int arr6[] = {7, -8, 9};
    assert(SumDigit(arr6, 3) == (7+8+9));

    // Test 7: Very long array with repetitions
    int arr7[] = {11, 11, 11};
    assert(SumDigit(arr7, 3) == 6);

    return 0;
}
