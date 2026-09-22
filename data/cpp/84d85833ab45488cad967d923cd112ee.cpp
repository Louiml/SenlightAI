// Given a vector of axis-aligned rectangles, each represented by four integers (x_min, y_min, x_max, y_max) inclusive, write a C++ function `int countOverlappingPairs(const std::vector<std::array<int,4>>& rects)` that counts the number of unordered pairs of rectangles that have a non-empty intersection (i.e., their areas overlap with at least one common point). Rectangles are closed sets, so touching at a corner or edge counts as overlapping. The input may contain duplicate rectangles, and the function must correctly handle all edge cases including zero, one, or many rectangles. Additionally, write a helper `bool overlaps(const std::array<int,4>& a, const std::array<int,4>& b)` that returns true if two rectangles overlap, and incorporate it into the main function. The solution must not modify the input.
#include <cassert>
#include <vector>
#include <array>

int main() {
    using Rect = std::array<int,4>;
    // Empty
    assert(countOverlappingPairs({}) == 0);
    // Single
    assert(countOverlappingPairs({Rect{0,0,1,1}}) == 0);
    // Two that overlap
    assert(countOverlappingPairs({Rect{0,0,2,2}, Rect{1,1,3,3}}) == 1);
    // Two that touch at an edge
    assert(countOverlappingPairs({Rect{0,0,1,1}, Rect{1,0,2,1}}) == 1);
    // Two that touch at a corner
    assert(countOverlappingPairs({Rect{0,0,1,1}, Rect{1,1,2,2}}) == 1);
    // Two that do not overlap
    assert(countOverlappingPairs({Rect{0,0,1,1}, Rect{2,2,3,3}}) == 0);
    // Duplicate rectangles count
    assert(countOverlappingPairs({Rect{0,0,1,1}, Rect{0,0,1,1}}) == 1);
    // Multiple with some overlapping
    std::vector<Rect> rects = {{0,0,1,1}, {1,1,2,2}, {2,2,3,3}, {0,0,2,2}};
    // Pairs: (0,1) touch, (0,3) overlap, (1,2) touch, (1,3) overlap, (2,3) touch → 5
    assert(countOverlappingPairs(rects) == 5);
    // Negative coordinates
    assert(countOverlappingPairs({Rect{-2,-2,-1,-1}, Rect{-1,-1,0,0}}) == 1);
    // Non-overlapping with negative coordinates
    assert(countOverlappingPairs({Rect{-5,-5,-4,-4}, Rect{0,0,1,1}}) == 0);
}
#include <vector>
#include <array>

// Returns true if two rectangles overlap (closed sets, touching counts).
bool overlaps(const std::array<int,4>& a, const std::array<int,4>& b) {
    return !(a[2] < b[0] || b[2] < a[0] || a[3] < b[1] || b[3] < a[1]);
}

// Counts unordered pairs of rectangles with non-empty intersection.
int countOverlappingPairs(const std::vector<std::array<int,4>>& rects) {
    const int n = static_cast<int>(rects.size());
    int count = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (overlaps(rects[i], rects[j])) {
                ++count;
            }
        }
    }
    return count;
}
// The main algorithm iterates over all unordered pairs (i, j) with i < j and checks whether the two rectangles overlap. Overlap occurs if and only if the projections on both axes intersect: `a[0] <= b[2] && b[0] <= a[2]` for x, and `a[1] <= b[3] && b[1] <= a[3]` for y. Since inclusive ranges are used, equality at boundaries is allowed. The function uses two nested loops, so for n rectangles, the time complexity is O(n^2) and the space complexity is O(1) besides the input storage. Edge cases: empty vector returns 0, single rectangle returns 0, duplicate rectangles count as pairs (since they overlap), and coordinates may be negative or unordered? The problem statement assumes inputs are valid with x_min <= x_max and y_min <= y_max, but for robustness we could still use the overlap formula which also works if reversed, though we assume proper format. We also apply const correctness and avoid modifying the vector.
