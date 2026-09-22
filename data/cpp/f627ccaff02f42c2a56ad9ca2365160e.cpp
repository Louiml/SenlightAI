Given a set of `n` intervals on a number line, where each interval is defined by a left endpoint `l`, a right endpoint `r`, and a weight `w`, write a C++ function that finds the minimum possible difference between the maximum and minimum weights among a selected subset of consecutive intervals (after sorting by weight) such that the union of the selected intervals covers the entire range from `1` to `m` inclusively. If no such subset exists, return `-1`. The function should accept the range limit `m` and a vector of intervals (each given as a tuple `{l, r, w}`), and return the minimal weight range as an integer.

The solution sorts all intervals by weight in ascending order. Then we use a two-pointer (sliding window) approach over the sorted intervals. We maintain a segment tree (or a multiset with frequency counts) to track the coverage of the integer points from `1` to `m`. Initially, the left pointer `pl` and right pointer `pr` are both at the start. We expand the right pointer, adding intervals to the coverage structure, until the union of intervals between `pl` and `pr` covers all points `1..m`. If coverage is complete, we update the answer with the difference between the weight of the interval at `pr` and the weight of the interval at `pl`. Then we shrink the window by moving the left pointer forward, removing intervals until coverage is no longer complete, and repeat the expansion. Edge cases include intervals that are out of the range `1..m`, duplicate weights, and when no subset can cover the full range (return `-1`). Time complexity is O(n log n) due to sorting and O(n log m) for the segment tree operations over the sliding window, with O(m) space for coverage tracking.

#include <vector>
#include <algorithm>
#include <climits>
#include <tuple>

// Returns the minimum weight difference to cover 1..m, or -1 if impossible.
int minWeightRange(int m, const std::vector<std::tuple<int, int, int>>& intervals) {
    int n = intervals.size();
    if (n == 0 || m <= 0) return -1;

    // Sort intervals by weight
    std::vector<std::tuple<int, int, int>> sorted = intervals;
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return std::get<2>(a) < std::get<2>(b);
    });

    // Segment tree to track coverage counts for each point 1..m
    std::vector<int> seg(4 * (m + 1), 0);
    std::vector<int> lazy(4 * (m + 1), 0);
    int fullyCovered = 0; // number of points with count > 0

    auto push = [&](int node, int l, int r) {
        if (lazy[node] != 0) {
            seg[node] += lazy[node] * (r - l + 1);
            if (l != r) {
                lazy[node * 2] += lazy[node];
                lazy[node * 2 + 1] += lazy[node];
            }
            lazy[node] = 0;
        }
    };

    // Range add: add val to all positions in [ql, qr]
    std::function<void(int, int, int, int, int, int)> add = [&](int node, int l, int r, int ql, int qr, int val) {
        push(node, l, r);
        if (ql <= l && r <= qr) {
            seg[node] += val * (r - l + 1);
            if (l != r) {
                lazy[node * 2] += val;
                lazy[node * 2 + 1] += val;
            }
            return;
        }
        int mid = (l + r) / 2;
        if (ql <= mid) add(node * 2, l, mid, ql, qr, val);
        if (qr > mid) add(node * 2 + 1, mid + 1, r, ql, qr, val);
        seg[node] = seg[node * 2] + seg[node * 2 + 1];
    };

    // Count of points with coverage > 0
    auto countCovered = [&]() {
        // Traverse tree to count leaves with seg value > 0 (efficient: we track fullyCovered manually)
        return fullyCovered;
    };

    // Maintain a boolean array to know when a point changes from 0 to >0 or vice versa
    std::vector<int> coverage(4 * (m + 1), 0);
    std::function<void(int, int, int, int, int, int)> updateCoverage = [&](int node, int l, int r, int idx, int delta) {
        push(node, l, r);
        if (l == r) {
            int old = coverage[node];
            coverage[node] += delta;
            if (old == 0 && coverage[node] > 0) fullyCovered++;
            if (old > 0 && coverage[node] == 0) fullyCovered--;
            return;
        }
        int mid = (l + r) / 2;
        if (idx <= mid) updateCoverage(node * 2, l, mid, idx, delta);
        else updateCoverage(node * 2 + 1, mid + 1, r, idx, delta);
    };

    auto addInterval = [&](const std::tuple<int,int,int>& inter) {
        int l = std::get<0>(inter), r = std::get<1>(inter);
        // Clip interval to 1..m
        l = std::max(l, 1);
        r = std::min(r, m);
        if (l > r) return;
        add(1, 1, m, l, r, 1);
        // Update coverage for each point (naive for small m, but for large m we can track via segment tree leaf counts)
        // For simplicity, we'll update each point in range; alternatively use a bucket approach. For this solution we'll use a simple frequency array if m is small; otherwise use segment tree leaf.
        // Here we'll use a direct array for coverage for simplicity, assuming m is not too large (<= 2e5). If m is large, use a segment tree with leaf counters.
        // We'll implement with a frequency vector of size m+1.
    };

    // Since we need to know whenever any point becomes covered/uncovered, and we need full coverage check,
    // a simpler approach: use a frequency array freq[1..m] and a counter coveredPoints.
    std::vector<int> freq(m + 1, 0);
    int coveredPoints = 0;

    auto addIntervalSimple = [&](const std::tuple<int,int,int>& inter, int delta) {
        int l = std::get<0>(inter), r = std::get<1>(inter);
        l = std::max(l, 1);
        r = std::min(r, m);
        if (l > r) return;
        for (int i = l; i <= r; i++) {
            if (freq[i] == 0 && delta == 1) coveredPoints++;
            if (freq[i] == 1 && delta == -1) coveredPoints--;
            freq[i] += delta;
        }
    };

    // But for efficiency, we can use a segment tree with lazy addition and track coverage via a separate bool array,
    // but for this reference we'll assume m is small enough for O(m) per update. If not, use a map or BIT.
    // To be safe, we'll use a segment tree that can count how many positions have count > 0.
    // Let's implement a segment tree that stores the minimum count in a segment, and a count of positions with min>0.
    // For range add, we need to track how many positions become zero or non-zero. Since we only care about zero vs non-zero,
    // we can use a segment tree with lazy propagation storing the minimum value in each segment and the count of that minimum.
    // This is a standard technique for "count of elements that are > 0" under range add.

    // Let's use that approach.

    std::vector<int> mn(4 * (m + 1), 0);
    std::vector<int> cnt(4 * (m + 1), 0);
    std::vector<int> lz(4 * (m + 1), 0);

    std::function<void(int,int)> build = [&](int node, int l, int r) {
        mn[node] = 0;
        cnt[node] = r - l + 1;
        if (l == r) return;
        int mid = (l + r) / 2;
        build(node * 2, l, mid);
        build(node * 2 + 1, mid + 1, r);
    };

    auto apply = [&](int node, int val) {
        mn[node] += val;
        lz[node] += val;
    };

    auto pushDown = [&](int node) {
        if (lz[node] != 0) {
            apply(node * 2, lz[node]);
            apply(node * 2 + 1, lz[node]);
            lz[node] = 0;
        }
    };

    auto pull = [&](int node) {
        int leftMin = mn[node * 2];
        int rightMin = mn[node * 2 + 1];
        if (leftMin == rightMin) {
            mn[node] = leftMin;
            cnt[node] = cnt[node * 2] + cnt[node * 2 + 1];
        } else if (leftMin < rightMin) {
            mn[node] = leftMin;
            cnt[node] = cnt[node * 2];
        } else {
            mn[node] = rightMin;
            cnt[node] = cnt[node * 2 + 1];
        }
    };

    std::function<void(int,int,int,int,int,int)> rangeAdd = [&](int node, int l, int r, int ql, int qr, int val) {
        if (ql <= l && r <= qr) {
            apply(node, val);
            return;
        }
        pushDown(node);
        int mid = (l + r) / 2;
        if (ql <= mid) rangeAdd(node * 2, l, mid, ql, qr, val);
        if (qr > mid) rangeAdd(node * 2 + 1, mid + 1, r, ql, qr, val);
        pull(node);
    };

    auto addIntervalSeg = [&](const std::tuple<int,int,int>& inter, int delta) {
        int l = std::get<0>(inter), r = std::get<1>(inter);
        l = std::max(l, 1);
        r = std::min(r, m);
        if (l > r) return;
        rangeAdd(1, 1, m, l, r, delta);
    };

    build(1, 1, m);

    int ans = INT_MAX;
    int pr = 0;
    for (int pl = 0; pl < n; pl++) {
        // expand pr until coverage is complete
        while (pr < n && mn[1] == 0) { // mn[1] is minimum count across all points; if >0 means all covered
            addIntervalSeg(sorted[pr], 1);
            pr++;
        }
        if (mn[1] > 0) { // all points covered
            int diff = std::get<2>(sorted[pr-1]) - std::get<2>(sorted[pl]);
            ans = std::min(ans, diff);
        }
        // remove intervals[pl] from window
        addIntervalSeg(sorted[pl], -1);
    }

    return ans == INT_MAX ? -1 : ans;
}

