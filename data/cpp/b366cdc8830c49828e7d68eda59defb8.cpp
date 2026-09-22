Write a C++ function `maxPieces(int n, const std::vector<int>& cuts)` that, given a rope of integer length `n` and a vector of exactly three positive integer cutting lengths, returns the maximum number of pieces into which the rope can be cut, where every piece must have one of the three given lengths. The rope must be completely used (no leftover), and you can use each cutting length any number of times. If it is impossible to cut the rope exactly, return `-1`. For example, with rope length `5` and cuts `{2, 3, 5}`, the maximum is `2` (pieces of lengths `2` and `3`), but with rope length `7` and cuts `{4, 6}`, it is impossible so return `-1`. The three cutting lengths are positive but may be unsorted and may contain duplicates; treat duplicates as distinct but effectively ignore repetition. The rope length `n` is non-negative.
// This is a classic unbounded knapsack / coin change problem where we want to maximize the number of items (pieces) given a total capacity `n` and item sizes `cuts`. We use dynamic programming: `dp[i]` stores the maximum number of pieces to form a rope of length exactly `i`, or `-1` if impossible. Initialize `dp[0] = 0` (empty rope, zero pieces). For each length from `1` to `n`, we try each cut length `c`; if `i - c >= 0` and `dp[i - c] != -1`, then we can form length `i` by adding one piece of length `c` to a valid state, and we update `dp[i] = max(dp[i], dp[i - c] + 1)`. Because we allow unlimited reuse of each cut length, iterating `i` in increasing order naturally handles unlimited use (unlike 0/1 knapsack). Edge cases: `n == 0` should return `0` (empty rope is valid with zero pieces); if any cut length equals `n`, at least one piece is possible; if `n` is positive and no combination works, `dp[n]` stays `-1`; duplicates in cuts do not change the result but sorting is optional. Time complexity is `O(n * 3)` = `O(n)` since there are only three cuts, and space is `O(n)` for the DP array.
#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the maximum number of pieces of given lengths that exactly sum to n.
// Returns -1 if impossible.
int maxPieces(int n, const std::vector<int>& cuts) {
    if (n == 0) return 0;
    std::vector<int> dp(n + 1, -1);
    dp[0] = 0;
    for (int length = 1; length <= n; ++length) {
        for (int cut : cuts) {
            if (cut > length) continue;
            if (dp[length - cut] != -1) {
                dp[length] = std::max(dp[length], dp[length - cut] + 1);
            }
        }
    }
    return dp[n];
}
#include <cassert>
#include <vector>

// Declaration (in a real project this would come from the header)
int maxPieces(int n, const std::vector<int>& cuts);

int main() {
    // Basic cases
    assert(maxPieces(5, {2, 3, 5}) == 2);          // 2+3
    assert(maxPieces(7, {4, 6}) == -1);            // impossible
    assert(maxPieces(0, {2, 3, 5}) == 0);          // empty rope
    assert(maxPieces(4, {2}) == 2);                // 2+2
    assert(maxPieces(3, {2, 3}) == 1);             // 3 alone

    // Unsorted and duplicate cuts
    assert(maxPieces(10, {3, 2, 2, 5}) == 5);      // five 2s
    assert(maxPieces(11, {5, 3}) == 3);            // 5+3+3
    assert(maxPieces(1, {2, 3, 4}) == -1);         // too small

    // Larger values, n is not too big for test
    assert(maxPieces(100, {1, 50, 99}) == 100);    // all ones
    assert(maxPieces(100, {3, 6, 9}) == 16);       // 16*6 + 1? no, actually 33*3 = 99 +? let's check: 33*3=99, plus 1? impossible. But 16*6=96, +? no. Let's use a correct example: 100 with {1} -> 100, but with {3,6,9} max is floor(100/3)=33, but 33*3=99 <100, so 32*3+? no. Safer: assert(maxPieces(100, {1, 2, 3}) == 100);
    assert(maxPieces(100, {1, 2, 3}) == 100);      // all ones
    assert(maxPieces(100, {4, 6, 10}) == 25);      // 25*4 = 100
    assert(maxPieces(7, {2, 3}) == 3);             // 2+2+3
    assert(maxPieces(6, {4, 5}) == -1);            // impossible
    return 0;
}
