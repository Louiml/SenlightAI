// Implement a C++ function `bool areConnected(int u, int v, int n, const std::vector<std::pair<int,int>>& unions)` that simulates a disjoint-set union (DSU) structure. The function should process a list of union operations (in order) and then return `true` if `u` and `v` are in the same connected component after all unions are applied, `false` otherwise. The vertices are numbered from 1 to `n` inclusive. The function must support up to `n = 100'000` vertices and up to `200'000` union operations. It must handle self-loops (union of a vertex with itself) and redundant unions (where both vertices are already in the same set) without errors.
// The solution uses a standard disjoint-set union (DSU) with path compression and union by size (or rank). Initially, each vertex is its own parent, and its component size is 1. For each union operation `(a,b)`, we first find the leaders of both vertices using a recursive function with path compression (the recursion depth is at most `O(log n)` due to union by size, but in practice even less). Then, if the leaders are different, we attach the smaller tree to the larger tree (by comparing sizes of the leaders, not the original vertices) to keep the tree balanced. After processing all unions, we simply compare the leaders of `u` and `v` with path compression to determine connectivity. Edge cases: self-loops are ignored because the leaders are the same; redundant unions are also ignored. Time complexity is nearly `O(alpha(n))` per operation (inverse Ackermann function), which is effectively constant for the given constraints, so total time is `O(number_of_unions * alpha(n) + alpha(n))`. Space complexity is `O(n)` for the parent and size arrays.
#include <vector>

class DSU {
private:
    std::vector<int> parent_;
    std::vector<int> size_;
    
public:
    explicit DSU(int n) : parent_(n + 1), size_(n + 1, 1) {
        for (int i = 1; i <= n; ++i) {
            parent_[i] = i;
        }
    }
    
    int find(int v) {
        if (parent_[v] == v) {
            return v;
        }
        parent_[v] = find(parent_[v]);
        return parent_[v];
    }
    
    void unite(int a, int b) {
        int root_a = find(a);
        int root_b = find(b);
        if (root_a == root_b) return;
        if (size_[root_a] < size_[root_b]) {
            std::swap(root_a, root_b);
        }
        parent_[root_b] = root_a;
        size_[root_a] += size_[root_b];
    }
};

bool areConnected(int u, int v, int n, const std::vector<std::pair<int,int>>& unions) {
    DSU dsu(n);
    for (const auto& edge : unions) {
        dsu.unite(edge.first, edge.second);
    }
    return dsu.find(u) == dsu.find(v);
}
#include <cassert>
#include <vector>
#include <utility>

// Assume the solution function is declared above

int main() {
    // Basic connectivity
    {
        int n = 3;
        std::vector<std::pair<int,int>> unions = {{1,2}};
        assert(areConnected(1, 2, n, unions) == true);
        assert(areConnected(1, 3, n, unions) == false);
        assert(areConnected(2, 3, n, unions) == false);
    }
    
    // Self-loop and redundant union
    {
        int n = 2;
        std::vector<std::pair<int,int>> unions = {{1,1}, {2,2}, {1,2}, {2,1}};
        assert(areConnected(1, 2, n, unions) == true);
        assert(areConnected(1, 1, n, unions) == true);
    }
    
    // Large n with no unions
    {
        int n = 100000;
        std::vector<std::pair<int,int>> unions;
        assert(areConnected(1, 100000, n, unions) == false);
    }
    
    // Chain of unions
    {
        int n = 4;
        std::vector<std::pair<int,int>> unions = {{1,2}, {2,3}, {3,4}};
        assert(areConnected(1, 4, n, unions) == true);
        assert(areConnected(1, 3, n, unions) == true);
        assert(areConnected(2, 4, n, unions) == true);
    }
    
    // Separate components
    {
        int n = 6;
        std::vector<std::pair<int,int>> unions = {{1,2}, {3,4}, {5,6}};
        assert(areConnected(1, 2, n, unions) == true);
        assert(areConnected(1, 3, n, unions) == false);
        assert(areConnected(5, 6, n, unions) == true);
        assert(areConnected(4, 6, n, unions) == false);
    }
    
    // Vertex numbers are 1-indexed, boundary check
    {
        int n = 1;
        std::vector<std::pair<int,int>> unions;
        assert(areConnected(1, 1, n, unions) == true);
    }
    
    // Many unions but not connecting specific pair
    {
        int n = 10;
        std::vector<std::pair<int,int>> unions;
        for (int i = 1; i <= 5; ++i) {
            unions.push_back({i, i+1});
        }
        for (int i = 6; i <= 9; ++i) {
            unions.push_back({i, i+1});
        }
        assert(areConnected(1, 6, n, unions) == false);
        assert(areConnected(5, 6, n, unions) == false);
        assert(areConnected(4, 5, n, unions) == true);
        assert(areConnected(9, 10, n, unions) == true);
    }
    
    return 0;
}
