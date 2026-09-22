/*
Write a C++ function `double treeDiameter(const std::vector<std::vector<std::pair<int, double>>>& adj)` that computes the diameter of a weighted, undirected, general tree given as an adjacency list. The tree has `n` nodes numbered from 0 to n-1, and each entry `adj[u]` is a vector of pairs `(v, weight)` representing an undirected edge between `u` and `v` with a positive double weight. The diameter is the maximum possible sum of edge weights along a simple path between any two nodes. The function should handle trees of size 1 (return 0) and trees with up to 10^5 nodes. It must be efficient and avoid recursion depth issues by using an iterative DFS or two BFS/DFS passes (farthest node from an arbitrary start, then farthest from that node). Return the diameter as a double.
*/

#include <vector>
#include <stack>
#include <utility>
#include <limits>

// Compute the diameter of a weighted undirected tree given as adjacency list.
// adj[u] contains pairs (v, weight) for each edge u-v with given weight.
// Returns the maximum sum of weights along any simple path.
double treeDiameter(const std::vector<std::vector<std::pair<int, double>>>& adj) {
    int n = static_cast<int>(adj.size());
    if (n == 0) return 0.0;
    
    // Function to perform iterative DFS to find the farthest node and its distance from start.
    auto farthest = [&](int start) -> std::pair<int, double> {
        std::vector<double> dist(n, -1.0);
        std::stack<int> st;
        st.push(start);
        dist[start] = 0.0;
        int farthestNode = start;
        double maxDist = 0.0;
        
        while (!st.empty()) {
            int u = st.top();
            st.pop();
            for (const auto& edge : adj[u]) {
                int v = edge.first;
                double w = edge.second;
                if (dist[v] < 0) { // unvisited
                    dist[v] = dist[u] + w;
                    st.push(v);
                    if (dist[v] > maxDist) {
                        maxDist = dist[v];
                        farthestNode = v;
                    }
                }
            }
        }
        return {farthestNode, maxDist};
    };
    
    // First pass: find an endpoint of the diameter.
    auto first = farthest(0);
    // Second pass: distance from that endpoint is the diameter.
    auto second = farthest(first.first);
    return second.second;
}

#include <cassert>
#include <vector>
#include <utility>

// Forward declaration of the solution function (already included above, but for clarity)
double treeDiameter(const std::vector<std::vector<std::pair<int, double>>>& adj);

int main() {
    // Test 1: Single node tree.
    std::vector<std::vector<std::pair<int, double>>> t1(1);
    assert(treeDiameter(t1) == 0.0);

    // Test 2: Two nodes with edge weight 5.
    std::vector<std::vector<std::pair<int, double>>> t2(2);
    t2[0].push_back({1, 5.0});
    t2[1].push_back({0, 5.0});
    assert(treeDiameter(t2) == 5.0);

    // Test 3: Path 0-1-2-3 with weights 1,2,3 → diameter 1+2+3=6.
    std::vector<std::vector<std::pair<int, double>>> t3(4);
    t3[0].push_back({1, 1.0});
    t3[1].push_back({0, 1.0});
    t3[1].push_back({2, 2.0});
    t3[2].push_back({1, 2.0});
    t3[2].push_back({3, 3.0});
    t3[3].push_back({2, 3.0});
    assert(treeDiameter(t3) == 6.0);

    // Test 4: Star with center 0 and leaves 1,2,3 with weights 2,4,6 → diameter 4+6=10.
    std::vector<std::vector<std::pair<int, double>>> t4(4);
    t4[0].push_back({1, 2.0});
    t4[1].push_back({0, 2.0});
    t4[0].push_back({2, 4.0});
    t4[2].push_back({0, 4.0});
    t4[0].push_back({3, 6.0});
    t4[3].push_back({0, 6.0});
    assert(treeDiameter(t4) == 10.0);

    // Test 5: Complex tree with branching.
    // Nodes: 0-1 (10), 1-2 (20), 1-3 (30), 3-4 (40). Diameter: 0-1-3-4 = 10+30+40=80, or 2-1-3-4=20+30+40=90.
    std::vector<std::vector<std::pair<int, double>>> t5(5);
    t5[0].push_back({1, 10.0});
    t5[1].push_back({0, 10.0});
    t5[1].push_back({2, 20.0});
    t5[2].push_back({1, 20.0});
    t5[1].push_back({3, 30.0});
    t5[3].push_back({1, 30.0});
    t5[3].push_back({4, 40.0});
    t5[4].push_back({3, 40.0});
    assert(treeDiameter(t5) == 90.0);

    // Test 6: All weights equal (1) on a path of 5 nodes → diameter 4.
    std::vector<std::vector<std::pair<int, double>>> t6(5);
    for (int i = 0; i < 4; ++i) {
        t6[i].push_back({i+1, 1.0});
        t6[i+1].push_back({i, 1.0});
    }
    assert(treeDiameter(t6) == 4.0);

    return 0;
}

// The classic approach for finding the diameter of a weighted tree is the two-pass method:  
// 1. Start from an arbitrary node (say 0), perform a traversal (DFS or BFS, but here iterative to avoid stack overflow) to find the node `A` that is farthest from it in terms of accumulated edge weights. Since the tree is unweighted for connectivity but weighted for distance, we accumulate weights along the path.  
// 2. From `A`, perform another traversal to find the farthest node `B` from `A`; the distance between `A` and `B` is the diameter.  
//
// This works because in a tree, the farthest node from any arbitrary node is an endpoint of a diameter. Edge cases: if the tree has only one node, both traversals return zero distance, so the diameter is 0. If the tree is a simple path, the method trivially finds the two ends. Weights are positive, so no zero or negative weights. Complexity: two traversals, each O(n) time, O(n) space for visited arrays and stack. Memory is O(n) for the stack and visited structures. Use `std::vector<std::pair<int,double>>` to store adjacency, and iterative DFS using a stack of pairs `(node, parent)` to compute distances (since tree has no cycles, parent avoids revisiting).
