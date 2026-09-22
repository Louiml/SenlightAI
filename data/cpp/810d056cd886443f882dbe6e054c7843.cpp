// Write a C++ function that takes a positive integer `n` and returns the approximate value of π calculated using the Leibniz formula: π = 4 × (1 - 1/3 + 1/5 - 1/7 + 1/9 - ...), where the series is summed for exactly `n` terms. The function should return a `double` value representing the approximation after `n` terms. The input `n` is guaranteed to be at least 1, but handle the general case gracefully by returning 0.0 for `n` less than 1 if you wish to be defensive. For large `n`, the approximation improves but the function must still execute efficiently.
// The Leibniz formula for π is an alternating series: each term is (±1)/(2k+1) for k starting at 0, where the sign alternates starting with +. To compute the sum for exactly `n` terms, loop from `k = 0` to `n-1`, adding `term = (k % 2 == 0 ? 1.0 : -1.0) / (2.0 * k + 1.0)` to a running sum. Then multiply the final sum by 4.0 to get the approximation. Edge cases: `n = 0` yields 0.0 (the empty sum times 4), though the specification says n is positive; still handle it defensively. `n = 1` yields 4.0. For very large `n`, the loop runs `n` times, each iteration doing constant work, so time complexity is O(n) and space complexity O(1). The result is a double, so no overflow concerns, but for extreme `n` the loop may be slow; that's inherent to the specification.
#include <vector>

/**
 * @brief Approximates π using the Leibniz series with exactly n terms.
 * 
 * The series is π/4 = 1 - 1/3 + 1/5 - 1/7 + ...
 * We sum n terms and multiply by 4.
 * 
 * @param n Number of terms to sum (must be >= 1; if < 1, returns 0.0).
 * @return double Approximation of π after summing n terms.
 */
double approximatePi(int n) {
    if (n < 1) {
        return 0.0;
    }
    
    double sum = 0.0;
    for (int k = 0; k < n; ++k) {
        // Alternating sign: + for even k, - for odd k
        double term = (k % 2 == 0) ? 1.0 : -1.0;
        term /= (2.0 * k + 1.0);
        sum += term;
    }
    return sum * 4.0;
}
#include <cassert>
#include <cmath>

int main() {
    // n = 1: 4 * (1) = 4.0
    assert(approximatePi(1) == 4.0);
    
    // n = 2: 4 * (1 - 1/3) = 4 * (2/3) = 8/3 ≈ 2.6666666667
    assert(std::abs(approximatePi(2) - (8.0/3.0)) < 1e-9);
    
    // n = 3: 4 * (1 - 1/3 + 1/5) = 4 * (13/15) = 52/15 ≈ 3.4666666667
    assert(std::abs(approximatePi(3) - (52.0/15.0)) < 1e-9);
    
    // n = 4: 4 * (1 - 1/3 + 1/5 - 1/7) ≈ 2.8952380952
    // Exact: 4 * (1 - 1/3 + 1/5 - 1/7) = 4 * (76/105) = 304/105
    assert(std::abs(approximatePi(4) - (304.0/105.0)) < 1e-9);
    
    // Guard for n = 0 (defensive): returns 0.0
    assert(approximatePi(0) == 0.0);
    
    // Negative n: returns 0.0
    assert(approximatePi(-5) == 0.0);
    
    // Large n: approximation should be closer to π than for n = 10
    assert(std::abs(approximatePi(1000) - 3.14159265358979) < 
           std::abs(approximatePi(10) - 3.14159265358979));
    
    return 0;
}
