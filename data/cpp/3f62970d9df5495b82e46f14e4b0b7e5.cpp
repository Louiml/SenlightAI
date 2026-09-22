// Write a C++ function `int countPairsWithSum(const std::vector<int>& nums, int target)` that counts the number of pairs of distinct indices `(i, j)` with `i < j` such that `nums[i] + nums[j] == target`. The function must handle negative numbers, zeros, and duplicates naturally (each occurrence counts as a separate element). The input vector is non-empty and its size is at most 100,000; the target and each element fit in a 32-bit signed integer. The function should not modify the input vector and must run efficiently. For example, with `nums = {1, 5, 4, 2, 3}` and `target = 5`, the pairs `(1,4)` and `(2,3)` exist, so the result is `2`. If `nums = {3, 3, 3}` and `target = 6`, there are 3 pairs (any two of the three indices), so the result is `3`.
#include <cassert>
#include <vector>

long long countPairsWithSum(const std::vector<int>& nums, int target);

int main() {
    // Basic example
    assert(countPairsWithSum({1, 5, 4, 2, 3}, 5) == 2);
    // Duplicate values
    assert(countPairsWithSum({3, 3, 3}, 6) == 3);
    // Negative numbers
    assert(countPairsWithSum({-1, 2, -2, 1, 0}, 0) == 2); // pairs: (-1,1), (-2,2)
    // Zero target with zeros
    assert(countPairsWithSum({0, 0, 0}, 0) == 3);
    // No pairs
    assert(countPairsWithSum({1, 2, 3}, 10) == 0);
    // Single element
    assert(countPairsWithSum({7}, 7) == 0);
    // Large value overflow check (100k copies of 1, target 2)
    std::vector<int> big(100000, 1);
    long long expected = static_cast<long long>(100000) * 99999 / 2;
    assert(countPairsWithSum(big, 2) == expected);
    // Pairs with same value and target = 2*x
    assert(countPairsWithSum({4, 4}, 8) == 1);
    // Mixed positive/negative
    assert(countPairsWithSum({-5, 5, -5, 5}, 0) == 4);
}
#include <vector>
#include <unordered_map>

// Count unordered pairs of distinct indices (i<j) such that nums[i]+nums[j]==target.
long long countPairsWithSum(const std::vector<int>& nums, int target) {
    std::unordered_map<int, long long> freq;
    long long total = 0;
    for (int x : nums) {
        int needed = target - x;
        auto it = freq.find(needed);
        if (it != freq.end()) {
            total += it->second;
        }
        ++freq[x];
    }
    return total;
}
// The goal is to count all unordered pairs of distinct indices that sum to `target`. A brute-force double loop takes O(n²) time, which is too slow for n up to 100,000. Instead, we use a frequency map (hash table) to count occurrences of each value as we iterate. For each element `x` encountered, we need to know how many previously seen elements equal `target - x`; that count is added to the result. Then we increment the frequency of `x`. This avoids double-counting because each pair is counted exactly once when the second element of the pair is processed. Edge cases: negative numbers and zero work normally because subtraction handles them. If `target - x` equals `x`, we still count previously seen instances of the same value, not the current one, so duplicate handling is correct. Time complexity is O(n) average, with O(n) worst-case for unordered_map, and space is O(n) for the map. The solution uses `std::unordered_map<int, long long>` because the number of pairs can exceed 32-bit (e.g., 100,000 identical values with target double that value: 100,000 choose 2 ≈ 5e9), so we return `long long` to be safe. The function is `const`-correct by taking a `const std::vector<int>&` and not modifying it.
