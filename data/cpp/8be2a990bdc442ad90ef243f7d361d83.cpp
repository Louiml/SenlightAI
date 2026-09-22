/*
Write a C++ function that computes the square root of a non-negative integer using an iterative Newton–Raphson (Babylonian) method without using the standard library’s `sqrt` or any floating-point math functions. The function must accept a signed integer, handle edge cases (0, 1, negative inputs), and return the result as a `float`. When the input is negative, return `-1.0f` as an error indicator. For all other inputs, the computed result must be accurate to at least 1e-4 absolute error compared to the true square root.
*/
#include <cmath> // for std::fabs, but we can avoid using sqrt; used only for testing tolerance if needed

// Computes the square root of a non-negative integer using the Babylonian method.
// Returns -1.0f for negative inputs (error indicator).
float findSquareRoot(int num) {
    if (num < 0) {
        return -1.0f;
    }
    if (num == 0) {
        return 0.0f;
    }
    if (num == 1) {
        return 1.0f;
    }

    float x = 0.0f;
    float y = static_cast<float>(num) / 2.0f; // initial guess
    while (y != x) { // stops when two successive approximations are bit-identical
        x = y;
        y = (num / x + x) / 2.0f;
    }
    return y;
}
#include <cassert>
#include <cmath>

int main() {
    // Edge cases
    assert(std::fabs(findSquareRoot(0) - 0.0f) < 1e-4);
    assert(std::fabs(findSquareRoot(1) - 1.0f) < 1e-4);
    assert(findSquareRoot(-5) == -1.0f);

    // Perfect squares
    assert(std::fabs(findSquareRoot(4) - 2.0f) < 1e-4);
    assert(std::fabs(findSquareRoot(9) - 3.0f) < 1e-4);
    assert(std::fabs(findSquareRoot(16) - 4.0f) < 1e-4);
    assert(std::fabs(findSquareRoot(25) - 5.0f) < 1e-4);

    // Non-perfect squares
    assert(std::fabs(findSquareRoot(2) - std::sqrt(2.0f)) < 1e-4);
    assert(std::fabs(findSquareRoot(3) - std::sqrt(3.0f)) < 1e-4);
    assert(std::fabs(findSquareRoot(10) - std::sqrt(10.0f)) < 1e-4);

    // Larger integer
    assert(std::fabs(findSquareRoot(123456) - std::sqrt(123456.0f)) < 1e-4);

    return 0;
}
// The main algorithm is the Babylonian method: start with an initial guess `y = num / 2.0f` (or a better initial guess like `num` itself for very small values). Then iteratively refine using `y = (y + num/y) / 2`. This converges quadratically to the true square root. The loop terminates when two consecutive iterations produce the same value (i.e., `y == x` after assigning `x = y`), but to avoid infinite loops due to floating-point precision, we can either use a small epsilon tolerance or rely on the fact that after a few iterations the value stabilizes. Edge cases: `num == 0` → return `0.0f`; `num == 1` → return `1.0f`; `num < 0` → return `-1.0f`. Also, for `num` very large, initial guess `num/2` works fine. For `num` = 2, 3, etc., the method converges in a handful of iterations. Time complexity: each iteration is O(1), and the number of iterations is roughly O(log log n) because convergence is quadratic; practically, it converges in under 10 iterations for integers up to millions. Space complexity: O(1) auxiliary space.
