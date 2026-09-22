// Write a C++ function `int countConnectedPairs(int N, const std::vector<std::pair<char,std::pair<int,int>>>& ops)` that simulates a dynamic connectivity problem. The function takes the number of nodes `N` (labeled 1 to N) and a list of operations, each either `'c'` (connect the two given nodes) or `'q'` (query whether the two given nodes are connected). The function should return the number of queries that receive a "yes" answer (i.e., the two nodes are already in the same connected component at the time of the query). Connections are undirected and transitive; if `a` is connected to `b` and `b` to `c`, then `a` is connected to `c`. Nodes are initially all isolated. The input list may contain any mix of connect and query operations, and the function must process them in order. There are no invalid node indices (all are within 1 to N). The function should handle N up to 100,000 and up to 100,000 operations efficiently. Edge cases include duplicate connections, queries of already connected nodes, and self-queries (a node with itself, which should always be considered connected).

#include <cassert>
#include <vector>
#include <utility>

// Assume the solution function is declared above
// int countConnectedPairs(int N, const std::vector<std::pair<char, std::pair<int, int>>>& ops);

int main() {
    // Test 1: Simple connect and query
    std::vector<std::pair<char, std::pair<int, int>>> ops1 = {
        {'c', {1, 2}},
        {'q', {1, 2}},
        {'q', {1, 3}}
    };
    assert(countConnectedPairs(3, ops1) == 1);

    // Test 2: Transitive connection
    std::vector<std::pair<char, std::pair<int, int>>> ops2 = {
        {'c', {1, 2}},
        {'c', {2, 3}},
        {'q', {1, 3}},
        {'q', {3, 1}},
        {'q', {1, 4}}
    };
    assert(countConnectedPairs(4, ops2) == 2);

    // Test 3: Self-query and no connections
    std::vector<std::pair<char, std::pair<int, int>>> ops3 = {
        {'q', {5, 5}},
        {'q', {1, 2}},
        {'c', {4, 5}},
        {'q', {4, 5}}
    };
    assert(countConnectedPairs(5, ops3) == 2);

    // Test 4: Duplicate connections and isolated nodes
    std::vector<std::pair<char, std::pair<int, int>>> ops4 = {
        {'c', {2, 3}},
        {'c', {3, 2}},  // duplicate union
        {'c', {2, 3}},  // duplicate again
        {'q', {2, 3}},
        {'q', {3, 2}},
        {'q', {1, 2}}
    };
    assert(countConnectedPairs(3, ops4) == 2);

    // Test 5: Larger chain, all queried
    std::vector<std::pair<char, std::pair<int, int>>> ops5;
    for (int i = 1; i < 5; ++i) {
        ops5.push_back({'c', {i, i+1}});
    }
    for (int i = 1; i <= 5; ++i) {
        for (int j = i; j <= 5; ++j) {
            ops5.push_back({'q', {i, j}});
        }
    }
    // After connecting 1-2-3-4-5, all pairs are connected, so all 15 queries return true.
    assert(countConnectedPairs(5, ops5) == 15);

    // Test 6: No operations
    std::vector<std::pair<char, std::pair<int, int>>> ops6;
    assert(countConnectedPairs(10, ops6) == 0);

    return 0;
}

#include <vector>
#include <utility>

// Count how many 'q' operations return true (i.e., nodes are connected).
int countConnectedPairs(int N, const std::vector<std::pair<char, std::pair<int, int>>>& ops) {
    // DSU structures
    std::vector<int> parent(N);
    std::vector<int> rank(N, 0);
    
    // Initialize: each node is its own parent
    for (int i = 0; i < N; ++i) {
        parent[i] = i;
    }
    
    // Find with path compression
    auto find = [&](int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]]; // path halving
            x = parent[x];
        }
        return x;
    };
    
    // Union by rank
    auto unite = [&](int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) return;
        if (rank[ra] < rank[rb]) {
            parent[ra] = rb;
        } else if (rank[ra] > rank[rb]) {
            parent[rb] = ra;
        } else {
            parent[rb] = ra;
            ++rank[ra];
        }
    };
    
    int successfulQueries = 0;
    
    for (const auto& op : ops) {
        char type = op.first;
        int a = op.second.first - 1; // convert to 0-based
        int b = op.second.second - 1;
        
        if (type == 'c') {
            unite(a, b);
        } else { // type == 'q'
            if (find(a) == find(b)) {
                ++successfulQueries;
            }
        }
    }
    
    return successfulQueries;
}

// The solution uses a Disjoint Set Union (DSU/Union-Find) data structure with path compression and union by rank (or size) to maintain connected components efficiently. Initially, every node is its own parent. For each operation:
// - If it's a `'c'` (connect), call `union(a, b)`, which unites the components containing `a` and `b` if they are different. If they are already in the same component, no change is needed.
// - If it's a `'q'` (query), call `find(a) == find(b)`; if true, increment a counter for the number of successful queries.
//
// Important edge cases:
// - The input uses 1-based indexing, so subtract 1 when mapping to 0-based internal indices.
// - Self-queries (`q x x`) always return true because trivially a node is connected to itself.
// - Duplicate connections are harmless; union will simply do nothing if already connected.
// - The DSU must be initialized correctly with `parent[i] = i` for all i.
// - Path compression in `find` ensures near-constant amortized time per operation.
//
// Time complexity: Each operation (connect or query) runs in nearly \(O(\alpha(N))\) amortized time due to the inverse Ackermann function, so the total is \(O(M \alpha(N))\), where M is the number of operations. Space complexity is \(O(N)\) for the parent and rank arrays.
