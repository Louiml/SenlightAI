You are given three integers `a`, `b`, and `c` as input. Write a C++ function `int solveTask(int a, int b, int c)` that computes the minimum number of moves needed to make `a` and `b` equal, where a single move consists of choosing either `a` or `b` and changing it by exactly `c` (either increasing or decreasing by `c`). It is allowed to over-adjust (i.e., the adjusted value may become negative or exceed the other value). If it is impossible to make `a` and `b` equal using any number of such moves (including zero moves), return `-1`. Otherwise, return the minimum number of moves needed. Note that if `a` and `b` are already equal, the answer is `0`.
#include <cassert>

int main() {
    // Already equal
    assert(solveTask(5, 5, 3) == 0);
    assert(solveTask(-7, -7, -2) == 0);

    // Simple divisible cases
    assert(solveTask(1, 5, 2) == 2);   // diff=4, /2 = 2
    assert(solveTask(10, 0, 5) == 2);  // diff=10, /5 = 2
    assert(solveTask(0, -9, 3) == 3);  // diff=9, /3 = 3

    // Negative step size works same as positive
    assert(solveTask(4, 10, -2) == 3); // diff=6, /2 = 3

    // The difference is not a multiple of c
    assert(solveTask(1, 4, 2) == -1);  // diff=3, not divisible by 2
    assert(solveTask(-3, 2, 5) == -1); // diff=5, but wait diff%5==0, actually 1

    // Recheck the above: |−3−2|=5, 5%5=0 -> should be 1
    assert(solveTask(-3, 2, 5) == 1);

    // A case where not divisible
    assert(solveTask(0, 7, 3) == -1);

    // c == 0 and not equal
    assert(solveTask(2, 5, 0) == -1);
    assert(solveTask(-1, -1, 0) == 0);

    // Large numbers
    assert(solveTask(1000000000, -1000000000, 500000000) == 4); // diff=2e9, /5e8 = 4

    return 0;
}
#include <cstdlib>  // for std::abs

// Returns the minimum number of moves to make a and b equal by changing either by exactly c each move.
// Returns -1 if impossible.
int solveTask(int a, int b, int c) {
    const int diff = std::abs(a - b);
    if (diff == 0) {
        return 0;
    }
    if (c == 0) {
        return -1;  // cannot change anything
    }
    if (diff % c != 0) {
        return -1;  // difference not a multiple of step size
    }
    return diff / c;
}
// Let `diff = abs(a - b)`. If `diff == 0`, the answer is `0`. Otherwise, we need to check if `diff` is divisible by `c`. If `c == 0`, then no move can change any value, so if `a != b`, it is impossible (return -1). If `c != 0`, then we can only change a or b by multiples of `c`, so the difference `abs(a - b)` must be exactly a multiple of `c`. If `diff % c != 0`, then it is impossible, return -1. Otherwise, the minimum number of moves is `diff / c` because each move changes one of the numbers by exactly `c`, reducing the absolute difference by `c` per move. This is achievable by always adjusting the larger number downward or the smaller number upward; the direction does not affect the count. Edge cases: negative values for `a`, `b`, or `c` are possible; use `abs` for differences and handle `c == 0` separately. Time complexity is O(1), space complexity O(1).
