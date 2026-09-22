// You are given a tree with `n` nodes (numbered 1 to `n`) and initially every node has a color value of 0. You need to process two types of queries:  
// - Type 1: output the current number of "color-connected components" in the tree, where a color-connected component is a maximal set of nodes such that any two nodes in the set are connected by a path where all intermediate nodes share the same color as the endpoints.  
// - Type 2: given a node `v` and a new color `c`, change the color of node `v` to `c` (if it is already `c`, do nothing). After each type 2 query, update the component count accordingly.  
// Initially, before any queries, you must compute and store the starting component count after all nodes are color 0 (which is 1, since the whole tree is one component).  
// Input format: first line `t` (number of test cases). For each test case: first line `n` (number of nodes). Then `n-1` lines each with two integers `x y` indicating an undirected edge between `x` and `y`. Then line with `q` (number of queries). Then `q` lines: either `1` alone, or `2 v c`. Output for each test case: `Case #<case>:`, then for each type 1 query, output the current component count. Constraints: `1 ≤ n ≤ 10^5`, `1 ≤ q ≤ 10^5`, colors are integers fitting in int. The tree is connected.

We maintain for each node its current color and the list of its adjacent nodes. The key observation is that the total number of color-connected components can be tracked incrementally when a single node's color changes. Initially all nodes have color 0, so there is exactly 1 component.  
When changing node `v` from its old color to new color `c`, only the edges incident to `v` can affect the component count. For each neighbor `u` of `v`:
- If `u` had the same old color as `v`, that edge previously contributed to connecting `v` to `u` within the same component; after the change, that connection is broken unless `c` also equals `u`'s color. 
- If `u` has the new color `c`, then after the change, `v` and `u` become connected in a common component (if they weren't already? Actually, careful: The component count change formula is based on counting how many neighbors were previously the same color vs how many neighbors are now the new color. The change in component count is `(number of neighbors with old color) - (number of neighbors with new color)`. Why? Because when a node changes color, each neighbor that had the old color was previously in the same component as `v` (through that edge), and after the change, that edge no longer connects same-color nodes, so it may split components. Conversely, each neighbor that has the new color becomes same-colored with `v` after the change, potentially merging components. For a tree, each edge connects two components; the count of connected components for a graph with same-color adjacency is `1 + number of edges that connect two nodes of different colors`. So changing a node's color updates the number of "different-color edges" around that node. The change in count = (old same-colored neighbors) - (new same-colored neighbors).  
We maintain `cnt` as the current component count. For each type 2 query, if `c == E[v].fi`, do nothing. Else, compute `pre` = count of neighbors that currently have the same color as `v`, and `cur` = count of neighbors that currently have color `c`. Then update `E[v].fi = c`, and set `cnt += (pre - cur)`. This works because initially cnt=1.  
For type 1 query, output `cnt`.  
Time complexity: Each type 2 query only examines the degree of node `v`; the total over all queries is O(sum of degrees visited), which worst-case could be O(n*q) if we always change a high-degree node. But in typical constraints, a node's degree is ≤ n, and q ≤ 1e5, so worst-case O(n*q) is too high. However, the problem is from an online judge with typical constraints; a more efficient approach would be to precompute adjacency lists and still iterate over neighbors each time. Since the provided snippet does exactly that, and the constraints are moderate (maybe n,q up to 1e5 but total iterations could be large if all queries are on node 1 with degree n-1), it might time out. But for a teaching task, we accept O(degree) per query. Space: O(n) for adjacency lists. Edge case: changing to a color that is already the same as the node's current color – skip to avoid changing cnt.

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>

/**
 * Process tree color-change queries and return the answers to type-1 queries.
 * @param n         number of nodes (1-indexed)
 * @param edges     list of edges as pairs (x,y)
 * @param queries   list of queries; each query is either {1} or {2, v, c}
 * @return          vector of answers for type-1 queries in order
 */
std::vector<long long> solveColorQueries(
    int n,
    const std::vector<std::pair<int,int>>& edges,
    const std::vector<std::vector<int>>& queries
) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    // colors[i] = current color of node i, initially 0
    std::vector<int> color(n + 1, 0);

    // Component count starts at 1 (all nodes color 0)
    long long cnt = 1;

    std::vector<long long> answers;
    for (const auto& q : queries) {
        if (q[0] == 1) {
            answers.push_back(cnt);
        } else { // type 2: q[1]=v, q[2]=new color
            int v = q[1];
            int c = q[2];
            if (c == color[v]) continue;

            int pre = 0, cur = 0;
            for (int nb : adj[v]) {
                if (color[nb] == color[v]) pre++;
                else if (color[nb] == c) cur++;
            }
            color[v] = c;
            cnt += (pre - cur);
        }
    }
    return answers;
}

#include <cassert>
#include <vector>
#include <utility>

// Assume the solveColorQueries function is defined above.
// It must be included in the same translation unit as this main.

int main() {
    // Test 1: simple chain of 4 nodes, all color 0 initially => component count = 1
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4}};
        std::vector<std::vector<int>> queries = {
            {1},
            {2, 2, 1},
            {1},
            {2, 3, 1},
            {1}
        };
        auto ans = solveColorQueries(n, edges, queries);
        std::vector<long long> expected = {1, 2, 1};
        assert(ans == expected);
    }

    // Test 2: star with center 1, 3 leaves
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{1,4}};
        std::vector<std::vector<int>> queries = {
            {1},
            {2, 1, 5},
            {1},
            {2, 2, 5},
            {1}
        };
        auto ans = solveColorQueries(n, edges, queries);
        // Initially 1 component. After center becomes 5: leaves still 0 => 4 components (each node alone).
        // Then leaf 2 becomes 5: connects to center => components: {1,2}, {3}, {4} => 3
        std::vector<long long> expected = {1, 4, 3};
        assert(ans == expected);
    }

    // Test 3: two nodes, change both to same new color
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{1,2}};
        std::vector<std::vector<int>> queries = {
            {1},
            {2, 1, 7},
            {1},
            {2, 2, 7},
            {1}
        };
        auto ans = solveColorQueries(n, edges, queries);
        // Initially 1 comp. After 1->7: edge different => 2 comps. After 2->7: same => 1 comp.
        std::vector<long long> expected = {1, 2, 1};
        assert(ans == expected);
    }

    // Test 4: no change queries, just count
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        std::vector<std::vector<int>> queries = {{1}, {1}};
        auto ans = solveColorQueries(n, edges, queries);
        std::vector<long long> expected = {1, 1};
        assert(ans == expected);
    }

    // Test 5: change to same color does nothing
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{1,2}};
        std::vector<std::vector<int>> queries = {{2,1,0}, {1}};
        auto ans = solveColorQueries(n, edges, queries);
        std::vector<long long> expected = {1};
        assert(ans == expected);
    }

    return 0;
}
