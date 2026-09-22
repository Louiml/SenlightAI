// Write a C++ function `int longestSpecialPath(const std::vector<std::vector<int>>& adj, const std::vector<int>& type)` that computes the maximum length of a path in a directed graph where a path is defined as a sequence of vertices (without repeating any vertex, since the graph is acyclic as guaranteed) and its "length" is counted as the number of type-0 vertices reachable after traversing a type-1 vertex. More specifically, starting from any vertex, the score of a path is the total number of times we move from a vertex of type 1 directly to a vertex of type 0, plus 1 (so a single vertex always contributes at least 1 to the maximum). The graph is a directed acyclic graph (DAG) with vertices labeled 0 to n-1, each having a type of either 0 or 1. The function must return the maximum possible score over all paths starting at any vertex. The input `adj` is an adjacency list where `adj[u]` contains the out-neighbors of vertex `u` (guaranteed to be a DAG, no cycles). The `type` vector of size n contains 0 or 1 for each vertex.
The problem is a longest-path variant on a DAG with a special edge-dependent weight. Since the graph is acyclic, we can solve it using dynamic programming with memoization (or topological order). For each vertex `cur`, define `dp[cur]` as the maximum score achievable starting from `cur` (including the contribution of `cur` itself as at least 1). The recurrence: initialize `dp[cur] = 1` (even if type is 0, we count the starting vertex as 1). Then for each neighbor `i` of `cur`, consider the transition:
- If `type[cur] == 1` and `type[i] == 0`, then moving from `cur` to `i` adds exactly 1 to the score of the path starting at `i` (because this specific transition counts as one occurrence). So the candidate is `dp[i] + 1`.
- Otherwise, no increment occurs, so the candidate is `dp[i]` (same score as the subpath starting at `i`).
Thus `dp[cur] = max(1, max over neighbors of (dp[i] + (type[cur]==1 && type[i]==0 ? 1 : 0)))`. The answer is the maximum `dp[cur]` over all vertices. Since the graph is a DAG, recursion with memoization works without cycles; but we must ensure that the recursion terminates. In the provided snippet, they use an array `seen` to avoid recomputation. We'll do the same with a `std::vector<int>` initialized to -1 for uncomputed values. Edge cases: vertices with no outgoing edges have `dp = 1`; all vertices type 0 gives maximum 1; all vertices type 1 gives maximum 1 unless there are 0-type vertices reachable. Since the graph is a DAG, no infinite recursion. Time complexity: O(n + m) where n is number of vertices and m is total edges, because each vertex is computed once and each edge is considered once. Space: O(n) for the memo array and recursion stack depth (O(n) in worst case for a chain). This matches the provided snippet's approach exactly.
#include <vector>
#include <algorithm>

// Computes the maximum score path in a DAG where each move from type 1 to type 0 adds 1.
// dp[cur] = max(1, max_{neighbor i} (dp[i] + (type[cur]==1 && type[i]==0 ? 1 : 0)))
// Returns the maximum dp over all vertices.
int longestSpecialPath(const std::vector<std::vector<int>>& adj, const std::vector<int>& type) {
    int n = (int)type.size();
    std::vector<int> memo(n, -1); // -1 means not computed yet

    // Recursive helper with memoization for a single vertex.
    std::function<int(int)> solve = [&](int cur) -> int {
        if (memo[cur] != -1) return memo[cur];

        int best = 1; // starting at cur alone gives score 1
        for (int next : adj[cur]) {
            int candidate = solve(next);
            if (type[cur] == 1 && type[next] == 0) {
                candidate += 1;
            }
            best = std::max(best, candidate);
        }
        memo[cur] = best;
        return best;
    };

    int answer = 0;
    for (int i = 0; i < n; ++i) {
        answer = std::max(answer, solve(i));
    }
    return answer;
}
#include <cassert>
#include <vector>
#include <functional>

// (Solution function is included here for the test to compile standalone)
int longestSpecialPath(const std::vector<std::vector<int>>& adj, const std::vector<int>& type) {
    int n = (int)type.size();
    std::vector<int> memo(n, -1);
    std::function<int(int)> solve = [&](int cur) -> int {
        if (memo[cur] != -1) return memo[cur];
        int best = 1;
        for (int next : adj[cur]) {
            int candidate = solve(next);
            if (type[cur] == 1 && type[next] == 0) {
                candidate += 1;
            }
            best = std::max(best, candidate);
        }
        memo[cur] = best;
        return best;
    };
    int ans = 0;
    for (int i = 0; i < n; ++i) ans = std::max(ans, solve(i));
    return ans;
}

