/*
Write a C++ function that solves the classic "egg dropping" problem: given `n` eggs and `k` floors, return the minimum number of trials needed in the worst case to find the critical floor from which an egg will not break. The function should be declared as `int eggDrop(int n, int k)`. Assume `n >= 1` and `k >= 0`. The algorithm must be implemented using memoization (top-down dynamic programming) with an auxiliary data structure (e.g., `std::unordered_map`) to store previously computed subproblems, avoiding recomputation. The function must handle the base cases correctly: if `k == 0` or `k == 1`, the answer is `k`; if `n == 1`, the answer is `k` (linear search). For general cases, the function should iterate over all possible first-drop floors `x` from 1 to `k`, compute the worst-case of two recursive branches: one where the egg breaks (reducing floors to `x-1` with `n-1` eggs) and one where it does not (reducing floors to `k-x` with `n` eggs), take the maximum of these two results, then take the minimum over all `x`, and finally add 1. The solution must be self-contained, use only standard headers, and be optimized with memoization to handle inputs up to, say, `n = 3` and `k = 1000` without excessive recursion depth or time.
*/

#include <unordered_map>
#include <string>
#include <algorithm>
#include <climits>

// Return the minimum worst-case number of trials for n eggs and k floors.
// Uses top-down dynamic programming with memoization.
int eggDrop(int n, int k) {
    static std::unordered_map<std::string, int> memo;
    // Base cases
    if (k == 0 || k == 1) {
        return k;
    }
    if (n == 1) {
        return k;
    }
    // Check memo
    std::string key = std::to_string(n) + "|" + std::to_string(k);
    auto it = memo.find(key);
    if (it != memo.end()) {
        return it->second;
    }
    int minimum = INT_MAX;
    for (int x = 1; x <= k; ++x) {
        // Branch where egg breaks: (n-1) eggs, x-1 floors
        int breaks = eggDrop(n - 1, x - 1);
        // Branch where egg survives: n eggs, k-x floors
        int survives = eggDrop(n, k - x);
        int worst = std::max(breaks, survives);
        minimum = std::min(minimum, worst);
    }
    int result = minimum + 1;
    memo[key] = result;
    return result;
}

#include <cassert>

int main() {
    // Base cases
    assert(eggDrop(2, 0) == 0);
    assert(eggDrop(2, 1) == 1);
    assert(eggDrop(1, 10) == 10);
    // Known values
    assert(eggDrop(2, 10) == 4);   // classic 2 eggs, 10 floors
    assert(eggDrop(2, 100) == 14); // 2 eggs, 100 floors
    assert(eggDrop(3, 14) == 4);   // 3 eggs, 14 floors (binary-like)
    assert(eggDrop(3, 100) == 9);  // 3 eggs, 100 floors
    // Larger case
    assert(eggDrop(2, 1000) == 45); // sqrt(2*1000) approx
    // Different n with same k
    assert(eggDrop(10, 5) == 3);    // enough eggs, binary search
    assert(eggDrop(5, 100) == 7);   // more eggs reduce trials
    // Consistency: more eggs should not increase trials
    assert(eggDrop(4, 30) <= eggDrop(3, 30));
    return 0;
}

// The problem is a dynamic programming classic. The state is defined by `(n, k)`, representing the number of eggs and floors. The recurrence is `eggDrop(n, k) = 1 + min_{x=1..k} max(eggDrop(n-1, x-1), eggDrop(n, k-x))`, where `x` is the floor from which we drop an egg. If the egg breaks, we need to search the lower `x-1` floors with one fewer egg; if it doesn't break, we search the upper `k-x` floors with the same number of eggs. The base cases are: when `k == 0` (no floors) or `k == 1` (one floor), we need exactly `k` trials (0 or 1). When `n == 1` (only one egg), we must try each floor from bottom to top, so we need `k` trials. Memoization stores results for each `(n, k)` key, typically encoded as a string `"n|k"` in an unordered_map, to avoid recomputation. The time complexity is `O(n * k^2)` in the worst case because for each state `(n, k)` we iterate over `k` possible `x` values, and there are `n * k` states. With memoization, each state is computed once. Space complexity is `O(n * k)` for the memo table. Edge cases include `k = 0` (should return 0), `k = 1` (return 1), and `n` larger than `ceil(log2(k))` where the answer becomes `ceil(log2(k))` (but the algorithm handles it automatically). For practical inputs like `n=2, k=10`, the answer is 4.
