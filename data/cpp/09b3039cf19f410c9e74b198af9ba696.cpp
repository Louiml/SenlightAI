/*
Write a C++ function `processQueries` that takes a vector of queries, each formatted as a tuple `(string op, int x, int y)`, where `op` is one of `"add"`, `"remove"`, or `"find"`. The function must process these queries offline: first, collect all `(x, y)` points that appear in any `"add"` or `"remove"` query (duplicates allowed, treated as distinct occurrences but only the maximum y per exact point matters). Sort these unique points lexicographically by `(x, y)`. For each query in order:
- If `op == "add"`, insert the point `(x, y)` into a data structure, but if the same exact point is added multiple times, the stored y is just the maximum of all added y values for that coordinate.
- If `op == "remove"`, delete the point `(x, y)` if it exists (setting its stored y to 0). If it does not exist, do nothing.
- If `op == "find"`, find the point with the smallest x coordinate strictly greater than `x`, and among points with that same x, the one with the smallest y coordinate such that y is strictly greater than `y`. If no such point exists, return `"−1"` (the string "-1"). Otherwise return the found point's `x` and `y` as a string `"x y"`.
Return a vector of strings (one per `"find"` query, in order). Assume the input `n` (number of queries) is at least 1, and all coordinates are non-negative integers fitting in 32-bit signed int. The function must be efficient for up to 200,000 queries.
*/
#include <vector>
#include <string>
#include <algorithm>
#include <tuple>
#include <sstream>
#include <utility>

// Segment tree supporting point updates and query for smallest index in a range where value > threshold
class SegmentTree {
    int n;
    std::vector<int> tree;
    std::vector<int> currentValue; // actual value at each leaf
    
    void build(int node, int l, int r) {
        if (l == r) {
            tree[node] = currentValue[l];
            return;
        }
        int mid = (l + r) / 2;
        build(node*2, l, mid);
        build(node*2+1, mid+1, r);
        tree[node] = std::max(tree[node*2], tree[node*2+1]);
    }
    
    void update(int idx, int val, int node, int l, int r) {
        if (l == r) {
            currentValue[idx] = val;
            tree[node] = val;
            return;
        }
        int mid = (l + r) / 2;
        if (idx <= mid) update(idx, val, node*2, l, mid);
        else update(idx, val, node*2+1, mid+1, r);
        tree[node] = std::max(tree[node*2], tree[node*2+1]);
    }
    
    int firstGreater(int ql, int qr, int threshold, int node, int l, int r) {
        if (r < ql || l > qr || tree[node] <= threshold) return -1;
        if (l == r) return l;
        int mid = (l + r) / 2;
        int res = firstGreater(ql, qr, threshold, node*2, l, mid);
        if (res != -1) return res;
        return firstGreater(ql, qr, threshold, node*2+1, mid+1, r);
    }
    
public:
    SegmentTree(const std::vector<int>& initial) : n(initial.size()), currentValue(initial) {
        tree.resize(4 * n);
        if (n > 0) build(1, 0, n-1);
    }
    
    void setValue(int idx, int val) {
        update(idx, val, 1, 0, n-1);
    }
    
    int getValue(int idx) const {
        return currentValue[idx];
    }
    
    // Return smallest index in [ql, qr] with value > threshold, or -1
    int searchLeft(int ql, int qr, int threshold) {
        if (ql > qr) return -1;
        if (n == 0) return -1;
        return firstGreater(ql, qr, threshold, 1, 0, n-1);
    }
};

