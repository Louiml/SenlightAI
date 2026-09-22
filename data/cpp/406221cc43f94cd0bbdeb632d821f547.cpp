Write a C++ function that, given an integer `n`, an `n × n` cost matrix `dat` (where `dat[i][j]` is the cost to repair or activate machine `j` starting from a currently working machine `i`), a binary string `s` of length `n` (where `s[i] == 'Y'` indicates machine `i` is initially working, and `'N'` indicates it is broken), and a required number `k` of working machines, returns the minimum total cost to reach at least `k` working machines by performing repair operations. Each operation pairs an already-working machine `i` with a broken machine `j` and incurs cost `dat[i][j]`, marking `j` as working. You may perform any number of operations in any order, and once a machine is working it stays working. If it is impossible to reach at least `k` working machines, return `-1`. The problem guarantees `1 ≤ n ≤ 16`, `1 ≤ k ≤ n`, and all costs are non-negative integers less than `10000`. The initial state may already have `k` or more working machines, in which case the answer is `0`.

// This is a classic dynamic programming over bitmasks problem. Represent the set of currently working machines as a bitmask of length `n`. The DP state is `dp[mask]` = minimum additional cost needed to reach at least `k` working machines starting from the given mask. The base case: if the number of set bits in `mask` is already `≥ k`, then `dp[mask] = 0` because no more repairs are needed. For a general mask, we iterate over all currently working machines `i` (bits set in `mask`) and all currently broken machines `j` (bits not set). If we repair `j` using `i`, the new mask is `mask | (1 << j)`, and the cost is `dat[i][j] + dp[new_mask]`. We take the minimum over all valid transitions. If no transition leads to a feasible solution (i.e., all children return infinity), then `dp[mask]` is infinity, and we return `-1` for the initial mask. We use memoization with a sentinel value (e.g., `-1` for uncomputed, and a large number like `10000` as "impossible") to avoid recursion cycles and bound the cost. The number of possible masks is at most `2^16 = 65536`, and for each mask we try at most `n^2` transitions, so the total time complexity is `O(2^n * n^2)` which is at most about 16 million operations, and space complexity is `O(2^n)` for the DP array. Edge cases: initial mask already meets `k`, unreachable states, and the case where there are zero working machines initially (but then if `k=0` answer is 0, but constraints say `k≥1`).

#include <vector>
#include <string>
#include <algorithm>
#include <climits>

// Computes the minimum cost to repair at least k machines.
// dat[i][j] = cost to repair machine j using working machine i.
// s[i] == 'Y' means machine i initially works.
// Returns -1 if impossible.
int minRepairCost(int n, const std::vector<std::vector<int>>& dat, const std::string& s, int k) {
    const int INF = 10000; // any cost < 10000, so INF means impossible
    std::vector<int> dp(1 << n, -1); // -1 means uncomputed

    // Recursive lambda with memoization
    std::function<int(int)> solve = [&](int mask) -> int {
        if (dp[mask] != -1) return dp[mask];

        // Count working machines
        int cnt = 0;
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) cnt++;
        }
        if (cnt >= k) return dp[mask] = 0;

        int best = INF;
        // Try repairing a broken machine j using any working machine i
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) { // i is working
                for (int j = 0; j < n; ++j) {
                    if (!(mask & (1 << j))) { // j is broken
                        int newMask = mask | (1 << j);
                        int sub = solve(newMask);
                        if (sub < INF) {
                            best = std::min(best, dat[i][j] + sub);
                        }
                    }
                }
            }
        }
        return dp[mask] = (best == INF ? INF : best);
    };

    // Build initial mask from string s: bit i set if s[i] == 'Y'
    int initialMask = 0;
    for (int i = 0; i < n; ++i) {
        if (s[i] == 'Y') initialMask |= (1 << i);
    }

    int ans = solve(initialMask);
    return (ans >= INF) ? -1 : ans;
}

#include <cassert>
#include <vector>
#include <string>

// Include the solution function here or link it.

int main() {
    // Test 1: Simple 2 machines, need both working
    {
        int n = 2;
        std::vector<std::vector<int>> dat = {{5, 3}, {2, 4}};
        std::string s = "YN"; // machine 0 works, machine 1 broken
        int k = 2;
        // Option: repair 1 using 0 cost 3, or using 1 (but 1 is broken). So min = 3.
        assert(minRepairCost(n, dat, s, k) == 3);
    }

    // Test 2: Already have enough working machines
    {
        int n = 3;
        std::vector<std::vector<int>> dat = {{1,2,3},{4,5,6},{7,8,9}};
        std::string s = "YYN";
        int k = 2;
        assert(minRepairCost(n, dat, s, k) == 0);
    }

    // Test 3: Impossible to reach k (no working machines initially, k>0)
    {
        int n = 2;
        std::vector<std::vector<int>> dat = {{1,1},{1,1}};
        std::string s = "NN";
        int k = 1;
        // No working machine to start repairs, impossible
        assert(minRepairCost(n, dat, s, k) == -1);
    }

    // Test 4: More complex, need all 4, several options
    {
        int n = 4;
        std::vector<std::vector<int>> dat = {
            {0, 1, 5, 5},
            {5, 0, 1, 5},
            {5, 5, 0, 1},
            {1, 5, 5, 0}
        };
        std::string s = "YNNN";
        int k = 4;
        // Possible sequence: 0->1 (1), then 1->2 (1), then 2->3 (1) => total 3.
        // Or 0->3 (5), etc. Minimal is 3.
        assert(minRepairCost(n, dat, s, k) == 3);
    }

    // Test 5: Need exactly k=1, but initial has 0 working but k=1 (but k>=1, so impossible)
    {
        int n = 1;
        std::vector<std::vector<int>> dat = {{10}};
        std::string s = "N";
        int k = 1;
        assert(minRepairCost(n, dat, s, k) == -1);
    }

    // Test 6: Need k=1, initial has one working already
    {
        int n = 3;
        std::vector<std::vector<int>> dat = {{9,9,9},{9,9,9},{9,9,9}};
        std::string s = "YNN";
        int k = 1;
        assert(minRepairCost(n, dat, s, k) == 0);
    }

    // Test 7: Multiple paths, ensure minimum is chosen
    {
        int n = 3;
        std::vector<std::vector<int>> dat = {
            {0, 10, 1},
            {10, 0, 10},
            {1, 10, 0}
        };
        std::string s = "YNN";
        int k = 3;
        // From 0: repair 2 via 0 (cost 1), then repair 1 via 0 (10) or via 2 (10) => total 11.
        // Or repair 1 via 0 (10), then repair 2 via 0 (1) => 11. So answer 11.
        assert(minRepairCost(n, dat, s, k) == 11);
    }

    // Test 8: Large n but simple cost (all 1), need all
    {
        int n = 5;
        std::vector<std::vector<int>> dat(n, std::vector<int>(n, 1));
        std::string s = "YNNNN";
        int k = 5;
        // Need 4 repairs each cost 1 => total 4
        assert(minRepairCost(n, dat, s, k) == 4);
    }

    return 0;
}
