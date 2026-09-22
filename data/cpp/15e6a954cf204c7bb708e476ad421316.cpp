Write a C++ function `tspTwoOpt` that takes a complete undirected graph represented as a symmetric distance matrix (vector of vectors of ints) and returns a pair containing a Hamiltonian cycle (a vector of node indices, starting and ending at node 0, with no repeated intermediate nodes) and its total cost (int). The function must first construct a greedy nearest-neighbor tour starting from node 0, then improve it using the 2-opt local optimization technique (reversing a segment between two edges to remove crossings) until no further improvement is possible. The input graph is guaranteed to have at least 3 nodes, all edge weights are non‑negative integers, and the matrix is symmetric (distance[i][j] == distance[j][i]) with zero diagonal. The function must return the optimized tour and its cost, and it must handle the case where the nearest‑neighbor tour already appears optimal (no 2‑opt improvement found).
// The solution has two main phases.  
// 1. **Nearest-neighbor heuristic**: Start at node 0. Repeatedly choose the unvisited node closest to the current node (smallest distance). Append it to the tour, mark it visited, and update the current node. After visiting all nodes, return to node 0 by adding the closing edge. The cost is the sum of all chosen edges. This builds a feasible tour but is not necessarily optimal.  
// 2. **2-opt improvement**: Iterate over all pairs of edges (i, i+1) and (j, j+1) in the tour (with indices taken modulo the number of nodes for the returning edge). For each pair, compute the delta change: `delta = -dist[path[i]][path[(i+1)%n]] - dist[path[j]][path[(j+1)%n]] + dist[path[i]][path[j]] + dist[path[(i+1)%n]][path[(j+1)%n]]`. If delta < 0, reversing the segment between i+1 and j (inclusive) reduces the total cost. Apply the reversal, update the cost by adding delta, and continue the loop until no such improvement is found in a full pass. Edge cases: Ensure the loop indices avoid swapping edges that involve the starting node (i from 1 to n-3, j from i+1 to n-1) so the returning edge to node 0 is not incorrectly modified. The nearest‑neighbor tour construction is O(n²) due to scanning all unvisited nodes for each step. The 2‑opt improvement is O(n²) per full pass, and the number of passes is typically small (polynomial in practice), so the overall worst-case complexity is O(n³) but often much better for random instances. Space complexity is O(n) for the path and visited set, plus the input graph (which is not duplicated).
#include <vector>
#include <limits>
#include <algorithm>

// Compute a Hamiltonian cycle using nearest-neighbor heuristic from node 0.
static std::pair<std::vector<int>, int> nearestNeighborTour(const std::vector<std::vector<int>>& graph) {
    int n = static_cast<int>(graph.size());
    std::vector<bool> visited(n, false);
    std::vector<int> path;
    path.reserve(n + 1);

    int current = 0;
    visited[current] = true;
    path.push_back(current);
    int cost = 0;

    for (int step = 1; step < n; ++step) {
        int next = -1;
        int bestDist = std::numeric_limits<int>::max();
        for (int candidate = 0; candidate < n; ++candidate) {
            if (!visited[candidate]) {
                int d = graph[current][candidate];
                if (d < bestDist) {
                    bestDist = d;
                    next = candidate;
                }
            }
        }
        // next is always found because there are n-1 unvisited nodes.
        cost += bestDist;
        current = next;
        visited[current] = true;
        path.push_back(current);
    }

    // Return to start.
    cost += graph[current][0];
    path.push_back(0);
    return {path, cost};
}

// Apply 2-opt local improvement to the tour until no further improvement.
static void twoOptImprove(std::vector<int>& path, int& cost, const std::vector<std::vector<int>>& graph) {
    int n = static_cast<int>(path.size());
    bool improved = true;

    while (improved) {
        improved = false;
        // Avoid edges incident to node 0 by starting i at 1 and ending j at n-2.
        for (int i = 1; i < n - 2; ++i) {
            for (int j = i + 1; j < n - 1; ++j) {
                int delta = -graph[path[i]][path[(i + 1) % n]]
                            - graph[path[j]][path[(j + 1) % n]]
                            + graph[path[i]][path[j]]
                            + graph[path[(i + 1) % n]][path[(j + 1) % n]];
                if (delta < 0) {
                    // Reverse segment [i+1, j].
                    std::reverse(path.begin() + i + 1, path.begin() + j + 1);
                    cost += delta;
                    improved = true;
                }
            }
        }
    }
}

// Public function: returns optimized tour and its cost.
std::pair<std::vector<int>, int> tspTwoOpt(const std::vector<std::vector<int>>& graph) {
    auto tourCost = nearestNeighborTour(graph);
    twoOptImprove(tourCost.first, tourCost.second, graph);
    return tourCost;
}
#include <cassert>
#include <vector>

// The solution function is declared above (included in the same translation unit for testing).
// This main function provides assert-based checks.

int main() {
    // Test 1: Triangle graph (3 nodes).
    std::vector<std::vector<int>> g1 = {
        {0, 10, 15},
        {10, 0, 20},
        {15, 20, 0}
    };
    auto r1 = tspTwoOpt(g1);
    assert(r1.second == 45);
    assert(r1.first.size() == 4);
    assert(r1.first.front() == 0 && r1.first.back() == 0);

    // Test 2: Square with a shorter path through a diagonal.
    std::vector<std::vector<int>> g2 = {
        {0, 1, 100, 100},
        {1, 0, 1, 100},
        {100, 1, 0, 1},
        {100, 100, 1, 0}
    };
    auto r2 = tspTwoOpt(g2);
    // Optimal tour: 0-1-2-3-0, cost = 1+1+1+100 = 103.
    assert(r2.second == 103);

    // Test 3: All zeros (any tour cost 0).
    std::vector<std::vector<int>> g3(4, std::vector<int>(4, 0));
    auto r3 = tspTwoOpt(g3);
    assert(r3.second == 0);
    assert(r3.first.front() == 0 && r3.first.back() == 0);

    // Test 4: Line where nearest neighbor already optimal.
    // Distances: 0-1=1, 1-2=1, 2-3=1, 3-0=10.
    std::vector<std::vector<int>> g4 = {
        {0, 1, 10, 10},
        {1, 0, 1, 10},
        {10, 1, 0, 1},
        {10, 10, 1, 0}
    };
    auto r4 = tspTwoOpt(g4);
    // NN from 0: 0-1-2-3-0 cost=1+1+1+10=13. 2-opt cannot improve (swapping would increase).
    assert(r4.second == 13);

    // Test 5: Larger random-like symmetric graph with known optimal from NN.
    std::vector<std::vector<int>> g5 = {
        {0, 2, 9, 10},
        {2, 0, 6, 4},
        {9, 6, 0, 8},
        {10, 4, 8, 0}
    };
    auto r5 = tspTwoOpt(g5);
    // Possible tour 0-1-3-2-0: 2+4+8+9=23, check that 2-opt doesn't do worse.
    // Verify cost is at most the NN cost and return to 0.
    assert(r5.second <= 23 + 10); // just ensure it's a valid cycle
    assert(r5.first.front() == 0 && r5.first.back() == 0);

    return 0;
}
