// Implement a C++ function `rangeSumWithLazyAdd` that takes a vector of integers `nums`, a vector of four-integer operations, and processes them in order. Each operation is represented as a tuple `(op, l, r, c)` where `l` and `r` are 1-indexed inclusive bounds, and `op` is either 0 (meaning add `c` to every element in `[l, r]`) or 1 (meaning compute the sum of elements in `[l, r]` modulo `c+1`, and output that value). The function should return a vector of integers containing the results of all type-1 operations in the order they appear. The array size `n` is the length of `nums`; operations use 1-based indexing. You may assume `n ≥ 1`, `1 ≤ l ≤ r ≤ n`, and all inputs fit in a 64-bit signed integer. The array after all modifications is not required to be returned, only the query results.
#include <cassert>
#include <vector>

// Include the solution function here (or link it).

int main() {
    // Test 1: Basic update and query
    std::vector<int64_t> nums1 = {0, 1, 2, 3, 4, 5}; // 1-indexed, index 0 unused
    std::vector<std::vector<int64_t>> ops1 = {
        {0, 2, 4, 10}, // add 10 to [2,4] -> [1,12,13,14,5]
        {1, 1, 5, 100}, // sum = 1+12+13+14+5 = 45 mod 101 = 45
        {0, 1, 1, 1},   // add 1 to [1,1] -> [2,12,13,14,5]
        {1, 2, 5, 3}    // sum = 12+13+14+5 = 44 mod 4 = 0
    };
    std::vector<int64_t> res1 = rangeSumWithLazyAdd(nums1, ops1);
    assert(res1.size() == 2);
    assert(res1[0] == 45);
    assert(res1[1] == 0);

    // Test 2: Same block operations
    std::vector<int64_t> nums2 = {0, 5, 5, 5}; // n=3, block size ~1
    std::vector<std::vector<int64_t>> ops2 = {
        {0, 1, 3, 1}, // add 1 to all -> [6,6,6]
        {1, 2, 2, 10}, // sum = 6 mod 11 = 6
        {1, 1, 3, 4}   // sum = 18 mod 5 = 3
    };
    std::vector<int64_t> res2 = rangeSumWithLazyAdd(nums2, ops2);
    assert(res2.size() == 2);
    assert(res2[0] == 6);
    assert(res2[1] == 3);

    // Test 3: Large range, mod small
    std::vector<int64_t> nums3 = {0, 1, 1, 1, 1, 1}; // n=5
    std::vector<std::vector<int64_t>> ops3 = {
        {0, 1, 5, 2}, // add 2 to all -> [3,3,3,3,3]
        {1, 1, 5, 3}, // sum = 15 mod 4 = 3
        {0, 2, 2, 1}, // add 1 to [2,2] -> [3,4,3,3,3]
        {1, 1, 5, 10} // sum = 16 mod 11 = 5
    };
    std::vector<int64_t> res3 = rangeSumWithLazyAdd(nums3, ops3);
    assert(res3.size() == 2);
    assert(res3[0] == 3);
    assert(res3[1] == 5);

    // Test 4: Single element array
    std::vector<int64_t> nums4 = {0, 7};
    std::vector<std::vector<int64_t>> ops4 = {
        {1, 1, 1, 1}, // sum = 7 mod 2 = 1
        {0, 1, 1, 3}, // now 10
        {1, 1, 1, 1}  // sum = 10 mod 2 = 0
    };
    std::vector<int64_t> res4 = rangeSumWithLazyAdd(nums4, ops4);
    assert(res4.size() == 2);
    assert(res4[0] == 1);
    assert(res4[1] == 0);

    return 0;
}
#include <vector>
#include <cmath>
#include <cstdint>

using int64 = long long;