int main() {
    // Test 1: Simple chain: 1 -> 0 -> 0, starting from type1 gives +1, then no more.
    {
        std::vector<std::vector<int>> adj = {{1}, {2}, {}};
        std::vector<int> type = {1, 0, 0};
        assert(longestSpecialPath(adj, type) == 2); // path 0->1 gives 1+1=2, 1->2 gives 1 (no increment), so max 2.
    }
    // Test 2: All type 0, no increments, so max is 1.
    {
        std::vector<std::vector<int>> adj = {{0}, {1}, {}}; // note self-loop? Actually DAG cannot have self-loop, but this is invalid; use proper DAG
        // Let's use: 0->1, 1->2, all type 0
        adj = {{1}, {2}, {}};
        std::vector<int> type = {0, 0, 0};
        assert(longestSpecialPath(adj, type) == 1);
    }
    // Test 3: All type 1, no 0 reachable, max is 1.
    {
        std::vector<std::vector<int>> adj = {{1}, {2}, {}};
        std::vector<int> type = {1, 1, 1};
        assert(longestSpecialPath(adj, type) == 1);
    }
    // Test 4: Disconnected vertices, one is type1 -> type0, others single.
    {
        std::vector<std::vector<int>> adj = {{1}, {}, {}};
        std::vector<int> type = {1, 0, 0};
        assert(longestSpecialPath(adj, type) == 2);
    }
    // Test 5: Multiple paths, pick the longest. Graph: 0(type1)->1(type0), 0->2(type0), 2->3(type0). Best path: 0->1 (score2) or 0->2->3 (score 2? 0->2 gives +1, 2->3 no +, total 2). So ans=2.
    {
        std::vector<std::vector<int>> adj = {{1, 2}, {}, {3}, {}};
        std::vector<int> type = {1, 0, 0, 0};
        assert(longestSpecialPath(adj, type) == 2);
    }
    // Test 6: Longer chain with multiple type1->type0 transitions.
    // 0(1)->1(0)->2(1)->3(0). Path 0->1 gives +1, 1->2 no, 2->3 gives +1, total score starting at 0: 1+1+1 = 3.
    {
        std::vector<std::vector<int>> adj = {{1}, {2}, {3}, {}};
        std::vector<int> type = {1, 0, 1, 0};
        assert(longestSpecialPath(adj, type) == 3);
    }
    // Test 7: Single vertex.
    {
        std::vector<std::vector<int>> adj = {{}};
        std::vector<int> type = {0};
        assert(longestSpecialPath(adj, type) == 1);
        type = {1};
        assert(longestSpecialPath(adj, type) == 1);
    }
    // Test 8: Graph with branching that yields different scores.
    // 0(1) -> 1(0) and 0 -> 2(1), 2 -> 3(0). Path 0->1 gives 2, 0->2->3 gives 1+1+1=3? Let's compute:
    // start at 0: dp[0] = max(1, dp[1]+1 (since 1 is type0) = 2, dp[2]+0 (since 2 type1) ). dp[2] = max(1, dp[3]+1 since 3 type0) = 2. So dp[0]=max(1,2,2)=2? Actually dp[2]=2, then candidate from 0 to 2 is dp[2]+0=2, so dp[0]=2. But what about starting at 2 directly? dp[2]=2. So max is 2, not 3. The transition 2->3 adds +1, but that's included in dp[2]=2. So answer=2.
    {
        std::vector<std::vector<int>> adj = {{1,2}, {}, {3}, {}};
        std::vector<int> type = {1, 0, 1, 0};
        assert(longestSpecialPath(adj, type) == 2);
    }
    // Test 9: A more complex DAG: 0(1)->1(0), 1->2(0), 2->3(1). Best path: 0->1 (score2), but 0->1->2 (score2), nothing adds later. So max=2.
    {
        std::vector<std::vector<int>> adj = {{1}, {2}, {3}, {}};
        std::vector<int> type = {1, 0, 0, 1};
        assert(longestSpecialPath(adj, type) == 2);
    }
    return 0;
}
