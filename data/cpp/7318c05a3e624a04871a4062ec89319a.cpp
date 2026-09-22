// Write a C++ function that takes three integers `a`, `b`, `c`, and a non-negative integer `k`. The function should compute and return the result of applying the following operation `k` times: starting with the expression `a - b`, each subsequent operation swaps the order to `b - a`, then back to `a - b`, and so on. In other words, if `k` is even, return `a - b`; if `k` is odd, return `b - a`. The input values can be any integers within the range of a standard 32-bit integer, but the result may exceed that range (so use a 64-bit type for the return). The parameter `c` is provided but is unused; include it in the signature for compatibility but ignore its value. The function must be named `alternatingDifference`, take exactly four parameters in the order `(int a, int b, int c, long long k)`, and return a `long long`.
// The core observation is that the expression alternates strictly between two states: `a - b` when the number of operations performed is even (including zero), and `b - a` when odd. Since we only care about the parity of `k`, we can directly check `k % 2 == 0`. The parameter `c` is meaningless and can be ignored. Edge cases: `k` can be zero (even), in which case we return `a - b`; negative `k` is not expected per the task, but if passed, the modulo behavior in C++ for negative numbers would be implementation-defined before C++11 (but since we treat `k` as non-negative, it's safe). Also, since the operands are 32-bit integers, their difference fits in a 64-bit signed integer (maximum range about ±4.3 billion, well within 64-bit range). Time complexity is O(1) as we only perform a constant number of operations; space complexity is O(1).
#include <cstddef> // not strictly needed, but for completeness

// Computes alternating difference based on parity of k.
// Parameters a, b, c are integers; c is unused.
// k is a non-negative operation count (0 means no operations).
// Returns a - b if k is even, else b - a, as long long.
long long alternatingDifference(int a, int b, int c, long long k) {
    // c is intentionally ignored.
    (void)c; // suppress unused parameter warning
    if (k % 2 == 0) {
        return static_cast<long long>(a) - static_cast<long long>(b);
    } else {
        return static_cast<long long>(b) - static_cast<long long>(a);
    }
}
#include <cassert>

int main() {
    assert(alternatingDifference(5, 3, 0, 0) == 2);
    assert(alternatingDifference(5, 3, 0, 1) == -2);
    assert(alternatingDifference(5, 3, 0, 2) == 2);
    assert(alternatingDifference(5, 3, 0, 1000000) == 2); // even
    assert(alternatingDifference(5, 3, 0, 999999) == -2); // odd
    assert(alternatingDifference(-10, 20, 7, 0) == -30);
    assert(alternatingDifference(-10, 20, 7, 1) == 30);
    assert(alternatingDifference(0, 0, 0, 3) == 0);
    assert(alternatingDifference(2147483647, -2147483648, 0, 0) == 4294967295LL);
    assert(alternatingDifference(2147483647, -2147483648, 0, 1) == -4294967295LL);
    return 0;
}
