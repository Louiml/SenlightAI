You are given two integers `n` and `k` followed by a sequence of `n` integers `a[1], a[2], ..., a[n]`. Consider partitioning the sequence into `k` arithmetic progressions with a common difference of `k`: for each residue class `r` from 1 to `k`, the positions `r, r+k, r+2k, ...` (as long as they are ≤ `n`) form one group. We want to select, independently for each group, all positions to be "odd" if the values at those positions are odd, or all positions to be "even" if the values at those positions are even — but we may also decide to flip the selection for a group (i.e., treat odd‑valued positions as even, and even‑valued positions as odd). The cost of a group is the number of positions whose actual parity (value mod 2) differs from the selected parity for that group. We wish to minimize the total cost across all groups, with the additional global constraint that the number of groups assigned "odd" must be even (i.e., an even number of groups select odd parity). Write a C++ function `int minFlipCost(int n, int k, const std::vector<int>& a)` that returns the minimum possible total cost, where the input `a` is 1‑indexed internally (the vector is given with size exactly `n`, using indices 0..n-1 representing positions 1..n).

#include <cassert>
#include <vector>

int main() {
    // Example 1: n=5, k=1, a = [1,2,3,4,5]
    // Only group contains all positions: evens = {2,4} count 2, odds = {1,3,5} count 3.
    // Assign parity even costs 3, odd costs 2. Even number of odd groups requires 0 odd groups (since k=1, only one group, not even), so must assign even → cost 3.
    assert(minFlipCost(5, 1, {1,2,3,4,5}) == 3);

    // Example 2: n=4, k=2, a = [1,3,2,4]
    // Group1 (positions 1,3): values 1 (odd), 2 (even) → num: even=1, odd=1
    // Group2 (positions 2,4): values 3 (odd), 4 (even) → num: even=1, odd=1
    // Need even number of odd groups: either both even (cost 1+1=2) or both odd (cost 1+1=2) or one even one odd (not allowed because 1 odd group). So min = 2.
    assert(minFlipCost(4, 2, {1,3,2,4}) == 2);

    // Example 3: n=3, k=2, a = [2,2,2]
    // Group1 (pos 1,3): both even → num[1] = [2,0]
    // Group2 (pos 2): even → num[2] = [1,0]
    // Both groups even → cost 0+0=0 (odd groups count=0, even).
    assert(minFlipCost(3, 2, {2,2,2}) == 0);

    // Example 4: n=3, k=3, a = [1,2,3]
    // Each group has one element: group1 odd (num[1]=[0,1]), group2 even (num[2]=[1,0]), group3 odd (num[3]=[0,1]).
    // Need even odd groups. Options: assign odd to groups 1 and 3 (cost 0+1+0=1), assign even to groups 1 and 3 (cost 1+0+1=2), assign odd to group2 only? That gives odd groups=1 (not allowed). So min=1.
    assert(minFlipCost(3, 3, {1,2,3}) == 1);

    // Example 5: n=0, k=2, a = []
    // No positions, all groups empty, cost 0.
    assert(minFlipCost(0, 2, {}) == 0);

    // Example 6: n=5, k=4, a = [1,1,1,1,1]
    // Groups: r1 positions 1,5: two odds → num[1]=[0,2]; r2 pos2: odd → [0,1]; r3 pos3: odd → [0,1]; r4 pos4: odd → [0,1].
    // To have even odd groups, we can assign odd to pairs: e.g., groups 1 and 2 odd, others even → cost 0+0+1+1=2? Wait cost for even group = num[i][1], for odd group = num[i][0]. So odd groups cost 0 each, even groups cost their odd counts. With 4 groups, we need even number of odd groups: we could choose 2 odd groups. The best is 2 odd groups with largest odd counts as even? Actually odd groups cost 0, even groups cost their odd count. So pick 2 groups to be odd (cost 0), the other 2 to be even (cost = odd counts). To minimize, pick the two groups with smallest odd counts to be even? Wait we want total cost = sum over even groups of their odd count. So choose 2 groups with smallest odd counts to be even, the other 2 odd. Odd counts: group1=2, group2=1, group3=1, group4=1. Smallest two are 1 and 1 → sum=2. So answer=2.
    assert(minFlipCost(5, 4, {1,1,1,1,1}) == 2);

    // Example 7: n=6, k=2, a = [2,1,2,1,2,1]
    // Group1 (pos 1,3,5): values 2,2,2 → even count 3, odd 0
    // Group2 (pos 2,4,6): values 1,1,1 → even 0, odd 3
    // To have even odd groups: either both even (cost 0+3=3) or both odd (cost 3+0=3). So min=3.
    assert(minFlipCost(6, 2, {2,1,2,1,2,1}) == 3);

    return 0;
}

