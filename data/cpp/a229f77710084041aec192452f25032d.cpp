// Write a C++ function `findBalancedBipartition` that takes an undirected graph represented by an adjacency list (as a `vector<vector<int>>`), with vertices numbered from 1 to n, and returns a `std::string` of length n containing only `'1'` and `'2'`, representing a valid 2-coloring (bipartition) of the graph that minimizes the absolute difference between the number of vertices assigned `'1'` and `'2'`, while also being lexicographically smallest among all such optimal colorings. If the graph is not bipartite (contains an odd cycle), return the string `"-1"`. The function signature is `std::string findBalancedBipartition(int n, const std::vector<std::vector<int>>& adj)`, where `adj` is a 1-indexed adjacency list (size `n+1`). If multiple valid colorings achieve the minimal difference, choose the one whose resulting string is lexicographically smallest (comparing standard string ordering). For disconnected graphs, you must handle each connected component separately, and the final coloring must color all vertices consistently within each component.
// The correct approach is to perform BFS or DFS-based bipartiteness testing on each connected component. For each component, we can compute a bipartition (two color classes) by alternating colors along a traversal. If any edge connects two vertices of the same color, the graph is not bipartite and we return `"-1"`. For each component, we have two possible color assignments: either swap the two color classes (because swapping all colors in a component still yields a valid bipartition). For each component, we must decide which orientation to use to minimize the global absolute difference between the total counts of `'1'` and `'2'` across all components. Since each component independently contributes `(size1, size2)` or `(size2, size1)` to the global counts, we can solve this via dynamic programming over components: let `dp[delta]` be a boolean indicating whether we can achieve a total difference (count1 - count2) equal to `delta`, shifting indices to handle negative values. For each component, we update the dp. After processing all components, we find the delta with minimal absolute value that is achievable; among ties, we want the lexicographically smallest output string. To construct the lexicographically smallest string among optimal differences, we can perform a greedy construction from vertex 1 to n: at each vertex, if it is unassigned, we traverse its component and try assigning it color `'1'` or `'2'` (corresponding to the two orientations), then check if the remaining vertices can achieve the target global difference. We can precompute component structure and then use the DP to check feasibility of a partial assignment. However, a simpler approach: since n is small (likely ≤ 1000 as in the snippet), we can enumerate all possible orientation choices per component via recursion and prune with the DP. But to be efficient and correct, we can do the following: first compute bipartition for each component (store the two sides). Then use DP to compute the set of achievable differences. Find optimal difference `d_opt` (min absolute). Then for lexicographically smallest string, iterate through vertices in order, and for each vertex decide its color by trying `'1'` first; if assigning that color to the entire component (in a consistent orientation) still allows achieving `d_opt` given the already fixed assignments, keep it; otherwise try `'2'`. Since each component is fixed by the color of its first encountered vertex, this greedy yields the lexicographically smallest string. Time complexity: O(n + q + C * n) for bipartition and DP where C is number of components; DP table size is O(n) width, each component update O(n), total O(n^2) which is fine for n ≤ 1000. Space O(n + n^2) for DP.
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <cstdlib>

