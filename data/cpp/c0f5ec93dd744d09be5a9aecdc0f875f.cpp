// Given an array of `n` 2D points with integer coordinates, and two integers `n` and `d`, write a C++ function `minimumStartingEnergy` that returns the minimum initial energy `E` (a non-negative integer) required to travel from point `0` to point `n-1`. You can move from any point `i` to any point `j` (both directions, but you may revisit points) if your current energy is at least `d * ManhattanDistance(i,j)`. When you arrive at point `k`, you gain `a[k]` energy, where `a[0]` and `a[n-1]` are `0`. You start at point `0` with energy `E` before adding `a[0]`. If reaching `n-1` is impossible for any finite energy, return `-1`. The input guarantees `n` between 2 and 100, `d` between 1 and 100, coordinates between -100 and 100, and `a[i]` between 0 and 100.

The problem is a feasibility search for the smallest initial energy `E`. Since the energy can grow via collected bonuses and the graph is complete (all directed edges allowed as long as energy condition is met), we can simulate a relaxation process similar to Bellman-Ford but where the "distance" constraint is energy-based. For a fixed `E`, we maintain `dp[i]` = maximum energy achievable when arriving at point `i` (before adding its bonus? Actually store after adding the bonus for that point). Initialize `dp[0] = E + a[0]` (since `a[0]=0` in the problem, but let's keep general). Then perform at most `n-1` full relaxation passes over all edges. For an edge `(u,v)` with Manhattan distance `dist`, if `dp[u]` is reachable (not -inf) and `dp[u] >= d * dist`, then we can move to `v` and set `dp[v] = max(dp[v], dp[u] - d * dist + a[v])`. Since we allow revisiting, the maximum energy at any node might increase over iterations; doing `n-1` passes suffices because each pass can propagate gains through at most one new edge in a path, and longer paths would require more than n-1 edges only if cycles are beneficial, but cycles can only add energy if the net energy gain per cycle is positive, which would make the node's dp grow unboundedly — however, since we only need to check reachability of `n-1` with finite energy, and the maximum bonus per node is bounded (100), the maximum useful energy is bounded by `(n-1)*100 + E` and hence a Bellman-Ford style relaxation with `n-1` passes is enough to detect if `n-1` becomes reachable, because any optimal simple path uses at most `n-1` edges (if a cycle is used, it must have non-negative net energy gain, so you can remove it without losing feasibility). Edge cases: unreachable for any E, or answer may be 0 if starting with 0 energy is enough. Binary search E between 0 and some upper bound (e.g., 1e9) works because feasibility is monotonic: if E works, larger E also works. Time complexity: O(log(maxE) * n * n^2) for binary search with n-1 passes over n(n-1)/2 edges, i.e., O(log(1e9) * n^3) ≈ 1e7 operations worst-case, fine. Space O(n).

#include <vector>
#include <algorithm>
#include <climits>

// Compute minimum initial energy to reach point n-1 from point 0.
// points: vector of (x, y), bonus: energy gained at each node (size n, bonus[0]=bonus[n-1]=0)
// d: energy cost per unit Manhattan distance.
// Returns -1 if impossible.
int minimumStartingEnergy(const std::vector<std::pair<int,int>>& points,
                          const std::vector<int>& bonus,
                          int d) {
    int n = static_cast<int>(points.size());
    if (n == 0) return -1;
    if (n == 1) return 0; // already at destination

    // Precompute all pairwise Manhattan distances
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int manhattan = std::abs(points[i].first - points[j].first) +
                            std::abs(points[i].second - points[j].second);
            dist[i][j] = dist[j][i] = manhattan;
        }
    }

    // Feasibility check for a given initial energy E
    auto feasible = [&](int E) -> bool {
        const long long NEG = LLONG_MIN / 4;
        std::vector<long long> dp(n, NEG);
        dp[0] = static_cast<long long>(E) + bonus[0];
        for (int iter = 0; iter < n - 1; ++iter) {
            bool changed = false;
            for (int u = 0; u < n; ++u) {
                if (dp[u] == NEG) continue;
                for (int v = 0; v < n; ++v) {
                    if (u == v) continue;
                    long long need = static_cast<long long>(d) * dist[u][v];
                    if (dp[u] >= need) {
                        long long candidate = dp[u] - need + bonus[v];
                        if (candidate > dp[v]) {
                            dp[v] = candidate;
                            changed = true;
                        }
                    }
                }
            }
            if (!changed) break;
        }
        return dp[n - 1] != NEG;
    };

    // Binary search minimal E
    int low = 0, high = 1000000000, answer = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (feasible(mid)) {
            answer = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return answer;
}

#include <cassert>
#include <vector>
#include <utility>

// Function declaration
int minimumStartingEnergy(const std::vector<std::pair<int,int>>& points,
                          const std::vector<int>& bonus,
                          int d);

int main() {
    // Simple two points, no bonuses, distance 1
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {1,0}};
        std::vector<int> bonus = {0, 0};
        assert(minimumStartingEnergy(pts, bonus, 1) == 1);
    }

    // Two points, distance 5, need exactly 5 energy
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {3,4}};
        std::vector<int> bonus = {0, 0};
        assert(minimumStartingEnergy(pts, bonus, 1) == 5);
    }

    // Triangle with a bonus at intermediate point that reduces needed starting energy
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {2,0}, {4,0}};
        std::vector<int> bonus = {0, 3, 0};
        // d=2, dist(0,1)=2 cost=4, dist(1,2)=2 cost=4
        // Start 0: need >=4 to go to 1, then gain 3, need >=4 to go to 2 => net need 4-3+4=5, so E=5
        assert(minimumStartingEnergy(pts, bonus, 2) == 5);
    }

    // No path possible because edge from 0 to 1 is too expensive and no other route
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {100,0}};
        std::vector<int> bonus = {0, 0};
        // d=1, cost=100, but no bonus, so need E=100, which is feasible, so not unreachable
        assert(minimumStartingEnergy(pts, bonus, 1) == 100);
    }

    // Unreachable because d large and no bonus enough
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {10,0}};
        std::vector<int> bonus = {0, 0};
        // Even with max E=1e9, cost=10*1e9? No, d=1e9, cost=1e10 > 1e9, so impossible
        assert(minimumStartingEnergy(pts, bonus, 1000000000) == -1);
    }

    // Cycle that allows negative energy path? Not relevant, just check a 3-node chain with no bonus
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {1,0}, {2,0}};
        std::vector<int> bonus = {0, 0, 0};
        // Need cost 1 then 1, so E=1
        assert(minimumStartingEnergy(pts, bonus, 1) == 1);
    }

    // Bonus allows starting with 0
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {1,0}, {2,0}};
        std::vector<int> bonus = {0, 10, 0};
        // Start with 0, go to 1 (cost 1), gain 10, go to 2 (cost 1) => works
        assert(minimumStartingEnergy(pts, bonus, 1) == 0);
    }

    // Larger path with multiple bonuses
    {
        std::vector<std::pair<int,int>> pts = {{0,0}, {1,1}, {2,2}, {3,3}};
        std::vector<int> bonus = {0, 5, 0, 0};
        // d=2, dists: 0-1=2 cost4, 1-2=2 cost4, 2-3=2 cost4
        // E=4: go 0->1 (0 left+5=5), then 1->2 (1 left), then 2->3 (-3 fail)
        // E=8: 8->1 (4 left+5=9), 1->2 (5 left), 2->3 (1 left) works, so E=8
        assert(minimumStartingEnergy(pts, bonus, 2) == 8);
    }

    return 0;
}
