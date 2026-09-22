// Write a C++ function named `simpsonComposite` that computes the approximate integral of a given mathematical function `double f(double x)` over a closed interval `[a, b]` using the composite Simpson's rule. The function must accept as parameters the lower limit `a`, the upper limit `b`, and an even number of subintervals `n` (with `n ≥ 2`). It must handle the case where `n` is odd by automatically adjusting it to the nearest even number less than or equal to `n` (i.e., if `n` is odd, decrement it by 1). The function should return the approximate integral value as a `double`. The mathematical function `f` can be defined externally (e.g., as a lambda or a separate function) and passed as an argument to `simpsonComposite` using a function pointer or `std::function`. The implementation must correctly sum the function evaluations at interior points, applying coefficients of 4 for odd-indexed points and 2 for even-indexed points, and finally combine with the endpoint evaluations. The solution must be self-contained, efficient, and handle edge cases appropriately.
#include <cassert>
#include <cmath>

// Test function: f(x) = 2*sin(x) + cos(x)
double testFunc(double x) {
    return 2.0 * sin(x) + cos(x);
}

int main() {
    // Integral of 2*sin(x) + cos(x) from 0 to pi/2 is exactly 3 (since 2*(-cos(pi/2)+cos(0)) + (sin(pi/2)-sin(0)) = 2*(1) + 1 = 3)
    // Using n=4 approximation should be close to 3.
    assert(std::fabs(simpsonComposite(0.0, M_PI/2.0, 4, testFunc) - 3.0) < 1e-6);

    // Using n=10 gives even closer result.
    assert(std::fabs(simpsonComposite(0.0, M_PI/2.0, 10, testFunc) - 3.0) < 1e-10);

    // Odd n is adjusted to even.
    assert(std::fabs(simpsonComposite(0.0, M_PI/2.0, 5, testFunc) - 3.0) < 1e-7);

    // n=0 is adjusted to 2.
    assert(std::fabs(simpsonComposite(0.0, M_PI/2.0, 0, testFunc) - 3.0) < 1e-2);

    // Zero interval [1,1] gives 0.
    assert(simpsonComposite(1.0, 1.0, 10, testFunc) == 0.0);

    // Test with a constant function: integral of 5 from 0 to 4 should be 20.
    auto constantFive = [](double) { return 5.0; };
    assert(std::fabs(simpsonComposite(0.0, 4.0, 4, constantFive) - 20.0) < 1e-12);

    return 0;
}
#include <functional>
#include <cmath>

// Computes the composite Simpson's rule approximation of ∫_a^b f(x) dx.
// n is the number of subintervals; if odd, it is reduced to the nearest even.
double simpsonComposite(double a, double b, int n, const std::function<double(double)>& f) {
    if (n < 2) n = 2;
    if (n % 2 != 0) --n; // make even

    const double h = (b - a) / n;
    double sum = f(a) + f(b);

    for (int i = 1; i < n; ++i) {
        double x = a + i * h;
        if (i % 2 == 1) {
            sum += 4.0 * f(x);
        } else {
            sum += 2.0 * f(x);
        }
    }

    return (h / 3.0) * sum;
}
// The composite Simpson's rule approximates the integral of `f` over `[a, b]` by dividing the interval into `n` equal subintervals of width `h = (b - a) / n`, where `n` must be even. The rule is:  
// `I ≈ (h/3) * [f(a) + f(b) + 4 * sum_{i=1,3,5,...}^{n-1} f(a + i*h) + 2 * sum_{i=2,4,6,...}^{n-2} f(a + i*h)]`.  
// The algorithm first ensures `n` is even: if `n` is odd, reduce it by 1 (so if `n` is 0 or negative, it should be set to at least 2). Then compute `h`, initialize an accumulator with `f(a) + f(b)`, and loop from `i = 1` to `n-1`, adding `4 * f(x_i)` if `i` is odd, and `2 * f(x_i)` if `i` is even. Finally multiply by `h/3`. Edge cases: if `n` is 0 or negative, set `n = 2` (or throw an exception, but the spec says adjust to nearest even number, so for `n < 2` we set to 2). If `a == b`, the integral is trivially 0, but the formula still works (h=0, result=0). Time complexity is `O(n)` because we evaluate the function `n+1` times (including endpoints). Space complexity is `O(1)` beyond the function pointer storage, as we only track a few local variables.
