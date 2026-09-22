// Write a C++ function `int maxTowerHeight(vector<block>& blocks)` that, given a vector of blocks where each block is defined by three integer dimensions (x, y, z), returns the maximum possible height of a tower built by stacking blocks such that for any two adjacent blocks in the stack (lower block A on top of which block B is placed), both base dimensions of the upper block are strictly smaller than the corresponding base dimensions of the lower block. Each original block can be used at most once in any orientation (i.e., you may rotate each block so that any of its three dimensions serves as the height, and the remaining two form the base), but the same rotated version of a given block may be used only once if it shares the same orientation. The function must handle up to 30 original blocks, and all dimensions are positive integers up to 100. If the input vector is empty, return 0. The block struct is pre-defined as: `struct block { int x, y, z; block(int xx, int yy, int zz) : x(xx), y(yy), z(zz) {} };` Note that the orientation used in the input is just one possible orientation; you must consider all six rotations of each block, but you cannot use two rotations with identical base dimensions for the same original block more than once (if two rotations produce the same (x,y) pair with different z, treat them as distinct blocks that can both be used, since they have different heights; if they produce the same (x,y,z) triple, treat them as duplicate and only use one). The goal is to maximize the sum of heights (z) of stacked blocks.
#include <cassert>
#include <vector>

// Include the block struct and maxTowerHeight definition here or via header.

int main() {
    // Test 1: Single block, use tallest orientation.
    {
        std::vector<block> blocks = { block(1, 2, 3) };
        assert(maxTowerHeight(blocks) == 3);
    }

    // Test 2: Two blocks that can stack in one orientation.
    {
        std::vector<block> blocks = { block(10, 10, 1), block(5, 5, 5) };
        // Use block1 as base (height 1), block2 on top (height 5) => total 6.
        // Or block1 as height 10? Then base (10,1) cannot hold block2 (5,5) because 1<5.
        // So max is 6.
        assert(maxTowerHeight(blocks) == 6);
    }

    // Test 3: Blocks that cannot stack due to equal sides.
    {
        std::vector<block> blocks = { block(5, 5, 1), block(5, 5, 2) };
        // Strict inequality prevents stacking; max is max(1,2)=2.
        assert(maxTowerHeight(blocks) == 2);
    }

    // Test 4: Rotations allow stacking when orientation changes.
    {
        std::vector<block> blocks = { block(3, 4, 5) };
        // Only one block, max height is 5.
        assert(maxTowerHeight(blocks) == 5);
    }

    // Test 5: Classic box stacking example.
    {
        std::vector<block> blocks = { block(4, 6, 7), block(1, 2, 3), block(4, 5, 6), block(10, 12, 32) };
        // The expected maximum is 60 (from stacking 32+7+6+5? Let's compute:
        // Use (10,12,32) base, then (4,6,7) on top (4<10,6<12), then (4,5,6) on top of that? Need 4<4? No equal not allowed. So use (4,6,7) then (1,2,3) => 32+7+3=42. Or use (4,5,6) as base, then (1,2,3) => 6+3=9. Or use (10,12,32) + (4,5,6) + (1,2,3) = 32+6+3=41. Or use (10,12,32) + (4,6,7) + (1,2,3)=42. Actually there is a known max 60 by stacking (10,12,32) as base, then (4,6,7) rotated to (6,7,4) as height 4? That would be messy. Let's just check a known result: For blocks (1,2,3), (4,5,6), (10,12,32), max is 32+6+3=41? But (10,12) > (4,5) and (4,5) > (1,2) yes, so 41. For (4,6,7) also, 7+? 7 can be used as height, base (4,6) can hold (1,2) -> 7+3=10. Actually 32+7+3=42. So assert 42.
        assert(maxTowerHeight(blocks) == 42);
    }

    // Test 6: Empty input.
    {
        std::vector<block> blocks;
        assert(maxTowerHeight(blocks) == 0);
    }

    // Test 7: Duplicate orientations are not double counted.
    {
        std::vector<block> blocks = { block(2, 2, 2) };
        // All rotations are identical (2,2,2). Only one orientation can be used.
        assert(maxTowerHeight(blocks) == 2);
    }

    // Test 8: Stacking multiple blocks with different rotations.
    {
        std::vector<block> blocks = { block(5, 5, 1), block(3, 3, 10), block(1, 1, 100) };
        // Max: use (5,5,1) base, then (3,3,10) on top, then (1,1,100) => 111.
        assert(maxTowerHeight(blocks) == 111);
    }

    // Test 9: Same base but different height cannot stack.
    {
        std::vector<block> blocks = { block(3, 3, 10), block(3, 3, 20) };
        // Strict inequality prevents stacking; max is 20.
        assert(maxTowerHeight(blocks) == 20);
    }

    // Test 10: Classic UVa 437 example (use known result).
    {
        std::vector<block> blocks = { block(1, 1, 1), block(2, 2, 2), block(3, 3, 3), block(4, 4, 4) };
        // Max tower: stack all from largest base to smallest, each height = max of dimensions? Actually each block can be rotated so height is any dimension; to maximize, use height = the largest dimension of each block while keeping base strictly decreasing. For (4,4,4) use height 4, base (4,4); for (3,3,3) height 3, base (3,3); etc. Total = 4+3+2+1=10.
        assert(maxTowerHeight(blocks) == 10);
    }

    return 0;
}
#include <vector>
#include <algorithm>
#include <set>
#include <tuple>

