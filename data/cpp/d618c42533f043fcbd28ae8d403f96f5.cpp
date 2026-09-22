// Write a C++ function `int minimumCostToGuess(int n)` that solves the following game: Given a number `n`, you must guess a secret integer chosen uniformly at random from `1` to `n`. Each time you guess a number `x` that is not the secret, you pay `x` dollars, and you are told whether the secret is higher or lower. The game ends when you guess correctly (you do not pay for the correct guess). The goal is to find the **minimum guaranteed cost** — i.e., the smallest amount of money such that there exists a strategy guaranteeing you will guess the secret without spending more than that amount, regardless of which number was chosen. Your function must return this minimum guaranteed cost. For example, for `n = 1`, the answer is `0` (you guess 1 and win immediately). For `n = 2`, if you guess 1 first, a wrong guess (secret = 2) costs you 1, then you guess 2 correctly, total = 1; if you guess 2 first, a wrong guess costs 2, then you guess 1, total = 2; the optimal strategy is to guess 1 first, so answer = 1. The function must handle `n` up to at least 50, and must be efficient.
#include <cassert>

int main() {
    // Base cases
    assert(minimumCostToGuess(0) == 0);
    assert(minimumCostToGuess(1) == 0);

    // Known values from problem statement / brute force
    assert(minimumCostToGuess(2) == 1);
    assert(minimumCostToGuess(3) == 2);
    assert(minimumCostToGuess(4) == 4);
    assert(minimumCostToGuess(5) == 6);
    assert(minimumCostToGuess(6) == 8);
    assert(minimumCostToGuess(7) == 10);
    assert(minimumCostToGuess(8) == 12);
    assert(minimumCostToGuess(10) == 16);
    assert(minimumCostToGuess(50) == 200); // Known answer for n=50 from LeetCode

    return 0;
}
#include <vector>
#include <algorithm>
#include <climits>

// Returns the minimum guaranteed cost to guess a secret number from 1 to n.
int minimumCostToGuess(int n) {
    if (n <= 1) return 0;

    // dp[i][j] = minimum guaranteed cost for interval [i, j]
    // We allocate size n+2 to safely index dp[i][k-1] and dp[k+1][j] without bounds issues.
    std::vector<std::vector<int>> dp(n + 2, std::vector<int>(n + 2, 0));

    // Build DP table by increasing interval length
    for (int len = 2; len <= n; ++len) {
        for (int start = 1; start + len - 1 <= n; ++start) {
            int end = start + len - 1;
            int best = INT_MAX;
            for (int pivot = start; pivot <= end; ++pivot) {
                // worst-case cost if we guess 'pivot'
                int cost = pivot + std::max(dp[start][pivot - 1], dp[pivot + 1][end]);
                best = std::min(best, cost);
            }
            dp[start][end] = best;
        }
    }

    return dp[1][n];
}
// This problem is a classic dynamic programming problem known as "Guess Number Higher or Lower II" (LeetCode 375). The optimal strategy involves picking a pivot guess `x` in `[start, end]`. If you guess `x` and it is not the secret, you will be told whether the secret is lower (in `[start, x-1]`) or higher (in `[x+1, end]`). To guarantee success in the worst case, you must prepare for the more expensive of the two subproblems, because the adversary (or worst-case secret) will force you into the harder side. So the cost of guessing `x` is `x` (the payment for the wrong guess) plus the maximum of the costs for the two subintervals. You choose `x` to minimize this worst‑case total. The base case is when the interval is empty (`start > end`) or has one element (`start == end`): in both cases you can guess the only (or no) number correctly without payment, so cost is `0`. The recurrence is:
// `dp[start][end] = min_{x in [start, end]} ( x + max(dp[start][x-1], dp[x+1][end]) )`.
// We can solve this via memoization (top‑down) or iterative tabulation (bottom‑up). The bottom‑up approach builds the dp table by increasing interval length. Time complexity is O(n³) because for each of O(n²) intervals we try O(n) pivots. Space complexity is O(n²) for the DP table. Edge cases include `n=0` (should return 0) and `n=1` (return 0). For large `n`, the result grows roughly like O(n log n) but fits well within `int` for `n` up to a few hundred; the problem constrains to at least 50.
