// Write a C++ function that takes an unweighted, undirected graph represented as an adjacency list (a `vector<vector<int>>`) along with the total number of vertices `v`, a source vertex `src`, and a destination vertex `des`. The function must return a `std::pair<bool, std::vector<int>>` where the boolean indicates whether a path exists between `src` and `des`, and if a path exists, the vector contains the shortest path from `src` to `des` (inclusive of both endpoints) in terms of the number of edges. If no path exists, the vector should be empty. The graph is 0-indexed, all vertices are integers from 0 to v-1, and there are no self-loops or duplicate edges. The function must be named `shortestPath` and must not modify the input graph. Handle the case where `src == des` by returning `{true, {src}}`.

// The problem requires finding the shortest path in an unweighted graph, which is naturally solved using Breadth-First Search (BFS). BFS explores nodes in increasing distance from the source, guaranteeing that the first time we reach the destination, we have found the minimum number of edges. We maintain a `visited` array to avoid revisiting nodes and an array `parent` or `predecessor` to record which node led to the discovery of each node. This allows us to reconstruct the path by backtracking from `des` to `src` after BFS completes. The initialization sets all distances to infinity and parents to -1, with `dist[src] = 0` and `parent[src] = src` (or a sentinel). We use a queue for BFS. Edge cases include: (1) `src == des` — the path is trivial, return immediately. (2) When the graph is disconnected and no path exists — BFS completes without visiting `des`, and we return `{false, {}}`. (3) The graph may have up to `v` vertices, and if `src` or `des` is out of range, but the problem guarantees valid input, so no extra checks are needed. The time complexity is O(V + E) because each vertex is enqueued at most once and each edge is scanned once. The space complexity is O(V) for the visited, parent, distance arrays, and the queue.

#include <vector>
#include <queue>
#include <utility>
#include <algorithm>

// Returns {exists, shortest_path} where shortest_path is the sequence of
// vertices from src to des inclusive. If no path, the vector is empty.
std::pair<bool, std::vector<int>> shortestPath(
    const std::vector<std::vector<int>>& adj,
    int v,
    int src,
    int des
) {
    // Trivial case: source equals destination
    if (src == des) {
        return {true, {src}};
    }

    // BFS setup
    std::vector<bool> visited(v, false);
    std::vector<int> parent(v, -1);
    std::queue<int> q;

    visited[src] = true;
    parent[src] = src; // sentinel for reconstruction
    q.push(src);

    bool found = false;

    while (!q.empty() && !found) {
        int u = q.front();
        q.pop();

        for (int neighbor : adj[u]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = u;
                q.push(neighbor);

                if (neighbor == des) {
                    found = true;
                    break;
                }
            }
        }
    }

    if (!found) {
        return {false, {}};
    }

    // Reconstruct path from des back to src
    std::vector<int> path;
    int current = des;
    while (current != src) {
        path.push_back(current);
        current = parent[current];
    }
    path.push_back(src);
    std::reverse(path.begin(), path.end());

    return {true, path};
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is already included above; this test compiles with it.
int main() {
    // Build the graph from the snippet: 8 vertices 0..7
    int v = 8;
    std::vector<std::vector<int>> adj(v);
    adj[0] = {1, 3};
    adj[1] = {0, 2};
    adj[2] = {1};
    adj[3] = {0, 4, 7};
    adj[4] = {3, 5, 6, 7};
    adj[5] = {4, 6};
    adj[6] = {4, 5, 7};
    adj[7] = {3, 4, 6};

    // Test 1: path from 0 to 7 exists, shortest length 2 (0-3-7 or 0-1-... but 0-3-7)
    auto res1 = shortestPath(adj, v, 0, 7);
    assert(res1.first == true);
    assert(res1.second.size() == 3);
    assert(res1.second[0] == 0 && res1.second[2] == 7);

    // Test 2: source equals destination
    auto res2 = shortestPath(adj, v, 4, 4);
    assert(res2.first == true);
    assert(res2.second.size() == 1 && res2.second[0] == 4);

    // Test 3: no path (add an isolated vertex 8, but we use v=9 with no edges)
    std::vector<std::vector<int>> adj2(9);
    adj2[0] = {1}; adj2[1] = {0};
    auto res3 = shortestPath(adj2, 9, 2, 3);
    assert(res3.first == false && res3.second.empty());

    // Test 4: path of length 1 (direct edge)
    auto res4 = shortestPath(adj2, 9, 0, 1);
    assert(res4.first == true);
    assert(res4.second.size() == 2 && res4.second[0] == 0 && res4.second[1] == 1);

    // Test 5: longer path in original graph: 0 to 5 (0-3-4-5)
    auto res5 = shortestPath(adj, v, 0, 5);
    assert(res5.first == true);
    assert(res5.second.size() == 4);
    assert(res5.second.front() == 0 && res5.second.back() == 5);

    // Test 6: disconnected source and destination with multiple components
    std::vector<std::vector<int>> adj3(6);
    adj3[0] = {1}; adj3[1] = {0};
    adj3[2] = {3}; adj3[3] = {2};
    auto res6 = shortestPath(adj3, 6, 4, 5);
    assert(res6.first == false && res6.second.empty());

    return 0;
}
