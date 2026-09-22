/*
Write a C++ function `solveGraph(int n, const std::vector<std::pair<int, int>>& edges)` that processes an undirected graph where vertices are labeled from 1 to `n`. The graph may have cycles and may not be connected. Starting a DFS from vertex 1 with initial distance 0, the function must simulate the following logic: count how many vertices have even distance from the start (including the start itself), count the total number of vertices reachable from vertex 1, detect if there is any cycle of odd length (using distance parity from the DFS tree), and additionally count vertices that are seen from an odd-distance vertex but have an odd recorded distance from the start (these are extra counted vertices during DFS). The final answer is:
- If an odd cycle is detected OR only vertex 1 is reachable (`con == 1`), answer is 0.
- Otherwise, if an odd cycle is detected, answer is `con` (the number of reachable vertices), but since odd cycle detection already forces 0, this branch never applies; the actual logic is: if odd cycle flag is true, answer = `con`; else answer = `cnt`. However, the original code then overrides with 0 if `con == 1`. Reproduce exactly the answer calculation from the snippet: if `flag` is true, answer = `con`; else answer = `cnt`; then if `con == 1`, answer = 0. The function should return the computed answer as an integer. The input graph has `n` vertices and `m` edges, where `m` is the size of the edges vector. The graph is undirected and may contain multiple edges or self-loops, but you can assume no repeated edges? No, handle duplicates as the original would (they could affect parity detection). The function must ignore any edges to vertices outside 1..n and must handle `n` possibly being 0 (then answer is 0). Return the answer for a single test case.
*/
#include <bits/stdc++.h>

