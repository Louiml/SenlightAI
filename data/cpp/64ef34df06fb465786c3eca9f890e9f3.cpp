Write a C++ function that, given a set of 2D points (as x and y coordinates), constructs a complete undirected graph where every pair of distinct points is connected by an edge whose weight is the Euclidean distance between them, and returns the total length of the minimum spanning tree (MST) of that graph. The function should take a vector of points (each point having `x` and `y` coordinates) and return the sum of edge weights in the MST as a `double`. The input will contain at least 1 point (if only one point, the MST length is 0). The output precision should be handled by the caller, but the function should compute using `double` arithmetic.

The problem requires finding the minimum spanning tree of a complete graph on the given points. Since the graph is dense (every pair of points is an edge), a standard Kruskal's algorithm with a union–find (disjoint set) data structure is appropriate. First, we generate all possible undirected edges (i, j) for i < j, computing the Euclidean distance as the edge weight. We store edges in a vector, then sort them by weight. We initialize a union–find structure with one set per vertex. Then we iterate over the sorted edges; for each edge, if its endpoints are in different sets, we union them and add the edge weight to the answer. The process stops when we have added exactly (numPoints - 1) edges, which is the number of edges in any spanning tree; however, since the graph is complete and connected, it is guaranteed that we can always find a spanning tree. Special cases: if there is only one point, the MST has length 0 and we return 0 immediately. Also, because we generate edges only for i < j, we avoid duplicate undirected edges. The time complexity is dominated by sorting O(K log K) where K = N*(N-1)/2 is the number of edges, and the union–find operations are nearly O(α(N)) per operation; thus total is O(N^2 log N). Space complexity is O(N^2) for storing all edges (since the graph is dense) plus O(N) for the union–find.

#include <vector>
#include <algorithm>
#include <cmath>

struct Point {
    double x, y;
};

// Edge structure for Kruskal's algorithm
struct Edge {
    int u, v;
    double w;
};

// Union-Find (Disjoint Set Union) class
class DSU {
    std::vector<int> parent, rank;
public:
    explicit DSU(int n) {
        parent.resize(n);
        rank.assign(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;
        if (rank[rx] < rank[ry]) parent[rx] = ry;
        else if (rank[rx] > rank[ry]) parent[ry] = rx;
        else { parent[ry] = rx; ++rank[rx]; }
        return true;
    }
};

/**
 * Compute the total length of the minimum spanning tree of the complete graph
 * on the given points. Edges are weighted by Euclidean distance.
 */
double minimumSpanningTreeLength(const std::vector<Point>& points) {
    int n = static_cast<int>(points.size());
    if (n <= 1) return 0.0;

    // Generate all undirected edges
    std::vector<Edge> edges;
    edges.reserve(n * (n - 1) / 2);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dx = points[i].x - points[j].x;
            double dy = points[i].y - points[j].y;
            double w = std::sqrt(dx * dx + dy * dy);
            edges.push_back({i, j, w});
        }
    }

    // Sort edges by weight
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
    });

    DSU dsu(n);
    double total = 0.0;
    int edgesUsed = 0;
    for (const auto& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            total += e.w;
            ++edgesUsed;
            if (edgesUsed == n - 1) break; // MST complete
        }
    }
    return total;
}

#include <cassert>
#include <cmath>
#include <vector>

int main() {
    // Single point -> 0
    std::vector<Point> p1 = {{0.0, 0.0}};
    assert(std::fabs(minimumSpanningTreeLength(p1) - 0.0) < 1e-9);

    // Two points -> distance between them
    std::vector<Point> p2 = {{0.0, 0.0}, {3.0, 4.0}};
    assert(std::fabs(minimumSpanningTreeLength(p2) - 5.0) < 1e-9);

    // Three points forming a right triangle (3-4-5) -> MST is sum of two shortest edges = 3+4 = 7
    std::vector<Point> p3 = {{0.0, 0.0}, {3.0, 0.0}, {0.0, 4.0}};
    assert(std::fabs(minimumSpanningTreeLength(p3) - 7.0) < 1e-9);

    // Four points: perfect square of side 1 -> MST length = 3 (three edges of length 1)
    std::vector<Point> p4 = {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}};
    assert(std::fabs(minimumSpanningTreeLength(p4) - 3.0) < 1e-9);

    // Points forming a line: (0,0), (1,0), (2,0) -> MST = 1 + 1 = 2
    std::vector<Point> p5 = {{0.0, 0.0}, {1.0, 0.0}, {2.0, 0.0}};
    assert(std::fabs(minimumSpanningTreeLength(p5) - 2.0) < 1e-9);
    
    // Duplicate points: two identical points at (0,0) and one at (1,0) -> MST edges: 0 and 1 = 1
    std::vector<Point> p6 = {{0.0, 0.0}, {0.0, 0.0}, {1.0, 0.0}};
    assert(std::fabs(minimumSpanningTreeLength(p6) - 1.0) < 1e-9);

    return 0;
}
