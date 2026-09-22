Given arrays `X[1..n]` and `Y[1..n]` representing points on a line, and an integer `m` (1 ≤ m ≤ n), partition the sequence of points into `m` contiguous groups. The cost of a group from index `l` to `r` is defined as the sum over all pairs `(i, j)` with `l ≤ i < j ≤ r` of `Y[i] * (X[j] - X[i])`. Write a C++ function `long long minimumPartitionCost(const std::vector<long long>& X, const std::vector<long long>& Y, int m)` that returns the minimum total cost over all valid partitions into exactly `m` contiguous non-empty groups. The input vectors are 1-indexed internally, with `X[1]` at index 1 and `Y[1]` at index 1 (i.e., vectors are zero-based but you may treat them as 1-indexed by ignoring index 0). The function must compute the answer efficiently for `n` up to 1000 and any `m` up to `n`. You may assume `X` is strictly increasing.

#include <cassert>
#include <vector>
#include <cstdint>

// Declaration of the function being tested
long long minimumPartitionCost(const std::vector<long long>& X,
                               const std::vector<long long>& Y,
                               int m);

int main() {
    // Test 1: simple increasing points, equal weights, m=2
    // X=[1,2,3], Y=[1,1,1]
    // Partition: {1} and {2,3} => cost[2][3]=1*(3-2)=1, total=1
    // Partition: {1,2} and {3} => cost[1][2]=1*(2-1)=1, total=1
    {
        std::vector<long long> X = {0,1,2,3};
        std::vector<long long> Y = {0,1,1,1};
        assert(minimumPartitionCost(X, Y, 2) == 1);
    }

    // Test 2: n=3, m=3 -> each alone cost 0
    {
        std::vector<long long> X = {0,1,5,10};
        std::vector<long long> Y = {0,2,3,4};
        assert(minimumPartitionCost(X, Y, 3) == 0);
    }

    // Test 3: m=1 -> total cost of all pairs
    // Points: (1,2),(3,4),(6,1)
    // cost: pairs (1,2): 2*(3-1)=4, (1,3):2*(6-1)=10, (2,3):4*(6-3)=12 => total 26
    {
        std::vector<long long> X = {0,1,3,6};
        std::vector<long long> Y = {0,2,4,1};
        assert(minimumPartitionCost(X, Y, 1) == 26);
    }

    // Test 4: m=2 on the same points
    // Option 1: {1} and {2,3}: cost[2][3] = 4*(6-3)=12 => total 12
    // Option 2: {1,2} and {3}: cost[1][2]=4, cost[3][3]=0 => total 4
    // Best is 4
    {
        std::vector<long long> X = {0,1,3,6};
        std::vector<long long> Y = {0,2,4,1};
        assert(minimumPartitionCost(X, Y, 2) == 4);
    }

    // Test 5: n=4, m=2
    // X=[1,2,4,8], Y=[1,2,3,4]
    // Compute all possibilities:
    // Partitions:
    // {1} | {2,3,4}: cost[2,4] = cost[2,3]+(8-4)*(2+3)= (4-2)*2=4? Let's compute carefully:
    // cost[2,3] = Y2*(X3-X2) = 2*(4-2)=4; cost[2,4] = cost[2,3] + (8-4)*(Y2+Y3)=4+4*5=24
    // total = cost[1,1]+24=24
    // {1,2} | {3,4}: cost[1,2]=Y1*(X2-X1)=1*(2-1)=1; cost[3,4]=Y3*(X4-X3)=3*(8-4)=12 => total=13
    // {1,2,3} | {4}: cost[1,3]=Y1*(X2-X1)+Y1*(X3-X1)+Y2*(X3-X2)=1*1+1*3+2*2=8; cost[4,4]=0 => total=8
    // Best = 8
    {
        std::vector<long long> X = {0,1,2,4,8};
        std::vector<long long> Y = {0,1,2,3,4};
        assert(minimumPartitionCost(X, Y, 2) == 8);
    }

    // Test 6: n=5, m=3
    // Points: X=[1,3,6,10,15], Y=[2,1,3,1,2]
    // Hard to compute by hand, but we can at least check the result is non-negative and reasonable.
    // Let's compute using brute force for small n to verify.
    // Brute force: all partitions into 3 groups (there are C(4,2)=6). We'll just check one call doesn't crash.
    {
        std::vector<long long> X = {0,1,3,6,10,15};
        std::vector<long long> Y = {0,2,1,3,1,2};
        long long ans = minimumPartitionCost(X, Y, 3);
        // Brute force to verify:
        long long brute = 1LL << 60;
        // partitions of 5 into 3 groups: split after p1 and p2 with 1<=p1<p2<=4
        for (int p1 = 1; p1 <= 3; ++p1) {
            for (int p2 = p1+1; p2 <= 4; ++p2) {
                // compute costs of [1..p1], [p1+1..p2], [p2+1..5]
                // helper to compute cost[l][r]
                auto segCost = [&](int l, int r) {
                    long long c = 0;
                    for (int i = l; i <= r; ++i) {
                        for (int j = i+1; j <= r; ++j) {
                            c += Y[i] * (X[j] - X[i]);
                        }
                    }
                    return c;
                };
                long long total = segCost(1,p1) + segCost(p1+1,p2) + segCost(p2+1,5);
                if (total < brute) brute = total;
            }
        }
        assert(ans == brute);
    }

    return 0;
}

