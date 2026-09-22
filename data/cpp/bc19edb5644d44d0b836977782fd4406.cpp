Given a sorted list of positive integers representing positions along a line, and a total length `m`, write a C++ function `maxCoverage` that takes the number of integers `n`, the total length `m`, and a vector of integers `positions` (1-indexed, strictly increasing, each between 1 and m-1). The function must compute and return the maximum possible total length that can be covered by a single continuous segment that starts at position 0, ends at position m, but may have one "gap" (a skipped interval) of length 1 or more, provided the gap is placed exactly at one of the given positions (i.e., the segment from 0 to m is divided into two contiguous parts, and we can skip exactly one contiguous interval starting at one of the given positions). More precisely, imagine you want to cover as much of the line [0,m] as possible by walking from 0 to m, but you are allowed to "jump over" (skip) exactly one contiguous interval of length ≥1 whose left endpoint is one of the given positions. You must still cover the intervals before and after the jump. The total covered length is the sum of the lengths of the two covered intervals. Determine the maximum possible covered length. The input positions are strictly increasing and all lie strictly between 0 and m. The function should return an integer representing the maximum total covered length.

// The line is split into n+1 intervals: interval 0 = [0, positions[0]], interval i for 1 ≤ i ≤ n-1 = [positions[i-1], positions[i]], and interval n = [positions[n-1], m]. The default pattern covers intervals with even index (0,2,4,...). The key is to compute prefix sums of the lengths of covered even-indexed intervals. Let `even_sum[i]` be the total length of even-indexed intervals among the first i intervals (i.e., indices 0..i-1). Then the default total covered is `even_sum[n+1]`, where `even_sum[n+1]` also includes the last interval if its index n is even. For a pivot at `positions[i-1]` (which corresponds to interval index i-1 being the last interval before the flip), the covered length is the even-indexed sum before that pivot (`even_sum[i]`) plus the total length from the pivot to m (`m - positions[i-1]`) minus the even-indexed sum after the pivot (`even_sum[n+1] - even_sum[i]`) because flipping means we cover the odd-indexed intervals there instead, and then subtract 1 for the mandatory jump. The answer is the maximum of the default and all pivot candidates. Edge cases: n=0 returns m; if n>0, the pivot may be any position, and the candidate can be computed in O(1) per pivot after prefix sums are built. The time complexity is O(n) for building prefix sums and O(n) for scanning pivots, so O(n) overall; space complexity is O(n) for the prefix sum array (can be reduced to O(1) by maintaining running values, but O(n) is clear and acceptable). The use of 64-bit integers avoids overflow.

#include <vector>
#include <algorithm>

// Computes the maximum total covered length given n marked positions on a segment [0, m].
// positions must be strictly increasing and each in (0, m).
long long maxCoverage(int n, long long m, const std::vector<long long>& positions) {
    if (n == 0) {
        return m;
    }
    // even_sum[i] = total length of even-indexed intervals among intervals 0..i-1.
    std::vector<long long> even_sum(n + 2, 0);
    for (int i = 1; i <= n; ++i) {
        even_sum[i] = even_sum[i - 1];
        long long interval_len = positions[i - 1] - (i == 1 ? 0 : positions[i - 2]);
        // Interval index is (i-1); add if it is even.
        if ((i - 1) % 2 == 0) {
            even_sum[i] += interval_len;
        }
    }
    // Add the last interval [positions[n-1], m] if its index n is even.
    even_sum[n + 1] = even_sum[n];
    if (n % 2 == 0) {
        even_sum[n + 1] += (m - positions[n - 1]);
    }
    
    long long best = even_sum[n + 1];  // No pivot.
    for (int i = 1; i <= n; ++i) {
        long long pos = positions[i - 1];
        // Covered before pivot: even_sum[i].
        // After pivot, we cover odd intervals from pos to m: (m - pos) - (even_sum[n+1] - even_sum[i]).
        long long candidate = even_sum[i] + (m - pos) - (even_sum[n + 1] - even_sum[i]) - 1;
        best = std::max(best, candidate);
    }
    return best;
}

#include <cassert>
#include <vector>

// Declaration of the function under test (assumed in solution).
long long maxCoverage(int n, long long m, const std::vector<long long>& positions);

int main() {
    // n=0 covers whole line.
    assert(maxCoverage(0, 10, {}) == 10);
    // Single position, pivot gives m-1.
    assert(maxCoverage(1, 5, {2}) == 4);
    // Example from analysis: n=2, positions [1,3], m=5 → best is default 3.
    assert(maxCoverage(2, 5, {1, 3}) == 3);
    // Example with n=3, positions [2,5,8], m=10 → best is 6.
    assert(maxCoverage(3, 10, {2, 5, 8}) == 6);
    // Case where pivot improves over default.
    // Intervals: [0,1] len1, [1,4] len3, [4,6] len2, [6,10] len4
    // Default even: len0+len2 = 1+2=3.
    // Pivot at 1: before=1, after odd from [1,4] and [6,10]? Indices: 1 (odd) len3, 2 (even), 3 (odd) len4 => sum=7, total=1+7-1=7.
    // Pivot at 4: before even up to index1? even indices <2: only 0 len1, after odd from index2: index2 even, index3 odd len4 => sum=4, total=1+4-1=4.
    // Pivot at 6: before even indices 0,2 sum=1+2=3, after odd from index3: len4, total=3+4-1=6.
    // So best=7.
    assert(maxCoverage(3, 10, {1, 4, 6}) == 7);
    // Large values with long long.
    assert(maxCoverage(2, 1000000000LL, {1, 999999999LL}) == 999999999LL);
    return 0;
}
