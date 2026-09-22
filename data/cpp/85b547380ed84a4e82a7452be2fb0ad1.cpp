Write a C++ function named `continuedFractionValue` that takes an integer `n` (number of iterations, assumed to be non-negative) and returns a `double` equal to `3 + 1/(6 + 1/(6 + ...))` with exactly `n` nested divisions, where the innermost operation is `1/(0 + 6)` for `n = 1`, and for `n = 0` the function should return `3.0`. The function must compute the value iteratively starting from `r = 0.0` and repeatedly updating `r = 1.0 / (r + 6.0)` exactly `n` times, then returning `r + 3.0`. Ensure the result is accurate to at least 10 decimal places.

The problem is a straightforward iterative calculation of a continued fraction. The main algorithm initializes a double `r = 0.0` and performs a loop `n` times, each time setting `r = 1.0 / (r + 6.0)`. After the loop, the final result is `r + 3.0`. Edge cases include `n = 0` (the loop does not execute, and `r` stays `0.0`, so the function returns `3.0`) and very large `n` (the value converges to a constant, so no overflow occurs as long as division denominators stay positive; `r` never becomes negative because `r` is always positive after the first iteration, and `0.0 + 6.0 = 6.0`). The time complexity is O(n) because the loop runs exactly `n` times, and space complexity is O(1) since only a single double variable is used.

#include <cstddef>

// Compute 3 + 1/(6 + 1/(6 + ...)) with exactly n nested divisions.
// n = 0 returns 3.0, n = 1 returns 3 + 1/6, etc.
double continuedFractionValue(int n) {
    double r = 0.0;
    for (int i = 0; i < n; ++i) {
        r = 1.0 / (r + 6.0);
    }
    return r + 3.0;
}

#include <cassert>
#include <cmath>

int main() {
    // n = 0: no divisions, result is 3.0
    assert(std::fabs(continuedFractionValue(0) - 3.0) < 1e-12);
    
    // n = 1: 3 + 1/6 = 3.1666666667
    assert(std::fabs(continuedFractionValue(1) - 3.1666666666666665) < 1e-10);
    
    // n = 2: 3 + 1/(6 + 1/6) = 3 + 1/(6.1666666667) = 3 + 0.1621621622 = 3.1621621622
    assert(std::fabs(continuedFractionValue(2) - 3.162162162162162) < 1e-10);
    
    // n = 3: compute directly using the iterative formula
    double r = 0.0;
    r = 1.0 / (r + 6.0); // i=0
    r = 1.0 / (r + 6.0); // i=1
    r = 1.0 / (r + 6.0); // i=2
    assert(std::fabs(continuedFractionValue(3) - (r + 3.0)) < 1e-12);
    
    // n = 10: should converge to ~3.162277660168379 (√10 ≈ 3.16227766)
    double val10 = continuedFractionValue(10);
    assert(std::fabs(val10 - 3.1622776601683795) < 1e-6);
    
    // n = 100: stable and near the limit
    double val100 = continuedFractionValue(100);
    assert(std::fabs(val100 - 3.1622776601683795) < 1e-10);
    
    return 0;
}
