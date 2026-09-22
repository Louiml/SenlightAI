Write a C++ function `int maxVerticalGap(std::vector<std::vector<int>>& points)` that takes a vector of points, where each point is a 2D coordinate represented as a vector of two integers `[x, y]`. The function should return the maximum vertical gap between two adjacent points when the points are sorted by their x-coordinate. A vertical gap is defined as the difference in x-coordinates between two consecutive points in the sorted order. You may assume the input contains at least two points, and each point has exactly two integer coordinates. Duplicate x-coordinates are allowed; if two points share the same x-coordinate, the gap between them is zero. The function should not modify the original input vector; instead, it should work on a copy or sort a local copy.

// The solution sorts the points by their x-coordinate (the first element of each inner vector) in ascending order. After sorting, the maximum vertical gap is the maximum difference between the x-coordinates of consecutive points in the sorted vector. We iterate from index 1 to `n-1`, compute `points[i][0] - points[i-1][0]`, and track the maximum of these differences. Edge cases: if all x-coordinates are identical, the maximum gap is 0. Since we sort a copy of the input to avoid modifying the original, the space complexity is O(n) for the copy (or O(1) if we choose to sort the input directly—but the task requires not modifying the input, so we make a copy). Time complexity is O(n log n) due to sorting, and space complexity is O(n) for the copy (or O(1) extra if we sort in place, but here we'll use a copy). The implementation uses `std::sort` with a lambda comparator for const-correctness and safety.

#include <vector>
#include <algorithm>

// Returns the maximum vertical gap (difference in x-coordinates) between
// adjacent points when sorted by x. The input vector is not modified.
int maxVerticalGap(std::vector<std::vector<int>> points) {
    if (points.size() < 2) return 0; // Not expected per constraints but safe
    
    // Sort a local copy by x-coordinate (first element) in ascending order.
    std::sort(points.begin(), points.end(),
              [](const std::vector<int>& a, const std::vector<int>& b) {
                  return a[0] < b[0];
              });
    
    int maxGap = 0;
    for (size_t i = 1; i < points.size(); ++i) {
        int gap = points[i][0] - points[i - 1][0];
        if (gap > maxGap) {
            maxGap = gap;
        }
    }
    return maxGap;
}

#include <cassert>

int main() {
    // Basic case with distinct x values
    std::vector<std::vector<int>> p1 = {{3, 1}, {1, 2}, {2, 3}, {5, 4}};
    assert(maxVerticalGap(p1) == 2); // sorted x: 1,2,3,5 -> gaps: 1,1,2 -> max 2

    // Duplicate x-coordinates
    std::vector<std::vector<int>> p2 = {{0, 1}, {0, 5}, {3, 2}};
    assert(maxVerticalGap(p2) == 3); // sorted x: 0,0,3 -> gaps: 0,3 -> max 3

    // All same x
    std::vector<std::vector<int>> p3 = {{7, 1}, {7, 2}, {7, 3}};
    assert(maxVerticalGap(p3) == 0);

    // Unsorted input with negative x
    std::vector<std::vector<int>> p4 = {{-5, 0}, {10, 9}, {-2, 1}, {4, 2}};
    // sorted x: -5,-2,4,10 -> gaps: 3,6,6 -> max 6
    assert(maxVerticalGap(p4) == 6);

    // Two points only
    std::vector<std::vector<int>> p5 = {{100, 0}, {0, 1}};
    assert(maxVerticalGap(p5) == 100);

    // Large gap at the end
    std::vector<std::vector<int>> p6 = {{1, 0}, {2, 0}, {2, 1}, {100, 99}};
    assert(maxVerticalGap(p6) == 98);

    // Verify original vector is not modified
    std::vector<std::vector<int>> original = {{5, 0}, {1, 1}, {3, 2}};
    std::vector<std::vector<int>> before = original;
    maxVerticalGap(original);
    assert(original == before);

    return 0;
}
