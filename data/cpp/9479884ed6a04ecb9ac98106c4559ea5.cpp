// Write a C++ function `int hanoiMoveCount(int n)` that returns the total number of individual disk moves required to solve the Tower of Hanoi puzzle with `n` disks, assuming the classic rules (move one disk at a time, never place a larger disk on a smaller one). The function should be recursive and must not simulate the actual moves—just compute the count mathematically. Handle the base case where `n == 0` by returning `0`. Do not use loops, global variables, or any external libraries beyond `<cassert>` and `<iostream>` if needed for testing (but your function itself must be self-contained).
// The classic Tower of Hanoi problem with `n` disks has a well-known recurrence: to move `n` disks from source to target, you first move the top `n-1` disks to an auxiliary rod, then move the largest disk once, then move the `n-1` disks from auxiliary to target. Therefore, the number of moves satisfies `M(n) = 2*M(n-1) + 1` with `M(0) = 0`. This recurrence yields the closed form `M(n) = 2^n - 1`, but the task explicitly requires a recursive implementation, so we implement the recurrence directly. Edge cases: `n = 0` returns `0`; negative input is undefined but we can assume valid non-negative input. Time complexity is `O(n)` because each recursive call reduces `n` by 1, and space complexity is `O(n)` due to the recursion stack depth.
#include <cassert>

// Recursively compute the number of moves for the Tower of Hanoi with n disks.
int hanoiMoveCount(int n) {
    if (n == 0) {
        return 0;
    }
    return 2 * hanoiMoveCount(n - 1) + 1;
}
int main() {
    assert(hanoiMoveCount(0) == 0);
    assert(hanoiMoveCount(1) == 1);
    assert(hanoiMoveCount(2) == 3);
    assert(hanoiMoveCount(3) == 7);
    assert(hanoiMoveCount(4) == 15);
    assert(hanoiMoveCount(5) == 31);
    assert(hanoiMoveCount(10) == 1023);
    assert(hanoiMoveCount(15) == 32767);
    return 0;
}