// Main function that processes all queries
std::vector<std::string> processQueries(const std::vector<std::tuple<std::string,int,int>>& queries) {
    // Collect all distinct points from add/remove queries
    std::vector<std::pair<int,int>> points;
    for (const auto& [op, x, y] : queries) {
        if (op == "add" || op == "remove") {
            points.emplace_back(x, y);
        }
    }
    // Sort and unique
    std::sort(points.begin(), points.end());
    points.erase(std::unique(points.begin(), points.end()), points.end());
    const int N = points.size();
    
    // Initial segment tree with zeros
    std::vector<int> initial(N, 0);
    SegmentTree seg(initial);
    
    std::vector<std::string> result;
    
    for (const auto& [op, x, y] : queries) {
        if (op == "add") {
            auto it = std::lower_bound(points.begin(), points.end(), std::make_pair(x, y));
            int idx = it - points.begin();
            int newVal = std::max(seg.getValue(idx), y);
            seg.setValue(idx, newVal);
        } else if (op == "remove") {
            auto it = std::lower_bound(points.begin(), points.end(), std::make_pair(x, y));
            if (it != points.end() && *it == std::make_pair(x, y)) {
                int idx = it - points.begin();
                seg.setValue(idx, 0);
            }
        } else { // "find"
            // Find first x strictly greater than given x
            auto it = std::lower_bound(points.begin(), points.end(), std::make_pair(x+1, 0));
            int L = it - points.begin();
            int R = N - 1;
            if (L > R) {
                result.push_back("-1");
            } else {
                int j = seg.searchLeft(L, R, y);
                if (j == -1) {
                    result.push_back("-1");
                } else {
                    result.push_back(std::to_string(points[j].first) + " " + std::to_string(points[j].second));
                }
            }
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <tuple>
#include <string>

// assume processQueries is defined above

int main() {
    // Basic test from the snippet
    std::vector<std::tuple<std::string,int,int>> q1 = {
        {"add", 1, 10},
        {"add", 2, 20},
        {"find", 1, 5},
        {"find", 1, 15},
        {"remove", 2, 20},
        {"find", 1, 15}
    };
    auto r1 = processQueries(q1);
    assert(r1 == std::vector<std::string>{"2 20", "-1", "-1"});

    // Duplicate add should keep max y
    std::vector<std::tuple<std::string,int,int>> q2 = {
        {"add", 5, 7},
        {"add", 5, 9},
        {"find", 4, 0},
        {"remove", 5, 7},
        {"find", 4, 0}
    };
    auto r2 = processQueries(q2);
    // After first two adds, point (5,9) is stored (max y). Find (x>4) returns (5,9)
    // remove (5,7) does nothing because current y is 9 not 7? Actually remove sets y to 0 at that exact point? 
    // The spec says remove sets the stored y to 0 for that exact point (x,y). But if current y is 9, removing (5,7) should have no effect because the point stored is (5,9) not (5,7). So result: "5 9", then "5 9" again.
    assert(r2 == std::vector<std::string>{"5 9", "5 9"});

    // Multiple points with same x but different y
    std::vector<std::tuple<std::string,int,int>> q3 = {
        {"add", 10, 100},
        {"add", 10, 50},
        {"find", 9, 60},
        {"find", 9, 110}
    };
    auto r3 = processQueries(q3);
    // Points: (10,50) and (10,100). After adds, stored ys: 50 and 100.
    // find with y=60: index with y>60 is (10,100) => "10 100"
    // find with y=110: none => "-1"
    assert(r3 == std::vector<std::string>{"10 100", "-1"});

    // No points at all
    std::vector<std::tuple<std::string,int,int>> q4 = {
        {"find", 0, 0}
    };
    auto r4 = processQueries(q4);
    assert(r4 == std::vector<std::string>{"-1"});

    // Remove nonexistent point
    std::vector<std::tuple<std::string,int,int>> q5 = {
        {"add", 3, 3},
        {"remove", 3, 4},
        {"find", 2, 2}
    };
    auto r5 = processQueries(q5);
    assert(r5 == std::vector<std::string>{"3 3"});

    return 0;
}
// We need a data structure that supports point updates (set y value at a given index) and queries: given a lower bound index `L` (first point with x > given x) and a threshold `Y`, find the smallest index `j >= L` such that `stored_y[j] > Y`. This is a classic "search for first index from left where value exceeds threshold" problem, solvable with a segment tree that stores the maximum y in each segment. 
// First, collect all distinct points from all `"add"` and `"remove"` queries, sort them, and assign each point a unique index via lower_bound. For updates: `add` sets the value at that index to `max(existing, y)`; `remove` sets it to 0. For `find`: we need the first point with x > given x (index from lower_bound on (x+1,0)), then we search the segment tree range `[L, n-1]` for the smallest index where the stored y > given y. This can be done by descending the tree: if the left child's max > threshold, go left; else go right. If no such index exists, return -1. 
// Edge cases: duplicate points in add/remove must be handled by distinct indices (using lower_bound on sorted unique points). The tree stores the maximum y per node; starting from 0 for all leaves. When adding, we update with max. When removing, we set to 0. Queries must handle L > n-1 (when no point has x > given x) returning -1. Time complexity: O((n + q) log n) where q is number of queries, n is distinct points. Space O(n).
