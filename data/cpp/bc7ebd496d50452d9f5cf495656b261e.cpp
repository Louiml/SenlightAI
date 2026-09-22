Given an array of non-negative integers `nums`, write a C++ function `int countGoodPartitions(const std::vector<int>& nums)` that returns the number of ways to partition the array into contiguous non-empty subarrays such that each subarray does not contain a number that appears in any other subarray. In other words, the value sets of the subarrays must be pairwise disjoint. Because the answer can be large, return it modulo \(10^9 + 7\). The function should handle arrays of size 1 through \(10^5\), with values up to \(10^9\). A partition is a sequence of cuts between elements, and all numbers in each resulting segment must be unique globally (no value may appear in more than one segment).
#include <cassert>
#include <vector>

int main() {
    // Single element
    assert(countGoodPartitions({7}) == 1);
    // All distinct: 3 elements -> 2^(3-1) = 4
    assert(countGoodPartitions({1, 2, 3}) == 4);
    // Two distinct, each repeated: {1,2,1,2} must be one block -> 2^0 = 1
    assert(countGoodPartitions({1, 2, 1, 2}) == 1);
    // {1,2,3,2,1} all values interleaved -> one block
    assert(countGoodPartitions({1, 2, 3, 2, 1}) == 1);
    // {1,2,1,3,2,3} : values 1 and 3 each appear twice, ranges overlap? 
    // 1: [0,2], 2:[1,4], 3:[3,5] -> merged into one block
    assert(countGoodPartitions({1, 2, 1, 3, 2, 3}) == 1);
    // {1,2,1} and distinct remaining: {1,2,1,3,4} 
    // 1: [0,2] block; then 3 and 4 each single blocks -> total 3 blocks -> 2^2=4
    assert(countGoodPartitions({1, 2, 1, 3, 4}) == 4);
    // {1,2,2,3,3,1,4,5} 
    // 1 [0,5], 2 [1,2], 3 [3,4] -> merge [0,5]; then 4 and 5 separate -> 3 blocks -> 4 ways
    assert(countGoodPartitions({1, 2, 2, 3, 3, 1, 4, 5}) == 4);
    // Large test: all identical -> single block
    std::vector<int> big(100000, 42);
    assert(countGoodPartitions(big) == 1);
    // Distinct large: n=10 -> 2^9 = 512
    std::vector<int> distinct = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(countGoodPartitions(distinct) == 512);
}
#include <vector>
#include <unordered_map>
#include <cstdint>

// Returns the number of valid partitions modulo 1e9+7.
int countGoodPartitions(const std::vector<int>& nums) {
    const int MOD = 1000000007;
    const int n = static_cast<int>(nums.size());
    if (n == 0) return 0;

    // Record the last occurrence index for each value.
    std::unordered_map<int, int> lastPos;
    for (int i = 0; i < n; ++i) {
        lastPos[nums[i]] = i;
    }

    // Count the number of mandatory blocks.
    int blocks = 0;
    int currentRight = -1;
    for (int i = 0; i < n; ++i) {
        // The current block must extend at least to the last occurrence of this value.
        currentRight = std::max(currentRight, lastPos[nums[i]]);
        if (i == currentRight) {
            ++blocks;  // Block ends here.
            if (i + 1 < n) {
                currentRight = lastPos[nums[i + 1]];
            }
        }
    }

    // Compute 2^(blocks - 1) modulo MOD.
    int exponent = blocks - 1;
    long long result = 1;
    long long base = 2;
    while (exponent > 0) {
        if (exponent & 1) {
            result = (result * base) % MOD;
        }
        base = (base * base) % MOD;
        exponent >>= 1;
    }
    return static_cast<int>(result);
}
// The core idea is that if a number appears at multiple positions, then all those positions must belong to the same segment, otherwise the same value would appear in multiple segments, violating disjointness. Therefore, the array is divided into "mandatory blocks": for each distinct value, the segment must cover from its first occurrence to its last occurrence. After merging overlapping intervals (since if two values' first–last ranges overlap, they must be in the same segment), we obtain a set of non-overlapping blocks that completely partition the array. Between any two consecutive blocks, we may either cut or not cut, giving \(2^{k-1}\) choices where \(k\) is the number of blocks, because each of the \(k-1\) gaps between blocks can independently be a cut (making a new segment) or not (merging adjacent segments). However, the problem counts partitions into subarrays, so each gap either is cut or not; total ways = \(2^{k-1}\) modulo \(10^9+7\).  
// Edge cases: if all numbers are distinct, each element is its own block, so \(k = n\), and the answer is \(2^{n-1}\). If the array has only one element, \(k=1\), answer = 1. The algorithm: first scan the array to record for each value its last occurrence (using a hash map). Then scan left to right, maintaining the current block's required right boundary (max last occurrence seen so far). When the current index equals the current boundary, we have just completed a block; increment block count and reset boundary to the next index's last occurrence (or continue). Time complexity \(O(n)\) on average with a hash map, space \(O(U)\) where \(U\) is number of distinct values.  
// Modulo: because \(2^{k-1}\) can be huge, use fast exponentiation: compute power of 2 mod 1e9+7.