// Solve the graph problem according to the given DFS logic.
// n: number of vertices (1..n), edges: undirected edges (u,v)
// Returns the final answer as per the specification.
int solveGraph(int n, const std::vector<std::pair<int, int>>& edges) {
    if (n == 0) return 0;
    const int MAXN = n + 2;
    std::vector<std::vector<int>> g(MAXN);
    // Build adjacency list, skipping invalid vertices
    for (const auto& e : edges) {
        int u = e.first, v = e.second;
        if (u < 1 || u > n || v < 1 || v > n) continue;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    std::vector<int> status(MAXN, 0); // 0 unvisited, 1 in recursion, 2 finished
    std::vector<int> dis(MAXN, 0);
    bool flag = false;
    int cnt = 0, con = 0;

    std::function<void(int, int)> dfs = [&](int pos, int d) {
        if (dis[pos] == 0) dis[pos] = d;
        con++;
        if (d % 2 == 0) cnt++;
        status[pos] = 1;
        for (int v : g[pos]) {
            if (status[v] == 0) {
                status[v] = 1;
                dfs(v, d + 1);
            } else if (status[v] == 1) {
                int dif = std::abs(dis[pos] - dis[v]);
                if (dif % 2 == 0 && dif != 0) {
                    flag = true;
                }
            } else { // status[v] == 2
                if (d % 2 == 1 && dis[v] % 2 != 0) {
                    cnt++;
                }
            }
        }
        status[pos] = 2;
    };

    // Start DFS from vertex 1 if it exists
    if (n >= 1) {
        dfs(1, 0);
    }

    int ans;
    if (flag) {
        ans = con;
    } else {
        ans = cnt;
    }
    if (con == 1) {
        ans = 0;
    }
    return ans;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is defined above; this is the test main.
int main() {
    // Test 1: Simple path 1-2-3, no odd cycle, con=3, cnt=2 (even depth: 1,3)
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        assert(solveGraph(3, edges) == 2);
    }
    // Test 2: Triangle (odd cycle) 1-2-3-1, flag set, con=3, answer=0? Actually flag true, ans=con=3? But con=3, not 1, so ans=3. The original does that. Check: dfs(1,0): dis1=0, cnt=1, neighbor2 (unvisited) -> dfs(2,1): dis2=1, cnt stays (d odd), neighbor1 back edge dif=1 odd -> no flag; neighbor3 unvisited -> dfs(3,2): dis3=2, cnt=2, neighbor1 back edge dif=2 even dif!=0 -> flag=true, neighbor2 back edge dif=1 odd no flag. Con=3, flag=true -> ans=3. Then con!=1 so not overridden.
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,1}};
        assert(solveGraph(3, edges) == 3);
    }
    // Test 3: Single vertex only, con=1 -> ans=0
    {
        std::vector<std::pair<int,int>> edges = {};
        assert(solveGraph(1, edges) == 0);
    }
    // Test 4: Star from 1 to others, no odd cycle, all leaves odd depth, cnt=1 (only vertex 1), con>1 -> ans=cnt=1
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{1,4}};
        assert(solveGraph(4, edges) == 1);
    }
    // Test 5: Edge 1-2 and 2-3 and 3-4, even cycle? 4-cycle: 1-2-3-4-1 -> all even? Actually that's even cycle, flag false, cnt = vertices even depth: 1(d0),3(d2) -> cnt=2, con=4 -> ans=2
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,1}};
        assert(solveGraph(4, edges) == 2);
    }
    // Test 6: n=0, edges empty, returns 0
    {
        std::vector<std::pair<int,int>> edges = {};
        assert(solveGraph(0, edges) == 0);
    }
    // Test 7: Disconnected: vertex 1 isolated, vertex 2 with self-loop? Self-loop: edge 2-2, but start from 1, con=1, ans=0
    {
        std::vector<std::pair<int,int>> edges = {{2,2}};
        assert(solveGraph(2, edges) == 0);
    }
    // Test 8: The given sample 1: n=5, edges: 1-2,2-3,3-4,1-4,3-5. Cycle 1-2-3-4-1 is even (differences? Actually that's 4-cycle), plus leaf 5. cnt: even depth vertices: 1(d0),3(d2)? but also 5? Let's simulate: dfs(1,0): dis1=0,cnt=1, neighbors 2,4. Go to 2(d1): dis2=1, neighbor1 back edge dif=1 odd, neighbor3(d2): dis3=2,cnt=2, neighbor4? from 3: neighbor2 back edge dif=1 odd, neighbor4 unvisited? but 4 also neighbor of 1 already? Actually from 1, also 4 is visited at some point. The graph has cycle 1-2-3-4-1, all edges even cycle, no odd cycle flag. After DFS, con=5, cnt=2? Let's trust logic, answer should be 2.
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{1,4},{3,5}};
        assert(solveGraph(5, edges) == 2);
    }
    // Test 9: Graph with cross edge causing status=2 increments: Example: 1-2-3 and 1-3? That is triangle already tested. Let's do 1-2,1-3,2-3? Triangle again. Try: 1-2,2-3,4-5? n=5, start from 1, con=3, cnt: even depth: 1,3? Actually 1(d0) even, 2(d1) odd, 3(d2) even -> cnt=2, ans=2
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{4,5}};
        assert(solveGraph(5, edges) == 2);
    }
    // Test 10: Multiple edges between same vertices: 1-2 twice. DFS: first neighbor 2 unvisited, second time status[2] is 1 (back edge) dif=1 odd, no flag. con=2, cnt=1 (vertex 1 even), ans=1
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,2}};
        assert(solveGraph(2, edges) == 1);
    }

    return 0;
}
// The solution performs a DFS from vertex 1, maintaining for each vertex:
// - `dis[v]`: the distance (depth) from the start when first visited, initialized to 0.
// - `status[v]`: 0 = unvisited, 1 = currently in recursion stack (discovered but not finished), 2 = fully processed.
//
// During DFS at position `pos` with current depth `d`, it:
// - If `dis[pos] == 0` (first time), set `dis[pos] = d`.
// - Increment `con` (number of reachable vertices).
// - If `d % 2 == 0`, increment `cnt` (count of even-distance reachable vertices).
// - Set status to 1.
// - For each neighbor `v`:
//   - If status[v] == 0 (unvisited), set status[v]=1 and recurse with depth d+1.
//   - Else if status[v] == 1 (back edge to ancestor in DFS tree), compute `dif = abs(dis[pos] - dis[v])`. If `dif % 2 == 0 && dif != 0`, set flag = 1 (odd cycle detection, because in an undirected graph a back edge with even difference indicates cycle of odd length).
//   - Else (status[v] == 2, already finished), if `d % 2 == 1 && dis[v] % 2 != 0`, increment `cnt`. This is a special case: when an odd-depth vertex sees a finished vertex that also has odd distance, that finished vertex was already counted earlier? But the original code counts it again. This adds extra counts, but note this can only happen if the graph has cross edges or cycles; the original code exhibits this behavior, so we must replicate it exactly.
// - After processing all neighbors, set status[pos] = 2.
//
// After DFS, compute answer:
// - If flag == true, ans = con, else ans = cnt.
// - If con == 1 (only start vertex reachable), ans = 0.
//
// The function must replicate this exact logic including the `cnt` increments in the else branch (status==2) and the final override. Time complexity is O(n + m) for DFS, space O(n) for arrays and adjacency lists. Edge cases: n=0 returns 0; graph disconnected – only reachable component matters; self-loops are handled because neighbor is itself, back edge with dif=0 is ignored (dif !=0 check); duplicate edges may cause extra status==2 increments or back edge checks but the logic still works.
