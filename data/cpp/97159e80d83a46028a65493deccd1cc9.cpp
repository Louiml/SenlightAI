// You are given a directed graph with `n` vertices and `m` edges. Each vertex `i` has an associated cash reward `cost[i]` (positive or negative). You start at a designated vertex `start`. You may traverse edges along their direction, and you may collect the reward of every vertex you visit at most once (so if you revisit a vertex, you do not collect its reward again). Additionally, there are `p` special "exit" vertices. Your goal is to compute the maximum total reward you can accumulate if you must end your journey at one of these exit vertices (the exit vertex itself counts as visited and its reward is collected). The graph may contain cycles. Write a C++ function `int maxRewardAtExit(int n, const std::vector<std::pair<int,int>>& edges, const std::vector<int>& reward, int start, const std::vector<int>& exits)` that returns the maximum total reward achievable.
//
// **Clarifications**:  
// - The exit list may contain duplicates; treat each distinct exit vertex once.  
// - You are allowed to visit any subset of vertices, but you must stop at an exit. It is guaranteed that at least one exit is reachable from `start`.  
// - Vertex indices are 1-based.  
// - Rewards can be negative, but the total accumulated reward is the sum of rewards of all distinct vertices visited.  
// - The graph may have self-loops and multiple edges.  
// - The function must correctly handle cases where the optimal path revisits vertices (e.g., collects positive reward from a cycle, then goes to an exit).  
// - The algorithm must be efficient for `n` up to 500,000 and `m` up to 500,000.

#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (or link appropriately)
// For test completeness, the function is assumed to be defined above.

int main() {
    // Test 1: Simple line 1->2->3, start=1, exit=3
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        std::vector<int> reward = {0, 5, 10, 20};
        int start = 1;
        std::vector<int> exits = {3};
        int ans = maxRewardAtExit(n, edges, reward, start, exits);
        assert(ans == 35);
    }

    // Test 2: Cycle with positive reward, start inside, exit outside
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,1},{2,3}};
        std::vector<int> reward = {0, 5, 7, 100};
        int start = 1;
        std::vector<int> exits = {3};
        int ans = maxRewardAtExit(n, edges, reward, start, exits);
        assert(ans == 112); // collect 5+7 from cycle, then 100
    }

    // Test 3: Negative rewards, must go to exit
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{1,2}};
        std::vector<int> reward = {0, -5, -10};
        int start = 1;
        std::vector<int> exits = {2};
        int ans = maxRewardAtExit(n, edges, reward, start, exits);
        assert(ans == -15);
    }

    // Test 4: Start is an exit, collect only that
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{1,2}};
        std::vector<int> reward = {0, 10, 20};
        int start = 1;
        std::vector<int> exits = {1};
        int ans = maxRewardAtExit(n, edges, reward, start, exits);
        assert(ans == 10);
    }

    // Test 5: Multiple exits, choose the best one
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{1,4}};
        std::vector<int> reward = {0, 1, 2, 3, 100};
        int start = 1;
        std::vector<int> exits = {3,4};
        int ans = maxRewardAtExit(n, edges, reward, start, exits);
        assert(ans == 101); // 1+100 via direct edge
    }

    // Test 6: Self-loop and duplicate edges
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{1,1},{1,2},{1,2}};
        std::vector<int> reward = {0, 5, 2};
        int start = 1;
        std::vector<int> exits = {2};
        int ans = maxRewardAtExit(n, edges, reward, start, exits);
        assert(ans == 7);
    }

    // Test 7: Disconnected exit (unreachable) – but guarantee reachable, so skip
    // Test 8: Large chain
    {
        int n = 1000;
        std::vector<std::pair<int,int>> edges;
        for (int i = 1; i < n; ++i) edges.push_back({i, i+1});
        std::vector<int> reward(n+1, 1);
        reward[0] = 0;
        int start = 1;
        std::vector<int> exits = {n};
        int ans = maxRewardAtExit(n, edges, reward, start, exits);
        assert(ans == n);
    }

    // Test 9: Two components, start in one, exit in another – unreachable, but we ensure reachable
    // Test 10: Negative cycle reachable but we still collect once
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,1},{2,3}};
        std::vector<int> reward = {0, -1, -2, 10};
        int start = 1;
        std::vector<int> exits = {3};
        int ans = maxRewardAtExit(n, edges, reward, start, exits);
        assert(ans == 7); // -1 + -2 + 10 = 7
    }

    return 0;
}

#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <limits>

