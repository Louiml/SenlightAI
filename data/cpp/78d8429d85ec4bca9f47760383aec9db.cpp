Implement a C++ function `bool connectedAfterUnions(int n, const std::vector<std::tuple<int, int, int>>& operations)` that processes a series of operations on an initially isolated set of `n` nodes labeled `1` through `n`. Each operation is a tuple `{type, a, b}`: if `type == 0`, it adds an undirected edge between nodes `a` and `b` (0-indexed input, but nodes are 1-indexed internally); if `type == 1`, it must report whether nodes `a` and `b` are currently in the same connected component. The function should return `true` if and only if every query of type `1` in the given order returns `true` (i.e., all queries report connectivity correctly), and `false` otherwise. The input may contain duplicate edges, self-loops (a == b), and queries before any edges are added (which should return `false` unless `a == b`). The function must handle up to `n = 200,000` nodes and up to `200,000` operations efficiently.

// The core problem is dynamic connectivity with edge additions only (no deletions), which is a classic use case for a Disjoint Set Union (DSU) / Union-Find data structure. We initialize a parent array with each node as its own root (`p[i] = i`). For each operation, we parse the tuple: if `type == 0`, we call `unite(a+1, b+1)` to merge the sets of the two nodes (note the +1 to convert from 0-indexed input to 1-indexed internal labeling). If `type == 1`, we call `get(a+1)` and `get(b+1)` and compare roots; if they differ, we immediately return `false` because the query's expected answer (connectivity) is false, but the task requires all queries to be true. If all queries pass, we return `true` at the end. Important edge cases include self-loops (unite with itself does nothing, and `get(a)==get(a)` is always true), duplicate edges (unite is idempotent), and queries before any edges (nodes are isolated, so only self-queries return true). To handle large inputs efficiently, we use path compression in `get` (recursive or iterative) and union by rank or random parent assignment to keep trees shallow. Time complexity: each `unite` and `get` operation is nearly constant amortized \(O(\alpha(n))\), where \(\alpha\) is the inverse Ackermann function, so overall \(O((n+q)\alpha(n))\). Space complexity is \(O(n)\) for the parent array.

#include <vector>
#include <tuple>
#include <numeric>

// Disjoint Set Union (Union-Find) with path compression and union by rank.
class DSU {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    explicit DSU(int n) {
        parent.resize(n + 1);
        rank.resize(n + 1, 0);
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int v) {
        if (parent[v] != v) {
            parent[v] = find(parent[v]); // path compression
        }
        return parent[v];
    }

    void unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) return;
        if (rank[ra] < rank[rb]) {
            parent[ra] = rb;
        } else if (rank[ra] > rank[rb]) {
            parent[rb] = ra;
        } else {
            parent[rb] = ra;
            rank[ra]++;
        }
    }

    bool is_connected(int a, int b) {
        return find(a) == find(b);
    }
};

// Process operations: tuples of {type, a, b}.
// type 0: add edge between a and b (0-indexed input, internally 1-indexed).
// type 1: query whether a and b are connected (0-indexed input).
// Returns true iff every type-1 query returns true (i.e., all reported connections are correct).
bool connectedAfterUnions(int n, const std::vector<std::tuple<int, int, int>>& operations) {
    DSU dsu(n);
    for (const auto& op : operations) {
        int type, a, b;
        std::tie(type, a, b) = op;
        if (type == 0) {
            dsu.unite(a + 1, b + 1); // convert to 1-indexed
        } else { // type == 1
            if (!dsu.is_connected(a + 1, b + 1)) {
                return false; // found a query expecting false connectivity, but we must have all true
            }
        }
    }
    return true;
}

#include <cassert>
#include <vector>
#include <tuple>

// The solution function is declared in the same translation unit for testing.
// (Assume the above code is included here.)

int main() {
    // Basic connected component test
    {
        std::vector<std::tuple<int, int, int>> ops = {
            {0, 0, 1},
            {0, 1, 2},
            {1, 0, 2}, // now connected via 0-1-2
            {1, 2, 0}
        };
        assert(connectedAfterUnions(3, ops) == true);
    }

    // Query before any edge -> should return false unless self
    {
        std::vector<std::tuple<int, int, int>> ops = {
            {1, 0, 1}
        };
        assert(connectedAfterUnions(2, ops) == false);
    }

    // Self-loop query returns true
    {
        std::vector<std::tuple<int, int, int>> ops = {
            {1, 1, 1}, // self query
            {1, 0, 0}
        };
        assert(connectedAfterUnions(3, ops) == true);
    }

    // Duplicate edges and self-loops
    {
        std::vector<std::tuple<int, int, int>> ops = {
            {0, 0, 0}, // self-loop edge
            {0, 0, 1},
            {0, 0, 1}, // duplicate
            {1, 0, 1},
            {1, 1, 0}
        };
        assert(connectedAfterUnions(2, ops) == true);
    }

    // Failing query due to disconnected nodes
    {
        std::vector<std::tuple<int, int, int>> ops = {
            {0, 0, 1},
            {1, 0, 2} // 2 is isolated
        };
        assert(connectedAfterUnions(3, ops) == false);
    }

    // Large n, many edges and queries
    {
        int n = 1000;
        std::vector<std::tuple<int, int, int>> ops;
        // Connect all nodes in a line
        for (int i = 0; i < n - 1; ++i) {
            ops.emplace_back(0, i, i + 1);
        }
        // Query all pairs that are connected
        for (int i = 0; i < n; ++i) {
            ops.emplace_back(1, i, n - 1);
        }
        assert(connectedAfterUnions(n, ops) == true);
    }

    // Mixed correct and incorrect queries (must fail)
    {
        std::vector<std::tuple<int, int, int>> ops = {
            {0, 0, 1},
            {1, 0, 1}, // true
            {1, 0, 2}, // false -> return false
        };
        assert(connectedAfterUnions(3, ops) == false);
    }

    return 0;
}
