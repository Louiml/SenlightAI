// Write a C++ function `bool canPartitionKSubsets(const std::vector<int>& nums, int k)` that determines whether the given vector of positive integers can be partitioned into `k` non-empty subsets such that the sum of each subset is equal. The function should return `true` if such a partition exists, and `false` otherwise. The input vector may contain duplicate values, and `k` is a positive integer. If `k` exceeds the number of elements, or the total sum is not divisible by `k`, or any single element exceeds the target subset sum, the function must return `false`. The solution should correctly handle edge cases such as `k = 1` (always true if the sum is positive), `nums` with all equal elements, and larger values where a greedy approach would fail.

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(canPartitionKSubsets({4, 3, 2, 3, 5, 2, 1}, 4) == true);
    assert(canPartitionKSubsets({1, 2, 3, 4}, 3) == false);
    
    // k = 1 always true for positive sum
    assert(canPartitionKSubsets({5, 6, 7}, 1) == true);
    
    // All equal elements
    assert(canPartitionKSubsets({2, 2, 2, 2}, 2) == true);
    
    // Single element and k=1
    assert(canPartitionKSubsets({10}, 1) == true);
    
    // Sum not divisible by k
    assert(canPartitionKSubsets({1, 2, 3, 4}, 2) == false);
    
    // k larger than n
    assert(canPartitionKSubsets({1, 2}, 5) == false);
    
    // Element larger than target
    assert(canPartitionKSubsets({8, 1, 1}, 2) == false);
    
    // Duplicate values with valid partition
    assert(canPartitionKSubsets({1, 1, 1, 1, 2, 2}, 2) == true);
    
    // Classic impossible case
    assert(canPartitionKSubsets({1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 3) == false);
    
    return 0;
}

#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>

// Determines if the vector of positive integers can be partitioned into k equal-sum subsets.
bool canPartitionKSubsets(const std::vector<int>& nums, int k) {
    int n = static_cast<int>(nums.size());
    if (k <= 0 || k > n) return false;
    
    int totalSum = std::accumulate(nums.begin(), nums.end(), 0);
    if (totalSum % k != 0) return false;
    
    int target = totalSum / k;
    
    // Sort descending to place larger numbers first, improving pruning.
    std::vector<int> sorted = nums;
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());
    
    if (sorted[0] > target) return false;
    
    std::vector<bool> visited(n, false);
    
    // Recursive backtracking lambda.
    std::function<bool(int, int, int)> solve = [&](int index, int remaining, int currentSum) -> bool {
        if (remaining == 0) return true;  // All subsets formed.
        if (currentSum > target) return false;
        
        if (currentSum == target) {
            // Start building the next subset from the beginning of the array.
            return solve(0, remaining - 1, 0);
        }
        
        for (int i = index; i < n; ++i) {
            if (visited[i]) continue;
            
            visited[i] = true;
            if (solve(i + 1, remaining, currentSum + sorted[i])) return true;
            visited[i] = false;
        }
        return false;
    };
    
    return solve(0, k, 0);
}

// The problem is the classic "Partition to K Equal Sum Subsets". The approach uses depth-first search (DFS) with backtracking and pruning. First, compute the total sum; if it is not divisible by `k`, return `false` immediately. The target sum for each subset is `avg = totalSum / k`. Sort the numbers in descending order to prioritize placing larger numbers first, which reduces the search space. If the largest element exceeds `avg`, it's impossible, so return `false`. Use a `visited` boolean array to track which elements have been assigned. The recursive function tries to build subsets one at a time: starting from a given index, attempt to place unused numbers into the current subset, accumulating `currsum`. If `currsum` equals `avg`, the subset is complete, so recurse with the next subset (`k-1`) and reset `currsum` to 0. If at any point `currsum` exceeds `avg`, prune that branch. The base case is when `k == 0`, meaning all subsets are formed, return `true`. Time complexity is exponential in the worst case, but pruning significantly improves performance; the upper bound is \(O(k \cdot 2^n)\) in the worst case, and space complexity is \(O(n)\) for the visited array and recursion stack.
