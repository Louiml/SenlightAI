/*
You are given an undirected complete graph with `N` vertices (2 ≤ N ≤ 50), where the cost of each edge is either a non-negative integer (given in an `N×N` matrix `A`, where `A[i][j] = A[j][i]`) or `-1`. A value of `-1` means that this edge is not fixed; instead, its weight becomes a variable `X` (a positive integer). You are also given a threshold `P` and an integer `K`. The function `f(X)` counts the number of unordered pairs of vertices `(i, j)` with `i < j` such that the shortest path distance between `i` and `j` (using edge weights, where `-1` edges are replaced by `X`) is at most `P`. Your task is to write a function `long long countGoodX(long long N, long long P, long long K, vector<vector<long long>> A)` that returns: `"Infinity"` represented as `LLONG_MAX`? No—return a `long long` value as follows: if the number of integer `X ≥ 1` that make `f(X) == K` is infinite, return `-1` (meaning "Infinity"). If no such `X` exists, return `0`. Otherwise, return the count of distinct positive integers `X` for which `f(X) == K`. The function signature must be exactly as above, and you may assume all input values fit in `long long` except that `X` can be as large as `10^10` (so the range of `X` to consider is `1` to `10^10` inclusive). The original code uses binary search on `X` because `f(X)` is non-increasing as `X` increases. Note: in the problem, the matrix `A` has `A[i][i] = 0` always, and `-1` means flexible edge. The graph is complete (every pair has either a fixed non‑negative cost or a flexible `-1`). The answer is the number of integer `X` in `[1, 10^10]` that satisfy `f(X) == K`. If that set is infinite (which can only happen if `f(X) == K` for all sufficiently large `X`), return `-1`. Provide a standalone function that solves this.
*/
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Count how many unordered pairs (i,j) have shortest path distance <= P
// when each flexible edge (-1) is assigned weight X.
static ll countPairs(ll N, ll P, const vector<vector<ll>>& A, ll X) {
    const ll INF = (ll)4e18; // larger than any possible distance
    vector<vector<ll>> dist(N, vector<ll>(N, INF));
    for (int s = 0; s < N; ++s) {
        dist[s][s] = 0;
        priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;
        pq.push({0, s});
        while (!pq.empty()) {
            auto [d, v] = pq.top(); pq.pop();
            if (d != dist[s][v]) continue;
            for (int w = 0; w < N; ++w) {
                ll c = (A[v][w] == -1) ? X : A[v][w];
                if (dist[s][w] > dist[s][v] + c) {
                    dist[s][w] = dist[s][v] + c;
                    pq.push({dist[s][w], w});
                }
            }
        }
    }
    ll cnt = 0;
    for (int i = 0; i < N; ++i)
        for (int j = i+1; j < N; ++j)
            if (dist[i][j] <= P) ++cnt;
    return cnt;
}

// Return the number of integer X in [1, 10^10] such that f(X) == K.
// Return -1 if that set is infinite (i.e., f(10^10) == K).
ll countGoodX(ll N, ll P, ll K, const vector<vector<ll>>& A) {
    const ll MAX_X = 10000000000LL; // 1e10
    ll fHigh = countPairs(N, P, A, MAX_X);
    ll fLow  = countPairs(N, P, A, 1);

    // If f at the upper bound equals K, then for all larger X it also equals K.
    if (fHigh == K) return -1;

    // If the monotonic function never hits K, return 0.
    if (fLow < K || fHigh > K) return 0;

    // Binary search for the smallest X where f(X) <= K.
    ll lo = 1, hi = MAX_X;
    while (lo < hi) {
        ll mid = (lo + hi) / 2;
        if (countPairs(N, P, A, mid) <= K) hi = mid;
        else lo = mid + 1;
    }
    ll L = lo; // first X with f(X) <= K, and since fLow >= K, this is exactly f(X) == K.

    // Binary search for the largest X where f(X) >= K.
    lo = 1; hi = MAX_X;
    while (lo < hi) {
        ll mid = (lo + hi + 1) / 2;
        if (countPairs(N, P, A, mid) >= K) lo = mid;
        else hi = mid - 1;
    }
    ll R = lo; // last X with f(X) >= K, and since fHigh <= K, this is exactly f(X) == K.

    // Interval [L, R] contains exactly the solutions.
    return R - L + 1;
}
#include <cassert>
#include <vector>
using namespace std;
using ll = long long;

