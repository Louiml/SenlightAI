// You are given an integer `n` representing a garden strip from position `0` to position `n`, and a vector `ranges` of size `n+1` where `ranges[i]` denotes the reach of a water tap located at position `i`. A tap at position `i` can water the interval `[max(0, i - ranges[i]), min(n, i + ranges[i])]` (inclusive on both ends). Write a C++ function `int minTaps(int n, const std::vector<int>& ranges)` that returns the minimum number of taps that must be opened so that every point along the garden from `0` to `n` is watered. If it is impossible to water the entire garden, return `-1`. You may open any subset of taps, each tap can be opened at most once, and watering intervals can overlap. The function must handle `n >= 0`, `ranges.size() == n+1`, and `ranges[i] >= 0`.

int main() {
    // Example 1: n=5, ranges = [3,4,1,1,0,0]
    // Tap0 covers [0,3], tap1 covers [0,5] -> 1 tap suffices
    assert(minTaps(5, std::vector<int>{3,4,1,1,0,0}) == 1);

    // Example 2: n=3, ranges = [0,0,0,0] -> no tap can water beyond its own point, impossible
    assert(minTaps(3, std::vector<int>{0,0,0,0}) == -1);

    // Example 3: n=7, ranges = [1,2,3,4,1,1,1,1]
    // Tap0 covers [0,1], tap1 covers [0,3], tap2 covers [0,5], tap3 covers [0,7] -> 1 tap
    assert(minTaps(7, std::vector<int>{1,2,3,4,1,1,1,1}) == 1);

    // Example 4: n=4, ranges = [0,1,0,1,0]
    // Tap0 covers [0,0], tap1 covers [0,2], tap2 covers [2,2], tap3 covers [2,4], tap4 covers [4,4]
    // Need tap1 and tap3 -> 2 taps
    assert(minTaps(4, std::vector<int>{0,1,0,1,0}) == 2);

    // Example 5: n=0, single point garden, no length needs watering -> 0 taps
    assert(minTaps(0, std::vector<int>{0}) == 0);

    // Example 6: n=5, ranges = [0,0,0,0,0,0] -> impossible
    assert(minTaps(5, std::vector<int>{0,0,0,0,0,0}) == -1);

    // Example 7: n=2, ranges = [2,0,0] -> tap0 covers whole garden? start=0, end=min(2,0+2)=2 -> 1 tap
    assert(minTaps(2, std::vector<int>{2,0,0}) == 1);

    // Example 8: n=2, ranges = [1, (can't fit, size must be n+1=3) 1,1] -> tap0 covers [0,1], tap1 covers [0,2], tap2 covers [1,2] -> 1 tap from tap1
    assert(minTaps(2, std::vector<int>{1,1,1}) == 1);

    // Example 9: n=3, ranges = [2,1,0,1]
    // tap0 covers [0,2], tap1 covers [0,2], tap2 covers [2,2], tap3 covers [2,3] -> tap0 and tap3, but tap1 also covers [0,2], need tap0 or tap1 plus tap3 = 2
    assert(minTaps(3, std::vector<int>{2,1,0,1}) == 2);

    // Example 10: n=1, ranges = [1,1] -> both cover whole garden, 1 tap
    assert(minTaps(1, std::vector<int>{1,1}) == 1);

    return 0;
}

#include <vector>
#include <algorithm>
#include <cassert>

// Returns the minimum number of taps needed to water the entire garden [0, n].
// Returns -1 if it is impossible.
int minTaps(int n, const std::vector<int>& ranges) {
    // bestEnd[start] = the farthest reachable end among all taps starting at 'start'
    std::vector<int> bestEnd(n + 1, 0);
    for (int i = 0; i <= n; ++i) {
        int start = std::max(0, i - ranges[i]);
        int end = std::min(n, i + ranges[i]);
        bestEnd[start] = std::max(bestEnd[start], end);
    }

    int taps = 0;
    int maxReachFromSelected = 0;   // how far the already selected taps cover
    int maxReachFromAny = 0;        // farthest reachable by any tap seen so far

    for (int i = 0; i <= n; ++i) {
        // If the current point is not reachable by any tap we have seen,
        // then there is a gap that cannot be covered.
        if (i > maxReachFromAny) {
            return -1;
        }
        // If we have reached the limit of the already selected taps,
        // we must select the tap that extends the farthest among those seen.
        if (i > maxReachFromSelected) {
            ++taps;
            maxReachFromSelected = maxReachFromAny;
        }
        // Update the farthest reachable point from any tap starting at or before i
        maxReachFromAny = std::max(maxReachFromAny, bestEnd[i]);
    }
    return taps;
}

// The problem is a classic interval coverage problem on a line. The key insight is to first transform the input into a structure that allows efficient greedy covering. For each tap at position `i`, compute its leftmost reachable point `start = max(0, i - ranges[i])` and its rightmost point `end = min(n, i + ranges[i])`. Since multiple taps may cover the same starting position, we keep the one with the farthest reach by storing in an array `bestEnd[start]` the maximum `end` among all taps starting at that exact position. After constructing `bestEnd` (size `n+1`), we iterate from left to right over positions `0` to `n`. Maintain `currEnd` as the farthest point reachable by the taps already selected, and `maxEnd` as the farthest point reachable by any tap whose start is at or before the current position. When we encounter a position `i` that is beyond `maxEnd`, it means there is a gap that no tap can cover, so we return `-1`. Whenever `i` is beyond `currEnd`, we are forced to open a new tap because the coverage from previously selected taps has ended; we increment the tap count and set `currEnd = maxEnd`. Then we update `maxEnd` with `bestEnd[i]`. At the end, after processing all positions, `taps` is the minimum count. Important edge cases: `n = 0` (single point, always possible with 0 taps, since no length needs watering; but the loop will handle it), taps with zero range only water their own point, and taps that extend beyond boundaries are clipped. The greedy is correct because at each step we choose the tap that extends the farthest among all those that start before or at the current coverage limit. Time complexity is O(n) for building `bestEnd` plus O(n) for the greedy scan, total O(n). Space complexity is O(n) for the `bestEnd` array.
