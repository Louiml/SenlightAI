// Given a vector of undirected graph edges where each edge connects two nodes labeled from 1 to N (N = number of edges) and exactly one redundant edge creates a cycle, write a C++ function `std::vector<int> findRedundantEdge(const std::vector<std::vector<int>>& edges)` that returns the redundant edge that appears last in the input. The graph initially is a tree (connected, no cycles) before adding one extra edge. Each edge is represented as a vector of two integers, and if multiple edges can be the redundant one, return the one that appears later in the input. Assume the input is non-empty and always contains exactly one redundant edge. The function must use the Union-Find (Disjoint Set Union) algorithm to detect the first edge that connects two nodes already in the same connected component.

The solution uses the Union-Find data structure with path compression and union by size (or simple parent array). Since the graph's nodes are labeled 1 to N, we map each node `v` to index `v-1` in a parent array initialized to -1 (meaning each node is its own root). For each edge `(u, v)` in order:
- Find the root of `u-1` and `v-1` using a recursive or iterative `find` function. If the roots are the same, then adding this edge creates a cycle, so this edge is the redundant one and we return it immediately.
- Otherwise, union the two distinct roots by setting one's parent to the other.

Since the input is guaranteed to have exactly one redundant edge, the loop always returns. Path compression makes `find` nearly constant amortized time. The time complexity is O(N α(N)) where α is the inverse Ackermann function (practically constant), and space complexity is O(N) for the parent array. The edge case where the redundant edge causes a cycle among nodes already connected is handled correctly because we return the first such edge encountered, which by the problem statement corresponds to the last edge in the input that creates a cycle.

#include <vector>
#include <numeric>

// Union-Find with path compression and union by assignment.
class UnionFind {
public:
    explicit UnionFind(int n) : parent_(n, -1) {}

    int find(int x) {
        if (parent_[x] == -1) return x;
        return parent_[x] = find(parent_[x]); // path compression
    }

    void unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra != rb) {
            parent_[ra] = rb; // simple union
        }
    }

private:
    std::vector<int> parent_;
};

// Return the redundant edge that, when added, creates a cycle.
std::vector<int> findRedundantEdge(const std::vector<std::vector<int>>& edges) {
    UnionFind uf(edges.size());
    for (const auto& edge : edges) {
        int u = edge[0] - 1; // convert to zero-based index
        int v = edge[1] - 1;
        if (uf.find(u) == uf.find(v)) {
            return edge; // first edge forming a cycle is the redundant one
        }
        uf.unite(u, v);
    }
    return {}; // should never reach here per problem constraints
}

#include <cassert>
#include <vector>

// (The solution function and UnionFind class are defined above, in the same translation unit.)

int main() {
    // Test 1: basic cycle of 3 nodes
    std::vector<std::vector<int>> edges1 = {{1,2}, {1,3}, {2,3}};
    std::vector<int> res1 = findRedundantEdge(edges1);
    assert((res1 == std::vector<int>{2,3}));

    // Test 2: cycle among 4 nodes, redundant edge is last
    std::vector<std::vector<int>> edges2 = {{1,2}, {2,3}, {3,4}, {1,4}};
    std::vector<int> res2 = findRedundantEdge(edges2);
    assert((res2 == std::vector<int>{1,4}));

    // Test 3: redundant edge appears not last but when cycle forms
    std::vector<std::vector<int>> edges3 = {{1,2}, {2,3}, {3,1}, {3,4}};
    std::vector<int> res3 = findRedundantEdge(edges3);
    assert((res3 == std::vector<int>{3,1}));

    // Test 4: redundant edge connects two nodes already in same component via path
    std::vector<std::vector<int>> edges4 = {{1,2}, {2,3}, {3,4}, {4,1}, {5,4}};
    std::vector<int> res4 = findRedundantEdge(edges4);
    assert((res4 == std::vector<int>{4,1}));

    // Test 5: same node pair appears twice (self-loop not allowed, but duplicate edge)
    std::vector<std::vector<int>> edges5 = {{1,2}, {2,3}, {1,2}};
    std::vector<int> res5 = findRedundantEdge(edges5);
    assert((res5 == std::vector<int>{1,2}));

    // Test 6: larger graph with cycle at the end
    std::vector<std::vector<int>> edges6 = {{1,2}, {2,3}, {3,4}, {4,5}, {5,2}};
    std::vector<int> res6 = findRedundantEdge(edges6);
    assert((res6 == std::vector<int>{5,2}));

    // Test 7: redundant edge appears first (already two nodes sharing same component later)
    std::vector<std::vector<int>> edges7 = {{1,2}, {2,3}, {3,1}};
    std::vector<int> res7 = findRedundantEdge(edges7);
    assert((res7 == std::vector<int>{3,1}));

    // Test 8: all nodes in one line, extra edge connects endpoints
    std::vector<std::vector<int>> edges8 = {{1,2}, {2,3}, {3,4}, {1,4}};
    std::vector<int> res8 = findRedundantEdge(edges8);
    assert((res8 == std::vector<int>{1,4}));

    // Test 9: duplicate edge in a simple tree (only cycle is the duplicate)
    std::vector<std::vector<int>> edges9 = {{1,2}, {2,3}, {1,2}};
    std::vector<int> res9 = findRedundantEdge(edges9);
    assert((res9 == std::vector<int>{1,2}));

    // Test 10: redundant edge is not last but creates cycle earlier
    std::vector<std::vector<int>> edges10 = {{1,2}, {3,4}, {2,3}, {1,4}};
    std::vector<int> res10 = findRedundantEdge(edges10);
    assert((res10 == std::vector<int>{1,4}));
}
