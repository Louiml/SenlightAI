/*
Write a C++ function that solves a variant of the Traveling Salesman Problem: given an `n × n` cost matrix (where `n ≤ 10`), starting from city 0, find the minimum total cost to visit every other city exactly once and return to city 0. However, a move is considered invalid if the direct cost between two cities is exactly `0`. If no valid complete tour exists, return `-1`. The cost matrix is guaranteed to have non-negative integers, and diagonal entries are always `0` but should be ignored (you never travel from a city to itself). The function should take the matrix as a `const` reference to a 2D vector of integers and return an integer.
*/

#include <vector>
#include <algorithm>
#include <climits>

// Solves the TSP variant where a direct cost of 0 is forbidden (except for diagonal).
// Returns -1 if no valid tour exists.
int minValidTourCost(const std::vector<std::vector<int>>& cost) {
    const int n = static_cast<int>(cost.size());
    if (n == 1) return 0;  // no movement needed

    std::vector<bool> visited(n, false);
    std::vector<int> path;  // stores order of cities to visit after city 0
    path.reserve(n - 1);

    int best = INT_MAX;

    // Recursive backtracking: pick the next city to visit after city 0
    std::function<void(int)> backtrack = [&](int depth) {
        if (depth == n - 1) {
            // Compute total cost for the current path
            long long total = 0;
            int current = 0;
            // From start to first city
            int to_first = cost[current][path[0]];
            if (to_first == 0) return;
            total += to_first;
            current = path[0];
            // Between consecutive cities
            for (int i = 1; i < static_cast<int>(path.size()); ++i) {
                int move_cost = cost[current][path[i]];
                if (move_cost == 0) return;
                total += move_cost;
                current = path[i];
            }
            // Back to start
            int to_home = cost[current][0];
            if (to_home == 0) return;
            total += to_home;

            if (total < best) best = static_cast<int>(total);
            return;
        }

        for (int i = 1; i < n; ++i) {
            if (!visited[i]) {
                visited[i] = true;
                path.push_back(i);
                backtrack(depth + 1);
                path.pop_back();
                visited[i] = false;
            }
        }
    };

    backtrack(0);
    return (best == INT_MAX) ? -1 : best;
}

#include <cassert>
#include <vector>

// The solution function is declared above; here we test it.

int main() {
    // Case 1: Simple 3-city example
    std::vector<std::vector<int>> cost1 = {
        {0, 1, 2},
        {3, 0, 4},
        {5, 6, 0}
    };
    // Valid tours: 0->1->2->0: 1+4+5=10; 0->2->1->0: 2+6+3=11
    assert(minValidTourCost(cost1) == 10);

    // Case 2: Contains a zero-cost edge making all tours invalid
    std::vector<std::vector<int>> cost2 = {
        {0, 0, 5},
        {4, 0, 6},
        {7, 8, 0}
    };
    // 0->1 has cost 0 → any tour using it invalid; 0->2->1->0: 5+6+4=15 but 0->2 cost is 5, 2->1 cost 6, 1->0 cost 4 all non-zero? Actually 0->2=5, 2->1=6, 1->0=4 all valid, so valid tour exists with cost 15.
    assert(minValidTourCost(cost2) == 15);

    // Case 3: No valid tour because every way back to 0 is blocked
    std::vector<std::vector<int>> cost3 = {
        {0, 1, 2},
        {3, 0, 4},
        {0, 6, 0}  // cost[2][0] = 0, so cannot return from city 2
    };
    // Only tour: 0->1->2->0 has last move cost 0 → invalid; 0->2->1->0: 2 + 6 + 3 = 11 but 0->2=2 ok, 2->1=6 ok, 1->0=3 ok, all valid → actually valid! Wait cost[2][0]=0 but 2->1 is 6, 1->0 is 3, so tour 0->2->1->0 is valid.
    // Let's fix: make cost[1][0]=0 too
    std::vector<std::vector<int>> cost3b = {
        {0, 1, 2},
        {0, 0, 4},
        {5, 6, 0}
    };
    assert(minValidTourCost(cost3b) == -1);  // 0->1 or 0->2, any return path blocked

    // Case 4: n=1
    std::vector<std::vector<int>> cost4 = {{0}};
    assert(minValidTourCost(cost4) == 0);

    // Case 5: n=2 with valid costs
    std::vector<std::vector<int>> cost5 = {
        {0, 7},
        {3, 0}
    };
    assert(minValidTourCost(cost5) == 10);  // 7+3

    // Case 6: n=2 with a blocked edge
    std::vector<std::vector<int>> cost6 = {
        {0, 0},
        {3, 0}
    };
    assert(minValidTourCost(cost6) == -1);  // 0->1 cost 0

    // Case 7: Larger matrix with a known minimum
    std::vector<std::vector<int>> cost7 = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    // Known TSP optimal is 80
    assert(minValidTourCost(cost7) == 80);

    return 0;
}

// The problem is a Hamiltonian cycle problem with a "zero-cost means impossible" constraint. Since `n ≤ 10`, the maximum number of permutations of the middle cities is `(n-1)!` which is at most `9! = 362880`, so brute-force backtracking is feasible. The main algorithm: fix city 0 as the start, then use a recursive backtracking function to pick an ordering of the remaining `n-1` cities. For each complete ordering, compute the total cost as the sum of costs from 0 to the first city, between consecutive cities, and from the last city back to 0. If any of these moves has cost exactly `0`, the tour is invalid and skipped. Track the minimum valid total cost. If no valid tour is found, return `-1`. Edge cases: when `n == 1`, the tour is trivial (start at city 0, return to city 0), but since we never travel between distinct cities, the cost is `0` (diagonal is 0), so the answer should be `0` (this is technically a valid zero-length tour, but the problem usually expects visiting all cities; handle by returning `0` if `n == 1`). Also, if `n == 2`, only one ordering exists. Time complexity is `O(n! * n)` for generating and evaluating each permutation, and space complexity is `O(n)` for the recursion stack and visited array (excluding the input matrix).
