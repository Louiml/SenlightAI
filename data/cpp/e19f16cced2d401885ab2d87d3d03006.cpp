/*
Write a standalone C++ function `int minimalConflicts(vector<int>& colors, const vector<vector<int>>& adjacency, int numColors, unsigned long maxSteps)` that attempts to color an undirected graph with `numColors` colors (labeled 1 to `numColors`) using the Min-Conflicts local search algorithm. The graph is given by an adjacency list, and the current coloring is stored in `colors` (initially arbitrary). The function must iteratively: (1) select a vertex that has at least one conflict (an edge connecting two vertices of the same color), choosing uniformly at random among all conflicted vertices; (2) for that vertex, try every color and compute the number of conflicts it would cause; (3) assign the color that minimizes conflicts, but if no color strictly reduces the conflict count from the current value, choose a random color instead; and (4) repeat until either a proper coloring is achieved (return the number of steps taken) or `maxSteps` is exhausted (return -1). The function must modify `colors` in place. Assume the graph is undirected, has at least one vertex, `numColors ≥ 1`, and that `colors.size()` equals the number of vertices. Your solution must not use any external libraries beyond the standard C++ headers.
*/
#include <vector>
#include <cstdlib>
#include <algorithm>

// Min-Conflicts graph coloring.
// Returns number of steps taken if a proper coloring is found, else -1.
// Modifies colors in place to a proper coloring if found.
int minimalConflicts(std::vector<int>& colors,
                     const std::vector<std::vector<int>>& adjacency,
                     int numColors,
                     unsigned long maxSteps) {
    int V = static_cast<int>(colors.size());
    if (V == 0) return 0; // vacuous

    auto countConflicts = [&](int vertex) {
        int count = 0;
        for (int nb : adjacency[vertex]) {
            if (colors[nb] == colors[vertex]) ++count;
        }
        return count;
    };

    auto totalConflicts = [&]() {
        int total = 0;
        for (int i = 0; i < V; ++i) total += countConflicts(i);
        return total / 2; // each undirected edge counted twice
    };

    for (unsigned long step = 0; step < maxSteps; ++step) {
        // Compute conflicts for all vertices
        std::vector<int> conflictCount(V);
        std::vector<int> conflicted;
        for (int i = 0; i < V; ++i) {
            conflictCount[i] = countConflicts(i);
            if (conflictCount[i] > 0) conflicted.push_back(i);
        }
        if (conflicted.empty()) {
            return static_cast<int>(step); // already solved
        }

        // Pick a random conflicted vertex
        int choice = conflicted[rand() % conflicted.size()];
        int currentConflicts = conflictCount[choice];

        // Find best color
        int bestColor = 1;
        int bestConflicts = currentConflicts;
        for (int c = 1; c <= numColors; ++c) {
            colors[choice] = c;
            int cConflicts = countConflicts(choice);
            if (cConflicts < bestConflicts) {
                bestConflicts = cConflicts;
                bestColor = c;
            }
        }

        // If no improvement, choose random color
        if (bestConflicts == currentConflicts) {
            bestColor = (rand() % numColors) + 1;
        }

        colors[choice] = bestColor;

        // Check if solved
        if (totalConflicts() == 0) {
            return static_cast<int>(step + 1);
        }
    }
    return -1;
}
#include <cassert>
#include <vector>
#include <cstdlib>

// Function declaration (actual implementation above)
int minimalConflicts(std::vector<int>& colors,
                     const std::vector<std::vector<int>>& adjacency,
                     int numColors,
                     unsigned long maxSteps);

bool isProperColoring(const std::vector<int>& colors,
                      const std::vector<std::vector<int>>& adjacency) {
    for (int i = 0; i < (int)colors.size(); ++i) {
        for (int nb : adjacency[i]) {
            if (colors[i] == colors[nb]) return false;
        }
    }
    return true;
}

