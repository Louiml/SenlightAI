Write a C++ function `std::vector<int> rangeQuery(const std::vector<std::pair<int,int>>& points, int sx, int tx, int sy, int ty)` that, given a list of 2D points (each represented as a pair of x,y coordinates, with their original indices implied by their position in the input vector, i.e., point at index `i` has id `i`), returns a vector of point IDs (indices) that lie within the axis-aligned rectangle defined by `sx <= x <= tx` and `sy <= y <= ty`. The returned IDs must be sorted in ascending order. The function should efficiently handle up to 100,000 points and up to 100,000 queries, using a k-d tree to avoid O(N) per query. For simplicity, the function processes only a single query per call; do not implement a batch query interface. The points are given with x and y coordinates that may be duplicated, and the query rectangle may be degenerate (sx == tx or sy == ty). If no points satisfy the condition, return an empty vector.

// The core idea is to build a k-d tree once (or per call if points are passed, but for efficiency, the function will build a k-d tree internally each time—though for a single query, this is acceptable, but better to note that in a real usage, building once for many queries is ideal; here we just implement the query). The k-d tree is constructed by recursively splitting points at the median along alternating axes: depth 0 splits by x, depth 1 by y, and so on. Each tree node stores the index of the point in the original array that serves as the split, and pointers to left and right children. For a rectangular query, we traverse the tree: at each node, check if the stored point is inside the rectangle; if so, add its ID. Then, we decide whether to recurse into children: if the current split axis is x, then we explore the left subtree if the rectangle's left bound `sx` is less than or equal to the split x-coordinate, and the right subtree if the rectangle's right bound `tx` is greater than or equal to that x. Similarly for y. This prunes subtrees that cannot contain any points in the rectangle. Edge cases: empty point list returns empty vector; points with identical coordinates; rectangle covering no points; rectangle that includes boundaries (inclusive). The tree is built recursively; we need to be careful with the sorting step, which can be done by copying the point indices and sorting based on axis coordinate. Time complexity: building the tree is O(N log N) per call (since we sort), query is O(√N + K) in 2D, but for a single query it's fine. Space O(N). However, in the solution, to keep it clean, we will build the k-d tree from the passed vector each time, which is O(N log N) plus query time. The function returns sorted IDs.

#include <vector>
#include <algorithm>
#include <utility>
#include <functional>

// Helper to compare pairs by x or y coordinate
bool lessX(const std::pair<int,int>& a, const std::pair<int,int>& b) {
    return a.first < b.first;
}
bool lessY(const std::pair<int,int>& a, const std::pair<int,int>& b) {
    return a.second < b.second;
}

// k-d tree node: stores point index in original vector, and child indices in tree array
struct KDNode {
    int point_idx;
    int left;
    int right;
    KDNode() : point_idx(-1), left(-1), right(-1) {}
};

// Recursive build function
// points: indices of original points (0..N-1) to consider
// l, r: range [l, r) in the indices array
// depth: current depth
// tree: output tree array
// org_points: original points vector
int buildKD(const std::vector<int>& indices, int l, int r, int depth, std::vector<KDNode>& tree, const std::vector<std::pair<int,int>>& org_points) {
    if (l >= r) return -1; // empty
    int mid = l + (r - l) / 2;
    
    // Sort indices[l..r) based on axis at this depth
    std::vector<int> temp(indices.begin()+l, indices.begin()+r);
    if (depth % 2 == 0) {
        std::sort(temp.begin(), temp.end(), [&](int i, int j) {
            return org_points[i].first < org_points[j].first;
        });
    } else {
        std::sort(temp.begin(), temp.end(), [&](int i, int j) {
            return org_points[i].second < org_points[j].second;
        });
    }
    // Copy sorted indices back
    for (int i = 0; i < (int)temp.size(); ++i) {
        indices[l + i] = temp[i];
    }
    
    int node_idx = (int)tree.size();
    tree.push_back(KDNode());
    tree[node_idx].point_idx = indices[mid];
    
    tree[node_idx].left = buildKD(indices, l, mid, depth+1, tree, org_points);
    tree[node_idx].right = buildKD(indices, mid+1, r, depth+1, tree, org_points);
    return node_idx;
}

