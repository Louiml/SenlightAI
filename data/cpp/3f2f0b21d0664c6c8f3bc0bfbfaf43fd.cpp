/*
Write a C++ function `countOverlappingPoints` that takes a vector of `Line` structs (where `Line` contains two integer-valued coordinate pairs `start` and `end`, each with `x` and `y` fields) and returns the number of grid points where at least two lines overlap. The grid is assumed to be large enough to cover all coordinates (coordinates are non-negative integers). All lines are either horizontal, vertical, or have a slope of exactly 1 or -1 (i.e., the change in `x` equals the change in `y` in absolute value). Count every integer lattice point that is covered by two or more distinct lines; if the same point is traversed multiple times by the same line, count it once per traversal (i.e., each line contributes to the point count once per visit). The function should handle lines that may have reversed endpoints (e.g., `start` may be to the right or below `end`). The function must return an integer representing the total number of points with at least two overlaps.
*/

#include <vector>
#include <cstdint>
#include <algorithm>

struct Coordinates {
    int32_t x;
    int32_t y;
};

struct Line {
    Coordinates start{};
    Coordinates end{};
};

// Count the number of grid points that are covered by at least two lines.
int countOverlappingPoints(const std::vector<Line>& lines) {
    if (lines.empty()) return 0;

    int32_t maxX = 0;
    int32_t maxY = 0;

    // Determine the grid bounds from all line endpoints.
    for (const auto& line : lines) {
        maxX = std::max({maxX, line.start.x, line.end.x});
        maxY = std::max({maxY, line.start.y, line.end.y});
    }

    // 2D grid flattened to 1D vector, initialized to zero.
    std::vector<int> board((static_cast<size_t>(maxX) + 1) * (static_cast<size_t>(maxY) + 1), 0);

    const size_t rowWidth = static_cast<size_t>(maxX) + 1;

    // Process each line.
    for (const auto& line : lines) {
        int32_t dx = line.end.x - line.start.x;
        int32_t dy = line.end.y - line.start.y;

        // Determine step direction for x and y (must be -1, 0, or 1).
        int32_t stepX = (dx > 0) - (dx < 0);
        int32_t stepY = (dy > 0) - (dy < 0);

        int32_t x = line.start.x;
        int32_t y = line.start.y;

        // Traverse all points from start to end inclusive.
        while (true) {
            board[static_cast<size_t>(x) + rowWidth * static_cast<size_t>(y)] += 1;
            if (x == line.end.x && y == line.end.y) {
                break;
            }
            x += stepX;
            y += stepY;
        }
    }

    // Count points with at least two overlaps.
    int overlapCount = 0;
    for (int value : board) {
        if (value >= 2) {
            ++overlapCount;
        }
    }

    return overlapCount;
}

#include <cassert>
#include <vector>

// Include the solution function here (for brevity, assume it is above).

int main() {
    // Test 1: Two horizontal lines crossing at one point.
    {
        std::vector<Line> lines = {
            {{0,0}, {5,0}},
            {{2,-1}, {2,1}}
        };
        // The only shared point is (2,0). Also vertical line covers (2,-1),(2,0),(2,1).
        // Horizontal covers (0..5,0). Overlap only at (2,0) => 1 point.
        assert(countOverlappingPoints(lines) == 1);
    }

    // Test 2: Diagonal lines overlapping at multiple points.
    {
        std::vector<Line> lines = {
            {{0,0}, {3,3}},
            {{1,1}, {4,4}}
        };
        // Overlap at (1,1),(2,2),(3,3) => 3 points.
        assert(countOverlappingPoints(lines) == 3);
    }

    // Test 3: Reversed endpoints handled correctly.
    {
        std::vector<Line> lines = {
            {{5,5}, {0,0}},
            {{0,5}, {5,0}}
        };
        // First line: (0,0) to (5,5). Second: (0,5) to (5,0).
        // Intersections: (0,0)? No. Check points: (0,0),(0,5) => no.
        // Let's see: (0,0) to (5,5) covers (0,0),(1,1),(2,2),(3,3),(4,4),(5,5).
        // (0,5) to (5,0) covers (0,5),(1,4),(2,3),(3,2),(4,1),(5,0).
        // No common points => 0.
        assert(countOverlappingPoints(lines) == 0);
    }

    // Test 4: Vertical line with reversed y direction.
    {
        std::vector<Line> lines = {
            {{3,10}, {3,2}},
            {{3,4}, {3,8}}
        };
        // First: y from 2 to 10 inclusive. Second: y from 4 to 8. Overlap at y=4,5,6,7,8 => 5 points.
        assert(countOverlappingPoints(lines) == 5);
    }

    // Test 5: No overlap.
    {
        std::vector<Line> lines = {
            {{0,0}, {1,0}},
            {{2,2}, {3,3}}
        };
        assert(countOverlappingPoints(lines) == 0);
    }

    // Test 6: Multiple lines intersecting at same point.
    {
        std::vector<Line> lines = {
            {{0,0}, {4,0}},
            {{2,-1}, {2,1}},
            {{0,0}, {2,2}}
        };
        // Horizontal covers (0..4,0). Vertical covers (2,-1..1). Diagonal covers (0,0),(1,1),(2,2).
        // Point (2,0) covered by horizontal and vertical -> overlap. Point (0,0) covered by horizontal and diagonal -> overlap. No others.
        // So count = 2.
        assert(countOverlappingPoints(lines) == 2);
    }

    // Test 7: Single point line.
    {
        std::vector<Line> lines = {
            {{1,1}, {1,1}},
            {{1,1}, {2,2}}
        };
        // Overlap at (1,1) only => 1.
        assert(countOverlappingPoints(lines) == 1);
    }

    // Test 8: Same line repeated.
    {
        std::vector<Line> lines = {
            {{0,0}, {2,0}},
            {{0,0}, {2,0}}
        };
        // All 3 points covered twice => 3.
        assert(countOverlappingPoints(lines) == 3);
    }

    // Test 9: Diagonal line with negative direction.
    {
        std::vector<Line> lines = {
            {{4,4}, {0,0}},
            {{0,0}, {4,4}}
        };
        // Same line twice, all 5 points covered twice => 5.
        assert(countOverlappingPoints(lines) == 5);
    }

    // Test 10: Empty list.
    {
        std::vector<Line> lines;
        assert(countOverlappingPoints(lines) == 0);
    }

    return 0;
}

// The solution approach is straightforward: first, determine the maximum `x` and `y` coordinates over all line endpoints to know the grid dimensions. Then create a 2D array (flattened to a 1D vector for efficiency) initialized to zeros, representing the number of lines covering each point. For each line, iterate over all integer points that lie on the line segment between `start` and `end`. Since lines are guaranteed to be axis-aligned or have slope ±1, the points are contiguous. For each such point, increment the corresponding counter in the board. After processing all lines, count how many board cells have a value ≥ 2. Important edge cases: vertical lines where `x` is constant and `y` varies, horizontal lines where `y` is constant and `x` varies, and diagonal lines where both `x` and `y` change by ±1 each step. Handle reversed endpoints by determining the direction of travel (delta = ±1 for each coordinate) from the start to the end. Ensure the final point is also included. The complexity is \(O(P + N \cdot L)\), where \(P\) is the number of grid points (maxX × maxY) and \(L\) is the average number of points per line. In the worst case, each line length can be up to max dimension, so total time is roughly proportional to sum of line lengths, which is at most \(N \cdot \text{maxDim}\). Space is \(O(\text{maxX} \times \text{maxY})\).
