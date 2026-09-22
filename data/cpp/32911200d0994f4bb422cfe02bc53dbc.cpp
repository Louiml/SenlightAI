// Write a C++ function `int maxDivisionsByTwo(int arr[], int n)` that takes an array of positive integers and its length, and returns the maximum number of times every number in the array can be divided by 2 simultaneously without any number becoming odd. The process stops as soon as at least one number in the array is odd. The function should handle arrays with a single element and cases where no division is possible (returning 0). You may assume the array is non-empty and contains only positive integers. The function must not modify the original array; it should work on a copy or perform read-only analysis.

// The algorithm simulates the repeated division by 2 on a copy of the array. Initialize a counter to 0. In each iteration, check whether all numbers are even—if any number is odd, stop. Otherwise, divide every number by 2 and increment the counter. Continue until the stopping condition is met. The number of iterations equals the count of successful full passes. Edge cases: If the first number is already odd, the loop never executes and the result is 0. For an array with all identical powers of two (e.g., [8,8,8]), the answer is the exponent (3), while for mixed numbers like [2,4], only one division works because after dividing 4→2, the array is [1,2] which contains an odd. Time complexity is O(k·n) where k is the result (bounded by the number of bits in the smallest number, at most ~30 for typical ints), and space complexity is O(n) for the copy (or O(1) if we use a count of trailing zeros). However, a more elegant O(n) approach is to compute the minimum number of trailing zeros across all numbers—that value is exactly the answer, because each global division consumes one trailing zero from every number. This avoids the loop and copies.

#include <vector>
#include <algorithm>
#include <cstddef>

// Returns the maximum number of simultaneous divisions by 2 possible for all numbers.
int maxDivisionsByTwo(const int arr[], std::size_t n) {
    if (n == 0) return 0;
    int min_trailing_zeros = __builtin_ctz(static_cast<unsigned>(arr[0]));
    for (std::size_t i = 1; i < n; ++i) {
        int zeros = __builtin_ctz(static_cast<unsigned>(arr[i]));
        min_trailing_zeros = std::min(min_trailing_zeros, zeros);
    }
    return min_trailing_zeros;
}

#include <cassert>
#include <cstddef>

// Declaration (already defined above)
int maxDivisionsByTwo(const int arr[], std::size_t n);

int main() {
    int arr1[] = {8, 8, 8};
    assert(maxDivisionsByTwo(arr1, 3) == 3);

    int arr2[] = {2, 4};
    assert(maxDivisionsByTwo(arr2, 2) == 1);

    int arr3[] = {3, 6, 12};
    assert(maxDivisionsByTwo(arr3, 3) == 0);

    int arr4[] = {16};
    assert(maxDivisionsByTwo(arr4, 1) == 4);

    int arr5[] = {12, 20, 28};
    assert(maxDivisionsByTwo(arr5, 3) == 2);

    int arr6[] = {1};
    assert(maxDivisionsByTwo(arr6, 1) == 0);

    int arr7[] = {1024, 1024, 2};
    assert(maxDivisionsByTwo(arr7, 3) == 1); // because 2 has only 1 trailing zero
}
