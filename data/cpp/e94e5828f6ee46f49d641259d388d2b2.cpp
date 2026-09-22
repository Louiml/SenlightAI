/*
Given an array of `n` positive integers and a target integer `k`, write a C++ function `int maxPairsWithSumK(const std::vector<int>& nums, int k)` that returns the maximum number of disjoint pairs `(i, j)` such that `nums[i] + nums[j] == k`. Each element can be used in at most one pair, and the pairs must be disjoint (i.e., no index is shared between two pairs). The input array may contain duplicate values, and the order of elements does not matter. The function must handle the case where `n` can be up to \(10^5\) and `k` up to \(10^9\), so an efficient algorithm is required.
*/
#include <vector>
#include <unordered_map>

// Returns the maximum number of disjoint pairs that sum to k.
int maxPairsWithSumK(const std::vector<int>& nums, int k) {
    std::unordered_map<int, int> freq;
    int pairs = 0;

    for (int x : nums) {
        int complement = k - x;
        if (freq[complement] > 0) {
            // Use one occurrence of the complement to form a pair.
            freq[complement]--;
            pairs++;
        } else {
            // Store this number for future pairing.
            freq[x]++;
        }
    }

    return pairs;
}
#include <cassert>
#include <vector>

int maxPairsWithSumK(const std::vector<int>& nums, int k);

int main() {
    // Basic cases
    assert(maxPairsWithSumK({1, 2, 3, 4}, 5) == 2);
    assert(maxPairsWithSumK({1, 2, 3}, 3) == 1);
    assert(maxPairsWithSumK({1, 1, 1}, 2) == 1);
    assert(maxPairsWithSumK({1, 2, 3, 4, 5}, 6) == 2); // (1,5) and (2,4)
    
    // Duplicate values
    assert(maxPairsWithSumK({2, 2, 2, 2}, 4) == 2);
    assert(maxPairsWithSumK({3, 3, 3}, 6) == 1);
    
    // No pairs
    assert(maxPairsWithSumK({1, 1, 1}, 3) == 0);
    assert(maxPairsWithSumK({5, 5, 5}, 10) == 1);
    
    // Self-complement (x == k/2)
    assert(maxPairsWithSumK({5, 5, 5, 5}, 10) == 2);
    
    // Larger array
    std::vector<int> big(100000, 1);
    assert(maxPairsWithSumK(big, 2) == 50000);
    
    return 0;
}
// The goal is to count the maximum number of disjoint pairs whose sum equals `k`. A greedy frequency-based approach works: maintain a frequency map (e.g., `std::map` or `std::unordered_map`) of leftover unmatched numbers. Iterate through the array once. For each number `x`, check if there is an unmatched value `k - x` already seen. If yes, we can form a valid pair, so increment the pair count and decrement the frequency of `k - x` (effectively consuming one occurrence). If not, increment the frequency of `x` as a candidate for future matching. This greedy strategy is optimal because for each element we either immediately form a pair with an earlier unmatched counterpart or store it for later; since we process left-to-right, every pair is counted exactly once when its second element is encountered. Edge cases: when `x == k - x` (i.e., `2x == k`), we must ensure we do not use the same element twice; the algorithm handles this naturally because the check uses the frequency of `k - x` before incrementing `x`, and after decrementing we do not increment `x` again. Duplicate values are managed correctly by frequencies. Time complexity is \(O(n)\) on average with a hash map (or \(O(n \log n)\) with `std::map`). Space complexity is \(O(n)\) for the map.
