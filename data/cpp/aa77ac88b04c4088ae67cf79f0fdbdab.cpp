Write a C++ function that, given a connected undirected weighted graph represented by a vector of edges (each edge contains two endpoint vertices and a positive integer weight) and an integer number of vertices, returns the total minimum cost of a spanning tree connecting all vertices, computed using Kruskal's algorithm. The vertices are labeled from 0 to `num_vertices - 1`. The input graph is guaranteed to be connected and may contain parallel edges (multiple edges between the same pair of vertices) but no self-loops. The function must return a `long long` to accommodate large costs, and if the graph is not connected, return -1.
#include <cassert>

int main() {
    // Test 1: Simple triangle with weights 1, 2, 3 -> MST cost = 1+2 = 3
    std::vector<Edge> edges1 = {{0,1,1}, {1,2,2}, {0,2,3}};
    assert(minimumSpanningTreeCost(3, edges1) == 3);

    // Test 2: Single vertex -> cost 0
    std::vector<Edge> edges2 = {};
    assert(minimumSpanningTreeCost(1, edges2) == 0);

    // Test 3: Parallel edges (0-1 weight 5 and 7), plus 1-2 weight 2 -> choose 5 and 2 => cost 7
    std::vector<Edge> edges3 = {{0,1,7}, {1,2,2}, {0,1,5}};
    assert(minimumSpanningTreeCost(3, edges3) == 7);

    // Test 4: Disconnected graph (two separate edges) -> return -1
    std::vector<Edge> edges4 = {{0,1,1}, {2,3,1}};
    assert(minimumSpanningTreeCost(4, edges4) == -1);

    // Test 5: Larger graph with cycle, where cheapest two edges form MST
    std::vector<Edge> edges5 = {{0,1,10}, {1,2,20}, {0,2,15}, {2,3,5}, {0,3,100}};
    // Correct MST edges: {2,3,5}, {0,1,10}, {0,2,15} => total 30
    assert(minimumSpanningTreeCost(4, edges5) == 30);

    // Test 6: All equal weights, connected graph of 4 vertices with 6 edges (complete graph weight 7)
    std::vector<Edge> edges6 = {{0,1,7},{0,2,7},{0,3,7},{1,2,7},{1,3,7},{2,3,7}};
    assert(minimumSpanningTreeCost(4, edges6) == 21); // 3 edges * 7

    // Test 7: Negative weights not allowed (but ensure positive only; edge case if negative would still work logically, but we assume positive per spec)
    // Test 8: Vertices are 0-indexed; ensure correct cost for a path
    std::vector<Edge> edges8 = {{0,1,1},{1,2,1},{2,3,1}};
    assert(minimumSpanningTreeCost(4, edges8) == 3);

    // Test 9: Disconnected but with parallel edges in different components
    std::vector<Edge> edges9 = {{0,1,1},{0,1,2},{2,2,0}}; // self-loop not allowed, but if present should be ignored? Not in spec, skip.
    // Instead: two components both with multiple edges -> disconnected returns -1
    std::vector<Edge> edges9b = {{0,1,2},{0,1,3},{2,3,1}};
    assert(minimumSpanningTreeCost(4, edges9b) == -1);

    // Test 10: Single edge graph with 2 vertices -> cost = edge weight
    std::vector<Edge> edges10 = {{0,1,42}};
    assert(minimumSpanningTreeCost(2, edges10) == 42);

    return 0;
}
#include <vector>
#include <algorithm>
#include <numeric>

// Union-Find (Disjoint Set Union) helper class with path compression and union by size.
class DSU {
private:
    std::vector<int> parent;
    std::vector<int> size;
public:
    explicit DSU(int n) : parent(n), size(n, 1) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // path compression
        }
        return parent[x];
    }
    bool unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) return false;
        // union by size
        if (size[ra] < size[rb]) std::swap(ra, rb);
        parent[rb] = ra;
        size[ra] += size[rb];
        return true;
    }
};

// Edge representation for the weighted graph.
struct Edge {
    int u;
    int v;
    int weight;
    // comparator for sorting by weight
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Compute the minimum spanning tree total cost using Kruskal's algorithm.
// Returns -1 if the graph is not connected.
long long minimumSpanningTreeCost(int num_vertices, const std::vector<Edge>& edges) {
    if (num_vertices <= 0) return -1;
    if (num_vertices == 1) return 0; // trivial spanning tree cost is 0

    // Sort edges by weight in ascending order
    std::vector<Edge> sorted_edges = edges;
    std::sort(sorted_edges.begin(), sorted_edges.end());

    DSU dsu(num_vertices);
    long long total_cost = 0;
    int edges_used = 0;

    for (const auto& e : sorted_edges) {
        if (dsu.unite(e.u, e.v)) {
            total_cost += e.weight;
            ++edges_used;
            if (edges_used == num_vertices - 1) {
                break; // spanning tree complete
            }
        }
    }

    if (edges_used != num_vertices - 1) {
        return -1; // graph is disconnected
    }
    return total_cost;
}
// The solution uses Kruskal's algorithm, which builds a minimum spanning tree by sorting all edges in non-decreasing order of weight and adding each edge in that order if it connects two different components, leveraging the union-find (disjoint-set) data structure with path compression and union by size for near-constant time operations. First, sort the edge list by weight. Then initialize a union-find structure for `num_vertices` components. Iterate through sorted edges; for each edge, if the endpoints belong to different sets, union them and add the edge's weight to the total cost. Stop early once `num_vertices - 1` edges have been added, as a spanning tree is complete. If after processing all edges the number of added edges is less than `num_vertices - 1`, the graph is disconnected, so return -1. Edge cases: parallel edges—the algorithm naturally picks the cheapest ones due to sorting; single vertex graph (num_vertices == 1) should return 0 because no edges are needed. Time complexity: O(E log E) due to sorting, where E is the number of edges, plus nearly O(E α(V)) for union-find operations. Space complexity: O(V) for the parent/rank arrays.