int main() {
    // Test 1: Single vertex, any coloring is proper.
    {
        std::vector<int> colors = {1};
        std::vector<std::vector<int>> adj = {{}};
        int steps = minimalConflicts(colors, adj, 3, 100);
        assert(steps >= 0);
        assert(isProperColoring(colors, adj));
    }

    // Test 2: Edge between two vertices, 2 colors, should find quickly.
    {
        std::vector<int> colors = {1, 1}; // conflict initially
        std::vector<std::vector<int>> adj = {{1}, {0}};
        int steps = minimalConflicts(colors, adj, 2, 100);
        assert(steps >= 0);
        assert(isProperColoring(colors, adj));
        assert(colors[0] != colors[1]);
    }

    // Test 3: Triangle needs 3 colors.
    {
        std::vector<int> colors = {1, 1, 1};
        std::vector<std::vector<int>> adj = {{1,2}, {0,2}, {0,1}};
        int steps = minimalConflicts(colors, adj, 3, 1000);
        assert(steps >= 0);
        assert(isProperColoring(colors, adj));
        assert(colors[0] != colors[1] && colors[0] != colors[2] && colors[1] != colors[2]);
    }

    // Test 4: Bipartite graph with 2 colors, always solvable.
    {
        // Path of 4 vertices
        std::vector<int> colors = {1, 2, 1, 2}; // already proper
        std::vector<std::vector<int>> adj = {{1}, {0,2}, {1,3}, {2}};
        int steps = minimalConflicts(colors, adj, 2, 100);
        assert(steps >= 0);
        assert(isProperColoring(colors, adj));
    }

    // Test 5: Impossible with too few colors, maxSteps exhausted.
    {
        // Triangle with only 2 colors -> impossible
        std::vector<int> colors = {1, 1, 1};
        std::vector<std::vector<int>> adj = {{1,2}, {0,2}, {0,1}};
        int steps = minimalConflicts(colors, adj, 2, 100);
        assert(steps == -1);
    }

    // Test 6: Empty graph (no edges), any coloring works.
    {
        std::vector<int> colors = {1, 1, 1};
        std::vector<std::vector<int>> adj = {{}, {}, {}};
        int steps = minimalConflicts(colors, adj, 1, 10);
        assert(steps >= 0);
        assert(isProperColoring(colors, adj));
    }

    // Test 7: Larger graph, random seed to avoid flakiness.
    {
        std::srand(42);
        // 5-cycle needs 3 colors
        std::vector<int> colors = {1,1,1,1,1};
        std::vector<std::vector<int>> adj = {{1,4}, {0,2}, {1,3}, {2,4}, {3,0}};
        int steps = minimalConflicts(colors, adj, 3, 10000);
        assert(steps >= 0);
        assert(isProperColoring(colors, adj));
    }

    return 0;
}
// The Min-Conflicts algorithm is a heuristic for CSPs like graph coloring. The core idea is to start from a random (or arbitrary) assignment and repeatedly pick a conflicted variable and change its value to minimize the number of conflicts it causes. In each step: compute conflicts for every vertex (count of neighbors with the same color), collect indices of vertices with conflict count > 0, randomly choose one such vertex, then evaluate each possible color for that vertex by counting how many neighbors share that color, and pick the color with the minimal conflict count. If the minimal count equals the current conflict count (no improvement), pick a random color. This randomness helps escape local minima. The termination check is whether the total number of conflicts (sum over all vertices of their conflict counts, divided by 2 for undirected edges) is zero. Edge cases: if the initial coloring already has zero conflicts, the loop should return 0 steps (or handle correctly). If no conflicted vertices exist at any step, that means a solution is found, so break and return the step count. If `maxSteps` is reached without a solution, return -1. Complexity: each step requires O(V + E) to recompute all conflicts, plus O(deg(v) * numColors) for the chosen vertex (but since we recompute conflicts for each candidate color by scanning neighbors, it's O(deg(v) * numColors)). Worst-case per step is O(V + E + numColors * maxDegree). In practice, the algorithm may or may not find a solution depending on the problem and `maxSteps`. Space complexity is O(V) for the conflict array and the list of conflicted vertices.
