/*
Given a sequence of `n` points in the 2D plane, each with integer coordinates `(x, y)`, write a C++ function `stableSortPoints` that takes a vector of pairs (or an array along with its size) and sorts the points using the following two-step stable sorting procedure: first sort by x-coordinate in ascending order, then sort by y-coordinate in ascending order, with stability ensuring that points with equal y-coordinates retain their relative order from the previous x-sort. The function should modify the input container in place and return it (or return void). The sorting must be performed using `std::stable_sort` with appropriate custom comparators. The input size `n` can be up to 100,000, and coordinates can be any valid `int` values, including negative and duplicates. The final order must match the output of the given snippet, and the function must be self-contained with no reliance on global variables.
*/

#include <vector>
#include <utility>
#include <algorithm>

// Sort points by y ascending, then by x ascending, using two stable sorts.
void stableSortPoints(std::vector<std::pair<int,int>>& points) {
    // First stable sort by x ascending
    std::stable_sort(points.begin(), points.end(),
        [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
            return a.first < b.first;
        });
    // Then stable sort by y ascending, preserving previous x order for ties
    std::stable_sort(points.begin(), points.end(),
        [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
            return a.second < b.second;
        });
}

#include <cassert>
#include <vector>
#include <utility>

// Assume stableSortPoints is defined above.

int main() {
    // Basic case
    std::vector<std::pair<int,int>> p1 = {{1,2},{0,3},{1,1},{0,2},{2,2}};
    stableSortPoints(p1);
    std::vector<std::pair<int,int>> expected1 = {{1,1},{0,2},{1,2},{2,2},{0,3}};
    assert(p1 == expected1);

    // All same points
    std::vector<std::pair<int,int>> p2 = {{5,5},{5,5},{5,5}};
    stableSortPoints(p2);
    assert(p2 == std::vector<std::pair<int,int>>({{5,5},{5,5},{5,5}}));

    // Negative and duplicate y
    std::vector<std::pair<int,int>> p3 = {{-1,0},{2,0},{0,-1},{-3,-1}};
    stableSortPoints(p3);
    std::vector<std::pair<int,int>> expected3 = {{0,-1},{-3,-1},{-1,0},{2,0}};
    assert(p3 == expected3);

    // Single element
    std::vector<std::pair<int,int>> p4 = {{7,8}};
    stableSortPoints(p4);
    assert(p4 == std::vector<std::pair<int,int>>({{7,8}}));

    // Already sorted by y, then check x tie-breaking
    std::vector<std::pair<int,int>> p5 = {{10,1},{5,1},{0,1},{3,1}};
    stableSortPoints(p5);
    std::vector<std::pair<int,int>> expected5 = {{0,1},{3,1},{5,1},{10,1}};
    assert(p5 == expected5);

    // Larger random-like case (basic check using std::is_sorted with custom comparator)
    std::vector<std::pair<int,int>> p6 = {{3,4},{1,2},{3,4},{1,2},{2,3}};
    stableSortPoints(p6);
    // Verify primary order by y, secondary by x
    for (size_t i = 0; i + 1 < p6.size(); ++i) {
        if (p6[i].second == p6[i+1].second) {
            assert(p6[i].first <= p6[i+1].first);
        } else {
            assert(p6[i].second < p6[i+1].second);
        }
    }

    return 0;
}

// The key insight is that two consecutive stable sorts with different keys achieve a lexicographic ordering with the last key as primary and the first as secondary. Here, we first stable sort by x (so points are ordered by x, and equal x's keep original order), then stable sort by y. Because the second sort is stable, points with equal y maintain their relative order from the first sort—which was already sorted by x. Thus, for points with the same y, they appear in increasing x order, giving a final arrangement sorted primarily by y, and secondarily by x. This matches the snippet’s output.  
// Important edge cases: duplicate x or y values, all points equal, single point, and negative coordinates. The algorithm runs in O(n log n) time for each stable sort, total O(n log n), and uses O(n) auxiliary space (the internal buffer of stable_sort may use temporary storage). No special handling needed for negatives or duplicates as the comparators use standard `<`.  
// The main algorithm is: define two comparator functions (or lambdas) that compare only the first (x) and only the second (y) components, then call `std::stable_sort` twice in the correct order. The function signature should accept a reference to `std::vector<std::pair<int,int>>` to allow in-place modification.
