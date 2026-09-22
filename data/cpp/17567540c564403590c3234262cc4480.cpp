You are given the number of houses `k` arranged around a circular lake, numbered from 0 to k-1 in clockwise order. There are `n` houses (where 1 ≤ n ≤ k) that have a Christmas tree, and their positions are given in a zero-indexed array `a` of length `n`, sorted in strictly increasing order. The distance between two houses is the shortest clockwise arc length along the circle (i.e., the minimum of the clockwise distance and counterclockwise distance). Write a C++ function `int shortestSegment(int k, const std::vector<int>& a)` that returns the minimum distance between a pair of consecutive tree houses when considering all adjacent pairs in the circular order (including the pair formed by the last and first tree). The input `k` is the total number of houses, and `a` contains distinct positions in the range [0, k-1]. For example, if k=10 and a={0,2,6}, the distances are: between 0 and 2 → 2, between 2 and 6 → 4, and between 6 and 0 (wrapping) → min(4,6)=4, so the answer is 2. The function must handle the case where `n` equals 1 (the sole house has distance 0 to itself). Optimize for large `k` (up to 10^9) but small `n` (up to 10^5). The algorithm must run in O(n log n) or better.
#include <cassert>
#include <vector>

// The solution function is declared above (or included).
// For clarity, we repeat it here in a test harness.

long long shortestSegment(long long k, std::vector<long long> a);