// The solution function is defined above (include it here for testing).
// We just prototype it here for the assert block.
ll countGoodX(ll N, ll P, ll K, const vector<vector<ll>>& A);

int main() {
    // Test 1: 2 vertices, one fixed edge cost 5, one flexible (-1).
    // Then f(X) = 1 if X <= P else 0? Actually the distance between nodes is min(5, X)?? Wait, there is only one edge? 
    // The graph is complete, but here N=2, A[0][1] = -1, A[0][0]=0, A[1][1]=0. So the only edge is flexible. 
    // Then f(X) = 1 if X <= P else 0. Let P=5, K=1.
    // For X in [1,5] -> f=1. For X>5 -> f=0. So count is 5.
    {
        ll N=2, P=5, K=1;
        vector<vector<ll>> A = {{0, -1}, {-1, 0}};
        assert(countGoodX(N, P, K, A) == 5);
    }

    // Test 2: Same as above but K=0. Then X>5 gives f=0, infinite set? For X in [6, 1e10]? Actually for X>5, f=0. 
    // The set is infinite because for X >=6 (and up to 1e10 and beyond) f=0. But fHigh = f(1e10) = 0, so return -1.
    {
        ll N=2, P=5, K=0;
        vector<vector<ll>> A = {{0, -1}, {-1, 0}};
        assert(countGoodX(N, P, K, A) == -1);
    }

    // Test 3: No solution. N=2, fixed edge cost 10, P=5, K=1.
    // f(X) = 1 if min(10, X) <= 5? But there is only one edge; since X>=1, 
    // distance = 10 for X>=10? Actually for X in [1,10], distance = X (since X<10), so f(X)=1 if X<=5, 0 if X>5? Wait: 
    // If X=1..5, distance=1..5 <=5 -> f=1. If X=6..9, distance=6..9 >5 -> f=0. If X>=10, distance=10>5 -> f=0.
    // So f(1)=1, f(1e10)=0. K=1 -> solutions X=1..5 -> count 5. Not "no solution". Let's test K=2 (impossible since only one pair) -> should return 0.
    {
        ll N=2, P=5, K=2;
        vector<vector<ll>> A = {{0, 10}, {10, 0}};
        assert(countGoodX(N, P, K, A) == 0);
    }

    // Test 4: N=3, all edges flexible. P=10, K=3 (all pairs). 
    // For any X, distance between any two distinct vertices is X (since only flexible edges). 
    // f(X) = 3 if X <= 10, else 0. So solutions X=1..10 -> count 10. fHigh=0, fLow=3, not -1.
    {
        ll N=3, P=10, K=3;
        vector<vector<ll>> A = {{0, -1, -1}, {-1, 0, -1}, {-1, -1, 0}};
        assert(countGoodX(N, P, K, A) == 10);
    }

    // Test 5: Same as test 4 but K=0. Then for X>10, f=0. fHigh=0, so infinite? Yes.
    {
        ll N=3, P=10, K=0;
        vector<vector<ll>> A = {{0, -1, -1}, {-1, 0, -1}, {-1, -1, 0}};
        assert(countGoodX(N, P, K, A) == -1);
    }

    // Test 6: N=4, one fixed edge cost 1 between 0-1, all others flexible. P=2, K=1 (only pair 0-1 has distance 1, others have X>2? 
    // Actually distance between 0-1 = min(1, X) = 1 always. Distance between any other pair = X (if both flexible) or min(fixed, X). 
    // Only fixed edge is between 0-1, all others flexible. So f(X) = 1 if X>2? Wait: pair 0-1 always 1 ≤2. 
    // Pairs that include 0 and j (j≠1) have distance = X (since edge 0-j flexible) so ≤2 only if X≤2. 
    // Pair 1-j similarly. Pair (2,3) flexible. So for X=1..2, all pairs have distance ≤2 -> f=6. For X≥3, only pair (0,1) has distance 1 ≤2, others have distance ≥3 >2 -> f=1. So f(1)=6, f(1e10)=1. 
    // K=1 -> solutions X≥3 (up to 1e10) -> infinite? Actually for X≥3, f=1, so infinite. So return -1.
    {
        ll N=4, P=2, K=1;
        vector<vector<ll>> A = {
            {0, 1, -1, -1},
            {1, 0, -1, -1},
            {-1, -1, 0, -1},
            {-1, -1, -1, 0}
        };
        assert(countGoodX(N, P, K, A) == -1);
    }

    // Test 7: N=3: A has fixed edge 0-1 = 5, others flexible. P=10, K=2 (pairs 0-1 and maybe one other). 
    // Let's compute: distance 0-1 = min(5, X) = for X≥5 -> 5 ≤10. For X<5, distance =X ≤4? Actually if X<5, distance = X ≤10. So 0-1 always ≤10. 
    // Pair 0-2: distance = X (flexible) ≤10 if X≤10, else >10. Pair 1-2 similarly. So f(X)= 1 (pair0-1) + 2 if X≤10? Actually if X≤10, all three pairs ≤10 -> f=3. If X>10, only 0-1 ≤10 -> f=1. So f(1)=3, f(1e10)=1. K=2 never occurs -> return 0.
    {
        ll N=3, P=10, K=2;
        vector<vector<ll>> A = {
            {0, 5, -1},
            {5, 0, -1},
            {-1, -1, 0}
        };
        assert(countGoodX(N, P, K, A) == 0);
    }

    // Test 8: N=2, fixed edge cost 0? But graph complete with non-negative. 
    // Use fixed edge cost 0 (self? No, 0 for i!=j is allowed? A[i][j] can be 0). 
    // Let A[0][1]=0, A[1][0]=0, P=0, K=1. f(X) is 1 for any X. So infinite? fHigh=1, return -1.
    {
        ll N=2, P=0, K=1;
        vector<vector<ll>> A = {{0, 0}, {0, 0}};
        assert(countGoodX(N, P, K, A) == -1);
    }

    // Test 9: N=2, no flexible edges, fixed cost 3, P=2, K=0. Then f(X) always 0 because distance 3>2. fHigh=0, fLow=0 -> -1.
    {
        ll N=2, P=2, K=0;
        vector<vector<ll>> A = {{0, 3}, {3, 0}};
        assert(countGoodX(N, P, K, A) == -1);
    }

    // Test 10: N=2, fixed cost 1, P=5, K=1. f(X)=1 always, return -1.
    {
        ll N=2, P=5, K=1;
        vector<vector<ll>> A = {{0, 1}, {1, 0}};
        assert(countGoodX(N, P, K, A) == -1);
    }

    return 0;
}
// The key observation is that the function `f(X)` (the count of pairs with shortest path ≤ P) is monotonic non‑increasing as `X` increases. Why? Because increasing `X` only increases the weights of flexible edges (or leaves them unchanged if `X` is large enough that the shortest path uses only fixed edges), so every shortest path distance is non‑decreasing. Therefore, the set of `X` for which `f(X) == K` forms a contiguous interval (possibly empty) of positive integers, but with a nuance: for very large `X`, `f(X)` becomes constant because flexible edges are effectively unusable—they have huge weight. The algorithm is:
// 1. Compute `hi = f(10^10)` and `lo = f(1)`. If `hi == K`, then for all `X ≥ 10^10` (and actually also for `X > 10^10` but we only care up to `10^10`) we have `f(X) == K`? Wait—monotonicity says if `f(10^10) == K`, then for all `X` in `[10^10, ∞)` `f(X) == K`? Actually since `f` is non‑increasing, if `f(10^10) == K`, then for any `X > 10^10`, `f(X) ≤ K`. But could it drop below `K`? Yes, because even for `X` larger than `10^10`, flexible edges get even heavier, but if all flexible edges are already effectively infinite at `X = 10^10`, then `f` is constant from that point. In this problem, we only consider `X` up to `10^10`. However, the original code returns "Infinity" when `f(10^10) == K`. That is because for any `X > 10^10`, the shortest paths using flexible edges become even larger, so `f(X)` can only decrease. If `f(10^10) == K`, then for all `X ≥ 10^10`, `f(X) == K` because `f` is integer‑valued and non‑increasing, but can it drop below `K` later? If at `X = 10^10` the shortest path distances are already determined by fixed edges only (because the flexible edges are so heavy that they are never used in any shortest path ≤ P), then increasing `X` further doesn't change `f`. So indeed, if `f(10^10) == K`, then for all `X ≥ 10^10`, `f(X) == K`. Therefore the number of solutions in `[1, 10^10]` is infinite (since `10^10` is just an upper bound but there are infinitely many larger integers). The original code returns "Infinity" in that case. In our function, we return `-1` to indicate "Infinity". Otherwise, if `f(1) < K` or `f(10^10) > K`, then no solution (return `0`). Otherwise, there is a contiguous interval `[L, R]` within `[1, 10^10]` such that `f(X) == K`. We find the left boundary: the smallest `X` where `f(X) ≤ K` (since monotonic decreasing, we want the first time it drops to `K`). Actually we want `L` = smallest `X` such that `f(X) == K`. Since `f` is non‑increasing, the set where `f(X) == K` is an interval. We binary search:
// - Find `leftBound` = maximum `X` such that `f(X) ≥ K` (i.e., for any `X` smaller than `leftBound`, `f(X) > K`). That gives the rightmost point where the count is still > K. Then `L = leftBound + 1`.
// - Find `rightBound` = minimum `X` such that `f(X) ≤ K` (i.e., for any `X` larger than `rightBound`, `f(X) < K`). That gives the leftmost point where count is already < K. Then `R = rightBound - 1`.
// Actually the original code does two binary searches with careful boundaries. The answer is `R - L + 1` if `L ≤ R`, else `0`. To avoid off‑by‑one errors, we can directly binary search for the smallest `X` with `f(X) ≤ K` (call it `posL`) and the largest `X` with `f(X) ≥ K` (call it `posR`). Then the interval is `[posL, posR]` if `posL ≤ posR`. Implementation details:
// - Compute `fHigh = f(10^10)`, `fLow = f(1)`.
// - If `fHigh == K` → return `-1` (Infinity). (Because for all larger `X`, `f` stays `K`.)
// - If `fLow < K` or `fHigh > K` → return `0`.
// - Now `fLow >= K` and `fHigh <= K`, but since `f` is non‑increasing and integer‑valued, there is at least one `X`. Find the left boundary `L`: the smallest `X` such that `f(X) == K`. Since `f(1) >= K` and `f(10^10) <= K`, the function crosses. Binary search for the smallest `X` with `f(X) ≤ K`. That gives `L`. Then binary search for the largest `X` with `f(X) ≥ K` (or equivalently `f(X) > K-1`). That gives `R`. Then answer is `R - L + 1`.
// We must implement `f(X)` efficiently. Since `N ≤ 50`, we can run Dijkstra from each source (like the original code). Computing all‑pairs shortest paths with edge weight `X` for flexible edges. This takes `O(N * (N log N + N))` = `O(N^2 log N)` per call to `f`. With up to ~60 binary search iterations (since range up to `10^10`), total is fine. Edge cases: `K` can be 0 or up to `N*(N-1)/2`. Also note that `P` can be large. The matrix `A` is symmetric with `A[i][i] = 0`. The flexible edges (`-1`) become `X`. For Dijkstra, we must be careful: the graph is complete, so for each vertex we iterate all `w`. The original code uses `c = X` if `A[v][w] == -1`. That is correct. Also note that the original code uses `1e+18` as infinity for distances, which is safe for `long long` because `P` is at most maybe `10^9`? But we need to be safe with `10^10` edge weights and `N=50`, so maximum path length is `50 * 10^10 = 5e11`, which is less than `1e18`. Time complexity per `f` is `O(N^2 log N + N^2)` = `O(N^2 log N)`. Space `O(N^2)` for the dp matrix.
// We write a clean function `long long countGoodX(...)`. The function returns `-1` for "Infinity", `0` for no solution, or a positive count. Provide a helper `countPairs(X)` that computes `f(X)`. The solution must be self‑contained and include all necessary headers.
