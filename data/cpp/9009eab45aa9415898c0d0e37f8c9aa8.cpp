// Write a C++ function `rangeMinUpdateQuery` that processes an array with two operations: range minimum query and range assignment update. Given an initial array of `n` integers and `q` operations, each operation is either a query `(1, l, r)` asking for the minimum value in the subarray `[l, r]` (1-indexed) or an update `(2, l, r, v)` that assigns value `v` to every element in `[l, r]`. The function should take the initial array and a list of operations as input and return a vector of integers containing the answers to all query operations in order. Use a blocked/sqrt-decomposition approach.

#include <cassert>
#include <vector>
#include <climits>

// The solution function is declared above (or include it here). For testing, we assume it's available.

int main() {
    // Test 1: basic updates and queries
    std::vector<int> a1 = {1, 2, 3, 4, 5};
    std::vector<std::vector<int>> ops1 = {
        {1, 1, 3},       // min of [1,3] = 1
        {2, 2, 4, 0},    // set [2,4] to 0
        {1, 2, 4},       // min of [2,4] = 0
        {1, 1, 5},       // min of [1,5] = 0
    };
    auto res1 = rangeMinUpdateQuery(a1, ops1);
    assert((res1 == std::vector<int>{1, 0, 0}));

    // Test 2: single element array
    std::vector<int> a2 = {7};
    std::vector<std::vector<int>> ops2 = {
        {1, 1, 1},       // 7
        {2, 1, 1, -3},   // set to -3
        {1, 1, 1},       // -3
    };
    auto res2 = rangeMinUpdateQuery(a2, ops2);
    assert((res2 == std::vector<int>{7, -3}));

    // Test 3: large range exactly covering blocks
    std::vector<int> a3 = {5, 3, 8, 1, 9, 2};
    // blockSize = sqrt(6)+1 = 3, blocks: [0..2], [3..5]
    std::vector<std::vector<int>> ops3 = {
        {1, 1, 6},       // min = 1
        {2, 1, 3, 10},   // set first block to 10
        {1, 1, 6},       // min = 2 (from last block)
        {2, 4, 6, 0},    // set last block to 0
        {1, 1, 6},       // min = 0
    };
    auto res3 = rangeMinUpdateQuery(a3, ops3);
    assert((res3 == std::vector<int>{1, 2, 0}));

    // Test 4: overlap with mixed partial/full blocks
    std::vector<int> a4 = {4, 2, 6, 1, 5, 3, 9, 7};
    std::vector<std::vector<int>> ops4 = {
        {2, 2, 7, -1},   // set [2..7] to -1 (indices 1..6)
        {1, 1, 8},       // min = -1
        {2, 1, 8, 100},  // set all to 100
        {1, 1, 8},       // min = 100
    };
    auto res4 = rangeMinUpdateQuery(a4, ops4);
    assert((res4 == std::vector<int>{-1, 100}));

    // Test 5: queries with negative numbers and no updates
    std::vector<int> a5 = {-5, -1, -10, 0, 3};
    std::vector<std::vector<int>> ops5 = {
        {1, 1, 5},   // -10
        {1, 4, 5},   // 0
        {1, 2, 2},   // -1
    };
    auto res5 = rangeMinUpdateQuery(a5, ops5);
    assert((res5 == std::vector<int>{-10, 0, -1}));

    return 0;
}

#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
#include <cassert>

