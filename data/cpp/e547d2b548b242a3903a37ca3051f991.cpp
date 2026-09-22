// Write a C++ function `countWaysToClimb(int n)` that returns the number of distinct ways to climb a staircase with `n` steps, where at each step you may take either 1 or 2 steps. The function must use a backtracking approach with recursion, pruning any path that would exceed the top step. Assume `n` is a non‑negative integer, with `n = 0` representing the ground (no steps to climb, so there is exactly 1 way: do nothing) and `n = 1` having exactly 1 way. Your implementation should count all valid sequences of 1‑step and 2‑step moves that sum exactly to `n`.
The solution uses recursive backtracking over the choices `{1, 2}`. The recursion tracks the current `state` (number of steps climbed so far) and a reference to a counter. At each call, if `state == n`, we increment the counter because we reached the top. For each choice, we prune if `state + choice > n` (which prevents going beyond the top). Otherwise, we recurse with `state + choice`. Since the recursion explores every valid combination of steps, the total number of leaf nodes equals the Fibonacci‑like count (for `n > 0`, it’s Fib(n+1)). The time complexity is `O(2^n)` in the worst case (exponential) and space complexity is `O(n)` due to the recursion depth. Edge cases: `n = 0` should return 1 (the empty sequence – this requires a base case check or letting the loop naturally find no choices, but the pruning condition `state + choice > n` means no recursion occurs; we need an explicit check `state == n` first so it returns 1). `n = 1` returns 1, `n = 2` returns 2, `n = 3` returns 3, etc.
#include <vector>

// Count the number of distinct ways to climb n steps using steps of 1 or 2.
// Uses backtracking with pruning of paths that exceed n.
// The count is stored in res[0] and passed by reference.
void backtrack(const std::vector<int>& choices, int state, int n, std::vector<int>& res) {
    // If we reached exactly n steps, increment the count.
    if (state == n) {
        res[0]++;
        return; // No further moves from here.
    }
    // Try each possible step choice.
    for (int choice : choices) {
        // Prune: skip choices that would overshoot n.
        if (state + choice > n)
            continue;
        // Recurse with the new state.
        backtrack(choices, state + choice, n, res);
        // No explicit backtracking needed because state is passed by value.
    }
}

// Public function: returns the number of ways to climb n steps (1 or 2 at a time).
int countWaysToClimb(int n) {
    if (n < 0)
        return 0; // Invalid input, but handle gracefully.
    std::vector<int> choices = {1, 2};
    int state = 0;
    std::vector<int> res = {0}; // res[0] will hold the count.
    backtrack(choices, state, n, res);
    return res[0];
}
#include <cassert>

int main() {
    // Test cases for various n values.
    assert(countWaysToClimb(0) == 1);  // Zero steps: one way (do nothing)
    assert(countWaysToClimb(1) == 1);  // 1-step
    assert(countWaysToClimb(2) == 2);  // 1+1, 2
    assert(countWaysToClimb(3) == 3);  // 1+1+1, 1+2, 2+1
    assert(countWaysToClimb(4) == 5);  // Fibonacci: 5
    assert(countWaysToClimb(5) == 8);
    assert(countWaysToClimb(10) == 89); // known value
    assert(countWaysToClimb(-1) == 0);  // invalid input gracefully handled
    assert(countWaysToClimb(0) == 1);   // recheck edge case
    return 0;
}