// Recursive query function
void queryKD(int node_idx, int sx, int tx, int sy, int ty, int depth, const std::vector<KDNode>& tree, const std::vector<std::pair<int,int>>& org_points, std::vector<int>& ans) {
    if (node_idx == -1) return;
    const auto& pt = org_points[tree[node_idx].point_idx];
    if (sx <= pt.first && pt.first <= tx && sy <= pt.second && pt.second <= ty) {
        ans.push_back(tree[node_idx].point_idx);
    }
    if (depth % 2 == 0) { // x axis
        if (tree[node_idx].left != -1 && sx <= pt.first) {
            queryKD(tree[node_idx].left, sx, tx, sy, ty, depth+1, tree, org_points, ans);
        }
        if (tree[node_idx].right != -1 && pt.first <= tx) {
            queryKD(tree[node_idx].right, sx, tx, sy, ty, depth+1, tree, org_points, ans);
        }
    } else { // y axis
        if (tree[node_idx].left != -1 && sy <= pt.second) {
            queryKD(tree[node_idx].left, sx, tx, sy, ty, depth+1, tree, org_points, ans);
        }
        if (tree[node_idx].right != -1 && pt.second <= ty) {
            queryKD(tree[node_idx].right, sx, tx, sy, ty, depth+1, tree, org_points, ans);
        }
    }
}

// Main function: returns sorted IDs of points inside the rectangle
std::vector<int> rangeQuery(const std::vector<std::pair<int,int>>& points, int sx, int tx, int sy, int ty) {
    if (points.empty()) return {};
    
    std::vector<int> indices(points.size());
    for (size_t i = 0; i < points.size(); ++i) indices[i] = static_cast<int>(i);
    
    std::vector<KDNode> tree;
    tree.reserve(points.size());
    buildKD(indices, 0, static_cast<int>(points.size()), 0, tree, points);
    
    std::vector<int> ans;
    queryKD(0, sx, tx, sy, ty, 0, tree, points, ans);
    std::sort(ans.begin(), ans.end());
    return ans;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is used directly.

int main() {
    // Example: points (0,0), (1,2), (3,1), (4,4)
    std::vector<std::pair<int,int>> pts = {{0,0},{1,2},{3,1},{4,4}};
    
    // Query full rectangle
    std::vector<int> r1 = rangeQuery(pts, -10, 10, -10, 10);
    assert((r1 == std::vector<int>{0,1,2,3}));
    
    // Query to get only point (1,2) -> id 1
    std::vector<int> r2 = rangeQuery(pts, 1, 1, 2, 2);
    assert((r2 == std::vector<int>{1}));
    
    // Query empty
    std::vector<int> r3 = rangeQuery(pts, 10, 20, 10, 20);
    assert(r3.empty());
    
    // Query boundary inclusive
    std::vector<int> r4 = rangeQuery(pts, 0, 3, 0, 1);
    // Points: (0,0) id0, (3,1) id2? but (3,1) has x=3,y=1, so inside. (1,2) y=2 outside. So [0,2]
    assert((r4 == std::vector<int>{0,2}));
    
    // Duplicate points
    std::vector<std::pair<int,int>> dup = {{1,1},{1,1},{2,2}};
    std::vector<int> r5 = rangeQuery(dup, 1, 2, 1, 2);
    assert((r5 == std::vector<int>{0,1,2}));
    
    // Single point
    std::vector<std::pair<int,int>> single = {{5,5}};
    std::vector<int> r6 = rangeQuery(single, 0, 10, 0, 10);
    assert((r6 == std::vector<int>{0}));
    
    // Degenerate rectangle: line x=2, y from 0 to 5
    std::vector<std::pair<int,int>> pts2 = {{2,1},{3,4},{2,5},{1,0}};
    std::vector<int> r7 = rangeQuery(pts2, 2, 2, 0, 5);
    // Points with x=2: id0 (2,1), id2 (2,5) -> both inside y range
    assert((r7 == std::vector<int>{0,2}));
    
    // Unsorted input, ensure IDs are sorted output
    std::vector<std::pair<int,int>> pts3 = {{10,10},{1,1},{5,5}};
    std::vector<int> r8 = rangeQuery(pts3, 0, 20, 0, 20);
    assert((r8 == std::vector<int>{0,1,2})); // sorted by ID: 0,1,2
    
    return 0;
}