// Solves range minimum query and range assignment update using sqrt decomposition.
// a: initial array (0-indexed). ops: vector of operations where each op is {type, l, r, v} (1-indexed, v ignored for query).
// Returns vector of answers to all type-1 operations.
std::vector<int> rangeMinUpdateQuery(const std::vector<int>& a, const std::vector<std::vector<int>>& ops) {
    int n = (int)a.size();
    if (n == 0) return {};
    
    int blockSize = (int)std::sqrt(n) + 1;
    int numBlocks = (n + blockSize - 1) / blockSize;
    
    // Copy the array (will be mutated during updates)
    std::vector<int> arr = a;
    // Lazy assignment tag per block; -1 means no pending assignment
    std::vector<int> lazy(numBlocks, -1);
    // Block minimum; for lazy blocks it's the assignment value, else min of arr in block
    std::vector<int> blockMin(numBlocks, INT_MAX);
    
    // Initialize block minimums
    for (int i = 0; i < n; ++i) {
        int b = i / blockSize;
        blockMin[b] = std::min(blockMin[b], arr[i]);
    }
    
    // Helper lambdas
    auto relax = [&](int b) {
        if (lazy[b] != -1) {
            int start = b * blockSize;
            int end = std::min(n, start + blockSize);
            for (int i = start; i < end; ++i) {
                arr[i] = lazy[b];
            }
            lazy[b] = -1;
        }
    };
    
    auto recomputeBlockMin = [&](int b) {
        // Only recompute if no lazy tag (otherwise blockMin already equals lazy)
        if (lazy[b] == -1) {
            int start = b * blockSize;
            int end = std::min(n, start + blockSize);
            blockMin[b] = INT_MAX;
            for (int i = start; i < end; ++i) {
                blockMin[b] = std::min(blockMin[b], arr[i]);
            }
        }
    };
    
    std::vector<int> results;
    
    for (const auto& op : ops) {
        int type = op[0];
        int l = op[1] - 1; // convert to 0-indexed
        int r = op[2] - 1;
        
        if (type == 1) { // query
            // Relax the blocks containing l and r? Actually not needed for whole blocks, but for individual elements we need relaxation.
            int ans = INT_MAX;
            for (int i = l; i <= r; ) {
                int b = i / blockSize;
                int blockStart = b * blockSize;
                int blockEnd = std::min(n, blockStart + blockSize);
                if (i == blockStart && i + blockSize - 1 <= r) {
                    // Whole block
                    ans = std::min(ans, blockMin[b]);
                    i += blockSize;
                } else {
                    // Partial: relax the block and read arr[i]
                    relax(b);
                    ans = std::min(ans, arr[i]);
                    ++i;
                }
            }
            results.push_back(ans);
        } else { // update, type == 2
            int v = op[3];
            // Relax edge blocks
            relax(l / blockSize);
            if (l / blockSize != r / blockSize) relax(r / blockSize);
            
            for (int i = l; i <= r; ) {
                int b = i / blockSize;
                int blockStart = b * blockSize;
                int blockEnd = std::min(n, blockStart + blockSize);
                if (i == blockStart && i + blockSize - 1 <= r) {
                    // Whole block: set lazy and blockMin
                    lazy[b] = v;
                    blockMin[b] = v;
                    i += blockSize;
                } else {
                    // Partial: assign individual
                    arr[i] = v;
                    ++i;
                }
            }
            // Recompute min for edge blocks (if they are not lazy already)
            recomputeBlockMin(l / blockSize);
            if (l / blockSize != r / blockSize) recomputeBlockMin(r / blockSize);
        }
    }
    
    return results;
}

// The code snippet uses square root decomposition (also known as block decomposition) to handle range updates and range minimum queries efficiently. The array is divided into blocks of size approximately `sqrt(n)`. For each block, maintain:
// - `a[]`: the actual array values (some may be stale if a block has a pending lazy assignment).
// - `cx[]`: a lazy tag for a block; if not `-1`, it means the entire block has been assigned this value; otherwise, the block is considered clean and `a[]` is valid.
// - `cm[]`: the minimum value of the block considering the lazy tag. When a block has a lazy tag, `cm[]` equals that tag; otherwise, it is the minimum of the actual `a[]` values in that block.
//
// For updates on range `[b, e]`:
// - First "relax" (push down) the lazy tags for the blocks containing `b` and `e`, so their actual array values become correct before partial changes.
// - Traverse the range in steps. If a segment aligns exactly with a whole block, set the lazy tag and block minimum to `val` for that entire block. Otherwise, update individual elements.
// - After modifications, recompute the block minimums for the two boundary blocks (the internal fully-covered blocks already have correct min from the lazy tag).
//
// For queries on range `[b, e]`:
// - Traverse the range. If a segment is a whole block, take its block minimum. Otherwise, relax the block containing that individual element (to ensure `a[i]` is correct) and take `a[i]`.
// - Return the minimum of all considered values.
//
// Edge cases: blocks of unequal size at the end; updates that span exactly one element; overlapping partial updates on the same block; and queries that entirely cover blocks. The algorithm must correctly handle integer overflow (use `int` but values can be large; note the original uses `int`). Time complexity is `O((n + q) * sqrt(n))` worst-case because each operation touches at most two partial blocks (each of size `sqrt(n)`) and up to `sqrt(n)` full blocks. Space complexity is `O(n)` for the array plus `O(sqrt(n))` for block metadata.
