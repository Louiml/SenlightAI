Given a positive integer `n` representing the number of points, followed by `n` pairs of integer coordinates `(x, y)`, and then a list of `m` undirected edges that already connect certain pairs of points (1-indexed), write a C++ function `double minimumAdditionalConnectionCost(int n, const vector<pair<int,int>>& points, const vector<pair<int,int>>& existingEdges)` that computes the minimum total Euclidean distance needed to connect all points into a single connected component. The existing edges are already built at zero additional cost, and you may add any new edges between any two points, paying the Euclidean distance between them. The function must return the total cost as a `double`, rounded to two decimal places (i.e., return the exact value that when formatted with `printf("%.2lf")` gives the correct rounded result). The input coordinates are integers between -1000 and 1000, `n` is between 1 and 500, and `m` can be zero. Assume that the existing edges may create cycles or multiple components, and that the points are distinct.

#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>

// Function declaration (as per solution above)
double minimumAdditionalConnectionCost(int n, const std::vector<std::pair<int,int>>& points,
                                       const std::vector<std::pair<int,int>>& existingEdges);

int main() {
    // Test 1: Already connected by existing edges -> cost 0
    {
        int n = 3;
        std::vector<std::pair<int,int>> pts = {{0,0}, {1,0}, {0,1}};
        std::vector<std::pair<int,int>> edges = {{1,2}, {2,3}};
        double ans = minimumAdditionalConnectionCost(n, pts, edges);
        assert(std::fabs(ans - 0.0) < 1e-9);
    }
    // Test 2: No existing edges, three points in an equilateral triangle (side 1) -> MST cost 2 (two edges of length 1)
    {
        int n = 3;
        std::vector<std::pair<int,int>> pts = {{0,0}, {1,0}, {0,1}};
        std::vector<std::pair<int,int>> edges;
        double ans = minimumAdditionalConnectionCost(n, pts, edges);
        // distances: (1,2)=1, (2,3)=sqrt(2), (1,3)=1 => MST = 1 + 1 = 2
        assert(std::fabs(ans - 2.0) < 1e-9);
    }
    // Test 3: Two existing edges forming a cycle but all connected -> cost 0
    {
        int n = 3;
        std::vector<std::pair<int,int>> pts = {{0,0}, {10,10}, {20,0}};
        std::vector<std::pair<int,int>> edges = {{1,2}, {2,3}, {3,1}};
        double ans = minimumAdditionalConnectionCost(n, pts, edges);
        assert(std::fabs(ans - 0.0) < 1e-9);
    }
    // Test 4: Single point -> cost 0
    {
        int n = 1;
        std::vector<std::pair<int,int>> pts = {{5,5}};
        std::vector<std::pair<int,int>> edges;
        double ans = minimumAdditionalConnectionCost(n, pts, edges);
        assert(std::fabs(ans - 0.0) < 1e-9);
    }
    // Test 5: Two components, each with one existing edge, need one more edge
    {
        int n = 4;
        std::vector<std::pair<int,int>> pts = {{0,0}, {1,0}, {10,0}, {11,0}};
        std::vector<std::pair<int,int>> edges = {{1,2}, {3,4}}; // two separate components
        double ans = minimumAdditionalConnectionCost(n, pts, edges);
        // Component A: points 1,2 ; Component B: points 3,4 ; closest edge between A and B: (2,3) distance 9
        assert(std::fabs(ans - 9.0) < 1e-9);
    }
    // Test 6: All points collinear, no existing edges, n=4 -> MST connects 3 edges of lengths 1,1,1 (if spaced equally)
    {
        int n = 4;
        std::vector<std::pair<int,int>> pts = {{0,0}, {1,0}, {2,0}, {3,0}};
        std::vector<std::pair<int,int>> edges;
        double ans = minimumAdditionalConnectionCost(n, pts, edges);
        assert(std::fabs(ans - 3.0) < 1e-9);
    }
    // Test 7: Existing edge connects two far apart points, but a shorter path through others exists? 
    // Actually existing edge is free, so must use it, but still connect all.
    {
        int n = 3;
        std::vector<std::pair<int,int>> pts = {{0,0}, {100,0}, {0,100}};
        std::vector<std::pair<int,int>> edges = {{1,2}}; // cost 0 for 1-2
        double ans = minimumAdditionalConnectionCost(n, pts, edges);
        // Need to connect point 3 to either 1 (dist 100) or 2 (dist sqrt(100^2+100^2)=141.42) => choose 100
        assert(std::fabs(ans - 100.0) < 1e-9);
    }
    // Test 8: Negative coordinates
    {
        int n = 3;
        std::vector<std::pair<int,int>> pts = {{-1,-1}, {2,3}, {5,-2}};
        std::vector<std::pair<int,int>> edges;
        double ans = minimumAdditionalConnectionCost(n, pts, edges);
        // Compute manually: 
        // d12 = sqrt(3^2+4^2)=5, d13 = sqrt(6^2+1^2)=sqrt(37)≈6.083, d23 = sqrt(3^2+(-5)^2)=sqrt(34)≈5.831
        // MST uses d12 (5) and d23 (5.831) -> total ≈10.831
        assert(std::fabs(ans - (5.0 + std::sqrt(34.0))) < 1e-9);
    }
    // Test 9: Large n but simple pattern – all points same? (distinct points assumed, but just in case)
    // Skip
    // Test 10: Realistic scenario with two existing edges that create one component and one isolated point
    {
        int n = 5;
        std::vector<std::pair<int,int>> pts = {{0,0}, {3,4}, {6,0}, {10,10}, {15,0}};
        std::vector<std::pair<int,int>> edges = {{1,2}, {2,3}}; // points 1,2,3 connected
        double ans = minimumAdditionalConnectionCost(n, pts, edges);
        // Now components: {1,2,3} and {4} and {5}. Need to connect 4 and 5 to the big component.
        // Compute distances: from 4 to each: to 1: sqrt(10^2+10^2)=14.14, to 2: sqrt(7^2+6^2)=9.22, to 3: sqrt(4^2+10^2)=10.77 -> best 9.22
        // from 5 to big component: to 1:15, to 2: sqrt(12^2+(-4)^2)=12.65, to 3: sqrt(9^2+0)=9 -> best 9
        // Then connect 5 to 4? Actually we need two edges: one connecting 4 to a point in big comp, and one connecting 5 to any component.
        // Best is to connect 4 to 2 (9.22) and 5 to 3 (9) or 5 to 4 (sqrt(5^2+(-10)^2)=11.18) – but then connect 4 to big comp, so total 9.22 + min(9, 11.18) = 18.22
        double expected = 9.22 + 9.0; // but we must check exact: 4->2 distance = sqrt(7^2+6^2)=sqrt(85)=9.2195, 5->3 distance=9
        assert(std::fabs(ans - (std::sqrt(85.0) + 9.0)) < 1e-9);
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <cmath>
#include <algorithm>

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;
public:
    UnionFind(int n) : parent(n + 1), rank(n + 1, 0) {
        for (int i = 1; i <= n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) return;
        if (rank[rx] < rank[ry]) {
            parent[rx] = ry;
        } else if (rank[rx] > rank[ry]) {
            parent[ry] = rx;
        } else {
            parent[ry] = rx;
            ++rank[rx];
        }
    }
};

struct Edge {
    int u, v;
    double cost;
    bool operator<(const Edge& other) const {
        return cost < other.cost;
    }
};

// Computes the minimum total Euclidean distance needed to connect all points,
// given some edges are already built at zero cost.
double minimumAdditionalConnectionCost(int n, const std::vector<std::pair<int,int>>& points,
                                       const std::vector<std::pair<int,int>>& existingEdges) {
    UnionFind uf(n);
    // Process existing edges: union the components they connect.
    for (const auto& e : existingEdges) {
        uf.unite(e.first, e.second);
    }
    
    // Generate all possible new edges between points in different components.
    std::vector<Edge> edges;
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            if (uf.find(i) != uf.find(j)) {
                double dx = static_cast<double>(points[i-1].first) - points[j-1].first;
                double dy = static_cast<double>(points[i-1].second) - points[j-1].second;
                double dist = std::sqrt(dx * dx + dy * dy);
                edges.push_back({i, j, dist});
            }
        }
    }
    
    // Kruskal's algorithm to find minimum spanning tree of the components.
    std::sort(edges.begin(), edges.end());
    double totalCost = 0.0;
    int components = 0;
    // Count distinct components initially (after existing edges).
    // Alternative: count edges used until components-1 edges are added.
    for (int i = 1; i <= n; ++i) {
        if (uf.find(i) == i) ++components;
    }
    int edgesNeeded = components - 1;
    int used = 0;
    for (const auto& e : edges) {
        if (used == edgesNeeded) break;
        int ru = uf.find(e.u);
        int rv = uf.find(e.v);
        if (ru != rv) {
            uf.unite(ru, rv);
            totalCost += e.cost;
            ++used;
        }
    }
    return totalCost;
}

