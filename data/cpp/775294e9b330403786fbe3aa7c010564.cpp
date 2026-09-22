Write a C++ function `discountedPrice(int D, int N)` that, given a discount rate `D` (an integer from 0 to 2, representing `100^D` as a multiplier) and a quantity `N` (a positive integer), returns the total cost as an integer. The cost is computed as `(100^D) * N`, but with a special rule: if `N` equals 100, the cost is `(100^D) * 101` instead. The function should handle large results (up to `100^2 * 101 = 1,010,000`) and must not use floating-point arithmetic. The inputs are always valid: `D` is 0, 1, or 2, and `N` is between 1 and 100 inclusive.

#include <cassert>

int main() {
    // D=0, multiplier=1
    assert(discountedPrice(0, 1) == 1);
    assert(discountedPrice(0, 99) == 99);
    assert(discountedPrice(0, 100) == 101);  // special rule

    // D=1, multiplier=100
    assert(discountedPrice(1, 1) == 100);
    assert(discountedPrice(1, 50) == 5000);
    assert(discountedPrice(1, 100) == 10100); // 100*101

    // D=2, multiplier=10000
    assert(discountedPrice(2, 1) == 10000);
    assert(discountedPrice(2, 100) == 1010000); // 10000*101

    // Random check with non-100 N
    assert(discountedPrice(2, 7) == 70000);
    assert(discountedPrice(1, 100) == 10100);
    assert(discountedPrice(0, 100) == 101);

    // Boundary: N=100 always adds 1 regardless of D
    assert(discountedPrice(0, 100) == 101);
    assert(discountedPrice(1, 100) == 10100);
    assert(discountedPrice(2, 100) == 1010000);
}

#include <cstdint>

// Computes the total cost for quantity N with discount level D.
// D is 0,1,2 representing 100^D multiplier.
// If N==100, the effective quantity becomes 101.
long long discountedPrice(int D, int N) {
    long long multiplier = 1;
    for (int i = 0; i < D; ++i) {
        multiplier *= 100;
    }
    long long effectiveN = N + (N == 100);  // adds 1 when N equals 100
    return multiplier * effectiveN;
}

// The core logic is straightforward: compute `multiplier = 1`; for each of the `D` levels, multiply `multiplier` by 100. Then, if `N == 100`, add 1 to `N` (since `N + (N == 100)` becomes `101` when `N` is 100, else just `N`). Finally, return `multiplier * adjustedN`. The expression `(N == 100)` is a boolean that converts to 1 when true, 0 otherwise, so we add that to `N`. This avoids any floating-point `pow` issues and keeps everything in integer arithmetic. Edge cases: `D=0` gives multiplier=1, so cost is just `N` (or `101` if `N==100`). `N` is always at least 1, so no zero or negative issues. Time complexity is O(D) (max 3 operations) and space is O(1).
