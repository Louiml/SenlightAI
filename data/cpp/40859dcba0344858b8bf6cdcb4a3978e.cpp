Write a standalone C++ function `minimumTotalCost` that takes a vector of non-negative integers representing the heights of stones numbered 0 through N-1. Starting from stone 0, you may jump from stone i to stone i+1 or i+2, incurring a cost equal to the absolute height difference between the two stones. Return the minimum total cost to reach the last stone (index N-1). Your function must solve this using memoization (top-down dynamic programming) with a helper recursive function, and must handle N=1 (return 0), N=2 (single jump), and large N up to 100,000 where heights can be as large as 10^9. Do not use iterative DP; the recursive approach must be used. The function signature is exactly: `long long minimumTotalCost(const std::vector<long long>& heights);`.

The problem is identical to the classic "Frog 1" from AtCoder DP contest. Define a recursive function `dp(i)` that returns the minimum cost to reach stone i. Base case: `dp(0)=0`. For i>0, the cost to reach i is the minimum of (cost to reach i-1 + |h[i]-h[i-1]|) and (if i>=2, cost to reach i-2 + |h[i]-h[i-2]|). To avoid recomputation, store results in a memoization array initialized to a large sentinel (e.g., 2e18). Because heights can be up to 1e9 and differences up to 1e9, and path length up to 100,000, the total cost can be up to ~1e14, so use `long long`. Edge cases: N=1 returns 0 (already at destination); N=2 forces the only jump i-1; ensure recursion terminates and memo is correctly sized to N (or a fixed large size). Time complexity: O(N) because each state is computed once and each state does constant work. Space complexity: O(N) for the memo array (plus O(N) recursion stack in worst case, but since it's linear recursion depth for the chain, stack depth is O(N) but that's acceptable for N=100,000; however, to be safe, note that recursion depth equals N, so for N=100,000 it's fine on typical systems with default stack). The solution uses const correctness and avoids global variables by passing the memo and heights as references.

#include <vector>
#include <cstdlib>
#include <algorithm>

// Core recursive function with memoization.
// Returns minimum cost to reach stone index i.
long long solveFrog(int i, const std::vector<long long>& h, std::vector<long long>& memo) {
    if (memo[i] != -1) return memo[i];
    if (i == 0) return memo[i] = 0;

    long long best = __builtin_inf(); // large sentinel
    // Jump from i-1
    best = std::min(best, solveFrog(i - 1, h, memo) + std::llabs(h[i] - h[i - 1]));
    // Jump from i-2 if possible
    if (i >= 2) {
        best = std::min(best, solveFrog(i - 2, h, memo) + std::llabs(h[i] - h[i - 2]));
    }
    return memo[i] = best;
}

// Entry point: compute minimum total cost to reach the last stone.
long long minimumTotalCost(const std::vector<long long>& heights) {
    int n = (int)heights.size();
    if (n == 0) return 0; // defensive, but task specifies non-empty

    std::vector<long long> memo(n, -1);
    return solveFrog(n - 1, heights, memo);
}

#include <cassert>
#include <vector>

// Assume the solution from above is included here.
// For completeness, we re-declare the function or include the code.

long long minimumTotalCost(const std::vector<long long>& heights);

int main() {
    // Single stone: no cost
    assert(minimumTotalCost({5}) == 0);

    // Two stones: just one jump
    assert(minimumTotalCost({10, 20}) == 10);

    // Three stones: jump 0->2 directly vs 0->1->2
    // h = {1, 100, 3}: direct 0->2 costs |3-1|=2, better than 1+97=98
    assert(minimumTotalCost({1, 100, 3}) == 2);

    // Classic frog1 example
    // h = {2, 9, 4, 5, 1, 6, 10} -> optimal? Let's compute manually
    // dp[0]=0
    // dp[1]=|9-2|=7
    // dp[2]=min(7+|4-9|=7+5=12, |4-2|=2) => 2
    // dp[3]=min(2+|5-4|=3, 7+|5-9|=11) =>3
    // dp[4]=min(3+|1-5|=7, 2+|1-4|=5) =>5
    // dp[5]=min(5+|6-1|=10, 3+|6-5|=4) =>4
    // dp[6]=min(4+|10-6|=8, 5+|10-1|=14) =>8
    assert(minimumTotalCost({2, 9, 4, 5, 1, 6, 10}) == 8);

    // All equal heights: cost 0 regardless of path
    std::vector<long long> flat(100, 7);
    assert(minimumTotalCost(flat) == 0);

    // Strictly increasing by 1: total cost must be N-1
    std::vector<long long> inc(1000);
    for (int i = 0; i < 1000; ++i) inc[i] = i;
    assert(minimumTotalCost(inc) == 999);

    // Large heights to test overflow handling
    std::vector<long long> big = {0, 1000000000LL, 0};
    // dp[1]=1e9, dp[2]=min(1e9+1e9=2e9, |0-0|=0) => 0
    assert(minimumTotalCost(big) == 0);

    // N=2 with large difference
    assert(minimumTotalCost({0, 1000000000LL}) == 1000000000LL);

    // Random small test: known optimal for {1, 2, 100, 1}
    // dp[0]=0
    // dp[1]=1
    // dp[2]=min(1+|100-2|=99, |100-1|=99) =>99
    // dp[3]=min(99+|1-100|=198, 1+|1-2|=2) =>2
    assert(minimumTotalCost({1, 2, 100, 1}) == 2);

    return 0;
}
