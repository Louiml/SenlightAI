// Write a C++ function `int shortestHamiltonianCycle(const std::vector<std::vector<int>>& costMatrix)` that takes a square matrix `costMatrix` of size `n` (where `n >= 1`), where `costMatrix[i][j]` is the cost of traveling directly from city `i` to city `j`. A cost of `0` means the edge does not exist (i.e., there is no direct route between those two cities). The function must find the minimum total cost of a Hamiltonian cycle that starts and ends at city `0` and visits every other city exactly once. If no such cycle exists (i.e., the graph is not complete in a way that allows a cycle through all vertices), return `INT_MAX`. The input matrix may contain asymmetric costs, and diagonal entries are ignored (they are irrelevant). The function should handle up to `n = 12` efficiently using branch-and-bound backtracking. The returned value is the integer minimum cost, and you must not modify the input matrix.

This is the classic "Travelling Salesman Problem" (TSP) on a directed graph where some edges may be missing (cost 0 means no edge). The algorithm uses a depth-first search with pruning: start at city 0, mark it visited, and recursively try every unvisited city that has a non-zero edge from the current source. We maintain a running cost; if at any point the accumulated cost plus the heuristic lower bound (just the current cost, since we have no lower-bound heuristic beyond the current sum) is already greater than or equal to the best answer found, we prune. When all cities are visited, we add the cost of returning to city 0; if that edge exists (cost > 0), we update the best answer. Important edge cases: (1) If `n == 1`, the cycle is just staying at city 0, cost is 0 (or if matrix[0][0] > 0 but we ignore diagonal, return 0). (2) If no cycle exists at all, the algorithm will not find a valid complete path, and it should return `INT_MAX`. (3) The pruning condition `cost + a[source][i] < ans` ensures we only explore promising branches. Time complexity is O(n!) in the worst case, but with pruning it is practical for n up to 12. Space complexity is O(n) for the visited array and the recursion stack, plus O(n^2) for the matrix copy if we pass by const reference (no copy needed). The algorithm is correct because it explores all possible permutations of cities starting from city 0, and takes the minimum over valid cycles.

#include <vector>
#include <climits>
#include <algorithm>

// Returns the minimum cost of a Hamiltonian cycle starting and ending at city 0.
// If no cycle exists, returns INT_MAX.
int shortestHamiltonianCycle(const std::vector<std::vector<int>>& costMatrix) {
    int n = static_cast<int>(costMatrix.size());
    if (n == 0) return 0;
    if (n == 1) return 0; // Single city, no travel needed

    int best = INT_MAX;
    std::vector<bool> visited(n, false);

    // Recursive backtracking function
    std::function<void(int, int, int)> dfs = [&](int source, int count, int currentCost) {
        // If all vertices visited, try to return to city 0
        if (count == n) {
            int returnCost = costMatrix[source][0];
            if (returnCost > 0) { // edge exists
                best = std::min(best, currentCost + returnCost);
            }
            return;
        }

        // Explore all unvisited neighbors with a non-zero edge
        for (int next = 1; next < n; ++next) {
            if (!visited[next] && costMatrix[source][next] > 0) {
                int newCost = currentCost + costMatrix[source][next];
                // Prune if newCost already >= best (no chance to improve)
                if (newCost < best) {
                    visited[next] = true;
                    dfs(next, count + 1, newCost);
                    visited[next] = false;
                }
            }
        }
    };

    // Start at city 0, visited count = 1, cost = 0
    visited[0] = true;
    dfs(0, 1, 0);

    return best;
}

#include <cassert>
#include <vector>
#include <climits>

// Declaration of the function under test
int shortestHamiltonianCycle(const std::vector<std::vector<int>>& costMatrix);

int main() {
    // Test 1: 3 cities with a complete directed graph: 0->1=10, 1->2=20, 2->0=30, plus reverse edges
    std::vector<std::vector<int>> matrix1 = {
        {0, 10, 15},
        {10, 0, 20},
        {15, 20, 0}
    };
    // The Hamiltonian cycle 0->1->2->0 costs 10+20+15 = 45; 0->2->1->0 costs 15+20+10 = 45
    assert(shortestHamiltonianCycle(matrix1) == 45);

    // Test 2: 2 cities with direct edge both ways: cost 5 and 7
    std::vector<std::vector<int>> matrix2 = {
        {0, 5},
        {7, 0}
    };
    // Cycle 0->1->0 costs 5+7=12
    assert(shortestHamiltonianCycle(matrix2) == 12);

    // Test 3: 3 cities but missing an edge (0 to 2 is 0), only one cycle possible
    std::vector<std::vector<int>> matrix3 = {
        {0, 10, 0},
        {0, 0, 20},
        {30, 0, 0}
    };
    // Only cycle: 0->1->2->0 costs 10+20+30=60
    assert(shortestHamiltonianCycle(matrix3) == 60);

    // Test 4: No possible cycle (missing return edge from last city)
    std::vector<std::vector<int>> matrix4 = {
        {0, 10, 0},
        {0, 0, 20},
        {0, 0, 0}
    };
    assert(shortestHamiltonianCycle(matrix4) == INT_MAX);

    // Test 5: Single city
    std::vector<std::vector<int>> matrix5 = {{0}};
    assert(shortestHamiltonianCycle(matrix5) == 0);

    // Test 6: Two cities with no return edge (0->1 exists, 1->0 missing)
    std::vector<std::vector<int>> matrix6 = {
        {0, 5},
        {0, 0}
    };
    assert(shortestHamiltonianCycle(matrix6) == INT_MAX);

    // Test 7: Asymmetric costs with lower cost one way
    std::vector<std::vector<int>> matrix7 = {
        {0, 1, 100},
        {100, 0, 2},
        {1, 100, 0}
    };
    // Cycle 0->1->2->0 costs 1+2+1=4; other cycle costs 100+100+100=300
    assert(shortestHamiltonianCycle(matrix7) == 4);

    // Test 8: 4 city complete graph with known minimum
    std::vector<std::vector<int>> matrix8 = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    // The minimum cycle is 0->1->3->2->0 costs 10+25+30+15 = 80
    assert(shortestHamiltonianCycle(matrix8) == 80);

    return 0;
}