int main() {
    // Basic cases
    assert(shortestSegment(10, {0,2,6}) == 2);
    assert(shortestSegment(10, {0,5}) == 5);
    assert(shortestSegment(10, {2,8}) == 4); // wrap-around shorter
    assert(shortestSegment(100, {0,10,20,90}) == 10);
    assert(shortestSegment(10, {3}) == 0); // single house
    assert(shortestSegment(10, {0}) == 0); // single house at 0
    // Unsorted input (function sorts internally)
    assert(shortestSegment(10, {6,0,2}) == 2);
    // Large k with two points
    assert(shortestSegment(1000000000, {0, 999999999}) == 1);
    // Two points where the wrap gap is smaller
    assert(shortestSegment(100, {10, 90}) == 20); // diff 80, wrap 20, min 20
    // All points equally spaced
    assert(shortestSegment(12, {0,4,8}) == 4);
    // Points close together
    assert(shortestSegment(20, {1,2,19}) == 1); // gaps:1,7, (20-18=2) min=1
    return 0;
}
#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the minimum distance between any two adjacent tree houses on a circular lake of size k.
// a contains positions of n houses (n>=1), each in [0,k-1]. Positions are distinct but not necessarily sorted.
long long shortestSegment(long long k, std::vector<long long> a) {
    int n = static_cast<int>(a.size());
    if (n <= 1) {
        return 0; // single house or empty trivial case
    }
    // Sort positions to find consecutive gaps in circular order.
    std::sort(a.begin(), a.end());

    // Initialize answer with the wrap-around gap from last to first.
    long long ans = k - (a.back() - a.front());

    // Consider all consecutive gaps in sorted order.
    for (int i = 0; i + 1 < n; ++i) {
        ans = std::min(ans, a[i + 1] - a[i]);
    }
    return ans;
}
// The problem is essentially finding the minimum distance between any two adjacent points on a circle of circumference `k`. Since the input array `a` is already sorted in increasing order, the distances between consecutive tree houses in the array are simply `a[i+1] - a[i]` for i from 0 to n-2. The only "wrap-around" distance is from the last element `a[n-1]` to the first element `a[0]` going clockwise, which is `k - (a[n-1] - a[0])` (since the total circle is k, the clockwise gap from a[n-1] to a[0] equals k - (a[n-1] - a[0])). Because the distance is the minimum of the two directions, but since all points lie on a circle, the clockwise gap from one point to the next going around the circle is exactly the arc length between them. For consecutive points in sorted order, the clockwise distance is simply the difference, and that is ≤ k/2? Not necessarily, but the shorter path is the minimum of that difference and k-difference. However, for adjacent points in the sorted order, the clockwise arc from a[i] to a[i+1] is just a[i+1]-a[i], and the counterclockwise arc is k-(a[i+1]-a[i]). The true shortest distance between these two houses is the minimum of these two, but when we consider all pairs that are "adjacent" in the circular sense (i.e., sorted order with wrap-around), the minimum of all these minimums is exactly the answer. But note: The pair (a[n-1], a[0]) is also adjacent on the circle. For that pair, the clockwise distance is k - (a[n-1] - a[0])? Actually, a[0] > a[n-1]? No, a[0] < a[n-1], so the clockwise distance from a[n-1] to a[0] going past the end is k - (a[n-1] - a[0]). The counterclockwise distance is (a[n-1] - a[0]). So the minimum for that pair is min(k - (a[n-1] - a[0]), a[n-1] - a[0]).
//
// The original code snippet incorrectly assumes the input is sorted? Actually the problem statement says "sorted in strictly increasing order" so we can rely on that. But the snippet does not sort; it takes input as given. To be safe, we should sort the array ourselves because the problem might not guarantee that in the original snippet context? The snippet's input is from cin and assumes the array is already sorted? The snippet uses a[n-1]-a[0] as initial ans, which only works if sorted. To make the task self-contained, we'll state that the input array is sorted. But our function should ideally handle unsorted input by sorting internally, at O(n log n) cost. That is fine.
//
// Algorithm: If n == 0? The problem says n ≥ 1. If n == 1, the only pair is the house with itself, distance 0. So return 0. Otherwise, sort the array (in case it is not sorted). Then compute the minimum of all differences a[i+1]-a[i] for i=0..n-2, and also the wrap-around distance: k - (a[n-1] - a[0]). However, careful: the actual shortest distance between two given houses is min(clockwise_gap, counterclockwise_gap). For adjacent pairs in sorted order, the clockwise gap is simply the difference, and the counterclockwise gap is k - difference. But the answer we want is the minimum over all pairs of that min. However, since we consider all adjacent pairs on the circle, the minimum of the clockwise gaps (differences) is actually the minimum distance because the counterclockwise gap for a pair is the sum of all other gaps, which is typically larger unless there are only two points. Let's test: For two points, say a={0,5}, k=10. Differences: 5, wrap: 10-5=5. min(5,5)=5. If we just take min of differences and wrap, we get 5. Good. For three points: k=10, a={0,3,7}. Differences: 3,4; wrap: 10-7=3? Actually wrap = k - (7-0)=3. min of {3,4,3} = 3. But the actual shortest distance between 7 and 0 is min(3,7)=3, correct. For a={0,4,6}, k=10: diff:4,2; wrap=10-6=4, min=2. That's correct. So the simple formula works: the answer is the minimum of all adjacent differences (including wrap). Why? Because for any pair of points that are adjacent on the circle (i.e., no other point lies on the shorter arc between them), the shorter distance is exactly the clockwise gap that avoids other points. The set of all such adjacent pairs corresponds exactly to consecutive index pairs plus the wrap. For each such pair, the true shortest distance is min(diff, k-diff). However, note that for consecutive indices, diff is always ≤ k/2? Not necessarily, but if diff > k/2, then k-diff < diff, so the shorter path would be the other direction, but then that pair would not be adjacent on the shorter side? Actually if diff > k/2, then the other direction is shorter, and that other direction would contain no other tree? Wait, if diff > k/2, then the arc from a[i] to a[i+1] going clockwise is long, but the counterclockwise arc passes through all other points? Actually, for consecutive indices in sorted order, there is no other point between them along the clockwise arc (since sorted). So the clockwise arc is the empty arc. The counterclockwise arc is complement, which contains all other points. So the shorter distance between these two points is indeed min(diff, k-diff). But the minimum over all such pairs might be smaller than the minimum of the diffs? For example, if n=2, a={2,8}, k=10: diff=6, wrap=4. min(6,4)=4, but the wrap is 4, which is k - (8-2)=4. So the answer is 4. But if we take min of diffs (6) and wrap (4), we get 4. So that's fine. Another example: n=4, k=100, a={0,10,20,90}. Diffs:10,10,70; wrap=10 (100-90). min=10. But consider pair (20,90) diff=70, min(70,30)=30, but 30 is not in our list. However, the pair (20,90) is not adjacent on the shorter arc because the shorter arc from 90 to 20 goes through 0, so those two are not adjacent in the circular order. The actual adjacent pairs are: (0,10) dist 10, (10,20) dist 10, (20,90) courteous distance is min(70,30)=30, but is that correct? Let's visualize: points 0,10,20,90 on circle of 100. The circular order clockwise: 0,10,20,90. Adjacent pairs: (0,10) diff 10, (10,20) diff 10, (20,90) diff 70 (clockwise), (90,0) diff 10 (since 100-90=10). The shortest distance between 20 and 90 is indeed min(70,30)=30, but 30 is not adjacent in circular order because between 90 and 20 going counterclockwise (short way) there is 0, but that arc length is 30 from 90 to 20 counterclockwise, passing through 0? Actually 90 to 20 counterclockwise: go from 90 down to 20, that passes through 0? No, 90 to 0 is 10, 0 to 20 is 20, total 30. So the points 0 and 10 lie on that arc. So (20,90) are not adjacent on the shorter side, but they are adjacent on the longer side. The true shortest distance between 20 and 90 is 30, but that distance is split into two segments (90-0 and 0-20). The minimum distance between any pair of adjacent houses in the circular sense (i.e., no other tree between them on either arc) is actually the minimum of the gaps between consecutive trees in sorted order, where each gap is the clockwise arc length. Because for any two trees, if there is another tree on the shorter arc, then the pair is not adjacent, and the distance is split. The minimal distance among all pairs will always be achieved by some adjacent pair in the sorted order (including wrap). So the answer is simply the minimum of all differences a[i+1]-a[i] for i=0..n-2, plus the wrap distance k - (a[n-1]-a[0]). In the example, that gives min(10,10,70,10)=10, which is correct. The pair (20,90) has distance 30, not minimal. So the formula holds. But wait, in the case n=2, a={2,8}, k=10: differences: 6, wrap=4, min=4. That is indeed the shortest distance between the two points, which is 4. Good. So the solution is simply: if n==1 return 0. Else, sort a, compute initial ans = k - (a[n-1]-a[0]) (the wrap gap), then for i=0..n-2, ans = min(ans, a[i+1]-a[i]). Return ans.
//
// Edge cases: n=1 -> return 0. k can be huge, differences can be up to k-1, use long long to avoid overflow if k up to 1e9 and n up to 1e5, but k and a[i] fit in int? Use long long for safety. Sorting takes O(n log n). Time complexity O(n log n), space O(n) for the vector (if we copy). We can take a const reference and copy into a local vector because we need to sort. But if input is guaranteed sorted, we could avoid sorting. However, the task says the array is sorted, so we can assume that. But to make the function robust, we'll sort anyway; O(n log n) is fine.
//
// Important edge case: If the array is not sorted, we must sort. Also if n=0? Not allowed per constraints.
//
// Test cases: simple ones, wrap-around case, duplicate? Distinct positions, so no duplicates.
