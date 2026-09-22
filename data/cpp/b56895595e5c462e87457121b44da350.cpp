/*
Write a C++ function named `geometricSeriesSum` that takes three parameters: a `double` value `a` (the first term), a `double` value `r` (the common ratio), and an `int` `n` (the number of terms, guaranteed to be non-negative). The function must compute and return the sum of the first `n` terms of the geometric series: \(a + ar + ar^2 + \dots + ar^{n-1}\). Use an iterative loop to accumulate the sum term by term, using `std::pow` to compute each power of `r`. The function must be correct for `n = 0` (returning `0.0`), for `n = 1` (returning `a`), for negative or positive ratios, and for large `n` (up to 1000). Ensure the function is declared `const`-correct and does not modify its parameters.
*/

#include <cmath>

// Compute the sum of the first n terms of a geometric series: a + ar + ar^2 + ... + ar^(n-1).
double geometricSeriesSum(const double a, const double r, const int n) {
    double total = 0.0;
    for (int j = 0; j < n; ++j) {
        total += a * std::pow(r, j);
    }
    return total;
}

#include <cassert>
#include <cmath>

// The solution function is declared above; here we test it.

int main() {
    // n = 0: empty sum is zero
    assert(geometricSeriesSum(1.0, 0.5, 0) == 0.0);

    // n = 1: sum is a
    assert(geometricSeriesSum(3.2, 0.9, 1) == 3.2);

    // Classic example: a=1, r=0.5, sum converges to 2 as n grows; test n=11
    double val = geometricSeriesSum(1.0, 0.5, 11);
    // Expected sum = 1*(1 - 0.5^11)/(1 - 0.5) = 2*(1 - 0.5^11) = 2 - 2^-10 = 2 - 1/1024 ≈ 1.9990234375
    assert(std::abs(val - 1.9990234375) < 1e-9);

    // Negative ratio: a=2, r=-1, n=4 -> 2 - 2 + 2 - 2 = 0
    assert(geometricSeriesSum(2.0, -1.0, 4) == 0.0);

    // a=5, r=3, n=3 -> 5 + 15 + 45 = 65
    assert(geometricSeriesSum(5.0, 3.0, 3) == 65.0);

    // r=0: a=7, n=5 -> 7 + 0 + 0 + 0 + 0 = 7
    assert(geometricSeriesSum(7.0, 0.0, 5) == 7.0);

    // Large n: a=1, r=2, n=10 -> 1 + 2 + 4 + ... + 512 = 1023
    assert(geometricSeriesSum(1.0, 2.0, 10) == 1023.0);

    // Non-integer a and r
    double val2 = geometricSeriesSum(0.5, 0.25, 3); // 0.5 + 0.125 + 0.03125 = 0.65625
    assert(std::abs(val2 - 0.65625) < 1e-9);

    return 0;
}

// The solution uses a simple loop that iterates exactly `n` times. For each iteration `j` from 0 to `n-1`, the term `a * std::pow(r, j)` is added to a running total. The starting total is `0.0`. The main edge case is `n = 0`, where the loop does not execute and the function returns `0.0`. For `n = 1`, the loop runs once and returns `a * r^0 = a`. For negative ratios, `std::pow` handles negative bases with integer exponents correctly, producing alternating signs. For `r = 0`, every term after the first is zero, so the sum is `a` for `n >= 1` and `0` for `n = 0`. The time complexity is \(O(n)\) because each term requires one `pow` call (which itself is \(O(\log j)\) in typical implementations, but for simplicity we treat it as constant per call), and the space complexity is \(O(1)\) since only a few local variables are used. No special handling for floating-point precision is required for the intended test cases.
