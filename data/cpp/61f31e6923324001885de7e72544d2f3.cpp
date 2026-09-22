Given a list of coin values (positive integers) and a target amount (also a positive integer), write a C++ function that returns a `std::vector<int>` containing a subset of the coin values whose sum is exactly equal to the target, if such a subset exists. If multiple subsets exist, prefer the one with the fewest coins; if there are still ties, prefer the one with the lexicographically smallest sequence (when viewed as a sorted list of indices in the original order). If no subset sums to the target, return an empty vector. The input list may contain duplicate coin values. Your solution must use a branch-and-bound search similar to the logic in the provided snippet, exploring the inclusion branch first and pruning when the current sum exceeds the target.
// We need to solve the subset-sum problem with an exact target and an optimization criterion (minimum number of coins, then lexicographically smallest index sequence). We model the search as a depth-first traversal of a binary tree where each node decides whether to include a particular coin. Coins are processed in the order given. At each step, we maintain the current sum and the current selection of indices. We use pruning: if the current sum plus the sum of all remaining coins is less than the target, we backtrack (can’t reach the target). If the current sum exceeds the target, we backtrack immediately. If the current sum equals the target, we record the candidate solution if it has fewer coins than the best found so far, or if it has the same count but the index sequence is lexicographically smaller (when comparing the sorted index lists element by element). To achieve the lexicographic tie-breaking, we can compare the full sorted index vectors lexicographically (std::lexicographical_compare). Since we explore inclusion first, the first solution found depth-first might not be optimal, so we must continue searching even after finding a solution, updating the best only when the criteria improve. We also track the best solution’s coin count and index vector. Time complexity is exponential in the worst case (subset-sum is NP-hard), but pruning with the remaining sum and overshoot bound helps. Space complexity is O(n) for the recursion stack and current selection. Edge cases: empty list, target zero (but target is positive per spec), duplicates allowed, and the case where no subset matches.
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>

/**
 * Find a subset of coins (by indices in the original order) that sums exactly to target.
 * Optimizes by fewest coins, then lexicographically smallest sorted index sequence.
 * Returns the chosen coin values in the original order (not sorted by value), or empty if none.
 */
std::vector<int> subsetSumExact(const std::vector<int>& coins, int target) {
    if (target <= 0) return {}; // target must be positive per spec
    int n = coins.size();
    
    // Precompute suffix sums for pruning
    std::vector<int> suffixSum(n + 1, 0);
    for (int i = n - 1; i >= 0; --i) {
        suffixSum[i] = suffixSum[i+1] + coins[i];
    }
    
    std::vector<int> bestIndices;      // best solution as indices
    std::vector<int> currentIndices;   // current recursion path
    int bestCount = INT_MAX;           // size of best solution
    
    // Recursive DFS: idx is current index to consider (0..n)
    // currentSum is sum so far
    std::function<void(int, int)> dfs = [&](int idx, int currentSum) {
        // Prune: can't reach target with remaining coins
        if (currentSum + suffixSum[idx] < target) return;
        // Prune: already exceeded target
        if (currentSum > target) return;
        
        // If we achieve the target
        if (currentSum == target) {
            // Check against current best
            if (currentIndices.size() < bestCount ||
                (currentIndices.size() == bestCount && 
                 std::lexicographical_compare(currentIndices.begin(), currentIndices.end(),
                                              bestIndices.begin(), bestIndices.end()))) {
                bestIndices = currentIndices;
                bestCount = currentIndices.size();
            }
            return; // any further addition would exceed or change sum
        }
        
        if (idx >= n) return; // no more coins
        
        // Inclusion branch first
        currentIndices.push_back(idx);
        dfs(idx + 1, currentSum + coins[idx]);
        currentIndices.pop_back();
        
        // Exclusion branch
        dfs(idx + 1, currentSum);
    };
    
    dfs(0, 0);
    
    // Convert indices to coin values in original order
    std::vector<int> result;
    result.reserve(bestIndices.size());
    for (int i : bestIndices) {
        result.push_back(coins[i]);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Basic exact subset
    {
        std::vector<int> coins = {3, 1, 4, 2};
        auto res = subsetSumExact(coins, 6);
        assert(res.size() == 2);
        // Possible: {1,4}? Actually 1+? Let's compute: {3,1,2}=6 but that's 3 coins; {1,4}+? 1+4=5; {2,4}=6 with 2 coins and indices {1,2}? Actually 4+2=6 indices {2,3} in original 0-based? coins[2]=4, coins[3]=2 -> indices [2,3] sorted lexicographically vs [0,1]? {3,1}=4 not. Let's check: best is that with fewest coins. {1,2,3}? wait 3+1+2=6 (3 coins). {4+2}=6 (2 coins) indices [2,3] sorted [2,3]. Another possibility {3, ?}? 3+? 3+1+?=6 -> 3+1+2=6 (3 coins). So best is [2,3] -> values [4,2]. But the order in result is original order: [4,2]? Actually we push coins[i] in the order of bestIndices, which is [2,3], so result = {coins[2]=4, coins[3]=2} = {4,2}. But many solutions might be valid; we just assert sum and size.
        int sum = 0; for (int v : res) sum += v;
        assert(sum == 6);
        assert(res.size() == 2); // fewest coins
    }
    
    // No solution
    {
        std::vector<int> coins = {2, 4, 6};
        auto res = subsetSumExact(coins, 5);
        assert(res.empty());
    }
    
    // Duplicate values, target equals single coin
    {
        std::vector<int> coins = {5, 5, 5};
        auto res = subsetSumExact(coins, 5);
        assert(res.size() == 1);
        assert(res[0] == 5);
    }
    
    // Exact sum of multiple coins
    {
        std::vector<int> coins = {1, 2, 3, 4, 5};
        auto res = subsetSumExact(coins, 9);
        int sum = 0; for (int v : res) sum += v;
        assert(sum == 9);
        assert(res.size() == 2); // {4,5} best
    }
    
    // Lexicographic tie-breaking: two ways with same count
    // coins = {1,2,2,3} target=3: subsets {1,2} (indices 0,1) and {3} (index 3) but {3} has 1 coin better. Need tie: target=4: {1,3} (indices 0,3) and {2,2} (indices 1,2) same count 2. Lexicographically sorted indices [0,3] vs [1,2] -> [0,3] is smaller.
    {
        std::vector<int> coins = {1, 2, 2, 3};
        auto res = subsetSumExact(coins, 4);
        assert(res.size() == 2);
        // The lexicographically smallest index set is {0,3} -> values [1,3]
        if (res[0] == 1 && res[1] == 3) {
            // good
        } else if (res[0] == 2 && res[1] == 2) {
            assert(false); // this is indices [1,2] which is larger lexicographically
        } else {
            assert(false);
        }
    }
    
    // Slightly larger test
    {
        std::vector<int> coins = {2, 3, 5, 7, 11};
        auto res = subsetSumExact(coins, 13);
        int sum = 0; for (int v : res) sum += v;
        assert(sum == 13);
        // fewest coins: 2+11=13 (2 coins) or 5+7+? 5+7=12, no. 2+3+? 2+3+?=13? 2+3+? 2+3+5=10, 2+3+7=12, 2+3+11=16. So best is 2+11 or 5+7? 5+7=12 no. also 3+? 3+? 3+? 3+? 13-3=10 ->  ? 5+? 5+? 5+? no, 7+3=10? Actually 3+5+?=13? 3+5+5=13? but only one 5. So only 2+11. assert size 2.
        assert(res.size() == 2);
    }
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
