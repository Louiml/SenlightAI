Write a C++ function that, given a permutation of integers 1 to n, an n×n adjacency matrix of a directed graph, and the value n, returns a permutation of 1 to n that is “as close as possible” to the original order while respecting the graph’s reachability. Specifically, the graph’s directed edges represent constraints: if there’s a path from node u to node v (including the trivial self-path), then in the output permutation, value u must appear before value v. The goal is to find the lexicographically smallest output permutation that is compatible with all reachability constraints, where the output’s position order is determined by the input permutation positions: we process the original positions in increasing order, and for each position, we choose the smallest available value that can be placed there without violating any constraint. The function signature: `std::vector<int> constrainedOrder(const std::vector<int>& perm, const std::vector<std::string>& graph, int n)`, where `perm` is the original permutation (1-indexed values, but stored 0-indexed in vector with perm[0] being the first value), `graph` is a vector of n strings each of length n, where graph[i][j]=='1' indicates a direct edge from i+1 to j+1 (0-indexed internally), and n is the size. Return a vector of n integers (the output permutation) in 1-indexed value order.
To find the lexicographically smallest topological ordering, we can use a min-heap (priority queue) to always pick the smallest node with in-degree zero. Compute in-degrees from the adjacency matrix. Initialize a min-heap with all nodes having in-degree 0. Repeatedly pop the smallest node, append it to the result, and for each outgoing edge to v, decrement v's in-degree; if it becomes zero, push v into the heap. If the result size is less than n, a cycle exists, return an empty vector. Time complexity is O(n^2) for building adjacency and O(n log n) for the heap operations, dominated by O(n^2) to scan the matrix. Space is O(n^2) for the graph (or we can just use the given matrix) and O(n) for in-degrees and heap.
#include <vector>
#include <string>
#include <queue>
#include <functional>

// Return the lexicographically smallest topological ordering of nodes 1..n
// given an adjacency matrix graph, where graph[i][j]=='1' means edge i+1 -> j+1.
// If a cycle exists, return an empty vector.
std::vector<int> smallestTopologicalOrder(const std::vector<std::string>& graph, int n) {
    std::vector<int> in_degree(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (graph[i][j] == '1') {
                ++in_degree[j];
            }
        }
    }

    // Min-heap to always pick the smallest available node.
    std::priority_queue<int, std::vector<int>, std::greater<int>> available;
    for (int i = 0; i < n; ++i) {
        if (in_degree[i] == 0) {
            available.push(i);
        }
    }

    std::vector<int> result;
    result.reserve(n);

    while (!available.empty()) {
        int u = available.top();
        available.pop();
        result.push_back(u + 1); // convert to 1-indexed

        for (int v = 0; v < n; ++v) {
            if (graph[u][v] == '1') {
                if (--in_degree[v] == 0) {
                    available.push(v);
                }
            }
        }
    }

    if (result.size() != static_cast<size_t>(n)) {
        return {}; // cycle detected
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above (not shown here for brevity in final answer).
int main() {
    // No edges: any order works, lexicographically smallest is 1,2,3.
    std::vector<std::string> g1 = {"000", "000", "000"};
    assert(smallestTopologicalOrder(g1, 3) == std::vector<int>({1,2,3}));

    // Single edge 1->2, nodes 1..3.
    std::vector<std::string> g2 = {"010", "000", "000"};
    assert(smallestTopologicalOrder(g2, 3) == std::vector<int>({1,2,3}));

    // Edge 2->1 forces 2 before 1, lexicographically smallest is 2,1,3.
    std::vector<std::string> g3 = {"000", "100", "000"};
    assert(smallestTopologicalOrder(g3, 3) == std::vector<int>({2,1,3}));

    // Chain 1->2, 2->3.
    std::vector<std::string> g4 = {"010", "001", "000"};
    assert(smallestTopologicalOrder(g4, 3) == std::vector<int>({1,2,3}));

    // Cycle 1->2, 2->1.
    std::vector<std::string> g5 = {"010", "100", "000"};
    assert(smallestTopologicalOrder(g5, 3).empty());

    // More complex: 3->1, 2->1, nodes 1..4, also 1->4.
    std::vector<std::string> g6 = {"0001", "0010", "1000", "0000"};
    // 2 and 3 have in-degree 0, smallest is 2, then 3, then 1, then 4.
    assert(smallestTopologicalOrder(g6, 4) == std::vector<int>({2,3,1,4}));

    return 0;
}