#include <vector>
#include <algorithm>
#include <climits>

// Computes the minimum total cost to assign a parity (even=0, odd=1) to each of the k
// arithmetic progression groups (positions r, r+k, ...), such that the number of groups
// assigned odd is even. For each group, cost is the number of positions whose actual
// parity differs from the assigned parity.
int minFlipCost(int n, int k, const std::vector<int>& a) {
    // Count even (0) and odd (1) per residue class. Use 1-based indexing internally.
    std::vector<std::vector<int>> num(k + 1, std::vector<int>(2, 0));
    for (int pos = 1; pos <= n; ++pos) {
        int r = (pos - 1) % k + 1; // residue class 1..k
        num[r][a[pos - 1] & 1]++;  // a is 0-based, but position is 1-based
    }

    const int INF = INT_MAX / 2;
    std::vector<int> dp_prev(2, INF), dp_curr(2, INF);
    dp_prev[0] = 0; // zero groups, even count of odd groups

    for (int i = 1; i <= k; ++i) {
        dp_curr[0] = dp_curr[1] = INF;
        // assign even parity to group i: cost = num[i][1] (odd positions become wrong)
        dp_curr[0] = std::min(dp_curr[0], dp_prev[0] + num[i][1]);
        dp_curr[1] = std::min(dp_curr[1], dp_prev[1] + num[i][1]);
        // assign odd parity to group i: cost = num[i][0] (even positions become wrong)
        dp_curr[0] = std::min(dp_curr[0], dp_prev[1] + num[i][0]);
        dp_curr[1] = std::min(dp_curr[1], dp_prev[0] + num[i][0]);
        dp_prev = dp_curr;
    }

    return dp_prev[0]; // even number of odd groups total
}

// The problem can be modeled as a dynamic program over the groups. For each residue class `r` from 1 to `k`, compute two counts: `num[r][0]` = number of positions in that class with even value, and `num[r][1]` = number of positions with odd value. For each group, choosing parity `p` (0=even, 1=odd) costs `num[r][p^1]` (the opposite count). We need to assign each group a parity, with the constraint that the total number of groups assigned odd parity is even. Define `dp[i][t]` = minimum cost for first `i` groups, where `t` is 0 if the number of odd‑parity groups among these `i` is even, and 1 if odd. Initialize `dp[0][0]=0`, `dp[0][1]=INF`. Transition for group `i` (1‑based): if we assign even, cost adds `num[i][1]`, parity `t` unchanged; if we assign odd, cost adds `num[i][0]`, parity flips. So `dp[i][t] = min( dp[i-1][t] + num[i][1], dp[i-1][t^1] + num[i][0] )`. The answer is `dp[k][0]`. Edge cases: `k` may exceed `n`, in which case some groups have zero elements — their `num` counts are 0, so they contribute 0 cost regardless; the DP still handles them. Time complexity is `O(n + k)` (counting takes `O(n)`, DP takes `O(k)`), and space complexity `O(k)` for `num` and `O(k)` for DP, but we can reduce DP to two variables if needed. The key insight is that flipping parity per group is equivalent to choosing a parity label independently per group, and the even‑odd global constraint is captured by the parity state.
