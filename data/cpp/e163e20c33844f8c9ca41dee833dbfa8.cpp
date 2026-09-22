/*
Write a C++ function that computes the value of a nested radical expression: given a positive integer `n`, the function should return `sqrt(1 + sqrt(2 + sqrt(3 + ... + sqrt(n)...)))` evaluated from the innermost term outward. Specifically, start with `sqrt(n)`, then repeatedly take the square root of `(i + previous_result)` for `i = n-1` down to `1`. The function must take a single `int` parameter and return a `double`. Handle the edge case where `n <= 0` by returning `0.0` (or you may define your own behavior, but it must be robust). The function should be const-correct and self-contained with no global variables.
*/

#include <cmath>

// Compute nested radical: sqrt(1 + sqrt(2 + sqrt(3 + ... + sqrt(n)...)))
// Returns 0.0 for n <= 0 (invalid input).
double nestedRadical(const int n) {
    if (n <= 0) {
        return 0.0;
    }
    double result = std::sqrt(static_cast<double>(n));
    for (int i = n - 1; i >= 1; --i) {
        result = std::sqrt(static_cast<double>(i) + result);
    }
    return result;
}

#include <cassert>
#include <cmath>

// The solution function is declared above (assumed available).
int main() {
    // n = 1: sqrt(1) = 1
    assert(std::fabs(nestedRadical(1) - 1.0) < 1e-9);
    // n = 2: sqrt(1 + sqrt(2)) ≈ sqrt(1 + 1.4142) ≈ sqrt(2.4142) ≈ 1.5538
    assert(std::fabs(nestedRadical(2) - 1.553773974) < 1e-6);
    // n = 3: sqrt(1 + sqrt(2 + sqrt(3))) ≈ sqrt(1 + sqrt(3.732)) ≈ sqrt(1 + 1.9319) ≈ sqrt(2.9319) ≈ 1.7123
    assert(std::fabs(nestedRadical(3) - 1.712265) < 1e-6);
    // n = 4: sqrt(1 + sqrt(2 + sqrt(3 + sqrt(4)))) ≈ 1.7498
    assert(std::fabs(nestedRadical(4) - 1.749845) < 1e-6);
    // n = 5: sqrt(1 + sqrt(2 + sqrt(3 + sqrt(4 + sqrt(5))))) ≈ 1.7706
    assert(std::fabs(nestedRadical(5) - 1.770634) < 1e-6);
    // n = 0 (invalid) returns 0
    assert(nestedRadical(0) == 0.0);
    // n = -1 (invalid) returns 0
    assert(nestedRadical(-1) == 0.0);
    // n = 10: the value should be between 1 and 2
    double val10 = nestedRadical(10);
    assert(val10 > 1.0 && val10 < 2.0);
    // n = 100: still between 1 and 2, and greater than the value for n=10
    double val100 = nestedRadical(100);
    assert(val100 > val10 && val100 < 2.0);
    // n = 1000: convergent to approx 1.99
    double val1000 = nestedRadical(1000);
    assert(val1000 > 1.9 && val1000 < 2.0);
    return 0;
}

// The problem asks for a nested radical: for `n = 1`, the result is simply `sqrt(1) = 1.0`. For `n > 1`, the expression is `sqrt(1 + sqrt(2 + sqrt(3 + ... + sqrt(n))))`. The natural evaluation order is bottom-up: first compute `sqrt(n)`, then for `i = n-1` down to `1`, update the accumulator to `sqrt(i + accumulator)`. This matches the given code snippet’s sum method but generalizes it. Edge cases: if `n <= 0`, there is no valid nested radical, so returning `0.0` is a safe convention (alternatively, you could throw or return NaN, but the task specifies robust handling). The main algorithm is a simple loop with a double accumulator; it runs in `O(n)` time because we perform `n` square root operations, and `O(1)` auxiliary space. Floating-point precision is adequate for reasonable `n` (e.g., up to a few thousand); for very large `n`, the result converges to about 2.0 and precision may degrade slightly but not cause correctness issues in typical test cases.
