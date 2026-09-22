// Write a C++ function that, given a positive integer `n`, computes the number of distinct paths from position `0` to position `n` in a directed graph where from any position `i` (for `0 <= i < n`), you may move to `i+1`, and if `i > 0` you may also move to `2*i` or `2*i+1`, provided the destination is at most `n`. Count paths modulo `998244353`. The function should take `int n` and return an `int` representing the count modulo `998244353`. Note that position `n` itself has no outgoing moves, and the path must end exactly at `n`; starting at `0` is fixed.
#include <cassert>

// Global main function for testing
int main() {
    // Small cases
    assert(countPaths(0) == 1);
    assert(countPaths(1) == 1);
    assert(countPaths(2) == 2); // 0->1->2, 0->1->? actually 0->1, then 1->2 and 2 is reached; also 0->1->2 via i+1 only; wait check: from 0 only to 1. From 1: to 2 (i+1), 2*1=2, 2*1+1=3(>2). So dp[1]=1, dp[2]=1(from 1 via i+1)+1(from 1 via 2*1)=2.
    assert(countPaths(3) == 3); // paths: 0-1-2-3, 0-1-3 (via 2*1+1), 0-1-2? Wait 1->2, 2->3 (i+1) and 2*2=4>3, so from 2 only to 3. So paths: 0-1-2-3, 0-1-3, also 0-1-2? Actually 2->3 yes. Also 0-1->? 1->3 directly. Also 0-1->2->3. That's 2 paths? Let's compute: dp[0]=1; dp[1]=dp[0]=1; dp[2]=dp[1] (from 1 to 2)=1; dp[3]=dp[2] (from 2 to 3, i+1) + dp[1] (from 1 to 3, 2*1+1)=1+1=2? Wait dp[2] is 1, dp[1] is 1, so dp[3]=2. But also from 1? Actually 1 can go to 3 via 2*1+1=3 yes. So total 2 paths: 0-1-2-3 and 0-1-3. So assert countPaths(3)==2.
    // Corrected assertion:
    assert(countPaths(3) == 2);
    assert(countPaths(4) == 3); // 0-1-2-3-4, 0-1-2-4, 0-1-3? 3->4? from 3: 3+1=4, but 3>0 so 6>4 no. So 0-1-3-4 and 0-1-2-4, 0-1-2-3-4. Total 3.
    assert(countPaths(5) == 5);
    // Large n to check modulo and efficiency
    int result = countPaths(200000);
    assert(result >= 0 && result < MOD);
}
#include <vector>

const long long MOD = 998244353;

// Counts the number of paths from 0 to n in the graph described.
int countPaths(int n) {
    if (n < 0) return 0;
    std::vector<long long> dp(n + 1, 0);
    dp[0] = 1;
    for (int i = 0; i < n; ++i) {
        long long cur = dp[i] % MOD;
        if (cur == 0) continue;
        // Move to i+1
        if (i + 1 <= n) {
            dp[i + 1] = (dp[i + 1] + cur) % MOD;
        }
        // Moves to 2*i and 2*i+1 only from i > 0
        if (i > 0) {
            if (2 * i <= n) {
                dp[2 * i] = (dp[2 * i] + cur) % MOD;
            }
            if (2 * i + 1 <= n) {
                dp[2 * i + 1] = (dp[2 * i + 1] + cur) % MOD;
            }
        }
    }
    return static_cast<int>(dp[n] % MOD);
}
// This is a graph path-counting problem best solved with dynamic programming. Define `dp[i]` as the number of ways to reach position `i` from the start `0`. Note that the transitions are directed: from `i` we can go to `i+1`, `2*i`, and `2*i+1` (the latter two only if `i > 0` and destination `<= n`). To compute `dp`, iterate from `0` to `n` in increasing order, and for each `i`, add `dp[i]` to `dp[i+1]` (if `i+1 <= n`), `dp[2*i]` (if `i>0` and `2*i <= n`), and `dp[2*i+1]` (if `i>0` and `2*i+1 <= n`). Since edges go to strictly larger indices (because `i+1 > i`, and `2*i > i` for `i>=1`, and `2*i+1 > i`), processing in increasing `i` is safe. Initialize `dp[0] = 1` (there is exactly one way to be at the start). The answer is `dp[n]` modulo `998244353`. Edge cases: `n=0` returns 1 (stay at start). `n=1`: from 0 only to 1, so 1 path. For large `n`, use `long long` for intermediate sums and modulo after each addition to avoid overflow. Time complexity is `O(n)` and space complexity is `O(n)`.
