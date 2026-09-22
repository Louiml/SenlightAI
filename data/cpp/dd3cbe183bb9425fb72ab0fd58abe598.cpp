/*
You are given a list of \(n\) projects. To start project \(i\), you must have at least \(a_i\) rating points. Completing project \(i\) changes your rating by \(b_i\) (which can be positive, negative, or zero). You begin with an initial rating \(r\). You may complete the projects in any order, but each project can be completed at most once. Your goal is to maximize the number of projects you can complete. Write a C++ function `int maxProjects(int n, int initialRating, const std::vector<int>& requirements, const std::vector<int>& changes)` that returns the maximum number of projects you can complete. Note that the given requirements and changes arrays are of length \(n\), and you may assume that all values fit in a 32-bit signed integer.
*/

#include <vector>
#include <algorithm>
#include <numeric>

// Comparison function for sorting projects
bool cmpProjects(const std::pair<int,int>& p1, const std::pair<int,int>& p2) {
    int b1 = p1.second;
    int b2 = p2.second;
    // Projects with non‑negative change come first
    if (b1 >= 0 && b2 < 0) return true;
    if (b1 < 0 && b2 >= 0) return false;
    if (b1 >= 0) {
        // For non‑negative changes, sort by requirement ascending
        return p1.first < p2.first;
    } else {
        // For negative changes, sort by (requirement + change) descending
        return p1.first + p1.second > p2.first + p2.second;
    }
}

// Returns the maximum number of projects that can be completed
int maxProjects(int n, int initialRating,
                const std::vector<int>& requirements,
                const std::vector<int>& changes) {
    // Create vector of pairs (requirement, change)
    std::vector<std::pair<int,int>> projects(n);
    for (int i = 0; i < n; ++i) {
        // Ensure requirement is at least the absolute value of change
        int req = requirements[i];
        int ch = changes[i];
        req = std::max(req, -ch);
        projects[i] = {req, ch};
    }

    // Sort projects with the custom comparator
    std::sort(projects.begin(), projects.end(), cmpProjects);

    // DP table: dp[i][j] = max rating after considering first i projects, having completed j
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, -1));
    dp[0][0] = initialRating;

    for (int i = 0; i < n; ++i) {
        int req = projects[i].first;
        int ch = projects[i].second;
        for (int j = 0; j < n; ++j) {
            if (dp[i][j] >= 0) {
                // Option 1: skip this project
                dp[i + 1][j] = std::max(dp[i + 1][j], dp[i][j]);
                // Option 2: take this project if rating is sufficient
                if (dp[i][j] >= req) {
                    dp[i + 1][j + 1] = std::max(dp[i + 1][j + 1], dp[i][j] + ch);
                }
            }
        }
    }

    // Find the maximum number of projects completed
    for (int j = n; j >= 0; --j) {
        if (dp[n][j] >= 0) {
            return j;
        }
    }
    return 0; // Should never reach here
}

#include <cassert>
#include <vector>
#include <iostream>

// Declare the solution function (already defined above)
int maxProjects(int n, int initialRating, const std::vector<int>& requirements, const std::vector<int>& changes);

int main() {
    // Test 1: Basic example
    {
        int n = 3;
        int r = 5;
        std::vector<int> a = {3, 4, 1};
        std::vector<int> b = {2, -3, 5};
        assert(maxProjects(n, r, a, b) == 3);
    }

    // Test 2: Impossible to complete any project
    {
        int n = 2;
        int r = 1;
        std::vector<int> a = {5, 10};
        std::vector<int> b = {0, 0};
        assert(maxProjects(n, r, a, b) == 0);
    }

    // Test 3: Only negative changes
    {
        int n = 3;
        int r = 10;
        std::vector<int> a = {8, 6, 4};
        std::vector<int> b = {-3, -2, -1};
        assert(maxProjects(n, r, a, b) == 2);
    }

    // Test 4: Large negative change increases requirement
    {
        int n = 2;
        int r = 5;
        std::vector<int> a = {1, 4};
        std::vector<int> b = {10, -10};
        assert(maxProjects(n, r, a, b) == 2);
    }

    // Test 5: All zero changes
    {
        int n = 4;
        int r = 3;
        std::vector<int> a = {3, 3, 2, 1};
        std::vector<int> b = {0, 0, 0, 0};
        assert(maxProjects(n, r, a, b) == 4);
    }

    // Test 6: Mixed with equal sums for negative
    {
        int n = 3;
        int r = 7;
        std::vector<int> a = {5, 6, 4};
        std::vector<int> b = {-2, -1, -3};
        // Best order: take (5,-2) -> rating 5, then (6,-1) -> rating 4, cannot take (4,-3) because need 4, have 4, after it rating 1 (but sorted order may differ). Let's check: total 3 possible? Let's reason: take (5,-2)->5, then (4,-3) requires 4, rating 5 -> ok -> rating 2, then (6,-1) requires 6, rating 2 no. Alternatively (6,-1)->6, then (4,-3)-> rating 3, then (5,-2) requires 5, rating 3 no. Best is 2.
        assert(maxProjects(n, r, a, b) == 2);
    }

    // Test 7: Single project
    {
        int n = 1;
        int r = 10;
        std::vector<int> a = {5};
        std::vector<int> b = {2};
        assert(maxProjects(n, r, a, b) == 1);
    }

    // Test 8: Single project impossible
    {
        int n = 1;
        int r = 3;
        std::vector<int> a = {5};
        std::vector<int> b = {-1};
        assert(maxProjects(n, r, a, b) == 0);
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}

// This is a classic greedy + DP problem. The key insight is that projects with non‑negative change \(b_i\) should be attempted before projects with negative \(b_i\). For projects with non‑negative \(b_i\), you should always take the ones with the smallest requirement \(a_i\) first (to maximize the chance of being able to afford them). For projects with negative \(b_i\), you should process them in decreasing order of \(a_i + b_i\) (the rating after completing the project) — equivalently, the sum of requirement and change — because this ordering maximizes the leftover rating after finishing each such project, which is the optimal greedy for minimizing loss.  
// First, transform each requirement so that \(a_i = \max(a_i, -b_i)\). This ensures that if the change is very negative, the requirement is at least the absolute value of the change, because after completing the project the rating would drop below zero otherwise (which is impossible).  
// Then sort all projects using the comparator: non‑negative changes first (sorted by requirement ascending), then negative changes sorted by \(a_i + b_i\) descending.  
// After sorting, run a dynamic programming over the projects. Let `dp[i][j]` store the maximum rating you can have after considering the first `i` projects and having completed exactly `j` of them. Initialize `dp[0][0] = initialRating` and all other values to `-1` (impossible). For each project `i` (0‑indexed), iterate over `j` from 0 to n‑1. If `dp[i][j]` is reachable (>= 0), two transitions are possible: skip project `i` (so `dp[i+1][j] = max(...)`), or complete it if `dp[i][j] >= a[i]`, then `dp[i+1][j+1] = max(dp[i+1][j+1], dp[i][j] + b[i])`.  
// Finally, the answer is the largest `j` such that `dp[n][j] >= 0`.  
// Time complexity: sorting is \(O(n \log n)\), DP is \(O(n^2)\). Space complexity: \(O(n^2)\) for the DP table.
