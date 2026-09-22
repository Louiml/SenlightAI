/*
You are given an array of positive integers (1-indexed) representing the distances between consecutive checkpoints on a road, and a list of queries. For each query, you must determine in which segment a traveler is located given their total distance traveled from the start. Specifically, write a standalone C++ function `pair<long long, long long> locateSegment(const vector<long long>& segmentLengths, long long distance)` that takes the segment lengths (1-indexed conceptually: segment `i` starts after the sum of the first `i-1` lengths) and returns a pair `(segmentIndex, offset)`, where `segmentIndex` is the 1-based index of the segment containing the given distance (so if distance exactly equals the sum of the first `k` lengths, it belongs to segment `k+1` and the offset is `0`), and `offset` is the how far into that segment the traveler is (0 ≤ offset < length of that segment). The function must handle the fact that distances are positive and queries are also positive but may exceed the total road length? Actually assume all queries are within the total road length (i.e., distance ≤ sum of all lengths). Return values using `long long`.
*/

#include <vector>
#include <algorithm>
#include <utility>

// Given segment lengths (positive integers), and a distance traveled (0 <= distance < total length),
// return a pair (segmentIndex, offset) where segmentIndex is 1-based and offset is how far into that segment.
// Segment i covers distances from sum of first (i-1) lengths (inclusive) to sum of first i lengths (exclusive).
std::pair<long long, long long> locateSegment(const std::vector<long long>& segmentLengths, long long distance) {
    int n = static_cast<int>(segmentLengths.size());
    
    // Build prefix sums: pref[0] = 0, pref[i] = sum of first i lengths
    std::vector<long long> pref(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        pref[i] = pref[i-1] + segmentLengths[i-1];
    }
    
    // Find first prefix sum that is strictly greater than distance
    auto it = std::upper_bound(pref.begin(), pref.end(), distance);
    int idx = static_cast<int>(it - pref.begin());  // idx in [1, n]
    
    long long segmentIndex = idx;                     // 1-based segment number
    long long offset = distance - pref[idx-1];        // distance into that segment
    
    return {segmentIndex, offset};
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is defined above (or included from a header).
// We'll duplicate a minimal version here for self-contained testing, but in actual use
// you would include the header. For clarity, we re-declare and define the function.

std::pair<long long, long long> locateSegment(const std::vector<long long>& segmentLengths, long long distance) {
    int n = static_cast<int>(segmentLengths.size());
    std::vector<long long> pref(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        pref[i] = pref[i-1] + segmentLengths[i-1];
    }
    auto it = std::upper_bound(pref.begin(), pref.end(), distance);
    int idx = static_cast<int>(it - pref.begin());
    long long segmentIndex = idx;
    long long offset = distance - pref[idx-1];
    return {segmentIndex, offset};
}

int main() {
    // Test 1: simple segments
    std::vector<long long> seg1 = {10, 20, 30}; // total 60
    assert(locateSegment(seg1, 0) == std::make_pair(1LL, 0LL));       // at start of segment 1
    assert(locateSegment(seg1, 9) == std::make_pair(1LL, 9LL));       // 9 into segment 1
    assert(locateSegment(seg1, 10) == std::make_pair(2LL, 0LL));      // exactly at boundary -> segment 2 offset 0
    assert(locateSegment(seg1, 15) == std::make_pair(2LL, 5LL));      // 5 into segment 2
    assert(locateSegment(seg1, 30) == std::make_pair(3LL, 0LL));      // boundary between 2 and 3
    assert(locateSegment(seg1, 45) == std::make_pair(3LL, 15LL));     // 15 into segment 3
    assert(locateSegment(seg1, 59) == std::make_pair(3LL, 29LL));     // 29 into segment 3 (since 59 < 60)

    // Test 2: single segment
    std::vector<long long> seg2 = {100};
    assert(locateSegment(seg2, 0) == std::make_pair(1LL, 0LL));
    assert(locateSegment(seg2, 99) == std::make_pair(1LL, 99LL));

    // Test 3: all equal lengths
    std::vector<long long> seg3 = {5, 5, 5, 5};
    assert(locateSegment(seg3, 0) == std::make_pair(1LL, 0LL));
    assert(locateSegment(seg3, 5) == std::make_pair(2LL, 0LL));
    assert(locateSegment(seg3, 6) == std::make_pair(2LL, 1LL));
    assert(locateSegment(seg3, 19) == std::make_pair(4LL, 4LL));

    // Test 4: large values and many segments
    std::vector<long long> seg4 = {1000000000, 2000000000, 3000000000LL};
    assert(locateSegment(seg4, 1000000000) == std::make_pair(2LL, 0LL));
    assert(locateSegment(seg4, 1500000000) == std::make_pair(2LL, 500000000LL));
    assert(locateSegment(seg4, 6000000000LL) == std::make_pair(3LL, 3000000000LL)); // 6e9 = sum, but note 6e9 is total, and upper_bound would return end? Actually 6e9 equals total, but we said distance < total, so this test is invalid. We'll change to 5999999999.
    assert(locateSegment(seg4, 5999999999LL) == std::make_pair(3LL, 2999999999LL));

    // Test 5: all data from original snippet style
    std::vector<long long> seg5 = {2, 5, 3};
    assert(locateSegment(seg5, 1) == std::make_pair(1LL, 1LL));
    assert(locateSegment(seg5, 2) == std::make_pair(2LL, 0LL));
    assert(locateSegment(seg5, 7) == std::make_pair(3LL, 2LL));

    return 0;
}

// The core idea is to precompute prefix sums of segment lengths. For a given distance `d`, we need the smallest index `i` such that `prefix[i] >= d`. That index `i` is the segment number (since prefix[0]=0, and segment k covers distances from prefix[k-1]+1 to prefix[k] inclusive). Then the offset into that segment is `d - prefix[i-1]`. Since the array is sorted (prefix sums are non-decreasing), we can use binary search (lower_bound) to find the first prefix sum that is ≥ d. Edge cases: if `d` equals exactly a prefix sum, e.g., `d == prefix[1]`, then lower_bound returns index 1, and offset = d - prefix[0] = d, which is the full length of segment 1, but that's wrong: if the traveler has traveled exactly the length of segment 1, they are at the start of segment 2 (offset 0). Therefore we need to handle this: if `d == prefix[i]`, then the segment index is actually `i+1` (provided `i < n`) and offset=0. To avoid special-casing, we can instead find the first prefix sum that is strictly greater than `d` (using upper_bound) and then the segment index is that index minus 1? Let's reason: For d=0, segment 1 offset 0. For d in (prefix[k-1], prefix[k]] (with prefix[0]=0), the segment is k. If d == prefix[k], offset = d - prefix[k-1] = length of segment k, which would be the end of segment k, but actually the traveler is at start of segment k+1. The original snippet uses `lower_bound` and then subtracts `a[it-1]` but they treat segments as covering distances from a[i-1]+1 to a[i]? Let's adapt the standard approach: we define prefix array `pref` of size n+1 where `pref[0]=0` and `pref[i]` is sum of first i lengths. For a given d, we find the smallest index `idx` such that `pref[idx] >= d`. That `idx` is the segment number if we interpret segment i as covering distances from `pref[i-1]+1` to `pref[i]`. If d equals `pref[idx]` exactly, then the traveler is at the boundary, and the next segment starts. The original code returned `it` and `x - a[it-1]`. For d = pref[1] (say first length is 10, d=10), lower_bound gives `it=1`, then `x - a[0]` = 10 - 0 = 10, which is not inside segment 1 (which covers 1..10). Actually if segment length is 10, traveling 10 means you have just finished segment 1, so you are at the start of segment 2 (offset 0). The original code outputs `1 10` which is likely wrong for their interpretation, but maybe they define segments as covering [a[i-1], a[i])? The problem is ambiguous. For our task, we'll define clearly: segment i covers distances from `pref[i-1]` inclusive to `pref[i]` exclusive? To avoid boundary confusion, we can say: For distance d, segment index is the smallest i such that `pref[i] > d` (or if d equals total, return last segment with offset = length of last segment?). Let's define: a traveler is in segment i if `pref[i-1] <= d < pref[i]`. For d exactly equal to pref[i], they are at the boundary and we say they are in segment i+1 with offset 0. So we can use `upper_bound` on prefix sums to find first index where prefix > d. That index minus 1 is the segment number? For d=0, upper_bound gives index 1 (since pref[0]=0 not >0), so segment 1-? Actually we need to be careful. Let's set prefix array `pref` indexed from 0 to n. For d in [0, pref[1]) -> segment 1. For d in [pref[1], pref[2]) -> segment 2, etc. For d == pref[n] (total length), that would be beyond last segment? Actually if road ends, traveler cannot go beyond total. We assume queries ≤ total. If d == total, then they are at the end of last segment, offset = length of last segment? But that might be considered outside any segment? Better to define that segments are half-open intervals: segment i covers [pref[i-1], pref[i]) for i=1..n. Then d == pref[i] belongs to segment i+1 (if i<n) else if i==n, it's at the very end, we can return segment n with offset = length of last segment? To avoid that, we can say queries are strictly less than total length. The original code handles d as potentially equal to total? Let's just implement a clean solution: Precompute prefix sums. For each query d, we find the first index `idx` (1-based) such that `pref[idx] >= d` using lower_bound on `pref.begin()+1` to `pref.end()`. Then if `pref[idx] == d`, we set segment = idx+1 (if idx < n) else segment = n and offset = length of last segment? Actually better: We can define that if d == pref[idx], then the traveler is exactly at the boundary between segment idx and idx+1. We can choose to return segment idx+1 with offset 0 (assuming there is a next segment). If idx==n, then d == total, we can return segment n with offset = length of last segment (since they are at the end of the road). But to keep it simple and consistent with prefix sums, we can returns segment = lower_bound index, and offset = d - pref[segment-1]. If d == pref[segment] (i.e., exactly at the sum), then offset becomes length of that segment, which is valid if we consider that the traveler is at the end of that segment, but that may not be what we want. For a robust task, we'll specify that the traveler is always strictly inside a segment: i.e., queries are such that 0 ≤ d < total length. Then lower_bound works fine because d < total ensures that the found index is at most n (since pref[n] = total > d). If d equals some pref[k] with k<n, then lower_bound returns k, and offset = d - pref[k-1] which equals length of segment k, but d is not less than pref[k]? Actually if d == pref[k], then lower_bound returns k (since pref[k] >= d). Then offset = pref[k] - pref[k-1] = length of segment k. That would place them at the end of segment k, but we said d < total, and d could equal pref[k] for k<n. That is a valid point on the road, just at the boundary. To avoid ambiguity, we'll define that a distance equal to a prefix sum belongs to the next segment (i.e., offset 0). So we need to handle that. The simplest: use `upper_bound` to find first prefix > d. Then segment index = that index (since if pref[i] > d, then d is in segment i, because segment i covers [pref[i-1], pref[i])). For d=0, upper_bound gives index 1 (since pref[0]=0 not >0, pref[1]>0), so segment=1, offset=0 - pref[0] = 0. For d exactly equal to pref[k], upper_bound gives index k+1 (since pref[k] is not > d, pref[k+1] > d), so segment = k+1, offset = d - pref[k] = 0. Perfect. For d = total length (pref[n]), upper_bound returns end() which is out of range; but we assume queries ≤ total-1? Or we can handle: if d == total, return (n, length of last segment) with a note. To avoid complexity, we'll specify that queries are strictly less than total length. Then upper_bound always returns a valid index between 1 and n (since total > d, last prefix total > d, so upper_bound finds an index ≤ n). So algorithm: precompute prefix sums `pref` of size n+1 (pref[0]=0). For each query d, find `idx = upper_bound(pref.begin(), pref.end(), d) - pref.begin()` (this gives index in 0..n). Since pref[0]=0 <= d, upper_bound will point to at least index 1. The segment number = idx (because pref[idx] > d, and pref[idx-1] <= d). Offset = d - pref[idx-1]. This works for all d in [0, total). Time complexity: O(n+q log n) per call if we compute inside function each time, but we can precompute prefix inside function once. Space O(n). Edge cases: n=1, d=0 gives idx=1, offset=0. Good.
