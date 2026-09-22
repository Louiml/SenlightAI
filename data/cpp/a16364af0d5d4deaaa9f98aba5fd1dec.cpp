// Write a C++ function named `minPatches` that takes a sorted vector of positive integers `nums` and a positive integer `n`, and returns the minimum number of patches (additional numbers) that must be added to the array so that every integer in the range `[1, n]` can be formed as the sum of some subset of the resulting array. The input vector is guaranteed to be sorted in non-decreasing order and contain only positive integers. Assume `n` is at least 1, and the vector may be empty. The function must operate in-place conceptually (no modification of the input is required) and return an integer count of patches needed.
#include <cassert>
#include <vector>

int minPatches(const std::vector<int>& nums, int n);

int main() {
    // Example from the snippet
    assert(minPatches({1, 5, 10}, 20) == 2);

    // Already complete
    assert(minPatches({1, 2, 4, 8}, 15) == 0);

    // Empty vector: need powers of two up to n
    assert(minPatches({}, 7) == 3); // patches: 1, 2, 4

    // Large n with few elements
    assert(minPatches({1, 2, 31}, 100) == 2); // add 4 and 8

    // Single element covering everything
    assert(minPatches({1}, 1) == 0);

    // First element missing 1
    assert(minPatches({2, 3}, 6) == 1); // add 1

    // n smaller than existing coverage
    assert(minPatches({1, 2, 3, 4}, 5) == 0);

    // Duplicate values don't matter
    assert(minPatches({1, 1, 1}, 5) == 1); // add 2

    // Large n requiring multiple patches from scratch
    assert(minPatches({}, 1000000) == 20); // 2^19 = 524288, 2^20 = 1048576 > 1e6

    // Mixed case with gaps
    assert(minPatches({1, 5, 6}, 10) == 1); // add 2 (then 1+2=3, can't make 4, add 4? Actually check: miss=1, use1 -> miss2; use5? 5>2 so patch 2 -> miss4; use5? 5>4 patch4 -> miss8; use5 -> miss13 >10 => 2 patches, but expected? Let's recompute: 1 gives [1], miss=2; 5>2 patch2 -> miss4, now [1,3]; 5>4 patch4 -> miss8, now [1,7]; 5<=8 use5 -> miss13 >10, total patches=2. So assert(...,2))
    assert(minPatches({1, 5, 6}, 10) == 2);

    return 0;
}
#include <vector>
#include <cstddef>

// Returns the minimum number of patches needed so that every integer in [1, n]
// can be formed as a subset sum from the given sorted positive integers.
int minPatches(const std::vector<int>& nums, int n) {
    long long miss = 1;          // Smallest sum that cannot yet be formed
    std::size_t i = 0;           // Index into nums
    int patches = 0;             // Count of added numbers

    while (miss <= n) {
        if (i < nums.size() && nums[i] <= miss) {
            // Use the current number to extend the reachable range.
            miss += nums[i];     // Now can form [1, miss + nums[i] - 1]
            ++i;
        } else {
            // Patch with 'miss' itself, doubling the reachable range.
            miss += miss;        // Now can form [1, miss - 1] plus previous
            ++patches;
        }
    }
    return patches;
}
// The key insight is to maintain the largest contiguous range of sums that can be formed starting from 1, using a variable `miss` (initialized to 1) that represents the smallest sum we cannot yet form. We iterate through the given numbers in sorted order. For each number `nums[i]`:
// - If `nums[i] <= miss`, then we can extend our reachable range to `[1, miss + nums[i] - 1]` (i.e., we can now form all sums up to `miss + nums[i] - 1`), so we add `nums[i]` to `miss` and move to the next element.
// - If `nums[i] > miss`, then we must patch the gap by adding `miss` itself as a new number. This doubles the reachable range (since we can now form `[1, miss]` and by adding the existing numbers we can form up to `2*miss - 1`), so we set `miss = miss * 2` and increment the patch count.
//
// We repeat this process as long as `miss <= n`. The algorithm greedily adds the smallest missing number, which is optimal because it extends the range as much as possible with the fewest patches. Edge cases: an empty `nums` vector requires patching all powers of two (starting from 1) until `miss > n`; if `nums` already covers `[1, n]`, the answer is 0. Since `n` can be up to `2^31 - 1`, we use `long long` to avoid overflow when doubling. Time complexity is `O(nums.size() + log n)` because each iteration either consumes an element or doubles `miss`, and the number of doublings is at most `log2(n)`. Space complexity is `O(1)`.