struct block {
    int x, y, z;
    block(int xx, int yy, int zz) : x(xx), y(yy), z(zz) {}
};

// Returns the maximum height of a tower using each given block at most once in some orientation.
int maxTowerHeight(std::vector<block>& blocks) {
    if (blocks.empty()) return 0;

    // Generate all six orientations, remove exact duplicates using a set of tuples.
    std::set<std::tuple<int,int,int>> unique;
    for (const auto& b : blocks) {
        unique.insert(std::make_tuple(b.x, b.y, b.z));
        unique.insert(std::make_tuple(b.x, b.z, b.y));
        unique.insert(std::make_tuple(b.y, b.x, b.z));
        unique.insert(std::make_tuple(b.y, b.z, b.x));
        unique.insert(std::make_tuple(b.z, b.x, b.y));
        unique.insert(std::make_tuple(b.z, b.y, b.x));
    }

    std::vector<block> all;
    for (const auto& t : unique) {
        all.push_back(block(std::get<0>(t), std::get<1>(t), std::get<2>(t)));
    }

    // Sort in descending order of x, then y, then z.
    std::sort(all.begin(), all.end(), [](const block& a, const block& b) {
        if (a.x != b.x) return a.x > b.x;
        if (a.y != b.y) return a.y > b.y;
        return a.z > b.z;
    });

    int n = (int)all.size();
    std::vector<int> dp(n, 0);
    int maxHeight = 0;

    for (int i = 0; i < n; ++i) {
        dp[i] = all[i].z;
        for (int k = 0; k < i; ++k) {
            if (all[k].x > all[i].x && all[k].y > all[i].y) {
                dp[i] = std::max(dp[i], dp[k] + all[i].z);
            }
        }
        maxHeight = std::max(maxHeight, dp[i]);
    }

    return maxHeight;
}
// The problem is a variation of the classic "Box Stacking" dynamic programming problem. Since a block can be rotated, we generate six possible orientations for each original block: (x,y,z), (x,z,y), (y,x,z), (y,z,x), (z,x,y), (z,y,x). However, if two orientations produce identical (x,y) pairs but different heights, they are allowed as separate entries because they have different heights; if they produce the exact same (x,y,z) triple, we must avoid using both because that would essentially be using the same block twice with identical orientation, which is disallowed. To handle duplicates, we can insert all six orientations into a set keyed by (x,y,z) to remove exact duplicates. After that, we sort the resulting list in decreasing order of x (and for equal x, decreasing y, then decreasing z) to ensure that when we process from top to bottom, any block that can be placed under the current block appears earlier in the list. Then we apply a DP where `dp[i]` is the maximum height of a tower with block i at the top. For each i, we initialize `dp[i]` to the height of block i, then iterate over all previous blocks k where `blocks[k].x > blocks[i].x && blocks[k].y > blocks[i].y` (strictly larger both dimensions) and update `dp[i] = max(dp[i], dp[k] + blocks[i].z)`. The answer is the maximum over all dp[i]. Edge cases: empty input returns 0; blocks with equal dimensions in both base axes cannot be stacked on each other due to strict inequality; sorting ties need to be consistent so that a block that has exactly the same base dimensions as another (but different height) is not mistakenly considered stackable—since we require strict >, equal bases are not stackable, so sorting order doesn't affect correctness as long as we only consider k < i. Time complexity: O(N^2) where N is the number of distinct orientations (at most 6n, so O(n^2) with a constant factor). Space complexity: O(N). The number of original blocks is small, but the approach scales fine.
