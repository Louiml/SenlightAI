Write a standalone C++ function `int minimumConnections(const std::vector<std::pair<int,int>>& points)` that takes a list of points in a 2D plane, where each point is an (x, y) coordinate pair. Two points are considered directly connected if they share the same x-coordinate or the same y-coordinate. Connectivity is transitive, meaning if point A is connected to B and B is connected to C, then A and C are in the same connected component. The function must return the minimum number of additional edges (connections) needed to make the entire set of points a single connected component, assuming you can add a new edge between any two existing points (the new edge can be between any two points regardless of their coordinates). Note that all input coordinates are integers, there may be duplicate points (treat duplicates as separate points in the list), and the number of points is at least 1. The function should not modify the input vector.

The problem reduces to finding the number of connected components in an undirected graph where each point is a vertex, and an edge exists between two vertices if their x-coordinates are equal or their y-coordinates are equal. Since adding one edge between any two points can merge at most two components, the minimum number of edges needed to connect all components is `(number_of_components - 1)`. We can compute the components using a depth-first search (DFS) or union-find. For simplicity, use DFS: iterate over all points, and for each unvisited point, start a DFS that visits all points reachable via same-x or same-y relationships. Complexity: For each starting DFS, we scan all other points to find neighbors, leading to O(N^2) time worst-case, where N is the number of points. Space complexity is O(N) for the visited array and the recursion stack (though recursion depth can be O(N) in the worst case). Edge cases: single point → components = 1, answer = 0; all points isolated (no shared x/y) → answer = N-1; duplicate points are treated as separate vertices but they share both x and y, so they are in the same component immediately.

#include <vector>
#include <utility>

// Compute the minimum number of additional edges to connect all points.
int minimumConnections(const std::vector<std::pair<int,int>>& points) {
    const int n = static_cast<int>(points.size());
    if (n <= 1) {
        return 0;
    }

    std::vector<bool> visited(n, false);
    int components = 0;

    // Depth-first search to mark all points in the same component.
    auto dfs = [&](int i, auto&& self) -> void {
        visited[i] = true;
        for (int j = 0; j < n; ++j) {
            if (i == j || visited[j]) {
                continue;
            }
            if (points[i].first == points[j].first || points[i].second == points[j].second) {
                self(j, self);
            }
        }
    };

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            ++components;
            dfs(i, dfs);
        }
    }

    return components - 1;
}

#include <cassert>
#include <vector>
#include <utility>

int minimumConnections(const std::vector<std::pair<int,int>>& points);

int main() {
    // Single point → no connections needed.
    assert(minimumConnections({{1, 2}}) == 0);

    // Two points sharing x → already connected.
    assert(minimumConnections({{1, 2}, {1, 5}}) == 0);

    // Two points with no shared coordinate → need 1 edge.
    assert(minimumConnections({{1, 2}, {3, 4}}) == 1);

    // Three points all isolated → need 2 edges.
    assert(minimumConnections({{1, 1}, {2, 2}, {3, 3}}) == 2);

    // Chain: (0,0)-(0,1)-(1,1) → one component.
    assert(minimumConnections({{0, 0}, {0, 1}, {1, 1}}) == 0);

    // Two components: first has two points, second has one.
    assert(minimumConnections({{0, 0}, {0, 1}, {2, 2}}) == 1);

    // Duplicate points are in same component.
    assert(minimumConnections({{5, 5}, {5, 5}, {1, 3}}) == 1);

    // All points share same x → one component.
    assert(minimumConnections({{7, 1}, {7, 2}, {7, 3}, {7, 4}}) == 0);

    // Mixed: component of 3, component of 2, isolated → need 2 edges.
    assert(minimumConnections({{0, 0}, {0, 1}, {1, 1}, {2, 2}, {2, 3}, {5, 5}}) == 2);

    // Large N but still verifiable.
    std::vector<std::pair<int,int>> pts;
    for (int i = 0; i < 100; ++i) {
        pts.push_back({i, i});
    }
    assert(minimumConnections(pts) == 99);

    return 0;
}