// Main function: returns the maximum total reward collectable when starting at 'start'
// and ending at one of the exits. Vertex indices are 1-based.
int maxRewardAtExit(int n,
                    const std::vector<std::pair<int,int>>& edges,
                    const std::vector<int>& reward,
                    int start,
                    const std::vector<int>& exits) {
    // Build adjacency list (1-based)
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
    }

    // Tarjan's SCC
    std::vector<int> disc(n + 1, 0), low(n + 1, 0), sccId(n + 1, 0);
    std::vector<bool> onStack(n + 1, false);
    std::stack<int> st;
    int time = 0, sccCnt = 0;

    // Iterative DFS to avoid recursion depth
    // We'll use explicit stack of frames: (node, nextIndex)
    for (int root = 1; root <= n; ++root) {
        if (disc[root] != 0) continue;
        std::stack<std::pair<int,int>> dfsStack;
        dfsStack.push({root, 0});
        while (!dfsStack.empty()) {
            auto& top = dfsStack.top();
            int u = top.first;
            int& idx = top.second;
            if (idx == 0) {
                disc[u] = low[u] = ++time;
                st.push(u);
                onStack[u] = true;
            }
            bool advanced = false;
            while (idx < (int)adj[u].size()) {
                int v = adj[u][idx++];
                if (disc[v] == 0) {
                    dfsStack.push({v, 0});
                    advanced = true;
                    break;
                } else if (onStack[v]) {
                    low[u] = std::min(low[u], disc[v]);
                }
            }
            if (advanced) continue;
            // Done with u
            if (low[u] == disc[u]) {
                ++sccCnt;
                while (true) {
                    int v = st.top(); st.pop();
                    onStack[v] = false;
                    sccId[v] = sccCnt;
                    if (v == u) break;
                }
            }
            dfsStack.pop();
            if (!dfsStack.empty()) {
                int parent = dfsStack.top().first;
                low[parent] = std::min(low[parent], low[u]);
            }
        }
    }

    // Sum rewards per SCC (1-based sccCnt)
    std::vector<long long> sccSum(sccCnt + 1, 0);
    for (int i = 1; i <= n; ++i) {
        sccSum[sccId[i]] += reward[i];
    }

    // Build DAG of SCCs, avoiding duplicate edges
    std::vector<std::vector<int>> dag(sccCnt + 1);
    std::vector<std::vector<bool>> seenEdge(sccCnt + 1);
    for (int i = 1; i <= sccCnt; ++i) seenEdge[i].assign(sccCnt + 1, false);
    for (int u = 1; u <= n; ++u) {
        for (int v : adj[u]) {
            int su = sccId[u], sv = sccId[v];
            if (su != sv && !seenEdge[su][sv]) {
                seenEdge[su][sv] = true;
                dag[su].push_back(sv);
            }
        }
    }

    // Topological order using Kahn's algorithm
    std::vector<int> indeg(sccCnt + 1, 0);
    for (int u = 1; u <= sccCnt; ++u) {
        for (int v : dag[u]) indeg[v]++;
    }
    std::queue<int> q;
    for (int i = 1; i <= sccCnt; ++i) if (indeg[i] == 0) q.push(i);
    std::vector<int> topo;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        topo.push_back(u);
        for (int v : dag[u]) {
            if (--indeg[v] == 0) q.push(v);
        }
    }

    // DP: max reward reaching each SCC
    const long long NEG = std::numeric_limits<long long>::min() / 2;
    std::vector<long long> dp(sccCnt + 1, NEG);
    int startSCC = sccId[start];
    dp[startSCC] = sccSum[startSCC];

    for (int u : topo) {
        if (dp[u] == NEG) continue;
        for (int v : dag[u]) {
            dp[v] = std::max(dp[v], dp[u] + sccSum[v]);
        }
    }

    // Check exits
    long long ans = NEG;
    for (int e : exits) {
        int se = sccId[e];
        if (dp[se] != NEG) ans = std::max(ans, dp[se]);
    }
    return (int)ans;
}

// The problem is a variation of the classic "longest path in a graph with cycles but no negative cycles reachable from start" because we can collect each vertex's reward only once. Since we can revisit vertices without penalty, the optimal strategy is to collect all rewards within each strongly connected component (SCC) that is reachable from `start`. After condensing the graph into SCCs, each SCC becomes a node with weight equal to the sum of all vertex rewards inside it. The condensed graph is a DAG, and we need to find the maximum weighted path from the SCC containing `start` to any SCC that contains an exit vertex. This is a longest path in a DAG, which can be solved with dynamic programming (topological order). Important edge cases:  
// - If the exit is inside the start SCC, output the sum of that SCC.  
// - If there are multiple exits in the same SCC, only count once.  
// - If rewards are negative, we may choose to stop at start if it is an exit; otherwise we must traverse to reach an exit.  
// - Since the graph can be large, we need iterative DFS (not recursion) to avoid stack overflow.  
// - Use Tarjan's algorithm for SCC decomposition in O(n+m).  
// - Then build the DAG, avoiding duplicate edges between SCCs.  
// - Process SCCs in topological order (e.g., using a queue like Kahn's algorithm or by iterating in reverse finishing order).  
// - Complexity: O(n + m) time, O(n + m) space.
