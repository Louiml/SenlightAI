You are given a vector of non-overlapping integer intervals `tiles`, where each interval `[l, r]` represents a contiguous range of white tiles on a number line (inclusive, with `l <= r`), and an integer `carpetLen` representing the length of a carpet that you can place anywhere on the number line (also inclusive of its endpoints, so the carpet covers positions `[x, x + carpetLen - 1]` for some integer `x`). The intervals are sorted by their left endpoints in increasing order. Write a C++ function `int maximumWhiteTiles(vector<vector<int>>& tiles, int carpetLen)` that returns the maximum total number of white tiles that can be covered by placing the carpet optimally. The carpet can be placed at any integer position, not necessarily aligned with interval boundaries, and you may assume all interval endpoints and `carpetLen` are positive integers. The total number of intervals is at most 10^5, and each interval length is at most 10^9. Your solution must be efficient enough for large inputs.
// The problem is a classic sliding window over the sorted intervals. Since the intervals are non‑overlapping and sorted by left endpoints, we can use two pointers to maintain the set of intervals that intersect with the current carpet position. The key observation: an optimal placement always has its left edge aligned with the left endpoint of one of the intervals (or at least we can shift it to such a position without losing coverage). Therefore, we iterate over each interval as a potential starting point for the carpet.
//
// For each left pointer `i`, we advance a right pointer `j` while the interval `tiles[j][0]` (its left endpoint) is within `tiles[i][0] + carpetLen - 1`. For the intervals fully inside the carpet (those with `tiles[j][1] <= tiles[i][0] + carpetLen - 1`), we add their full length to a running sum. The last partially covered interval (if any) contributes only `(carpetStart + carpetLen - 1) - tiles[j][0] + 1`. We update the answer with the maximum of `currentSum + partial` for each `i`, then subtract the full length of the interval at `i` (since we'll move the left pointer forward) before incrementing `i`.
//
// Edge cases: a single interval longer than the carpet (then the answer is `carpetLen`), intervals that are exactly at the boundary, and when `carpetLen` is very large covering all intervals (the answer is the total sum of all tile counts). Also, because intervals are inclusive, the number of white tiles in `[l, r]` is `r - l + 1`. Time complexity is O(n) with two pointers, and space complexity O(1) besides input storage.
#include <vector>
#include <algorithm>

// Returns the maximum number of white tiles covered by a carpet of given length.
// tiles: sorted non-overlapping intervals [l, r] inclusive, l <= r.
// carpetLen: length of the carpet (positive integer).
int maximumWhiteTiles(std::vector<std::vector<int>>& tiles, int carpetLen) {
    int n = tiles.size();
    long long currentSum = 0;
    int right = 0;
    long long best = 0;

    for (int left = 0; left < n; ++left) {
        long long carpetEnd = static_cast<long long>(tiles[left][0]) + carpetLen - 1;

        // Advance right pointer while the interval's left edge is within carpet range.
        while (right < n && static_cast<long long>(tiles[right][0]) <= carpetEnd) {
            // If this interval is fully inside the carpet, add its whole length.
            if (static_cast<long long>(tiles[right][1]) <= carpetEnd) {
                currentSum += static_cast<long long>(tiles[right][1]) - tiles[right][0] + 1;
                ++right;
            } else {
                break; // This interval is only partially covered; stop adding full lengths.
            }
        }

        // Calculate partial coverage from the interval at 'right' (if exists and overlaps).
        long long partial = 0;
        if (right < n && static_cast<long long>(tiles[right][0]) <= carpetEnd) {
            partial = carpetEnd - tiles[right][0] + 1;
        }

        best = std::max(best, currentSum + partial);

        // Remove the contribution of tiles[left] before moving left pointer.
        // It was either fully added earlier or will be handled by partial in next iterations.
        if (right > left) {
            // tiles[left] was fully added when 'right' passed it.
            currentSum -= static_cast<long long>(tiles[left][1]) - tiles[left][0] + 1;
        } else {
            // right == left: no full intervals were added, nothing to subtract.
        }
        // If right == left, we need to increment right to avoid infinite loop.
        if (right == left) {
            ++right;
        }
    }

    return static_cast<int>(best);
}
#include <cassert>
#include <vector>

// declaration of the function (provided above)
int maximumWhiteTiles(std::vector<std::vector<int>>& tiles, int carpetLen);

int main() {
    // Example 1 from LeetCode
    std::vector<std::vector<int>> tiles1 = {{1,5},{10,11},{12,18},{20,25},{30,32}};
    assert(maximumWhiteTiles(tiles1, 10) == 9);

    // Example 2
    std::vector<std::vector<int>> tiles2 = {{10,11},{1,1}}; // not sorted, but we sort? Actually we assume sorted. We'll sort for test.
    std::sort(tiles2.begin(), tiles2.end());
    assert(maximumWhiteTiles(tiles2, 2) == 2);

    // Single interval longer than carpet
    std::vector<std::vector<int>> tiles3 = {{1,10}};
    assert(maximumWhiteTiles(tiles3, 5) == 5);

    // Carpet covers all intervals
    std::vector<std::vector<int>> tiles4 = {{1,2},{4,5},{8,10}};
    assert(maximumWhiteTiles(tiles4, 20) == 7);

    // Carpet fits exactly one interval
    std::vector<std::vector<int>> tiles5 = {{1,3},{5,7},{9,12}};
    assert(maximumWhiteTiles(tiles5, 3) == 3);

    // Overlap at boundary exactly
    std::vector<std::vector<int>> tiles6 = {{1,5},{6,10}};
    assert(maximumWhiteTiles(tiles6, 5) == 5); // best is cover [1,5] or [6,10] fully

    // Large gaps, carpet covers parts of two intervals
    std::vector<std::vector<int>> tiles7 = {{1,2},{10,15}};
    assert(maximumWhiteTiles(tiles7, 6) == 6); // covers [10,15] fully

    // All intervals of length 1
    std::vector<std::vector<int>> tiles8 = {{1,1},{3,3},{5,5}};
    assert(maximumWhiteTiles(tiles8, 2) == 1); // only one tile fits

    // Carpet length 1 over long interval
    std::vector<std::vector<int>> tiles9 = {{1,100}};
    assert(maximumWhiteTiles(tiles9, 1) == 1);

    // Stress small case with two adjacent intervals
    std::vector<std::vector<int>> tiles10 = {{1,4},{5,9}};
    assert(maximumWhiteTiles(tiles10, 5) == 5); // cover [1,5] or [5,9] fully

    return 0;
}