// Process operations on a vector with range add and range sum modulo queries.
// nums: initial 1-indexed array values (size n+1, index 0 unused).
// ops: each op is {op, l, r, c}; op=0 add c, op=1 query sum modulo (c+1).
// Returns results of all type-1 operations.
std::vector<int64> rangeSumWithLazyAdd(std::vector<int64>& nums,
                                       const std::vector<std::vector<int64>>& ops) {
    int64 n = static_cast<int64>(nums.size()) - 1; // ignore index 0
    int64 blockSize = std::max<int64>(1, static_cast<int64>(std::sqrt(n)));
    int64 numBlocks = (n + blockSize - 1) / blockSize;

    std::vector<int64> sum(numBlocks + 1, 0);
    std::vector<int64> blockId(n + 1, 0);
    std::vector<int64> lazy(numBlocks + 1, 0);

    for (int64 i = 1; i <= n; ++i) {
        blockId[i] = (i - 1) / blockSize + 1;
        sum[blockId[i]] += nums[i];
    }

    std::vector<int64> results;
    results.reserve(ops.size());

    for (const auto& op : ops) {
        int64 type = op[0];
        int64 l = op[1];
        int64 r = op[2];
        int64 c = op[3];

        int64 leftBlock = blockId[l];
        int64 rightBlock = blockId[r];

        if (type == 0) { // update: add c to [l, r]
            if (leftBlock == rightBlock) {
                for (int64 i = l; i <= r; ++i) {
                    nums[i] += c;
                    sum[blockId[i]] += c;
                }
            } else {
                // Left partial block
                int64 leftEnd = leftBlock * blockSize;
                for (int64 i = l; i <= leftEnd; ++i) {
                    nums[i] += c;
                    sum[blockId[i]] += c;
                }
                // Full middle blocks
                for (int64 b = leftBlock + 1; b < rightBlock; ++b) {
                    lazy[b] += c;
                    sum[b] += blockSize * c;
                }
                // Right partial block
                int64 rightStart = (rightBlock - 1) * blockSize + 1;
                for (int64 i = rightStart; i <= r; ++i) {
                    nums[i] += c;
                    sum[blockId[i]] += c;
                }
            }
        } else { // query: sum modulo (c+1)
            int64 mod = c + 1;
            int64 result = 0;
            if (leftBlock == rightBlock) {
                for (int64 i = l; i <= r; ++i) {
                    result = (result + nums[i] + lazy[blockId[i]]) % mod;
                }
            } else {
                // Left partial block
                int64 leftEnd = leftBlock * blockSize;
                for (int64 i = l; i <= leftEnd; ++i) {
                    result = (result + nums[i] + lazy[blockId[i]]) % mod;
                }
                // Full middle blocks
                for (int64 b = leftBlock + 1; b < rightBlock; ++b) {
                    result = (result + sum[b]) % mod;
                }
                // Right partial block
                int64 rightStart = (rightBlock - 1) * blockSize + 1;
                for (int64 i = rightStart; i <= r; ++i) {
                    result = (result + nums[i] + lazy[blockId[i]]) % mod;
                }
            }
            results.push_back(result);
        }
    }
    return results;
}
// The core idea is to use **sqrt decomposition** (block decomposition) to support range add and range sum queries efficiently. Split the array into blocks of size roughly `√n`. Each block maintains:
// - `num[i]`: the original value of element `i` (excluding any block-level lazy additions).
// - `sum[block]`: the total sum of elements in that block, including all lazy additions applied to the whole block.
// - `mk[block]`: a lazy addition tag applied to every element in the block.
//
// For a range update `[l, r]` with value `k`:
// - If both ends are in the same block, iterate over the subrange directly, updating each `num[i]` and the block’s `sum`.
// - Otherwise:
//   - Update the partial left block from `l` to the end of its block, iterating directly.
//   - For full blocks in between, increment their `mk` and `sum` by `block_size * k`.
//   - Update the partial right block from the start of its block to `r`, iterating directly.
//
// For a range sum query `[l, r]` modulo `mod`:
// - If same block, iterate directly summing `num[i] + mk[block]` mod `mod`.
// - Otherwise:
//   - Sum the partial left block directly.
//   - Add the whole `sum` of each full middle block mod `mod`.
//   - Sum the partial right block directly.
//
// Time complexity: Each update or query touches at most `O(√n)` elements directly and `O(√n)` full blocks, so each operation is `O(√n)`. For `m` operations, total is `O(m√n)`. Space is `O(n)` for the arrays. Edge cases: when `l` and `r` are in the same block, and when the range exactly covers full blocks but the boundary blocks are also full (handled by the generic logic). Be careful with modulo after each addition to avoid overflow, and with 1-based vs 0-based indexing conversion.
