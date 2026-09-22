Write a C++ function `double minimumSpanningTreeWeight(const std::vector<std::pair<double, double>>& points)` that accepts a list of 2D points and returns the total weight (sum of Euclidean distances) of a Minimum Spanning Tree (MST) connecting all points. The graph is complete, meaning every pair of points is directly connected by an edge whose weight is the Euclidean distance between them. If the input contains fewer than two points, return `0.0`. The function must handle floating-point coordinates (including negative and zero values) and produce results accurate to at least two decimal places. Edge cases include duplicate points (distance zero between them), points forming a straight line, and a relatively large number of points (up to 1000). The solution must not rely on any external libraries beyond the C++ standard library.

The problem is the classic Minimum Spanning Tree on a complete graph. The most straightforward approach is Kruskal’s algorithm: generate all \(n(n-1)/2\) edges, sort them by weight, and use a Disjoint Set Union (DSU) structure to add edges in increasing order, skipping edges that would create a cycle. The total weight is accumulated. This works correctly even with floating-point weights; sorting uses `<` on `double`, which is fine for comparison. Duplicate points produce zero-weight edges, which are naturally processed first. For fewer than two points, no edges exist, so the result is zero. Time complexity is dominated by generating edges \(O(n^2)\) and sorting them \(O(n^2 \log n)\), where \(n\) is the number of points. Space complexity is \(O(n^2)\) for edge storage and \(O(n)\) for DSU. Another approach is Prim’s algorithm with an \(O(n^2)\) time complexity, which avoids storing all edges; however, Kruskal is simpler to implement given the complete graph. We must be careful about precision: the sum of distances may accumulate floating-point errors, but with `double` and typical constraints it is acceptable. The DSU must use path compression and union by size/rank for efficiency. Also, the function should be `const`-correct, taking the input vector by `const&` and not modifying it.

#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>

// Return the total weight of a Minimum Spanning Tree over the given points.
// If there are fewer than two points, returns 0.0.
double minimumSpanningTreeWeight(const std::vector<std::pair<double, double>>& points) {
    const size_t n = points.size();
    if (n < 2) return 0.0;

    struct Edge {
        size_t u, v;
        double w;
    };

    std::vector<Edge> edges;
    edges.reserve(n * (n - 1) / 2);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            double dx = points[i].first - points[j].first;
            double dy = points[i].second - points[j].second;
            double dist = std::sqrt(dx * dx + dy * dy);
            edges.push_back({i, j, dist});
        }
    }

    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
    });

    std::vector<int> parent(n);
    std::vector<int> rank(n, 0);
    std::iota(parent.begin(), parent.end(), 0);

    std::function<int(int)> find = [&](int x) -> int {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    };

    auto unite = [&](int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return;
        if (rank[x] < rank[y]) std::swap(x, y);
        parent[y] = x;
        if (rank[x] == rank[y]) rank[x]++;
    };

    double total = 0.0;
    size_t edges_needed = n - 1;
    size_t edges_added = 0;

    for (const auto& e : edges) {
        if (edges_added == edges_needed) break;
        if (find(e.u) != find(e.v)) {
            unite(e.u, e.v);
            total += e.w;
            edges_added++;
        }
    }

    return total;
}

#include <cassert>
#include <cmath>
#include <vector>

// Include the solution function declaration here if not already.
// double minimumSpanningTreeWeight(const std::vector<std::pair<double, double>>& points);

int main() {
    // Single point -> zero
    assert(minimumSpanningTreeWeight({{0.0, 0.0}}) == 0.0);

    // Two identical points -> zero distance
    assert(minimumSpanningTreeWeight({{1.0, 1.0}, {1.0, 1.0}}) == 0.0);

    // Two points at distance 3
    assert(std::abs(minimumSpanningTreeWeight({{0.0, 0.0}, {3.0, 0.0}}) - 3.0) < 1e-9);

    // Three points forming a right triangle: (0,0), (3,0), (0,4)
    // Edges: 3,4,5 -> MST uses 3 and 4, total 7
    assert(std::abs(minimumSpanningTreeWeight({{0.0, 0.0}, {3.0, 0.0}, {0.0, 4.0}}) - 7.0) < 1e-9);

    // Four points on a square: (0,0), (1,0), (0,1), (1,1)
    // MST uses three edges of length 1 each -> total 3
    std::vector<std::pair<double, double>> square = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}};
    assert(std::abs(minimumSpanningTreeWeight(square) - 3.0) < 1e-9);

    // Points along a line equally spaced by 2: n=5, total = 8
    std::vector<std::pair<double, double>> line;
    for (int i = 0; i < 5; ++i) line.push_back({static_cast<double>(i * 2), 0.0});
    assert(std::abs(minimumSpanningTreeWeight(line) - 8.0) < 1e-9);

    // Duplicate points together with others: (0,0), (0,0), (10,0)
    // MST uses edge of 0 and edge of 10 -> total 10
    assert(std::abs(minimumSpanningTreeWeight({{0.0, 0.0}, {0.0, 0.0}, {10.0, 0.0}}) - 10.0) < 1e-9);

    // Negative coordinates: (-1,-1), (1,1) distance sqrt(8)
    double expected = std::sqrt(8.0);
    assert(std::abs(minimumSpanningTreeWeight({{-1.0, -1.0}, {1.0, 1.0}}) - expected) < 1e-9);

    return 0;
}
