/*
Write a standalone C++ function `breadthFirstTraversal` that performs a breadth-first search (BFS) on an undirected graph represented by an adjacency list. The function should take as parameters: a `std::vector<std::list<int>>` adjacency list, a source vertex `s`, an integer `n` for the number of vertices (0-indexed), and three output arrays passed by reference: `std::vector<int>& distance`, `std::vector<int>& parent`, and `std::vector<int>& color`. The function must set `distance[v]` to the shortest-path distance from `s` to `v` (using `INT_MAX` for unreachable vertices), set `parent[v]` to the predecessor of `v` in the BFS tree (using `-1` for the source and for unreachable vertices), and set `color[v]` to 0 (white), 1 (gray), or 2 (black) to reflect the BFS state after completion (all vertices should be black or white; gray should not remain). The function must initialize the source’s distance to 0 and parent to -1 before processing. The graph may be disconnected, and vertices are labeled 0 through n-1. Do not include a `main` function in your solution—only the function and any helpers.
*/
#include <list>
#include <vector>
#include <climits>

/**
 * Performs breadth-first search on an undirected graph.
 * @param adj Adjacency list (0-indexed vertices).
 * @param s Source vertex.
 * @param n Number of vertices (0..n-1).
 * @param distance Output: shortest distance from s to each vertex (INT_MAX if unreachable).
 * @param parent Output: BFS tree parent (-1 for source or unreachable).
 * @param color Output: 0=white, 1=gray, 2=black after BFS.
 */
void breadthFirstTraversal(const std::vector<std::list<int>>& adj, int s, int n,
                           std::vector<int>& distance,
                           std::vector<int>& parent,
                           std::vector<int>& color) {
    // Initialize all vertices
    distance.assign(n, INT_MAX);
    parent.assign(n, -1);
    color.assign(n, 0); // white

    // Source initialization
    distance[s] = 0;
    parent[s] = -1;
    color[s] = 1; // gray

    // Queue for BFS using list (front/pop_front)
    std::list<int> q;
    q.push_back(s);

    while (!q.empty()) {
        int u = q.front();
        q.pop_front();

        // Explore all neighbors
        for (int v : adj[u]) {
            if (color[v] == 0) { // white
                color[v] = 1;    // gray
                distance[v] = distance[u] + 1;
                parent[v] = u;
                q.push_back(v);
            }
        }
        color[u] = 2; // black
    }
}
#include <cassert>
#include <list>
#include <vector>
#include <climits>

// Function under test (declared from solution)
void breadthFirstTraversal(const std::vector<std::list<int>>& adj, int s, int n,
                           std::vector<int>& distance,
                           std::vector<int>& parent,
                           std::vector<int>& color);

int main() {
    // Test 1: Simple connected path 0-1-2
    {
        std::vector<std::list<int>> adj(3);
        adj[0].push_back(1);
        adj[1].push_back(0);
        adj[1].push_back(2);
        adj[2].push_back(1);
        std::vector<int> d, p, c;
        breadthFirstTraversal(adj, 0, 3, d, p, c);
        assert(d[0] == 0); assert(p[0] == -1); assert(c[0] == 2);
        assert(d[1] == 1); assert(p[1] == 0); assert(c[1] == 2);
        assert(d[2] == 2); assert(p[2] == 1); assert(c[2] == 2);
    }

    // Test 2: Disconnected graph
    {
        std::vector<std::list<int>> adj(4);
        adj[0].push_back(1);
        adj[1].push_back(0);
        // vertices 2 and 3 isolated
        std::vector<int> d, p, c;
        breadthFirstTraversal(adj, 0, 4, d, p, c);
        assert(d[0] == 0); assert(d[1] == 1); assert(d[2] == INT_MAX); assert(d[3] == INT_MAX);
        assert(p[2] == -1); assert(p[3] == -1);
        assert(c[0] == 2); assert(c[1] == 2); assert(c[2] == 0); assert(c[3] == 0);
    }

    // Test 3: Source isolated
    {
        std::vector<std::list<int>> adj(3);
        // no edges
        std::vector<int> d, p, c;
        breadthFirstTraversal(adj, 1, 3, d, p, c);
        assert(d[1] == 0); assert(p[1] == -1); assert(c[1] == 2);
        assert(d[0] == INT_MAX); assert(p[0] == -1); assert(c[0] == 0);
    }

    // Test 4: Star graph with center 1
    {
        std::vector<std::list<int>> adj(5);
        adj[1].push_back(0); adj[0].push_back(1);
        adj[1].push_back(2); adj[2].push_back(1);
        adj[1].push_back(3); adj[3].push_back(1);
        adj[1].push_back(4); adj[4].push_back(1);
        std::vector<int> d, p, c;
        breadthFirstTraversal(adj, 1, 5, d, p, c);
        assert(d[1] == 0);
        for (int i = 0; i < 5; ++i) if (i != 1) {
            assert(d[i] == 1);
            assert(p[i] == 1);
            assert(c[i] == 2);
        }
    }

    // Test 5: Cycle with duplicate edges and self-loop
    {
        std::vector<std::list<int>> adj(3);
        adj[0].push_back(0); // self-loop
        adj[0].push_back(1);
        adj[0].push_back(1); // duplicate
        adj[1].push_back(0);
        adj[1].push_back(2);
        adj[2].push_back(1);
        adj[2].push_back(2); // self-loop
        std::vector<int> d, p, c;
        breadthFirstTraversal(adj, 0, 3, d, p, c);
        assert(d[0] == 0); assert(d[1] == 1); assert(d[2] == 2);
        assert(p[0] == -1); assert(p[1] == 0); assert(p[2] == 1);
        assert(c[0] == 2); assert(c[1] == 2); assert(c[2] == 2);
    }

    return 0;
}
// The solution follows the classic BFS algorithm. Initialization: set every vertex’s color to white (0), distance to `INT_MAX`, and parent to -1, except the source which gets distance 0, parent -1, and color gray (1). Use a `std::list<int>` as a queue, push the source, then while the queue is non-empty, pop the front vertex `u`, iterate over its adjacency list, and for each neighbor `v` still white, set its color to gray, distance to `distance[u] + 1`, parent to `u`, and push it onto the queue. After processing all neighbors, set `u`’s color to black (2). Edge cases: (1) the source may be isolated—it will be processed and turned black; (2) disconnected vertices remain white with `INT_MAX` distance and -1 parent; (3) the graph may have self-loops or multiple edges—these are handled naturally because a neighbor already gray or black is skipped. Time complexity is O(n + m) where m is the total number of edges (sum of adjacency list sizes), since each vertex is enqueued exactly once and each adjacency list is scanned once. Space complexity is O(n) for the queue and arrays; the adjacency list itself is given.
