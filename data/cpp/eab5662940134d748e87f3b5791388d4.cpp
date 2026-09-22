/*
Write a C++ function `bool canReachACell(int n, int x, int k)` that determines whether a grasshopper, starting at position `x` on a number line with cells numbered `0` through `n`, can ever land exactly on cell `0` by making jumps that change its position by exactly either `+k` or `-k` per jump (jumps may be in either direction, any number of jumps, including zero). The grasshopper starts at `x` and may jump as many times as needed, but cannot leave the range `[0, n]` at any intermediate step. Return `true` if it can land on cell `0`, otherwise return `false`. Note that the provided snippet had bugs (including an empty `if` and flawed logic), so your solution must be correct and robust.
*/

#include <cstddef>

// Determine if starting at position x, with jumps of exactly k in either direction,
// and staying within [0, n], the grasshopper can reach position 0.
bool canReachACell(int n, int x, int k) {
    // n is not needed in logic, but kept for signature completeness.
    // The reachable positions are exactly those congruent to x modulo k.
    // To reach 0, we need x % k == 0.
    return x % k == 0;
}

int main() {
    // Basic cases
    assert(canReachACell(20, 0, 3) == true);   // already at 0
    assert(canReachACell(20, 3, 3) == true);   // one jump left to 0
    assert(canReachACell(20, 6, 3) == true);   // two jumps left to 0
    assert(canReachACell(20, 10, 3) == false); // 10 % 3 != 0
    assert(canReachACell(20, 2, 3) == false);  // 2 % 3 != 0
    assert(canReachACell(20, 17, 3) == false); // 17 % 3 = 2
    assert(canReachACell(20, 18, 3) == true);  // 18 % 3 = 0, jump left 6 times
    // Larger values
    assert(canReachACell(100, 99, 11) == true);  // 99 % 11 = 0
    assert(canReachACell(100, 98, 11) == false); // 98 % 11 = 10
    assert(canReachACell(100, 100, 10) == true); // 100 % 10 = 0
    return 0;
}

// The key observation is that with jumps of size `k`, the set of reachable positions from `x` using any number of `±k` steps is exactly all positions congruent to `x` modulo `k`. However, we are restricted to staying within `[0, n]`. Starting at `x`, we can move left by `k` repeatedly until we either hit `0` or go below `0`. If `x` is a multiple of `k` (i.e., `x % k == 0`), then we can exactly reach `0` by moving left `x/k` steps, and every intermediate cell along the way (`x-k`, `x-2k`, ... `0`) lies within `[0, n]` because it is a decreasing sequence from a valid starting position. If `x` is not a multiple of `k`, then no sequence of `±k` jumps can ever land on `0`, because the remainder modulo `k` is invariant. We do not need to consider moving right at all, because moving right only increases the remainder distance from 0; if we could somehow go right and later come back to 0, the modulo argument still prevents it. Therefore the condition is simply `x % k == 0`. Edge cases: `x == 0` always returns true (zero jumps), `k == 0` would be invalid but in constraints `k > 0`; `n` does not affect the condition because moving left from `x` never exceeds `n`. Time complexity is O(1), space complexity O(1).
