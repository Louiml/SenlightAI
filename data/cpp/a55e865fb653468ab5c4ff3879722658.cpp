You are given three positive integers `a`, `b`, and `n`. Write a C++ function `minimumMovesToExceed` that takes three `long long` parameters (a, b, n) and returns the minimum number of operations required until either `a` or `b` becomes strictly greater than `n`. In each operation, you must replace the smaller of the two current values with the sum of both current values (if they are equal, you may add `b` to `a` i.e., treat `a` as the smaller one). For example, starting with `a=1`, `b=2`, `n=10`: after first operation `a=3`, then `b=5`, then `a=8`, then `b=13` → done, so answer is 4. You must handle large values up to 10^18, so avoid overflow by carefully choosing which value to update. Return the count as a `long long`.
#include <cassert>

int main() {
    // Base cases
    assert(minimumMovesToExceed(1, 2, 10) == 4);
    assert(minimumMovesToExceed(1, 1, 1) == 1); // after one op: 2>1
    assert(minimumMovesToExceed(5, 5, 4) == 0); // already > n
    assert(minimumMovesToExceed(2, 3, 100) == 8);
    // Large values, no overflow
    assert(minimumMovesToExceed(1, 1, 1000000000000000000LL) == 63); // Fibonacci-like growth
    // Equal values
    assert(minimumMovesToExceed(3, 3, 7) == 2); // 6, then 9>7
    // One already exceeds
    assert(minimumMovesToExceed(100, 1, 50) == 0);
    // Small numbers
    assert(minimumMovesToExceed(1, 2, 3) == 2); // a=3, b=2 -> a=3 <=3? then b=5
    // Edge: both equal and n large
    assert(minimumMovesToExceed(1, 2, 1000000) == 26);
    return 0;
}
#include <algorithm>

// Returns the minimum number of operations until either a or b exceeds n.
// In each operation, add the larger value to the smaller one.
long long minimumMovesToExceed(long long a, long long b, long long n) {
    long long moves = 0;
    while (a <= n && b <= n) {
        if (a < b) {
            a += b;
        } else {
            b += a;
        }
        ++moves;
    }
    return moves;
}
// The solution is a greedy simulation: in each step, add the smaller value to the larger one (i.e., update the smaller variable by adding the larger to it). This maximizes the growth of the smaller value, ensuring the values grow as fast as possible. Continue until either value exceeds `n`. Because each operation at least doubles the smaller value (since it becomes the sum of the two, which is at least twice the smaller if the larger is at least as big), the number of iterations is logarithmic in `n` relative to the initial values — at most about 60 for 10^18 since Fibonacci-like growth is exponential. Edge cases: if one value already exceeds `n` at the start, return 0. Equal values: we can choose to update `a` (the code snippet uses `if a < b` else update `b`, which effectively treats `a` as smaller when equal). Time complexity is O(log n) per call, space O(1). No overflow because we only add two `long long`s that remain ≤ 2n roughly, but to be safe we can stop as soon as one exceeds `n`; since we add the larger to the smaller, the sum cannot exceed about 2n before termination, which fits in signed 64-bit for n up to 10^18 (2*10^18 < 9.22*10^18).
