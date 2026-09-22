Given two distinct non-origin integer coordinate points in the first quadrant (both coordinates are non-negative, and at least one coordinate is non-zero), write a C++ function `bool constructSquare(pair<int,int> p1, pair<int,int> p2, vector<pair<int,int>>& square)` that determines whether these two points can serve as two vertices of a square whose sides are parallel to the x-axis and y-axis, and whose other vertices also have non-negative integer coordinates. The square must have both vertices on its boundary (not necessarily adjacent). If possible, output the four vertices of such a square in any order; if not possible, indicate failure. The function should return `true` and fill `square` with the four vertices if a valid square exists; otherwise return `false`. The vertices must be integers, non-negative, and the square’s sides must be axis-aligned. If both points are at the origin, it's invalid (a degenerate square). Assume input coordinates are non-negative integers.

A square with axis-aligned sides is determined by a pair of opposite corners or a pair of adjacent corners. Since both points are arbitrary (but not both origin), we need to check all possible placements. The square's sides have equal length `L`, and coordinates are non-negative integers. For two vertices, they could be:
1. Adjacent: then the difference in one coordinate equals `L` and the other coordinate is equal. That is, if p1 and p2 differ only in x (same y), then the other two vertices are (x1, y1±L) and (x2, y2±L) with appropriate sign such that all coordinates non-negative. Similarly for difference in y.
2. Opposite corners: then the midpoints must coincide, and the differences in x and y must be equal in absolute value. The other corners are (x1, y2) and (x2, y1).
Additionally, they could be adjacent but with the shared coordinate, but there is a possibility that the square extends to negative coordinates; we must reject if any vertex would be negative. Also both points cannot be the same point, and if both are at origin, it's invalid.
The algorithm: Sort the two points to avoid duplication. Check three cases:
- If p1.second == p2.second (same y): adjacent horizontally. Let L = |p1.first - p2.first|. The other two vertices have y = p1.second + L and y = p1.second - L. For each of these two y-values, if both y-values are non-negative, then the square is valid, but also need to ensure that the new vertices have the same x as p1 and p2 respectively. However, we must ensure the square has non-negative coordinates; so we try both directions: y + L and y - L. For each, check if the new y is >=0. If yes, construct square and return true.
- If p1.first == p2.first (same x): adjacent vertically. Similarly, try x + L and x - L.
- If neither coordinate is equal: they cannot be adjacent; must be opposite corners. Then the other corners are (p1.first, p2.second) and (p2.first, p1.second). All four must have coordinates >=0. Also need to check that the distance along x equals distance along y (i.e., |p1.first - p2.first| == |p1.second - p2.second|) because a square has equal side lengths; if not equal, it's not a square. If equal, then construct the four vertices.
Edge cases: p1 == p2 (same point) invalid. Also if both are at origin (0,0) then it's degenerate, invalid. Also note that the square's sides must be parallel to axes, so diagonal case is only when points are opposite corners.
Time complexity O(1) and space O(1) (excluding the output vector).

#include <vector>
#include <utility>
#include <algorithm>
#include <cstdlib>

// Given two distinct non-origin points with non-negative integer coordinates,
// determine if they can be two vertices of an axis-aligned square with all
// vertices having non-negative integer coordinates.
// If possible, fill 'square' with the four vertices and return true; else return false.
bool constructSquare(const std::pair<int,int>& p1,
                     const std::pair<int,int>& p2,
                     std::vector<std::pair<int,int>>& square) {
    // Sort points to have a canonical order
    std::pair<int,int> a = p1, b = p2;
    if (a > b) std::swap(a, b);

    // Degenerate: same point or both at origin
    if (a == b) return false;
    if (a.first == 0 && a.second == 0 && b.first == 0 && b.second == 0) return false;

    // Case 1: Adjacent horizontally (same y)
    if (a.second == b.second) {
        int L = std::abs(a.first - b.first);
        // Try extending upward
        if (a.second + L >= 0 && b.second + L >= 0) {
            square = {a, b, {a.first, a.second + L}, {b.first, b.second + L}};
            return true;
        }
        // Try extending downward
        if (a.second - L >= 0 && b.second - L >= 0) {
            square = {a, b, {a.first, a.second - L}, {b.first, b.second - L}};
            return true;
        }
        return false;
    }

    // Case 2: Adjacent vertically (same x)
    if (a.first == b.first) {
        int L = std::abs(a.second - b.second);
        // Try extending right
        if (a.first + L >= 0 && b.first + L >= 0) {
            square = {a, b, {a.first + L, a.second}, {b.first + L, b.second}};
            return true;
        }
        // Try extending left
        if (a.first - L >= 0 && b.first - L >= 0) {
            square = {a, b, {a.first - L, a.second}, {b.first - L, b.second}};
            return true;
        }
        return false;
    }

    // Case 3: Opposite corners (neither x nor y equal)
    // For a square, the horizontal and vertical distances must be equal
    if (std::abs(a.first - b.first) != std::abs(a.second - b.second)) return false;
    std::pair<int,int> c = {a.first, b.second};
    std::pair<int,int> d = {b.first, a.second};
    // All coordinates must be non-negative
    if (c.first < 0 || c.second < 0 || d.first < 0 || d.second < 0) return false;
    square = {a, b, c, d};
    return true;
}

#include <cassert>
#include <vector>
#include <utility>
#include <algorithm>

int main() {
    std::vector<std::pair<int,int>> result;

    // Adjacent horizontally, extend upward
    assert(constructSquare({1,0}, {2,0}, result) == true);
    assert(result.size() == 4);
    assert(std::find(result.begin(), result.end(), std::make_pair(1,0)) != result.end());
    assert(std::find(result.begin(), result.end(), std::make_pair(2,0)) != result.end());
    assert(std::find(result.begin(), result.end(), std::make_pair(1,1)) != result.end());
    assert(std::find(result.begin(), result.end(), std::make_pair(2,1)) != result.end());

    // Adjacent vertically, extend right
    assert(constructSquare({0,1}, {0,2}, result) == true);
    assert(std::find(result.begin(), result.end(), std::make_pair(1,1)) != result.end());
    assert(std::find(result.begin(), result.end(), std::make_pair(1,2)) != result.end());

    // Opposite corners
    assert(constructSquare({2,3}, {5,6}, result) == true);
    assert(std::find(result.begin(), result.end(), std::make_pair(2,6)) != result.end());
    assert(std::find(result.begin(), result.end(), std::make_pair(5,3)) != result.end());

    // Invalid: adjacent horizontally but extension would be negative
    assert(constructSquare({3,0}, {4,0}, result) == false);

    // Invalid: opposite corners but distances not equal (rectangle)
    assert(constructSquare({1,2}, {4,5}, result) == false);

    // Invalid: same point
    assert(constructSquare({1,1}, {1,1}, result) == false);

    // Invalid: both at origin
    assert(constructSquare({0,0}, {0,0}, result) == false);

    // Valid: one point at origin, other at (2,2) -> opposite corners
    assert(constructSquare({0,0}, {2,2}, result) == true);
    assert(std::find(result.begin(), result.end(), std::make_pair(0,2)) != result.end());
    assert(std::find(result.begin(), result.end(), std::make_pair(2,0)) != result.end());

    // Valid: one point at origin, other at (0,3) -> adjacent vertically, extend right
    assert(constructSquare({0,0}, {0,3}, result) == true);
    assert(std::find(result.begin(), result.end(), std::make_pair(3,0)) != result.end());
    assert(std::find(result.begin(), result.end(), std::make_pair(3,3)) != result.end());

    return 0;
}