// The problem is a variant of the Minimum Spanning Tree (MST) problem where some edges are pre‑existing at zero cost. The goal is to connect all components formed by the existing edges using the cheapest possible new edges. The approach is:  
// 1. Initialize a union‑find (disjoint set) structure with each point as its own set.  
// 2. For each existing edge `(u, v)`, union the sets containing `u` and `v` if they are different. This merges the components that are already connected for free.  
// 3. After processing all existing edges, we have a forest of `k` components. The remaining task is to build an MST that connects these components, where the weight of an edge between two points is the Euclidean distance. Since any edge between two points that are already in the same component is irrelevant, we consider only edges between points in different components.  
// 4. Generate all possible edges `(i, j)` with `i < j` and if `find(i) != find(j)`, add that edge to a candidate list. Then run Kruskal’s algorithm on this list to find the minimum total cost to merge all components.  
// 5. The algorithm correctly handles cases where existing edges already connect all points (then the cost is 0), when m=0 (pure MST over all points), and when cycles exist in the existing edges (union-find ignores redundant edges).  
// Time complexity: Building all candidate edges takes O(n²) time, and sorting them takes O(n² log n). Union‑find operations are nearly O(α(n)) each. With n up to 500, this is efficient. Space complexity is O(n²) for the edge list, plus O(n) for union‑find arrays.
