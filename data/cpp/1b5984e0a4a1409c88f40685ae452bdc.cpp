/*
Write a C++ function named `findMinimumCycleCost` that takes a positive integer `n` (the number of vertices) and a square matrix `cost` of size `n x n` (as a `const std::vector<std::vector<int>>&`) where `cost[i][j]` is the travel cost from vertex `i` to vertex `j`. A cost of `0` means there is no direct edge. The function should find the minimum total cost of any Hamiltonian cycle (a cycle that visits every vertex exactly once and returns to the starting vertex). If no such cycle exists (i.e., the graph is not Hamiltonian), return `-1`. The function must not modify the input matrix. Assume `n >= 1`; if `n == 1`, treat it as a trivial cycle with cost `0` (since the cycle is just staying at the only vertex). The graph may be directed or undirected, and costs may be positive or negative but never less than `-10^5` and never greater than `10^5`. However, zeros are always treated as "no edge", so even if a real edge has cost `0`, it cannot be used.
*/

#include <vector>
#include <algorithm>
#include <climits>

// Helper recursive function for DFS.
void dfs(int current, int start, int depth, int n, int sum,
         const std::vector<std::vector<int>>& cost,
         std::vector<bool>& visited, std::vector<int>& candidates) {
    if (depth == n - 1) {
        // Last vertex before returning to start.
        if (cost[current][start] != 0) {
            candidates.push_back(sum + cost[current][start]);
        }
        return;
    }
    for (int next = 0; next < n; ++next) {
        if (!visited[next] && cost[current][next] != 0) {
            visited[next] = true;
            dfs(next, start, depth + 1, n, sum + cost[current][next],
                cost, visited, candidates);
            visited[next] = false;
        }
    }
}

// Returns the minimum cost of a Hamiltonian cycle, or -1 if none exists.
int findMinimumCycleCost(int n, const std::vector<std::vector<int>>& cost) {
    if (n == 1) {
        return 0; // trivial cycle with no edges
    }
    std::vector<int> candidates;
    std::vector<bool> visited(n, false);
    for (int start = 0; start < n; ++start) {
        visited[start] = true;
        dfs(start, start, 0, n, 0, cost, visited, candidates);
        visited[start] = false;
    }
    if (candidates.empty()) {
        return -1;
    }
    return *std::min_element(candidates.begin(), candidates.end());
}

#include <cassert>
#include <vector>

int findMinimumCycleCost(int n, const std::vector<std::vector<int>>& cost);

int main() {
    // 1 vertex: trivial cycle cost 0
    assert(findMinimumCycleCost(1, {{0}}) == 0);

    // Triangle with positive edges: 1->2->3->1 costs 1+2+3 = 6
    {
        std::vector<std::vector<int>> cost = {{0, 1, 0}, {0, 0, 2}, {3, 0, 0}};
        assert(findMinimumCycleCost(3, cost) == 6);
    }

    // Triangle with an impossible edge (zero means missing), no cycle exists
    {
        std::vector<std::vector<int>> cost = {{0, 1, 0}, {0, 0, 2}, {0, 0, 0}};
        assert(findMinimumCycleCost(3, cost) == -1);
    }

    // Directed 4-cycle with extra edges: 0->1 (5),1->2(3),2->3(1),3->0(2) = 11
    {
        std::vector<std::vector<int>> cost = {
            {0, 5, 0, 0},
            {0, 0, 3, 0},
            {0, 0, 0, 1},
            {2, 0, 0, 0}
        };
        assert(findMinimumCycleCost(4, cost) == 11);
    }

    // Negatives: 0->1(-2),1->2(-3),2->0(-4) = -9
    {
        std::vector<std::vector<int>> cost = {
            {0, -2, 0},
            {0, 0, -3},
            {-4, 0, 0}
        };
        assert(findMinimumCycleCost(3, cost) == -9);
    }

    // Two vertices with no return edge -> no cycle
    {
        std::vector<std::vector<int>> cost = {{0, 1}, {1, 0}}; // This has cycle 0->1->0 cost 2
        assert(findMinimumCycleCost(2, cost) == 2);
    }

    // Two vertices missing one direction -> no cycle
    {
        std::vector<std::vector<int>> cost = {{0, 1}, {0, 0}};
        assert(findMinimumCycleCost(2, cost) == -1);
    }

    // Larger graph, multiple cycles; the minimum is 8: 0->1(1),1->2(2),2->3(3),3->0(2)=8 and other cycle is 0->2(4),2->1(1),1->3(5),3->0(2)=12
    {
        std::vector<std::vector<int>> cost = {
            {0, 1, 4, 0},
            {0, 0, 2, 5},
            {0, 1, 0, 3},
            {2, 0, 2, 0}
        };
        assert(findMinimumCycleCost(4, cost) == 8);
    }

    return 0;
}

// The problem is a variant of the Traveling Salesman Problem (TSP) using DFS with backtracking. The main approach is to try every vertex as a starting point, then perform a depth-first search that visits all other vertices exactly once, building the cycle path. At each step, we track the current vertex, the accumulated cost, the depth (how many vertices visited so far), and the fixed starting vertex (the final target). When we have visited `n-1` vertices (meaning we have just visited the last vertex before returning), we check if there is an edge from the current vertex back to the starting vertex; if so, we add that return edge's cost to the accumulated sum and record that candidate cost. After exploring all possible starting points and all permutations, we take the minimum of all recorded candidate cycle costs. If no cycle was found (the candidate list is empty), return `-1`.
//
// Important edge cases:
// - `n == 1`: no edges are used; the cycle cost is `0`, return `0`.
// - A missing edge is represented by `0`, so we must skip any move where `cost[current][next] == 0`.
// - Multiple starting vertices are attempted because the cycle can start anywhere; the DFS ensures each vertex is visited exactly once.
// - Since we allow negative costs, the minimum could be negative, so we cannot initialize the answer to infinity; we collect all valid cycle costs and then find the smallest.
//
// Time complexity: In the worst case, for each starting vertex we try all permutations of the remaining `n-1` vertices, so total complexity is `O(n * n!)`. For `n` up to maybe 10 or 11, this is feasible. Space complexity is `O(n)` for the visited array and the recursion stack, plus `O(k)` for the candidate list where `k` is the number of valid cycles found (at most `n!`).
