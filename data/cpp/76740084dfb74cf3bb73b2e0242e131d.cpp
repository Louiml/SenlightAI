// Write a C++ function named `alternatingSeriesSum` that takes a positive integer `n` as input and returns the sum of the series `1 - 2 + 3 - 4 + ... + n`, where the sign alternates: positive for odd numbers and negative for even numbers. The function should handle any positive integer `n` (including large values that fit within a 64-bit signed integer range) and return the result as a `long long` to avoid overflow. For example, for `n = 4`, the computation is `1 - 2 + 3 - 4 = -2`; for `n = 5`, it is `1 - 2 + 3 - 4 + 5 = 3`. If `n` is not positive (i.e., `n <= 0`), the function should return `0`.
#include <cassert>

int main() {
    // Positive cases with alternating signs
    assert(alternatingSeriesSum(1) == 1);
    assert(alternatingSeriesSum(2) == -1);
    assert(alternatingSeriesSum(3) == 2);
    assert(alternatingSeriesSum(4) == -2);
    assert(alternatingSeriesSum(5) == 3);
    assert(alternatingSeriesSum(10) == -5);
    assert(alternatingSeriesSum(11) == 6);
    
    // Edge cases: non-positive input returns 0
    assert(alternatingSeriesSum(0) == 0);
    assert(alternatingSeriesSum(-5) == 0);
    
    // Large input to test the formula and avoid overflow
    assert(alternatingSeriesSum(1000000) == -500000);
    assert(alternatingSeriesSum(1000001) == 500001);
    
    // Larger than int but still within long long range
    assert(alternatingSeriesSum(2000000000) == -1000000000LL);
    assert(alternatingSeriesSum(2000000001) == 1000000001LL);
    
    return 0;
}
#include <cstdint>

// Returns the sum of the alternating series 1 - 2 + 3 - 4 + ... + n.
// For n <= 0, returns 0.
long long alternatingSeriesSum(const int n) {
    if (n <= 0) {
        return 0;
    }
    
    // Formula: if n is even, sum = -n/2; if odd, sum = (n/2) + 1.
    if (n % 2 == 0) {
        return -static_cast<long long>(n) / 2;
    } else {
        return static_cast<long long>(n) / 2 + 1;
    }
}
// The series follows a clear pattern: odd indices contribute positively, even indices contribute negatively. The simplest and most direct approach is to iterate from 1 to `n` and add `i` if `i` is odd, or subtract `i` if `i` is even. This is straightforward and correct for all `n`, with a time complexity of O(n) and O(1) auxiliary space. However, a more efficient formula exists: the sum equals `ceil(n/2)` if `n` is odd, and `-(n/2)` if `n` is even. This can be expressed as `(n % 2 == 0) ? -(n / 2) : (n / 2 + 1)`. This formula runs in O(1) time and O(1) space, and also avoids overflow from summing many terms—though with `long long` even the iterative version works for practical inputs. Edge cases include `n = 1` (returns `1`), `n = 2` (returns `-1`), and `n` being even/odd. Since `n` is positive by specification, no negative input handling is strictly required, but returning `0` for non-positive inputs is safe.
//
// For the solution, I will implement the O(1) formula, which is elegant and efficient. In the iterative approach, using `long long` for the accumulator is important to avoid overflow for large `n`; with the formula, the multiplication and division are safe in `long long` for the given constraint. I will also apply `const` to the parameter to indicate it is not modified.
