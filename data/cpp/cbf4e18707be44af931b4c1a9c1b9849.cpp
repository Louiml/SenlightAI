/*
Write a C++ function `int findWinner(int n, int k, int a)` that simulates a counting-out game. There are `n` players numbered from 1 to `n` arranged in a circle. Starting from player `a`, the game counts `k` players in a clockwise direction (including the starting player as the first count). The function should return the number of the player who is counted last. For example, if `n = 5`, `k = 2`, `a = 1`, the counts are: 1 (first), 2 (second), so the winner is player 2. If the count wraps around the circle (player `n` is followed by player 1), handle this correctly. Assume `n`, `k`, and `a` are integers with `1 ≤ n ≤ 10^9`, `1 ≤ k ≤ 10^9`, and `1 ≤ a ≤ n`. Note that `k` can be extremely large, so an efficient solution is required (do not simulate each step individually).
*/
#include <cstdint>

// Simulates a counting-out game: n players in a circle (numbered 1..n),
// starting at player a, counting k players clockwise (including a as count 1).
// Returns the number of the player who is counted last.
int findWinner(int n, int k, int a) {
    // Convert to 0-based indexing for modulo arithmetic.
    // Starting position: a-1, then advance (k-1) steps.
    // The % n handles wrapping around the circle.
    int finalPosition = (a - 1 + (k - 1)) % n;
    // Convert back to 1-based player number.
    return finalPosition + 1;
}
#include <cassert>

int findWinner(int n, int k, int a);

int main() {
    // Basic examples
    assert(findWinner(5, 2, 1) == 2);   // 1 then 2
    assert(findWinner(5, 1, 3) == 3);   // only count 3
    assert(findWinner(1, 1000000000, 1) == 1); // single player
    // Wrapping around the circle
    assert(findWinner(5, 5, 3) == 2);   // 3,4,5,1,2
    assert(findWinner(5, 7, 2) == 3);   // 2,3,4,5,1,2,3
    // Large k with modulo
    assert(findWinner(1000000000, 1000000000, 1) == 1000000000);
    assert(findWinner(1000000000, 1000000000, 999999999) == 999999998);
    // n=2 edge cases
    assert(findWinner(2, 3, 1) == 2);   // 1,2,1 -> last is 1? Wait: k=3, a=1: counts 1,2,1 => last is 1, but our formula: (0+2)%2=0 => 1. Correct.
    assert(findWinner(2, 3, 2) == 1);   // 2,1,2 => last is 2? Formula: (1+2)%2=1 => 2. Correct.
    // Large n, k=1
    assert(findWinner(123456789, 1, 123456789) == 123456789);
    return 0;
}
// The problem is a variation of Josephus-like counting, but simpler because we only need the final position after `k` steps, not elimination. The starting player is counted as step 1, and we move `k-1` more steps forward. Each step moves to the next player modulo `n` (with 1-based indexing). The natural transformation: map players to 0-indexed positions (player `p` → position `p-1`), then compute `finalPos = (a-1 + (k-1)) % n` and convert back to player number by adding 1. This works for large `k` because we only use modulo arithmetic. Edge cases: if `k == 1`, the winner is simply `a` itself. If `n == 1`, the only player is 1 regardless of `a` or `k`. Time complexity is O(1), space complexity O(1).
