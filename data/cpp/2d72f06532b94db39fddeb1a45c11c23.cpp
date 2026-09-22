/*
You are given a 2D plane with two fixed "silo" points, S1 and S2. There are N "barn" points, each of which must be assigned to either S1 or S2 as its supply source. A barn assigned to S1 has a Manhattan distance `d1[i]` to S1; assigned to S2 has distance `d2[i]` to S2. Additionally, there are A pairs of barns that "hate" each other: they must NOT be assigned to the same source. There are B pairs of barns that "like" each other: they MUST be assigned to the same source. Define the maximum travel distance among all barns as the maximum over all barns of their assigned distance (either d1 or d2). You may also connect S1 and S2 with a road of Manhattan length `len = |S1.x - S2.x| + |S1.y - S2.y|`, but you do not have to use it; the road is only relevant if you want to allow a barn to travel to the other silo (i.e., a barn assigned to S1 may travel to S2 only if the road is used, adding extra distance `len` to its travel). The goal is to find the smallest possible value `D` such that there exists an assignment of every barn to either S1 or S2 (respecting hate and like constraints) and for any two barns i, j (i<j), the travel distance is not too large: specifically, for any pair, the sum of their individual travel distances (each either d1 or d2) must be ≤ D, with the additional rule that if they are assigned to different silos, you must add the road length `len` to their combined distance (because they need to share the road). Write a C++ function `int minMaxTravel(int N, int A, int B, int S1x, int S1y, int S2x, int S2y, const std::vector<std::pair<int,int>>& barnCoords, const std::vector<std::pair<int,int>>& hatePairs, const std::vector<std::pair<int,int>>& likePairs)` that returns the minimum possible value of D, or -1 if no assignment exists for any D (i.e., the constraints are contradictory). Use 0‑based indices for barns (0..N-1) in the hate and like pairs. The coordinates are integers with absolute value ≤ 10^5. The maximum possible D is 5,000,000 (you may assume an upper bound). Provide a solution using 2-SAT and binary search.
*/
#include <vector>
#include <cstring>
#include <algorithm>
#include <cmath>

class TwoSatSolver {
public:
    int n;
    std::vector<std::vector<int>> adj;
    std::vector<int> mark;
    std::vector<int> stack;
    int top;

    TwoSatSolver(int vars) : n(vars*2), adj(vars*2), mark(vars*2, 0), stack(vars*2) { top = 0; }

    void addEdge(int u, int v) { // u => v
        adj[u].push_back(v);
    }

    void addImplication(int a, int b) { // a implies b
        addEdge(a, b);
    }

    bool dfs(int u) {
        if (mark[u ^ 1]) return false;
        if (mark[u]) return true;
        mark[u] = true;
        stack[top++] = u;
        for (int v : adj[u]) {
            if (!dfs(v)) return false;
        }
        return true;
    }

    bool solve() {
        for (int i = 0; i < n; i += 2) {
            if (!mark[i] && !mark[i^1]) {
                top = 0;
                if (!dfs(i)) {
                    while (top > 0) mark[stack[--top]] = false;
                    if (!dfs(i^1)) return false;
                }
            }
        }
        return true;
    }
};