// Returns the lexicographically smallest balanced bipartition string for the graph,
// or "-1" if the graph is not bipartite.
std::string findBalancedBipartition(int n, const std::vector<std::vector<int>>& adj) {
    // visited[v] = 0 (unvisited), 1 (color 0), 2 (color 1)
    std::vector<int> color(n + 1, 0);
    // For each component, store the two sides as lists of vertices
    std::vector<std::vector<int>> comp_side0, comp_side1;

    for (int start = 1; start <= n; ++start) {
        if (color[start] != 0) continue;
        // BFS to color component
        std::queue<int> q;
        q.push(start);
        color[start] = 1;
        std::vector<int> side0, side1;
        side0.push_back(start);
        bool is_bipartite = true;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int u : adj[v]) {
                if (color[u] == 0) {
                    color[u] = 3 - color[v]; // opposite color
                    if (color[u] == 1) side0.push_back(u);
                    else side1.push_back(u);
                    q.push(u);
                } else if (color[u] == color[v]) {
                    is_bipartite = false;
                }
            }
        }
        if (!is_bipartite) return "-1";
        comp_side0.push_back(side0);
        comp_side1.push_back(side1);
    }

    int C = comp_side0.size();
    // DP over achievable difference (count1 - count2), shift by n to handle negatives
    // Possible differences range from -n to n, so size 2*n+1
    int shift = n;
    std::vector<bool> dp(2 * n + 1, false);
    dp[shift] = true; // difference 0 achievable with no components processed

    for (int c = 0; c < C; ++c) {
        std::vector<bool> ndp(2 * n + 1, false);
        int sz0 = comp_side0[c].size();
        int sz1 = comp_side1[c].size();
        for (int d = 0; d <= 2 * n; ++d) {
            if (!dp[d]) continue;
            int diff = d - shift;
            // Option 1: assign side0 as color '1', side1 as '2'
            ndp[d + (sz0 - sz1)] = true;
            // Option 2: assign side0 as '2', side1 as '1'
            ndp[d - (sz0 - sz1)] = true;
        }
        dp.swap(ndp);
    }

    // Find optimal difference (min abs)
    int opt_diff = 0;
    bool found = false;
    for (int abs_diff = 0; abs_diff <= n; ++abs_diff) {
        if (dp[shift + abs_diff] || dp[shift - abs_diff]) {
            opt_diff = abs_diff;
            found = true;
            break;
        }
    }
    if (!found) return "-1"; // should not happen

    // Reconstruct lexicographically smallest string greedily
    // We'll process components in order of their smallest vertex index.
    // For output, we need to assign a string of length n.
    std::string result(n, '?');
    // For each component, we need to know which orientation was chosen.
    // We'll iterate vertices 1..n; when we meet an unassigned vertex (belongs to some component),
    // we try assigning that vertex as '1' and see if the remaining components can achieve opt_diff.
    // To do this, we rebuild dp but with some components fixed.

    // First, map each vertex to its component id
    std::vector<int> comp_id(n + 1, -1);
    std::vector<bool> assigned_comp(C, false);
    for (int c = 0; c < C; ++c) {
        for (int v : comp_side0[c]) comp_id[v] = c;
        for (int v : comp_side1[c]) comp_id[v] = c;
    }

    // Helper: compute achievable differences given a subset of components already fixed.
    // We'll store for each component the two possible difference contributions.
    std::vector<int> contrib0(C), contrib1(C);
    for (int c = 0; c < C; ++c) {
        contrib0[c] = (int)comp_side0[c].size() - (int)comp_side1[c].size(); // side0 as '1'
        contrib1[c] = -contrib0[c]; // side0 as '2'
    }

    // We'll do greedy over vertices: for each vertex in order, try to assign '1' if possible.
    // But we must ensure consistency within a component. So when we encounter the first vertex of a component,
    // we decide its orientation, and then assign all vertices accordingly.
    // To decide feasibility, we need DP with some components fixed.
    // Since n is small, we can recompute DP from scratch each time O(C*n) which is O(n^2) total.

    // Precompute prefix DP and suffix DP? Simpler: do DFS over components order.
    // But easier: we can just iterate over all possible orientation assignments? 2^C could be large.
    // Since C <= n, and n <= 1000, we can do DP with reconstruction: use the standard DP backtracking.
    // But for lexicographic smallest, we need careful order.

    // Approach: process vertices in increasing order. For each vertex v, if already assigned, skip.
    // Else, its component c = comp_id[v]. Try orientation A (side0 as '1') first.
    // Check if there exists a completion to achieve opt_diff. To check, we need to know filled difference so far,
    // and the remaining components (unfixed) can achieve the target difference.
    // We'll maintain a list of remaining components and recompute DP for them each time.

    // Implementation detail: we'll build a vector of components sorted by min vertex.
    std::vector<std::pair<int,int>> comp_order; // (min_vertex, component_id)
    for (int c = 0; c < C; ++c) {
        int minv = n + 1;
        for (int v : comp_side0[c]) minv = std::min(minv, v);
        for (int v : comp_side1[c]) minv = std::min(minv, v);
        comp_order.push_back({minv, c});
    }
    std::sort(comp_order.begin(), comp_order.end());

    // For DP with some fixed components, we can start with a base DP that includes all fixed contributions.
    // We'll process components in order of comp_order, and when we reach one, we decide orientation.
    // For greedy lexicographic, we need to decide the orientation of a component when its first vertex appears.
    // So we'll iterate vertices 1..n; when we see an unassigned vertex, we find its component,
    // and try to assign orientation A (side0 as '1') if feasible.

    std::vector<bool> comp_fixed(C, false);
    int current_diff = 0; // difference from fixed components

    // We need a function to check if with a given current_diff, and a set of unfixed components,
    // we can achieve total difference opt_diff or -opt_diff (since opt_diff is absolute value).
    // Actually we need to know if we can achieve exactly +opt_diff or -opt_diff from the set of unfixed components?
    // But the total difference must be either +opt_diff or -opt_diff. Since opt_diff is minimal absolute.
    // So for fixed components, we have a current_diff (could be any integer).
    // The remaining unfixed components can contribute arbitrary differences. We need current_diff + achievable_from_rest == +opt_diff OR == -opt_diff.
    // This is equivalent to: achievable_from_rest can be (opt_diff - current_diff) or (-opt_diff - current_diff).
    // So we need to check if either target is achievable via DP on unfixed components.

    // We'll write a helper that builds a DP for a given list of unfixed components.
    // But we'd call this many times; each O(U*n) where U is number of unfixed. Total O(n^2) okay.

    // First, precompute DP for each suffix? Actually we can process components greedily in order.
    // But lexicographic order may require deciding a component later. So we need the flexibility.

    // Let's just implement the greedy with recomputing DP on the fly. Since n <= 1000, worst-case O(n^3) maybe okay? Let's estimate: for each vertex we might recompute DP over remaining components each time. In worst case all components are singletons, so C=n, each recompute O(n^2) => O(n^3) too high. Better approach: use DP with reconstruction.

    // Alternative: We can compute for each component its two possible contributions. We need to decide orientations to achieve a total sum S (where S = opt_diff or -opt_diff). Among all valid assignments, we want lexicographically smallest string. This is a subset-sum-like problem with two choices per component. We can do a DP that stores a bitmask for reconstruction? Not feasible.

    // Simpler: Since n is at most 1000, we can try all possible orientation assignments? 2^C is too big if C=1000.

    // But note: the lexicographic order depends on the actual vertex labels. We can use a greedy approach that processes components in order of their minimum vertex, but within a component, all vertices are fixed together. So we only need to decide the orientation of each component. The order of decisions determines lexicographic order of the output string. Because within a component, if we assign the smallest vertex as '1', all vertices in side0 get '1' and side1 get '2'. So the string's characters depend on the orientation. To get lexicographically smallest, when we process components in increasing order of their smallest vertex, we try orientation A (smallest vertex gets '1') and check if the remaining components can achieve the target difference. If yes, we commit. This is a standard greedy with feasibility check. The number of decisions is C (each component once), and each feasibility check can be done by a DP over the remaining components. Since C <= n, each DP is O(n^2) worst case, total O(n^3) which might be borderline for n=1000 but acceptable in practice? Actually each DP over remaining components takes O(n * U) where U is number of unfixed components. Summing over C decisions, total O(n * C^2) worst case O(n^3) = 1e9 maybe too high. But we can optimize by using a DP that includes fixed contributions incrementally.

    // Better: Use the classic DP backtracking: we maintain dp array with possible differences and for each state, we store the choice made (orientation) for reconstruction. But lexicographic order requires careful tie-breaking.

    // Given the constraints are not specified, but typical such tasks have n up to maybe 1000, an O(n^2) solution is expected. We can do the following:
    // For each component, we know its contribution options. We can compute a DP that records for each difference the lexicographically smallest string? That would be exponential.

    // Pragmatic approach: Since the original snippet used n <= 1000 and brute force over j from 1..n, we can implement a simpler but correct algorithm: For each component, we have two orientations. We can use recursion with memoization on (component_index, current_diff) to find if a solution exists and then greedily choose lexicographically smallest by trying orientation A first. Because C is at most n, recursion depth at most n, and each state visited once, but we need to build the string. So we can implement a recursive function that processes components in order of minimum vertex, and for each component, tries orientation A first, then checks feasibility using DP on the remaining components. To make feasibility check efficient, we can precompute a suffix DP array: suff_dp[i][d] indicates whether using components i..C-1 (in the order of comp_order) we can achieve difference d. Then for a given current_diff and unfixed component index, we can check in O(1) if we can reach opt_diff. This is doable.

    // Let's order components by min vertex, and build suffix DP.
    std::vector<int> comp_min(C);
    for (int c = 0; c < C; ++c) {
        int minv = n + 1;
        for (int v : comp_side0[c]) minv = std::min(minv, v);
        for (int v : comp_side1[c]) minv = std::min(minv, v);
        comp_min[c] = minv;
    }
    // Order by comp_min
    std::vector<int> order(C);
    for (int i = 0; i < C; ++i) order[i] = i;
    std::sort(order.begin(), order.end(), [&](int a, int b) { return comp_min[a] < comp_min[b]; });

    // suffix DP: suff[i][d] for i from 0..C (i means components from i to C-1)
    int total_range = 2 * n + 1;
    std::vector<std::vector<bool>> suff(C + 1, std::vector<bool>(total_range, false));
    suff[C][shift] = true;
    for (int i = C - 1; i >= 0; --i) {
        int c = order[i];
        int diff0 = contrib0[c];
        // diff1 = -diff0
        for (int d = 0; d < total_range; ++d) {
            if (!suff[i + 1][d]) continue;
            int nd = d + diff0;
            if (nd >= 0 && nd < total_range) suff[i][nd] = true;
            nd = d - diff0;
            if (nd >= 0 && nd < total_range) suff[i][nd] = true;
        }
    }

    // Now greedy: process components in order.
    std::vector<bool> orientation_choice(C, false); // false = side0 as '1', true = side0 as '2'
    int current_diff = 0;
    bool possible = true;
    for (int i = 0; i < C; ++i) {
        int c = order[i];
        // Try orientation false (side0 '1') first
        int new_diff = current_diff + contrib0[c];
        // Check if from i+1..C-1 we can achieve either +opt_diff or -opt_diff total difference
        bool ok_false = false;
        if (suff[i + 1][shift + (opt_diff - new_diff)] || suff[i + 1][shift + (-opt_diff - new_diff)]) {
            ok_false = true;
        }
        if (ok_false) {
            orientation_choice[c] = false;
            current_diff = new_diff;
        } else {
            // Try orientation true
            int new_diff2 = current_diff - contrib0[c];
            if (suff[i + 1][shift + (opt_diff - new_diff2)] || suff[i + 1][shift + (-opt_diff - new_diff2)]) {
                orientation_choice[c] = true;
                current_diff = new_diff2;
            } else {
                possible = false;
                break;
            }
        }
    }
    if (!possible) return "-1";

    // Build result string
    for (int c = 0; c < C; ++c) {
        if (!orientation_choice[c]) {
            // side0 -> '1', side1 -> '2'
            for (int v : comp_side0[c]) result[v - 1] = '1';
            for (int v : comp_side1[c]) result[v - 1] = '2';
        } else {
            // side0 -> '2', side1 -> '1'
            for (int v : comp_side0[c]) result[v - 1] = '2';
            for (int v : comp_side1[c]) result[v - 1] = '1';
        }
    }
    return result;
}
#include <cassert>
#include <string>
#include <vector>

