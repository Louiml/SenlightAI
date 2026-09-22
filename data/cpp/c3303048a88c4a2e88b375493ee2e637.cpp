Write a C++ function named `productSign` that takes a `std::vector<int>` (by `const` reference) and returns an integer representing the sign of the product of all elements in the vector, without actually computing the product (which could overflow). Specifically, return `1` if the product is positive, `-1` if the product is negative, and `0` if the product is zero. The vector may contain any integers, including negative numbers, zeros, large magnitudes, and may be empty. For an empty vector, treat the product as positive (i.e., return `1`). Your function must be efficient and avoid overflow by only examining the signs of the elements.

The product of a set of integers is positive if there is an even number of negative factors, negative if there is an odd number of negative factors, and zero if any factor is zero. Therefore, we can traverse the vector once: if we encounter a zero, we immediately return `0`. Otherwise, we count the number of negative values (or toggle a boolean flag each time we see a negative value). After the loop, if the count of negatives is odd, the product is negative and we return `-1`; otherwise, we return `1`. The empty vector case is handled because the loop never executes, leaving the negative count at `0` (even), so we return `1` as specified. This algorithm runs in O(n) time and uses O(1) extra space. The main edge cases are: (1) a zero appearing anywhere means the result is `0`; (2) an empty input returns `1`; (3) large values never cause overflow because we never multiply; (4) even a single negative flips the sign.

#include <vector>

// Return the sign of the product of all elements in nums.
// 1 for positive, -1 for negative, 0 for zero.
// Empty vector is considered positive (returns 1).
int productSign(const std::vector<int>& nums) {
    bool negative = false; // Track parity of negative count
    for (int num : nums) {
        if (num == 0) {
            return 0; // Product is zero
        }
        if (num < 0) {
            negative = !negative; // Toggle parity
        }
    }
    return negative ? -1 : 1;
}

#include <cassert>
#include <vector>

// The function is declared above (see Solution section).
int main() {
    // Basic positive product
    assert(productSign({1, 2, 3}) == 1);
    // Basic negative product
    assert(productSign({-1, 2, 3}) == -1);
    // Even number of negatives -> positive
    assert(productSign({-1, -2, 3}) == 1);
    // Odd number of negatives -> negative
    assert(productSign({-1, -2, -3}) == -1);
    // Zero in the array
    assert(productSign({0, 5, -3}) == 0);
    assert(productSign({-1, 0, -2}) == 0);
    // Empty vector returns 1
    assert(productSign({}) == 1);
    // Single positive
    assert(productSign({7}) == 1);
    // Single negative
    assert(productSign({-7}) == -1);
    // Large magnitudes (no overflow)
    assert(productSign({2147483647, -2147483647, 2}) == -1);
    assert(productSign({-2147483647, -2147483647, 2}) == 1);
    return 0;
}
