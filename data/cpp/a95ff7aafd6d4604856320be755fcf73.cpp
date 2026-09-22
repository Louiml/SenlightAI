// Write a standalone C++ function that accepts a `std::vector<int>` (or a plain array with its size) and returns the absolute difference between the sum of elements at even indices (0, 2, 4, ...) and the sum of elements at odd indices (1, 3, 5, ...). The function must be named `evenOddDifference` and return an integer. The input vector may be empty, contain only one element, or contain negative numbers. The function should handle these cases gracefully, returning 0 for an empty vector and the sole element's value for a single-element vector (since the odd sum is 0). Use `const` references where appropriate, and ensure the solution is self-contained with only standard library headers.
#include <cassert>
#include <vector>
#include <cstdlib>

// Solution function (provided above, but repeated here for self-containment of test)
int evenOddDifference(const std::vector<int>& values) {
    int evenSum = 0;
    int oddSum = 0;
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i % 2 == 0) {
            evenSum += values[i];
        } else {
            oddSum += values[i];
        }
    }
    return std::abs(evenSum - oddSum);
}

int main() {
    // Original example: {1,3,5,2,4,5,1} even sum = 1+5+4+1=11, odd sum = 3+2+5=10, diff=1
    std::vector<int> v1 = {1,3,5,2,4,5,1};
    assert(evenOddDifference(v1) == 1);

    // Empty vector
    std::vector<int> v2;
    assert(evenOddDifference(v2) == 0);

    // Single element vector
    std::vector<int> v3 = {42};
    assert(evenOddDifference(v3) == 42);

    // All negative
    std::vector<int> v4 = {-1,-2,-3,-4};
    // even sum = -1 + -3 = -4, odd sum = -2 + -4 = -6, diff = |-4 - (-6)| = 2
    assert(evenOddDifference(v4) == 2);

    // Alternating signs
    std::vector<int> v5 = {10, -5, 3, -2, 7};
    // even sum = 10 + 3 + 7 = 20, odd sum = -5 + -2 = -7, diff = 27
    assert(evenOddDifference(v5) == 27);

    // Two elements
    std::vector<int> v6 = {5, 5};
    // even sum = 5, odd sum = 5, diff = 0
    assert(evenOddDifference(v6) == 0);

    return 0;
}
#include <vector>
#include <cstdlib>  // for std::abs

// Returns the absolute difference between sums of even-indexed and odd-indexed elements.
int evenOddDifference(const std::vector<int>& values) {
    int evenSum = 0;
    int oddSum = 0;
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i % 2 == 0) {
            evenSum += values[i];
        } else {
            oddSum += values[i];
        }
    }
    return std::abs(evenSum - oddSum);
}
// The core idea is to iterate over the array once, maintaining two accumulators: one for even-indexed elements and one for odd-indexed elements. For each index `i`, check if `i % 2 == 0`; if so, add to the even sum, otherwise add to the odd sum. After the loop, return the absolute value of `even - odd` using `std::abs` from `<cstdlib>` or `<cmath>`. Edge cases: an empty vector should result in both sums being 0, so the difference is 0; a vector with one element will add that element to the even sum and nothing to odd, so the difference is the absolute value of that element. Negative numbers do not affect the correctness because we take the absolute difference at the end. The time complexity is O(n) for a vector of size n, and the space complexity is O(1) for the two sum variables.
