// Given two positive integers `n` and `k`, write a C++ function `long long int findAnswer(long long int n, long long int k)` that computes and returns the smaller of the two roots of the quadratic equation derived from the expression `(3 + 2*n ± sqrt(8*n + 8*k + 9)) / 2`. The function should compute both candidates using integer arithmetic (avoiding floating-point division) and return the smaller one. Ensure the function handles large values of `n` and `k` (up to 10^9) without overflow, and note that the expression under the square root is always a perfect square for valid inputs (i.e., `8*n + 8*k + 9` is a perfect square). If the square root is not an integer, the function may assume the input guarantees it is; however, for robustness, you can use `sqrtl` and round to the nearest integer, then verify. Return the smaller root as a `long long int`.
// The expression `(3 + 2*n ± sqrt(8*n + 8*k + 9)) / 2` yields two integer candidates. The problem asks to return the minimum of them. Since `n` and `k` are positive integers up to 10^9, the term `8*n + 8*k + 9` can be up to about 1.6e10, which fits within 64-bit `long long`. The square root of that is at most about 126,491, which is small, but the term `3 + 2*n` can be up to about 2e9, so the sum with the sqrt is fine. The division by 2 is integer division, which in C++ truncates toward zero, but because the numerator is guaranteed to be even for valid inputs (since `8*n+8*k+9` is an odd perfect square, its sqrt is odd, and `3+2*n` is odd, so odd+odd=even), the result is exact. To avoid floating-point inaccuracies, we can compute the integer square root using `sqrtl` and cast to `long long`, then adjust by testing. Alternatively, use binary search for the integer square root. Then compute `candidate1 = (3 + 2*n + sqrtVal) / 2` and `candidate2 = (3 + 2*n - sqrtVal) / 2`, and return `min(candidate1, candidate2)`. Edge cases: when `n` is very large, `3 + 2*n` may overflow `int` but not `long long`; using `long long` throughout is safe. Time complexity is O(1) (the integer sqrt via `sqrtl` is constant time), space O(1).
#include <cmath>
#include <algorithm>

// Compute the smaller root of the quadratic expression:
//   (3 + 2*n ± sqrt(8*n + 8*k + 9)) / 2
// Assumes the input yields an integer square root.
long long int findAnswer(long long int n, long long int k) {
    // Compute the radicand safely within long long.
    long long int radicand = 8 * n + 8 * k + 9;

    // Compute integer square root using long double precision.
    long long int sqrtVal = static_cast<long long int>(std::sqrtl(static_cast<long double>(radicand)));

    // Adjust for potential rounding errors (rare but safe).
    while ((sqrtVal + 1) * (sqrtVal + 1) <= radicand) {
        ++sqrtVal;
    }
    while (sqrtVal * sqrtVal > radicand) {
        --sqrtVal;
    }

    // Compute both roots.
    long long int root1 = (3 + 2 * n + sqrtVal) / 2;
    long long int root2 = (3 + 2 * n - sqrtVal) / 2;

    return std::min(root1, root2);
}
#include <cassert>

int main() {
    // Example from the snippet: n=1, k=1 -> radicand=8+8+9=25, sqrt=5, roots=(5+5)/2=5 and (5-5)/2=0, min=0
    assert(findAnswer(1, 1) == 0);
    // n=2, k=3 -> radicand=16+24+9=49, sqrt=7, roots=(7+7)/2=7 and (7-7)/2=0, min=0
    assert(findAnswer(2, 3) == 0);
    // n=3, k=2 -> radicand=24+16+9=49, sqrt=7, roots=(9+7)/2=8 and (9-7)/2=1, min=1
    assert(findAnswer(3, 2) == 1);
    // n=10, k=100 -> radicand=80+800+9=889 not perfect square? Actually 889 is not a perfect square, but let's test with valid ones.
    // n=1, k=3 -> radicand=8+24+9=41 not perfect square, but assume valid inputs.
    // Use known perfect square: n=0? Not allowed (positive). n=1, k=2 -> radicand=8+16+9=33 not perfect.
    // Instead pick n=5, k=5 -> radicand=40+40+9=89 not perfect.
    // For a valid case: choose n=1, k=0 -> radicand=8+0+9=17 not perfect.
    // Let's find: n=1,k=4 -> 8+32+9=49 -> sqrt=7 -> roots=(5+7)/2=6, (5-7)/2=-1, min=-1
    assert(findAnswer(1, 4) == -1);
    // n=2,k=2 -> 16+16+9=41 not perfect.
    // n=3,k=4 -> 24+32+9=65 not perfect.
    // n=4,k=6 -> 32+48+9=89 not.
    // Let's use n=2,k=5 -> 16+40+9=65 not.
    // For simplicity, also test with large values where radicand is a perfect square.
    // n=100, k=100 -> 800+800+9=1609, sqrt≈40.112, not perfect.
    // But the task guarantees perfect square, so test with n=1,k=1 (25) and n=1,k=4 (49) and n=3,k=2 (49).
    // Add a test with n=3,k=2 again, and n=5,k=10 -> radicand=40+80+9=129 not.
    // Instead, compute a valid pair: n=12,k=15 -> 96+120+9=225, sqrt=15, roots=(27+15)/2=21, (27-15)/2=6, min=6
    assert(findAnswer(12, 15) == 6);
    // n=1,k=1 already done.
    // n=2,k=8 -> 16+64+9=89 not.
    // n=3,k=6 -> 24+48+9=81, sqrt=9, roots=(9+9)/2=9, (9-9)/2=0, min=0
    assert(findAnswer(3, 6) == 0);
    // n=4,k=2 -> 32+16+9=57 not.
    // n=5,k=15 -> 40+120+9=169, sqrt=13, roots=(13+13)/2=13, (13-13)/2=0, min=0
    assert(findAnswer(5, 15) == 0);
    // n=6,k=3 -> 48+24+9=81, sqrt=9, roots=(15+9)/2=12, (15-9)/2=3, min=3
    assert(findAnswer(6, 3) == 3);
    return 0;
}