#include <cassert>
#include <vector>
#include <tuple>

int minWeightRange(int m, const std::vector<std::tuple<int, int, int>>& intervals);

int main() {
    // Basic test: intervals cover 1..5, weights 1,2,3 => min range is 2
    std::vector<std::tuple<int,int,int>> intervals1 = {{1,3,1}, {2,5,3}};
    assert(minWeightRange(5, intervals1) == 2);

    // Impossible coverage
    std::vector<std::tuple<int,int,int>> intervals2 = {{1,2,1}, {4,5,2}};
    assert(minWeightRange(5, intervals2) == -1);

    // Single interval covers all
    std::vector<std::tuple<int,int,int>> intervals3 = {{1,10,7}};
    assert(minWeightRange(10, intervals3) == 0);

    // Overlapping intervals with same weight
    std::vector<std::tuple<int,int,int>> intervals4 = {{1,3,5}, {3,5,5}};
    assert(minWeightRange(5, intervals4) == 0);

    // Need larger weight range to cover
    std::vector<std::tuple<int,int,int>> intervals5 = {{1,2,1}, {2,4,10}, {4,5,2}};
    assert(minWeightRange(5, intervals5) == 9); // weight diff between 1 and 10

    // Duplicate intervals, sorting ensures correct
    std::vector<std::tuple<int,int,int>> intervals6 = {{1,5,3}, {1,5,3}, {1,5,1}};
    assert(minWeightRange(5, intervals6) == 0);

    // Empty intervals
    std::vector<std::tuple<int,int,int>> intervals7 = {};
    assert(minWeightRange(3, intervals7) == -1);

    // Intervals partially outside range
    std::vector<std::tuple<int,int,int>> intervals8 = {{-5,0,4}, {1,6,2}, {7,10,9}};
    assert(minWeightRange(6, intervals8) == 7);

    // Many intervals but need minimal range
    std::vector<std::tuple<int,int,int>> intervals9 = {{1,1,1}, {2,2,2}, {3,3,3}, {1,3,0}};
    assert(minWeightRange(3, intervals9) == 0);

    // Larger m with several intervals
    std::vector<std::tuple<int,int,int>> intervals10 = {{1,2,5}, {2,3,7}, {3,5,8}, {1,5,20}};
    assert(minWeightRange(5, intervals10) == 3); // using intervals with weights 5,7,8

    return 0;
}
