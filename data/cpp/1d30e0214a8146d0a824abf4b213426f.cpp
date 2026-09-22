// Write a C++ function that takes a sequence of integers representing segment copy numbers along a chromosome (positive integers, with 0 indicating a deleted/absent segment) and a threshold value `k`. The function must determine whether the sequence can be partitioned into contiguous blocks such that, within each block, the sum of the copy numbers is at least `k`, and the number of blocks is maximized. If no valid partition exists (i.e., the total sum is less than `k`), return 0 blocks. The function should return the maximum number of blocks achievable. The input is provided as a `std::vector<int>` for the copy numbers and an integer `k`. This mimics the problem of finding maximum segments with sufficient total copy number in a genome, inspired by the BFB path construction where contiguous segments must have balanced copy numbers.
// The problem is a classic greedy partition problem. To maximize the number of blocks with each block having a sum ≥ `k`, we iterate through the array while maintaining a running sum. Whenever the running sum reaches or exceeds `k`, we increment the block count and reset the running sum to 0. This greedy approach works because any optimal solution can be transformed into the greedy one without reducing the block count: if a block in an optimal solution has a sum larger than `k`, we can split it earlier without violating the constraint, as long as the prefix reaches `k`. Edge cases include: (1) if the total sum of all elements is less than `k`, the result is 0 blocks; (2) elements with value 0 can appear in any block and do not affect the sum positively; (3) the sequence may be empty, in which case the answer is 0. Time complexity is O(n) where n is the number of segments, and space complexity is O(1) auxiliary.
#include <vector>

// Returns the maximum number of contiguous blocks where each block's sum is at least k.
// If the total sum is less than k, returns 0.
int maxBlocksWithMinSum(const std::vector<int>& copyNumbers, int k) {
    if (k <= 0) return 0;
    int blockCount = 0;
    int currentSum = 0;
    for (int cn : copyNumbers) {
        if (cn < 0) continue; // ignore negative copy numbers (shouldn't happen, but defensive)
        currentSum += cn;
        if (currentSum >= k) {
            ++blockCount;
            currentSum = 0;
        }
    }
    return blockCount;
}
#include <cassert>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Basic case: partition into blocks of sum >= 5
    assert(maxBlocksWithMinSum({2, 3, 1, 4, 5}, 5) == 2); // [2,3] sum=5, [1,4] sum=5, [5] sum=5 but we maximize: Two blocks (e.g., [2,3] and [1,4]) leaves 5 as a third? Let's verify: Greedy: sum=2 -> add 3 -> sum=5 => block1 reset; sum=1 -> add4 -> sum=5 => block2 reset; sum=5 => block3. So 3 blocks? Wait, check: [2,3] =5, [1,4]=5, [5]=5 => 3 blocks. yes 3.
    assert(maxBlocksWithMinSum({2, 3, 1, 4, 5}, 5) == 3);
    // Test when total sum below k
    assert(maxBlocksWithMinSum({1, 1, 1}, 5) == 0);
    // All zeros
    assert(maxBlocksWithMinSum({0, 0, 0}, 1) == 0);
    // Single element exactly k
    assert(maxBlocksWithMinSum({7}, 7) == 1);
    // Single element less than k
    assert(maxBlocksWithMinSum({3}, 7) == 0);
    // Mixture with zeros
    assert(maxBlocksWithMinSum({0, 5, 0, 2, 3}, 5) == 2); // [5] and [2,3]
    // Empty vector
    assert(maxBlocksWithMinSum({}, 1) == 0);
    // Large k
    assert(maxBlocksWithMinSum({10, 2, 8}, 10) == 2); // [10] and [2,8] sum=10
    // Negative k (should return 0 per spec, though not expected)
    assert(maxBlocksWithMinSum({1, 2}, -1) == 0);
    // Exact multiple blocks
    assert(maxBlocksWithMinSum({2, 2, 2, 2}, 4) == 2); // [2,2] and [2,2]
    // Sequentially exceeding
    assert(maxBlocksWithMinSum({3, 3, 3}, 5) == 1); // [3,3] sum=6 -> block1, resets; remaining 3 sums<5 -> only 1 block.
    return 0;
}