#include <vector>
#include <algorithm>
#include <cstdint>

// Precompute cost[l][r] for all 1<=l<=r<=n (0-indexed internally).
// cost[l][r] = sum over l<=i<j<=r of Y[i]*(X[j]-X[i]).
static void computeCosts(const std::vector<long long>& X,
                         const std::vector<long long>& Y,
                         std::vector<std::vector<long long>>& cost) {
    int n = (int)X.size() - 1; // 1-indexed, X[0] unused
    cost.assign(n + 2, std::vector<long long>(n + 2, 0));
    for (int l = 1; l <= n; ++l) {
        long long sumY = Y[l];
        for (int r = l + 1; r <= n; ++r) {
            cost[l][r] = cost[l][r - 1] + (X[r] - X[r - 1]) * sumY;
            sumY += Y[r];
        }
    }
}

// Returns the minimum total cost to partition points 1..n into exactly m groups.
long long minimumPartitionCost(const std::vector<long long>& X,
                               const std::vector<long long>& Y,
                               int m) {
    int n = (int)X.size() - 1; // 1-indexed inputs
    if (m <= 0 || m > n) return -1; // invalid
    if (m == 1) {
        // Only one group: cost of the whole interval
        std::vector<std::vector<long long>> cost;
        computeCosts(X, Y, cost);
        return cost[1][n];
    }
    if (m == n) return 0; // each point alone, cost 0

    const long long INF = (1LL << 62); // large enough
    std::vector<std::vector<long long>> cost;
    computeCosts(X, Y, cost);

    // dp[k][j] = min cost for first j points into k groups
    // s[k][j] = optimal split point for dp[k][j]
    std::vector<std::vector<long long>> dp(m + 1, std::vector<long long>(n + 1, INF));
    std::vector<std::vector<int>> s(m + 1, std::vector<int>(n + 2, 0));

    // base case k=1
    for (int j = 1; j <= n; ++j) {
        dp[1][j] = cost[1][j];
        s[1][j] = 0;
    }

    // fill for k=2..m
    for (int k = 2; k <= m; ++k) {
        // Process j from n down to 1 to maintain monotonicity of s
        for (int j = n; j >= 1; --j) {
            // ensure we can form k groups with j points: need j >= k
            if (j < k) {
                dp[k][j] = INF;
                s[k][j] = 0;
                continue;
            }
            long long best = INF;
            int bestP = -1;
            int lo = (k - 1);
            if (k > 1 && j < n) lo = std::max(lo, s[k - 1][j]);
            int hi = j - 1;
            if (j < n) hi = std::min(hi, s[k][j + 1]);
            if (j == n) {
                // For j==n, we can search full range from k-1 to n-1
                lo = k - 1;
                hi = n - 1;
            }
            for (int p = lo; p <= hi; ++p) {
                long long val = dp[k - 1][p] + cost[p + 1][j];
                if (val < best) {
                    best = val;
                    bestP = p;
                }
            }
            dp[k][j] = best;
            s[k][j] = bestP;
        }
    }
    return dp[m][n];
}

// This is a classic dynamic programming with divide-and-conquer optimization problem. First, precompute the cost matrix `cost[l][r]` for every contiguous segment. Using a recurrence: `cost[l][r] = cost[l][r-1] + (X[r] - X[r-1]) * sum_{i=l}^{r-1} Y[i]`. This can be done in O(n^2) time and O(n^2) memory. Then define `dp[k][j]` = minimum cost to partition the first `j` points into `k` groups. Base: `dp[1][j] = cost[1][j]` for all `j`. Transition: `dp[k][j] = min_{p from k-1 to j-1} { dp[k-1][p] + cost[p+1][j] }`. The divide-and-conquer optimization applies because `cost[][]` satisfies the quadrangle inequality (it is Monge). We store `s[k][j]` as the optimal split point for `dp[k][j]`, and when computing `dp[k][j]`, we only search `p` in `[s[k-1][j], s[k][j+1]]` (with boundary handling for `j==n`). This reduces the time from O(m n^2) to O(m n log n) or O(m n) on average (here O(m n) due to monotonicity). Edge cases: when `m=1`, answer is `cost[1][n]`; when `m=n`, each point is its own group, cost is 0. The algorithm runs in O(m n) time after O(n^2) preprocessing, with O(n^2) memory. Use `long long` to avoid overflow.
