Write a C++ function that takes a non-negative integer `n` and returns the sum of squares of all integers from `1` to `n`, i.e., `1² + 2² + ... + n²`. The function must handle the edge case where `n` is 0 (returning 0) and be efficient for large `n` up to 10⁶. If `n` is negative, throw an `std::invalid_argument` exception. The solution should use a closed-form formula rather than a loop to achieve O(1) time complexity, and must be `const`-correct where possible.
#include <cassert>
#include <cstdint>
#include <stdexcept>

int64_t sumOfSquares(int n);

int main() {
    // Basic cases
    assert(sumOfSquares(0) == 0);
    assert(sumOfSquares(1) == 1);
    assert(sumOfSquares(2) == 5);   // 1 + 4
    assert(sumOfSquares(3) == 14);  // 1 + 4 + 9
    assert(sumOfSquares(4) == 30);  // 1 + 4 + 9 + 16
    assert(sumOfSquares(5) == 55);  // 1 + 4 + 9 + 16 + 25

    // Larger value
    assert(sumOfSquares(100) == 338350);

    // Negative input must throw
    bool threw = false;
    try {
        sumOfSquares(-1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Edge: large n within limit
    assert(sumOfSquares(1000000) == 333333833333500000LL);
}
#include <stdexcept>   // for std::invalid_argument
#include <cstdint>     // for int64_t

// Returns the sum of squares from 1 to n using the closed-form formula.
// Throws std::invalid_argument if n is negative.
int64_t sumOfSquares(int n) {
    if (n < 0) {
        throw std::invalid_argument("n must be non-negative");
    }
    // Use int64_t to avoid overflow in the intermediate multiplication.
    int64_t nn = static_cast<int64_t>(n);
    return nn * (nn + 1) * (2 * nn + 1) / 6;
}
// The sum of squares from 1 to n has a well-known closed-form expression: `n*(n+1)*(2n+1)/6`. This formula eliminates the need for iteration. Edge cases: for `n = 0`, the formula gives `0*1*1/6 = 0`, which is correct. For negative `n`, the formula would produce meaningless results, so we explicitly throw an exception. The division by 6 is exact for all integers, but we compute the numerator first as a 64-bit integer (`long long`) to avoid overflow, since `n` can be up to 10⁶, making the numerator up to ~10¹⁸, which fits in `long long` (max ~9.2×10¹⁸). Time complexity is O(1), and space complexity is O(1). No loops, no recursion, just constant-time arithmetic.
