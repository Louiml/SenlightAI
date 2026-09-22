// Given an array of `n` non-negative integers (where `2 ≤ n ≤ 10`), a target sum `m` (with `1 ≤ m ≤ 10000`), and the values `a[i]` each in the range `[1, 100]`, you must select exactly one positive perfect square `x_i` for each index `i` (where `x_i` is among `1, 4, 9, ..., 10000`, i.e., squares of integers from 1 to 100) such that the total sum of the selected squares equals exactly `m`. Your goal is to minimize the total cost defined as the sum over all `i` of `(a[i] - (sqrt(x_i) - 1))^2`, where `sqrt(x_i)` is the integer square root (i.e., if `x_i = k^2`, then `sqrt(x_i) = k`). If no such selection exists that sums exactly to `m`, return `-1`. Write a C++ function `int minCost(vector<int>& a, int m)` that solves this problem.
#include <cassert>
#include <vector>

// Declaration of the function under test (would be included from the solution)
int minCost(std::vector<int>& a, int m);

int main() {
    // Test 1: Simple case, n=2, a={3,4}, m=10 -> choose squares 1 and 9 (k=1,3)
    // Cost = (3-0)^2 + (4-2)^2 = 9+4=13. Alternative: 4+6? Not perfect squares. So answer 13.
    {
        std::vector<int> a = {3, 4};
        assert(minCost(a, 10) == 13);
    }
    // Test 2: Impossible, n=2, m=3 (cannot sum two perfect squares each >=1 to 3)
    {
        std::vector<int> a = {1, 1};
        assert(minCost(a, 3) == -1);
    }
    // Test 3: All zeros possible: n=1, a={5}, m=1 -> choose square 1 (k=1), cost=(5-0)^2=25
    {
        std::vector<int> a = {5};
        assert(minCost(a, 1) == 25);
    }
    // Test 4: Exact match with multiple squares, n=2, a={1,1}, m=8 -> choose 4 and 4 (k=2,2)
    // Cost = (1-1)^2 + (1-1)^2 = 0
    {
        std::vector<int> a = {1, 1};
        assert(minCost(a, 8) == 0);
    }
    // Test 5: Larger value, n=3, a={10,10,10}, m=3 -> each chooses 1 (k=1), sum=3, cost = 9^2*3=243
    {
        std::vector<int> a = {10, 10, 10};
        assert(minCost(a, 3) == 243);
    }
    // Test 6: Edge: m too small for n>1, n=2, m=1 -> impossible (need at least 1+1=2)
    {
        std::vector<int> a = {2, 2};
        assert(minCost(a, 1) == -1);
    }
    // Test 7: Check early break, n=3, a={1,2,3}, m=25 -> e.g., squares 16,4,5? no 5. Try 9+16+0? no. Actually 25 = 9+16+0? no 0 not allowed. Possible: 25 = 1+8+16? 8 not square. 1+4+20? no. So maybe impossible. Let's find: 25 = 1+1+23? no. 4+4+17? no. 9+9+7? no. 16+4+5? no. 25+0? no. So likely -1.
    {
        std::vector<int> a = {1, 2, 3};
        assert(minCost(a, 25) == -1);
    }
    // Test 8: Ensure correct min among multiple options, n=2, a={5,5}, m=10 -> options: 1+9 (k=1,3) cost=(5-0)^2+(5-2)^2=25+9=34; 9+1 (k=3,1) same; 4+6 no; so 34
    {
        std::vector<int> a = {5, 5};
        assert(minCost(a, 10) == 34);
    }
    // Test 9: n=4, a={1,1,1,1}, m=4 -> each chooses 1 (k=1) cost 0
    {
        std::vector<int> a = {1, 1, 1, 1};
        assert(minCost(a, 4) == 0);
    }
    // Test 10: Stress n=10, all a=100, m=10000 -> each chooses 1000? but squares max 100^2=10000 only for one; for 10 elements sum would exceed. So choose 1000 each impossible. Actually need sum exactly 10000, with 10 squares each at least 1, so max sum 10*10000=100000, we can choose many 0? no. Choose each 1000? 1000 not square. Choose one 10000 and rest 0? no 0 not allowed. So maybe impossible? Let's just pick simple: n=2, a={100,100}, m=2 -> choose 1 and 1, cost = 99^2*2 = 19602
    {
        std::vector<int> a = {100, 100};
        assert(minCost(a, 2) == 19602);
    }
    return 0;
}
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>

// Returns the minimum total cost to select one perfect square per index
// such that the sum of selected squares equals exactly m, or -1 if impossible.
int minCost(std::vector<int>& a, int m) {
    int n = a.size();
    if (n == 0) return (m == 0) ? 0 : -1;
    
    // Precompute all possible squares up to m
    std::vector<int> squares;
    for (int k = 1; k * k <= m; ++k) {
        squares.push_back(k * k);
    }
    if (squares.empty()) return -1;
    
    // dp_prev: map from sum to min cost up to previous index
    std::unordered_map<int, int> dp_prev;
    for (int sq : squares) {
        int k = static_cast<int>(std::sqrt(sq));
        int cost = (a[0] - (k - 1)) * (a[0] - (k - 1));
        dp_prev[sq] = cost;
    }
    
    for (int i = 1; i < n; ++i) {
        std::unordered_map<int, int> dp_curr;
        for (const auto& entry : dp_prev) {
            int prev_sum = entry.first;
            int prev_cost = entry.second;
            for (int sq : squares) {
                int new_sum = prev_sum + sq;
                if (new_sum > m) break; // squares are sorted, so break early
                int k = static_cast<int>(std::sqrt(sq));
                int new_cost = prev_cost + (a[i] - (k - 1)) * (a[i] - (k - 1));
                auto it = dp_curr.find(new_sum);
                if (it == dp_curr.end()) {
                    dp_curr[new_sum] = new_cost;
                } else {
                    it->second = std::min(it->second, new_cost);
                }
            }
        }
        if (dp_curr.empty()) return -1; // no way to continue
        dp_prev = std::move(dp_curr);
    }
    
    auto it = dp_prev.find(m);
    return (it != dp_prev.end()) ? it->second : -1;
}
// This problem is a resource-constrained optimization that can be modeled as a dynamic programming over indices and accumulated square sums. For each index `i` (from 0 to n-1), we maintain a map `dp[i]` where `dp[i][s]` is the minimum total cost up to index `i` using a total sum of squares exactly equal to `s`. Initialization: for the first element, for every possible square `k^2` (with `1 ≤ k ≤ 100`), set `dp[0][k^2] = (a[0] - (k-1))^2`. Then for each subsequent index `i`, we iterate over all possible squares `k^2` (up to `m`, since sums exceeding `m` are useless) and combine each existing state from `dp[i-1]` (with sum `s_prev`) with the new square, updating `dp[i][s_prev + k^2]` to the minimum of its current value and `dp[i-1][s_prev] + (a[i] - (k-1))^2`. Because there are at most 100 squares per index and the number of distinct sums is bounded by `m`, the complexity is O(n * 100 * m) in the worst case, but in practice `m` can be up to 10000 and n up to 10, so worst-case operations are about 10 million, which is acceptable. Space complexity is O(n * m) if we store all maps, but we only need the previous map to compute the next, reducing to O(m) for the current and previous maps. Edge cases: if `m` is not reachable (e.g., `m` is too small to be formed by n squares each at least 1), the map for the last index will not contain `m`, so return -1. Also note that the square values are limited to those `k^2 <= m`, because any larger square would exceed the target sum.
