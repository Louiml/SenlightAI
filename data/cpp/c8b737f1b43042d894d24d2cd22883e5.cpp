Given an array of integers where some positions are marked as unknown with the value `-1`, write a C++ function `replaceUnknowns` that takes a vector of integers as input and returns a pair `(maxAbsoluteDifference, replacementValue)` such that: all `-1` entries are replaced by a single integer `k` chosen to minimize the maximum absolute difference between any two adjacent elements (including newly replaced entries). If there are no `-1` entries, then `k` can be any integer (choose `42` for consistency). The function must compute the minimal possible maximum absolute adjacent difference after replacement, and return that minimal difference along with the chosen `k`. If multiple `k` values achieve the same minimal difference, any valid `k` is acceptable; however, the reference solution uses the midpoint of the feasible range. The input vector may be non-empty, and values are in the range `[0, 10^9]`. Note: `-1` only appears as a placeholder, not as a real value.

// The key idea is to determine the constraints on `k` imposed by the known neighbors of `-1` entries. For each `-1`, its known neighbors (if any) must be within distance `m` of `k` after replacement. If we denote the set of all known values that are adjacent to at least one `-1` as `S`, then for any `k`, the maximum difference between `k` and any element in `S` is `max(|k - min(S)|, |k - max(S)|)`. Let `lo = min(S)`, `hi = max(S)`. To minimize the maximum distance from `k` to all points in `S`, choose `k` as the middle point of the interval `[lo, hi]`, i.e., `k = (lo + hi) / 2` (integer division, truncating toward zero for negatives, but here values are non-negative, so fine). The minimal possible maximum distance to the interval endpoints is `max((hi - lo + 1)/2, something)` actually more precisely the maximum distance from `k` to `lo` and `hi` is `max(k - lo, hi - k)`, which is minimized at `ceil((hi - lo)/2)`. Since `k` is integer, the minimal maximum distance to `S` is `(hi - lo + 1) / 2` when `hi - lo` is odd? Let’s derive: For integer `k`, the minimal `max(k - lo, hi - k)` is `ceil((hi - lo)/2)`. That’s the value `d_S`. Then the overall maximum adjacent difference after replacement must be at least `d_S`, but also at least the maximum difference between any two known adjacent elements that are both not `-1` (since those are fixed). Let `base` be the maximum absolute difference over all adjacent pairs where neither is `-1` (ignoring pairs involving `-1` because they will be replaced). The answer `m` is `max(base, d_S)`. The chosen `k` is the integer that minimizes `max(k - lo, hi - k)`; any integer between `lo` and `hi` gives a maximum distance of at most `max(hi - k, k - lo)`, and the minimal of that is achieved by `k = (lo + hi) / 2` (truncated integer division) for non-negative values. Edge cases: no `-1` at all → no constraints from unknowns, set `k = 42` and `m = base` (max difference between all adjacent known pairs). All entries `-1` → no known neighbors, so any `k` works, set `k = 42`, `m = 0` (since all adjacent pairs become `|k - k| = 0`). If only one `-1` at an end with one known neighbor, then `lo = hi = that neighbor`, so `k = lo`, `d_S = 0`. The algorithm: first compute `base` by scanning the array for adjacent pairs where both are not `-1`. Then scan for known neighbors of `-1` to compute `lo` and `hi`. If any known neighbor exists, compute `k = (lo + hi)/2`, `d = max(k - lo, hi - k)` (which equals `(hi - lo + 1)/2` using integer division? Actually `(hi - lo + 1)/2` is `ceil((hi-lo)/2)` for non-negative difference). The final `m = max(base, d)`. If no known neighbors, set `k = 42`, `m = base`. Time complexity `O(n)`, space `O(1)`.

#include <vector>
#include <algorithm>
#include <utility>
#include <cstdlib>

// Given a vector where -1 marks unknown entries, replace all -1 with a single
// integer k chosen to minimize the maximum absolute difference between adjacent
// elements. Return {maxAbsoluteDifference, k}.
std::pair<int, int> replaceUnknowns(const std::vector<int>& a) {
    int n = static_cast<int>(a.size());
    int base = 0;  // max diff between adjacent known pairs (both not -1)
    for (int i = 1; i < n; ++i) {
        if (a[i] != -1 && a[i-1] != -1) {
            base = std::max(base, std::abs(a[i] - a[i-1]));
        }
    }

    int lo = 1000000000;  // min known neighbor of any -1
    int hi = 0;           // max known neighbor of any -1
    bool hasNeighbor = false;

    for (int i = 0; i < n; ++i) {
        if (a[i] == -1) {
            if (i > 0 && a[i-1] != -1) {
                hasNeighbor = true;
                lo = std::min(lo, a[i-1]);
                hi = std::max(hi, a[i-1]);
            }
            if (i + 1 < n && a[i+1] != -1) {
                hasNeighbor = true;
                lo = std::min(lo, a[i+1]);
                hi = std::max(hi, a[i+1]);
            }
        }
    }

    if (!hasNeighbor) {
        return {base, 42};
    }

    int k = (lo + hi) / 2;  // midpoint, integer truncation
    int d = std::max(k - lo, hi - k);  // max distance from k to any neighbor
    int result = std::max(base, d);
    return {result, k};
}

#include <cassert>
#include <vector>
#include <utility>

// Test the replaceUnknowns function
int main() {
    {
        std::vector<int> a = {5, -1, 9};
        auto [m, k] = replaceUnknowns(a);
        // lo=5, hi=9, k=7, d=max(2,2)=2, base=0 → m=2
        assert(m == 2 && k == 7);
    }
    {
        std::vector<int> a = {1, 2, 3};
        auto [m, k] = replaceUnknowns(a);
        assert(m == 1 && k == 42);
    }
    {
        std::vector<int> a = {-1, -1, -1};
        auto [m, k] = replaceUnknowns(a);
        assert(m == 0 && k == 42);
    }
    {
        std::vector<int> a = {10, -1, -1, 10};
        auto [m, k] = replaceUnknowns(a);
        // known neighbors: 10,10 → lo=hi=10, k=10, d=0, base=0 → m=0
        assert(m == 0 && k == 10);
    }
    {
        std::vector<int> a = {0, -1, 8, -1, 4};
        // known neighbors: 0,8 from first -1; 8,4 from second -1 → lo=0, hi=8
        // k=4, d=max(4,4)=4, base: pairs (8,4) diff=4, (0,-1) skip, (-1,8) skip → base=4
        auto [m, k] = replaceUnknowns(a);
        assert(m == 4 && k == 4);
    }
    {
        std::vector<int> a = {100, -1, -1, 0};
        // known neighbors: 100,0 → lo=0, hi=100, k=50, d=max(50,50)=50, base=0 → m=50
        auto [m, k] = replaceUnknowns(a);
        assert(m == 50 && k == 50);
    }
    {
        std::vector<int> a = {2, -1, 10, -1, 2};
        // known neighbors: 2,10 from first; 10,2 from second → lo=2, hi=10, k=6, d=4
        // base: pair (10,2) diff=8 → base=8, but (2,-1) skip, (-1,10) skip, (10,-1) skip → base=8
        // wait: pair (10,2) both non--1 → diff=8. So base=8, d=4 → m=8
        auto [m, k] = replaceUnknowns(a);
        assert(m == 8 && k == 6);
    }
    {
        std::vector<int> a = {5, -1, 5};
        auto [m, k] = replaceUnknowns(a);
        // lo=5,hi=5,k=5,d=0, base=0 → m=0
        assert(m == 0 && k == 5);
    }
    return 0;
}
