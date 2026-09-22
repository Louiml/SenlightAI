Write a C++ function named `calculateTotalEarnings` that takes two integers: `numA` representing a count of item A sold (each priced at 5000) and `numB` representing a count of item B sold (each priced at 9800). The function should return the total earnings as a `long long` integer to safely handle large counts. Assume `numA` and `numB` are non-negative and can be up to 1,000,000 each. The function must be reusable, pure (no I/O), and correctly handle zero or both-zero inputs.
The solution is straightforward: compute `total = numA * 5000LL + numB * 9800LL` using `long long` arithmetic to avoid overflow (e.g., `1e6 * 9800 = 9.8e9` which exceeds 32-bit `int` range). Use `const` for the price constants. Edge cases: both inputs zero returns 0, one input zero returns the product of the other, and maximum inputs produce a value well within `long long` range. Time complexity is O(1) and space complexity is O(1).
#include <cstdint>

// Returns total earnings from selling numA items at 5000 each and numB items at 9800 each.
// Both input counts are non-negative.
long long calculateTotalEarnings(int numA, int numB) {
    const long long priceA = 5000LL;
    const long long priceB = 9800LL;
    return static_cast<long long>(numA) * priceA + static_cast<long long>(numB) * priceB;
}
#include <cassert>

int main() {
    // Basic cases
    assert(calculateTotalEarnings(1, 0) == 5000);
    assert(calculateTotalEarnings(0, 1) == 9800);
    assert(calculateTotalEarnings(2, 3) == 2*5000 + 3*9800); // 10000+29400=39400
    // Both zero
    assert(calculateTotalEarnings(0, 0) == 0);
    // Large counts to check no overflow in int
    assert(calculateTotalEarnings(1000000, 1000000) == 1000000LL*5000 + 1000000LL*9800);
    // Mixed large and small
    assert(calculateTotalEarnings(1000000, 1) == 5000000000LL + 9800);
    assert(calculateTotalEarnings(1, 1000000) == 5000 + 9800000000LL);
    // Negative counts (not expected but still computes as if they were provided; the function treats them as signed)
    // But the task says non-negative, so these are not required; still, test that it computes correctly.
    assert(calculateTotalEarnings(-1, 0) == -5000); // if called, it will produce negative, but we don't need to test this per spec.
    // More standard positive tests
    assert(calculateTotalEarnings(10, 20) == 10*5000 + 20*9800);
    return 0;
}
