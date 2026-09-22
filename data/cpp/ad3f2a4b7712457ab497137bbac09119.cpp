/*
Write a C++ function `findGreatestProduct` that takes a vector of integers and returns the maximum product that can be obtained by multiplying exactly two distinct elements from the vector. The vector is guaranteed to contain at least two elements and may contain negative numbers, zeros, and duplicates. For example, for `{-10, -3, 5, 6, 2}`, the greatest product is 30 (from 5×6), but note that two negatives can also produce a large positive product, such as `{-10, -3}` → 30; however, the largest overall would be `{ -10, -2 }` → 20 versus `{6,5}→30`. Ensure your function handles the case where the two largest negatives might multiply to a larger positive than the two largest positives, and also handle zero appropriately (e.g., `{-1, 0, 2}` → 0). The function should be efficient and work for any integer range, including large values that may overflow 32-bit `int`—use `long long` for the result.
*/
#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the maximum product of any two distinct elements in the vector.
// Uses long long to avoid integer overflow. Assumes at least two elements.
long long findGreatestProduct(const std::vector<int>& nums) {
    // Track the two largest and two smallest values.
    long long max1 = LLONG_MIN; // largest
    long long max2 = LLONG_MIN; // second largest
    long long min1 = LLONG_MAX; // smallest
    long long min2 = LLONG_MAX; // second smallest

    for (int value : nums) {
        long long v = value; // promote to long long for comparisons
        // Update largest two
        if (v > max1) {
            max2 = max1;
            max1 = v;
        } else if (v > max2) {
            max2 = v;
        }
        // Update smallest two
        if (v < min1) {
            min2 = min1;
            min1 = v;
        } else if (v < min2) {
            min2 = v;
        }
    }

    // Candidate products: product of two largest (likely positive) and product of two smallest (may be negative*negative).
    long long productMax = max1 * max2;
    long long productMin = min1 * min2;

    return std::max(productMax, productMin);
}
#include <cassert>
#include <vector>
#include <cstdint>

int main() {
    // Two positives
    assert(findGreatestProduct({1, 2, 3, 4}) == 12);
    // Two negatives produce larger than positives
    assert(findGreatestProduct({-10, -3, 5, 6, 2}) == 30);
    // All negatives: product of two smallest (closest to zero) is largest
    assert(findGreatestProduct({-5, -2, -1}) == 2); // (-2)*(-1) = 2
    // Mixed with zero
    assert(findGreatestProduct({-5, 0, 2}) == 0);
    // Exactly two elements
    assert(findGreatestProduct({-7, 3}) == -21);
    // Duplicate values
    assert(findGreatestProduct({10, 10, 1}) == 100);
    // Large values to test overflow safety
    assert(findGreatestProduct({100000, 100000}) == 10000000000LL);
    // Negative and positive with large magnitude
    assert(findGreatestProduct({-100000, 100000, -1, 2}) == 10000000000LL);
    // No positive numbers, but zeros and negatives
    assert(findGreatestProduct({-1, -2, 0}) == 2);
    return 0;
}
// The solution requires finding two distinct elements whose product is maximized. The maximum product of two numbers is achieved either by multiplying the two largest positive numbers or the two smallest (most negative) numbers, because a negative times a negative yields a positive. If the vector contains only negative numbers, the product of the two smallest (i.e., closest to zero) negatives will be the largest; but if there are fewer than two positives, we still consider both candidate pairs. The algorithm: iterate through the vector once, keeping track of the largest two values and the smallest two values. After the pass, compute `max1 * max2` and `min1 * min2` (where min1 ≤ min2 are the two smallest values) and return the larger product. This works because any other combination (e.g., a positive and a negative) will be ≤ the product of the two largest positives if that product is positive, or ≤ the product of the two smallest negatives when the product is positive. Edge cases: vector size exactly 2 returns the product of those two; all zeros returns 0; one positive and many negatives: the answer is either the product of the two largest positives (if there are two) or the product of the two smallest negatives (since their product is positive and might exceed the positive*negative). Time complexity is O(n) with a single pass, and space complexity is O(1). We use `long long` to avoid overflow.