// Main solution function
int minMaxTravel(int N, int A, int B, int S1x, int S1y, int S2x, int S2y,
                 const std::vector<std::pair<int,int>>& barnCoords,
                 const std::vector<std::pair<int,int>>& hatePairs,
                 const std::vector<std::pair<int,int>>& likePairs) {
    // Precompute distances
    std::vector<int> d1(N), d2(N);
    int len = std::abs(S1x - S2x) + std::abs(S1y - S2y);
    for (int i = 0; i < N; ++i) {
        d1[i] = std::abs(barnCoords[i].first - S1x) + std::abs(barnCoords[i].second - S1y);
        d2[i] = std::abs(barnCoords[i].first - S2x) + std::abs(barnCoords[i].second - S2y);
    }

    // Binary search
    int low = 0, high = 5000000, ans = -1;
    while (low <= high) {
        int mid = (low + high) / 2;
        TwoSatSolver solver(N);

        // Hate constraints: i != j => (i ∨ j) and (¬i ∨ ¬j)
        for (const auto& p : hatePairs) {
            int u = p.first << 1;
            int v = p.second << 1;
            solver.addImplication(u, v ^ 1); // u => ¬v
            solver.addImplication(u ^ 1, v); // ¬u => v
            solver.addImplication(v, u ^ 1); // v => ¬u
            solver.addImplication(v ^ 1, u); // ¬v => u
        }

        // Like constraints: i == j => (i ∨ ¬j) and (¬i ∨ j)
        for (const auto& p : likePairs) {
            int u = p.first << 1;
            int v = p.second << 1;
            solver.addImplication(u, v); // u => v
            solver.addImplication(u ^ 1, v ^ 1); // ¬u => ¬v
            solver.addImplication(v, u); // v => u
            solver.addImplication(v ^ 1, u ^ 1); // ¬v => ¬u
        }

        // Distance constraints for all pairs i<j
        for (int i = 0; i < N; ++i) {
            for (int j = i+1; j < N; ++j) {
                int u = i << 1; // false = S1, true = S2
                int v = j << 1;
                // Both to S1: forbidden if d1[i]+d1[j] > mid
                if (d1[i] + d1[j] > mid) {
                    solver.addImplication(u, v ^ 1); // if i=>S1 then j cannot be S1 => j=>S2
                    solver.addImplication(v, u ^ 1);
                }
                // Both to S2: forbidden if d2[i]+d2[j] > mid
                if (d2[i] + d2[j] > mid) {
                    solver.addImplication(u ^ 1, v); // if i=>S2 then j cannot be S2 => j=>S1
                    solver.addImplication(v ^ 1, u);
                }
                // i->S1, j->S2: forbidden if d1[i]+d2[j]+len > mid
                if (d1[i] + d2[j] + len > mid) {
                    solver.addImplication(u, v); // if i=>S1 then j cannot be S2 => j=>S1
                    solver.addImplication(v ^ 1, u ^ 1); // if j=>S2 then i cannot be S1 => i=>S2
                }
                // i->S2, j->S1: forbidden if d2[i]+d1[j]+len > mid
                if (d2[i] + d1[j] + len > mid) {
                    solver.addImplication(u ^ 1, v ^ 1); // if i=>S2 then j cannot be S1 => j=>S2
                    solver.addImplication(v, u); // if j=>S1 then i cannot be S2 => i=>S1
                }
            }
        }

        if (solver.solve()) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return ans;
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Just two barns, no constraints, same silo distance small
    {
        int N=2, A=0, B=0;
        int S1x=0, S1y=0, S2x=10, S2y=0;
        std::vector<std::pair<int,int>> barns = {{0,1}, {0,-1}};
        std::vector<std::pair<int,int>> hate, like;
        int ans = minMaxTravel(N, A, B, S1x, S1y, S2x, S2y, barns, hate, like);
        assert(ans == 2); // both to S1, d1 = 1 each, sum = 2
    }
    // Test 2: Contradictory hate and like on same pair
    {
        int N=2, A=1, B=1;
        int S1x=0, S1y=0, S2x=10, S2y=0;
        std::vector<std::pair<int,int>> barns = {{0,1}, {0,-1}};
        std::vector<std::pair<int,int>> hate = {{0,1}};
        std::vector<std::pair<int,int>> like = {{0,1}};
        int ans = minMaxTravel(N, A, B, S1x, S1y, S2x, S2y, barns, hate, like);
        assert(ans == -1);
    }
    // Test 3: Two barns far apart on opposite sides, must be on different silos (hate) to reduce distance
    {
        int N=2, A=1, B=0;
        int S1x=0, S1y=0, S2x=100, S2y=0;
        std::vector<std::pair<int,int>> barns = {{-10,0}, {110,0}};
        std::vector<std::pair<int,int>> hate = {{0,1}};
        std::vector<std::pair<int,int>> like;
        // D must allow cross assignment: d1[0]=10, d2[1]=10, len=100 => sum=120
        // Or both to S1: 10+110=120, both to S2: 110+10=120, cross: 10+10+100=120
        // Minimum D=120
        int ans = minMaxTravel(N, A, B, S1x, S1y, S2x, S2y, barns, hate, like);
        assert(ans == 120);
    }
    // Test 4: Three barns, like chain, all must be same, so pick the closer silo
    {
        int N=3, A=0, B=2;
        int S1x=0, S1y=0, S2x=10, S2y=0;
        std::vector<std::pair<int,int>> barns = {{1,0}, {2,0}, {3,0}};
        std::vector<std::pair<int,int>> hate;
        std::vector<std::pair<int,int>> like = {{0,1},{1,2}};
        // All to S1: d1 = 1,2,3 => max pair sum = 1+2=3? Wait pair sums: (0,1) sum=3, (0,2)=4, (1,2)=5 => max=5
        // All to S2: d2 = 9,8,7 => pairs: 17,16,15 => max=17
        // So best D=5
        int ans = minMaxTravel(N, A, B, S1x, S1y, S2x, S2y, barns, hate, like);
        assert(ans == 5);
    }
    // Test 5: Single barn, no constraints, D should be min(d1, d2) but problem requires pair sums; with N=1, no pairs, so any D>=0 works, but since we output minimal? Actually pair sum constraint only applies to pairs. So no constraints, min D=0 (any assignment). But the binary search starts at 0, so ans=0.
    {
        int N=1, A=0, B=0;
        int S1x=0, S1y=0, S2x=100, S2y=0;
        std::vector<std::pair<int,int>> barns = {{50,0}};
        std::vector<std::pair<int,int>> hate, like;
        int ans = minMaxTravel(N, A, B, S1x, S1y, S2x, S2y, barns, hate, like);
        assert(ans == 0);
    }
    // Test 6: Two barns, both closer to S1, but hate forces different silos -> cross assignment adds len, so D must be large
    {
        int N=2, A=1, B=0;
        int S1x=0, S1y=0, S2x=100, S2y=0;
        std::vector<std::pair<int,int>> barns = {{1,0}, {2,0}};
        std::vector<std::pair<int,int>> hate = {{0,1}};
        std::vector<std::pair<int,int>> like;
        // If assign to different: e.g., 0->S1 (d1=1), 1->S2 (d2=98), len=100 => sum=1+98+100=199
        // Or 0->S2 (98), 1->S1 (2) => 98+2+100=200
        // Both to S1: 1+2=3, both to S2: 98+99=197 (but hate forbids same)
        // So minimal D = 199
        int ans = minMaxTravel(N, A, B, S1x, S1y, S2x, S2y, barns, hate, like);
        assert(ans == 199);
    }
    // Test 7: More complex: 3 barns, no constraints, but one is very far from both, check cross combos
    {
        int N=3, A=0, B=0;
        int S1x=0, S1y=0, S2x=10, S2y=0;
        std::vector<std::pair<int,int>> barns = {{0,1}, {10,1}, {5,100}};
        // distances: barn0: d1=1, d2=11; barn1: d1=11, d2=1; barn2: d1=105, d2=95; len=10
        // Best assignment? Try barn0->S1, barn1->S2, barn2->S1: pairs:
        // (0,1) cross: 1+1+10=12, (0,2): 1+105=106, (1,2) cross: 11+95+10=116 => max=116
        // Try barn0->S1, barn1->S2, barn2->S2: (0,1)=12, (0,2) cross:1+95+10=106, (1,2) same S2:1+95=96 => max=106
        // Try all S1: 1+11=12, 1+105=106, 11+105=116 => max=116
        // So minimal D=106
        int ans = minMaxTravel(N, A, B, S1x, S1y, S2x, S2y, barns, hate, like);
        assert(ans == 106);
    }
    // Test 8: Large D bound, but impossible due to contradictory distance? Actually if coordinates are so far that even max D (5e6) not enough? But max D is large enough for coordinates <=1e5, so always feasible. But test with hate+like contradiction already covers -1.
    // Test 9: All barns at same point, like all to each other, so all same silo, distances equal -> D = sum of two distances = 2*d
    {
        int N=4, A=0, B=3;
        int S1x=0, S1y=0, S2x=100, S2y=0;
        std::vector<std::pair<int,int>> barns = {{5,0}, {5,0}, {5,0}, {5,0}};
        std::vector<std::pair<int,int>> hate;
        std::vector<std::pair<int,int>> like = {{0,1},{1,2},{2,3}};
        // All must be same silo. d1=5, d2=95. If all to S1, max pair sum = 5+5=10. If all to S2, 95+95=190. So D=10.
        int ans = minMaxTravel(N, A, B, S1x, S1y, S2x, S2y, barns, hate, like);
        assert(ans == 10);
    }
    // Test 10: N=2, no constraints, one barn closer to S1, other to S2, but cross adds len, maybe same silo is better
    {
        int N=2, A=0, B=0;
        int S1x=0, S1y=0, S2x=100, S2y=0;
        std::vector<std::pair<int,int>> barns = {{1,0}, {99,0}};
        // d1: 1, 99; d2: 99, 1; len=100
        // same S1: 1+99=100; same S2: 99+1=100; cross: 1+1+100=102 or 99+99+100=298 -> best 100
        int ans = minMaxTravel(N, A, B, S1x, S1y, S2x, S2y, barns, hate, like);
        assert(ans == 100);
    }
    return 0;
}
// This problem is a classic "2-SAT with binary search on maximum allowed distance" problem. Each barn i is represented by a boolean variable: false means assigned to S1, true means assigned to S2. Hate pairs (i,j) give clauses `(i != j)` which is `(¬i ∨ ¬j) ∧ (i ∨ j)`. Like pairs give `(i == j)` which is `(i ∨ ¬j) ∧ (¬i ∨ j)`. For the distance constraints, for each pair (i,j) with i<j, we must forbid certain combinations that exceed D. There are four possible assignments: (S1,S1), (S2,S2), (S1,S2), (S2,S1). The first two are forbidden if `d1[i]+d1[j] > D` or `d2[i]+d2[j] > D` respectively. The cross assignments are forbidden if `d1[i]+d2[j]+len > D` (barn i→S1, j→S2) and `d2[i]+d1[j]+len > D` (i→S2, j→S1). Each forbidden combination yields two implications (forbidden assignment means if one variable is fixed, the other must be the opposite). Build the implication graph (2N nodes: 2*i for false, 2*i+1 for true), add edges for all constraints. Then use a standard recursive 2-SAT solver that tries to assign values greedily: for each unassigned variable, try assigning it to false first; if that leads to a contradiction, backtrack and try true; if both fail, no solution. The function `buildGraph(D)` constructs the graph, and `solve()` runs the 2-SAT decision. Binary search D from 0 to 5,000,000. Since the constraints are monotonic (larger D makes more assignments allowed), we binary search for the smallest D that satisfies. Time complexity: O((N^2 + A + B) * log(maxD)) per test, but since N is up to 1000 and the graph has up to ~2*10^6 edges, the 2-SAT solver is O(N+E) per attempt, so overall O((N^2) * log(5e6)) ≈ O(10^6 * 23) which is acceptable. Space is O(N^2) for edges. Edge cases: contradictory hate/like pairs, or impossible distance constraints even for max D (then answer -1). Also note that the constant road length `len` is always added when crossing silos, regardless of physical travel; it’s part of the problem definition.
