Write a C++ function that accepts a fixed-size array of integers (up to 20 elements), the actual number of elements used (`r`), and returns the sum of all elements that are divisible by 3 (including negative multiples of 3). The input array should not be modified. The function must be robust when `r` is 0 or negative, returning 0 in those cases. Assume the array itself is always valid and contains at least as many elements as the given count.
#include <cassert>
#include <cstddef>

// The function declared above is assumed to be included or defined before this point.
int main() {
    int a1[] = {3, 6, 9, 12};
    assert(sumDivisibleBy3(a1, 4) == 30);          // all divisible
  
    int a2[] = {1, 2, 4, 5};
    assert(sumDivisibleBy3(a2, 4) == 0);            // none divisible
  
    int a3[] = {-3, 0, 5, -9, 7};
    assert(sumDivisibleBy3(a3, 5) == -12);          // -3 + 0 + (-9)
  
    int a4[] = {10, 20, 30};
    assert(sumDivisibleBy3(a4, 0) == 0);            // empty range
  
    int a5[] = {3, 1, 2};
    assert(sumDivisibleBy3(a5, 1) == 3);            // only first element
  
    int a6[] = {0, 0, 0};
    assert(sumDivisibleBy3(a6, 3) == 0);            // zeros are divisible
  
    int a7[] = {-6, 7, -3, 8, 9};
    assert(sumDivisibleBy3(a7, 2) == -6);           // only first two checked
  
    // Ensure the function does not modify the array (optional check)
    int a8[] = {3, 6, 9};
    sumDivisibleBy3(a8, 3);
    assert(a8[0] == 3 && a8[1] == 6 && a8[2] == 9);
  
    return 0;
}
#include <cstddef> // for size_t

// Returns the sum of all elements in arr[0..r-1] that are divisible by 3.
// If r <= 0, returns 0. The input array is not modified.
int sumDivisibleBy3(const int arr[], std::size_t r) {
    int total = 0;
    if (r == 0) {
        return 0;
    }
    for (std::size_t i = 0; i < r; ++i) {
        if (arr[i] % 3 == 0) {
            total += arr[i];
        }
    }
    return total;
}
// The solution involves iterating over the first `r` elements of the array, checking each element for divisibility by 3 using the modulus operator (`element % 3 == 0`). Since negative multiples of 3 also satisfy this condition (e.g., -6 % 3 == 0), no special sign handling is required. The main edge case is when `r <= 0`: in that case, the function should return 0 without accessing the array. Additionally, values like 0 are divisible by 3 and should be included. The algorithm runs in O(r) time and uses O(1) auxiliary space, as it only maintains an accumulator. The input array is passed by const reference to guarantee it is not modified, and the count is passed by value for simplicity.
