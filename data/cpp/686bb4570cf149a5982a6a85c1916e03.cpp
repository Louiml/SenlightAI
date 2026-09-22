/*
Write a C++ function `minKBitFlips` that takes a vector of integers `nums` (where each element is either 0 or 1) and an integer `k` (1 ≤ k ≤ nums.size()), and returns the minimum number of k-length consecutive subarray flips needed to turn all elements into 1s. A flip operation toggles every bit in a contiguous subarray of length exactly `k` (changing 0→1 and 1→0). If it is impossible to make all bits 1, return -1. The function should not modify the input vector unless needed for internal computation; it must be const-correct and efficient for large inputs (up to 10^5 elements). The solution should use a greedy left-to-right sweep with a sliding-window flip counter to avoid simulating every flip explicitly.
*/

#include <vector>
#include <queue>

// Returns the minimum number of k-length flips to make all bits 1, or -1 if impossible.
// Time: O(n), Space: O(k) using a queue to track flip end positions.
int minKBitFlips(std::vector<int>& nums, int k) {
    int n = nums.size();
    int flips = 0;          // total flips performed
    int currentFlips = 0;   // number of flips currently active at the current position
    std::queue<int> activeFlips; // stores the ending index (exclusive) of each active flip

    for (int i = 0; i < n; ++i) {
        // Remove flips that have ended before index i
        if (!activeFlips.empty() && activeFlips.front() == i) {
            activeFlips.pop();
            --currentFlips;
        }

        // Effective value at position i considering all active flips
        int effective = nums[i] ^ (currentFlips & 1);

        if (effective == 0) {
            // Need to flip starting at i
            if (i + k > n) {
                return -1; // Not enough room to flip
            }
            ++flips;
            ++currentFlips;
            activeFlips.push(i + k);
        }
    }
    return flips;
}

#include <cassert>
#include <vector>

int minKBitFlips(std::vector<int>& nums, int k);

int main() {
    // Example 1 from LeetCode
    std::vector<int> nums1 = {0,1,0};
    assert(minKBitFlips(nums1, 1) == 2);

    // Example 2
    std::vector<int> nums2 = {1,1,0};
    assert(minKBitFlips(nums2, 2) == -1);

    // Example 3
    std::vector<int> nums3 = {0,0,0,1,0,1,1,0};
    assert(minKBitFlips(nums3, 3) == 3);

    // Already all ones
    std::vector<int> nums4 = {1,1,1,1};
    assert(minKBitFlips(nums4, 2) == 0);

    // k = 1, count zeros
    std::vector<int> nums5 = {1,0,1,0,1};
    assert(minKBitFlips(nums5, 1) == 2);

    // Single element, k=1, zero -> flip once
    std::vector<int> nums6 = {0};
    assert(minKBitFlips(nums6, 1) == 1);

    // Single element, k=1, one -> no flips
    std::vector<int> nums7 = {1};
    assert(minKBitFlips(nums7, 1) == 0);

    // Impossible because k larger than array
    std::vector<int> nums8 = {0,1,0,1};
    assert(minKBitFlips(nums8, 5) == -1);

    // All zeros, k=2, length 4 -> flip first two, then last two
    std::vector<int> nums9 = {0,0,0,0};
    assert(minKBitFlips(nums9, 2) == 2);

    // Edge case: k = n, array zeros -> flip whole array once
    std::vector<int> nums10 = {0,0,0};
    assert(minKBitFlips(nums10, 3) == 1);

    // Edge case: k = n, array ones -> no flip
    std::vector<int> nums11 = {1,0,0,1};
    assert(minKBitFlips(nums11, 4) == -1); // impossible because one zero in middle, k=4 flip flips all, but after flip zeros remain at ends? Actually 1001 -> flip all => 0110 still zeros -> -1

    return 0;
}

// The key observation is a greedy approach: scan the array from left to right. When encountering a position `i` that is still 0 (after accounting for flips that affect it), we must flip the subarray starting at `i` (ending at `i+k-1`) because any flip starting earlier would have already been considered, and no flip starting later can affect `i`. To efficiently know whether a position is currently 0 or 1 without explicitly flipping the whole array, maintain a running `flipCount` representing how many flips currently affect the current index. At index `i`, the effective value is `nums[i] XOR (flipCount % 2)`. If the result is 0, we perform a flip: increment the answer, toggle `flipCount`, and record that a flip ends at `i+k` by storing a marker in an auxiliary array (or a deque). When we pass `i+k`, we decrement `flipCount` because that flip no longer affects future indices. If during this process we ever attempt to flip but `i+k` exceeds the array size, it is impossible and we return -1. Edge cases: all bits already 1 → answer 0; k=1 → answer is count of zeros; impossible scenarios when a zero is too close to the end. Time complexity is O(n) with O(n) auxiliary space for the end markers (or O(k) using a deque). Space can be reduced to O(k) by using a queue, but O(n) is acceptable and simpler.