// Assume the function is defined above in the same translation unit.
// For the test, we include the declaration.

int main() {
    // Test 1: Single edge (2 vertices)
    {
        int n = 2;
        std::vector<std::vector<int>> adj(n + 1);
        adj[1].push_back(2);
        adj[2].push_back(1);
        std::string res = findBalancedBipartition(n, adj);
        assert(res == "12" || res == "21"); // both valid, lexicographically smallest is "12"
        // Our function should return "12" because it tries '1' first for vertex 1.
        assert(res == "12");
    }

    // Test 2: Triangle (odd cycle) -> not bipartite
    {
        int n = 3;
        std::vector<std::vector<int>> adj(n + 1);
        adj[1].push_back(2); adj[2].push_back(1);
        adj[2].push_back(3); adj[3].push_back(2);
        adj[3].push_back(1); adj[1].push_back(3);
        std::string res = findBalancedBipartition(n, adj);
        assert(res == "-1");
    }

    // Test 3: Path of 4 vertices -> bipartite, balanced difference 0
    {
        int n = 4;
        std::vector<std::vector<int>> adj(n + 1);
        adj[1].push_back(2); adj[2].push_back(1);
        adj[2].push_back(3); adj[3].push_back(2);
        adj[3].push_back(4); adj[4].push_back(3);
        std::string res = findBalancedBipartition(n, adj);
        // Possible colorings: "1212", "2121", both balanced. Lexicographically smallest is "1212".
        assert(res == "1212");
    }

    // Test 4: Disconnected graph: edge (1,2) and edge (3,4)
    {
        int n = 4;
        std::vector<std::vector<int>> adj(n + 1);
        adj[1].push_back(2); adj[2].push_back(1);
        adj[3].push_back(4); adj[4].push_back(3);
        // Each component is size 2. To get balanced overall (2 vs 2), orient both components the same way.
        // Lexicographically smallest: "1212"
        std::string res = findBalancedBipartition(n, adj);
        assert(res == "1212");
    }

    // Test 5: Single vertex connected to two leaves: star with 3 leaves (n=4)
    {
        int n = 4;
        std::vector<std::vector<int>> adj(n + 1);
        adj[1].push_back(2); adj[2].push_back(1);
        adj[1].push_back(3); adj[3].push_back(1);
        adj[1].push_back(4); adj[4].push_back(1);
        // Bipartition: center (1) one side, leaves (2,3,4) other side. Difference = 2 (1 vs 3) or 3 vs 1.
        // Lexicographically smallest among difference-2: assign 1 as '1', leaves as '2' => "1222"
        std::string res = findBalancedBipartition(n, adj);
        assert(res == "1222");
    }

    // Test 6: Larger bipartite graph with multiple components to check DP.
    // n=6, two triangles? No, triangles not bipartite. Use two 3-vertex paths.
    {
        int n = 6;
        std::vector<std::vector<int>> adj(n + 1);
        // Path 1-2-3
        adj[1].push_back(2); adj[2].push_back(1);
        adj[2].push_back(3); adj[3].push_back(2);
        // Path 4-5-6
        adj[4].push_back(5); adj[5].push_back(4);
        adj[5].push_back(6); adj[6].push_back(5);
        // Each path has sizes (1,2) or (2,1). To minimize difference, we can do (1,2)+(2,1) = total 3 vs 3 diff 0.
        // Lexicographically smallest: assign vertex 1 '1', vertex 2 '2', vertex 3 '1' (component1 yields 2 of '1'? Actually side0 has {1,3} size 2, side1 {2} size1 -> if orient side0 as '1', that gives 1->'1',2->'2',3->'1'. Component2 similarly orient to balance? Let's compute: component1 oriented side0 as '1' gives diff (2-1)=+1; component2 oriented side0 as '2' gives diff -1. Total diff 0. String: vertex1 '1',2 '2',3 '1'; vertex4 (side0 of comp2) as '2',5 as '1',6 as '2' => "121"+"212" = "121212". Lexicographically smallest valid? Try orient comp1 as side0='1', comp2 as side1='1'? Actually if comp2 side0 as '2' then side1 '1', giving 4->'2',5->'1',6->'2'. So "121212". Could we get "112122"? No because within comp1, vertices 1 and 3 must same, 2 opposite. So "121212" is likely minimal.
        std::string res = findBalancedBipartition(n, adj);
        assert(res == "121212");
    }

    // Test 7: Graph with isolated vertices (n=3, edges: 1-2) => vertex 3 isolated.
    {
        int n = 3;
        std::vector<std::vector<int>> adj(n + 1);
        adj[1].push_back(2); adj[2].push_back(1);
        // Vertex 3 isolated. To balance, we can put 3 on the smaller side.
        // Component1 sizes (1,1) diff 0. Component2 sizes (1,0) diff 1 or -1.
        // Optimal diff = 0? If comp2 side0 as '1' gives diff +1, total diff +1 (abs1). If side1 as '1' gives diff -1, total diff -1. So minimal abs diff = 1.
        // Lexicographically smallest: try vertex 1 as '1', vertex 2 as '2', and vertex 3 as '1' gives "121" (diff 2-1=1). Could vertex 3 be '2'? Then "122" diff 1-2=-1 abs1, but "121" < "122". So "121".
        std::string res = findBalancedBipartition(n, adj);
        assert(res == "121");
    }

    // Test 8: Large balanced bipartite complete graph K_{2,2} (cycle of 4)
    {
        int n = 4;
        std::vector<std::vector<int>> adj(n + 1);
        adj[1].push_back(2); adj[1].push_back(4); adj[2].push_back(1); adj[2].push_back(3);
        adj[3].push_back(2); adj[3].push_back(4); adj[4].push_back(1); adj[4].push_back(3);
        // Bipartition: {1,3} and {2,4} diff 0. Lexicographically smallest: "1212".
        std::string res = findBalancedBipartition(n, adj);
        assert(res == "1212");
    }

    return 0;
}
