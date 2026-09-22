Write a C++ function `countQuadrants` that takes a vector of pairs of integers representing 2D coordinates and returns an array of five integers (or a struct) containing the counts of points in Quadrant 1 (x>0, y>0), Quadrant 2 (x<0, y>0), Quadrant 3 (x<0, y<0), Quadrant 4 (x>0, y<0), and points on an axis (where either x == 0 or y == 0). The function must handle negative numbers, zero coordinates, and empty input (returning all zeros). The input is provided as `std::vector<std::pair<int, int>>`, and the output should respect the order Q1, Q2, Q3, Q4, AXIS. The function should not print anything; it must only compute and return the counts.
The solution iterates through each coordinate pair exactly once. For each point, it first checks whether either coordinate is zero; if so, increment the axis counter. Otherwise, classify based on the signs of x and y: both positive → Q1; x negative, y positive → Q2; both negative → Q3; x positive, y negative → Q4. Edge cases include points exactly on the origin (0,0) which counts as axis, and all-zero coordinates. The algorithm runs in O(n) time, where n is the number of points, and uses O(1) auxiliary space beyond the output array. No special handling is needed for duplicate points or large coordinate values (within int range).
#include <vector>
#include <array>
#include <utility>

// Counts points in each quadrant and on axes, returning {Q1, Q2, Q3, Q4, AXIS}.
std::array<int, 5> countQuadrants(const std::vector<std::pair<int, int>>& points) {
    std::array<int, 5> counts = {0, 0, 0, 0, 0}; // Q1, Q2, Q3, Q4, AXIS

    for (const auto& [x, y] : points) {
        if (x == 0 || y == 0) {
            counts[4]++; // AXIS
            continue;
        }
        if (x > 0) {
            if (y > 0) counts[0]++; // Q1
            else counts[3]++;       // Q4
        } else {
            if (y > 0) counts[1]++; // Q2
            else counts[2]++;       // Q3
        }
    }

    return counts;
}
#include <cassert>
#include <vector>
#include <utility>
#include <array>

// The solution function is above.

int main() {
    // Empty input
    assert(countQuadrants({}) == std::array<int, 5>({0, 0, 0, 0, 0}));

    // Only axis points
    std::vector<std::pair<int, int>> axisPoints = {{0, 5}, {0, 0}, {-3, 0}};
    assert(countQuadrants(axisPoints) == std::array<int, 5>({0, 0, 0, 0, 3}));

    // One point per quadrant
    std::vector<std::pair<int, int>> quadPoints = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
    assert(countQuadrants(quadPoints) == std::array<int, 5>({1, 1, 1, 1, 0}));

    // Mixed with axis and duplicates
    std::vector<std::pair<int, int>> mixed = {{2, 3}, {-4, 5}, {-6, -7}, {8, -9}, {0, 1}, {5, 0}, {-2, -2}};
    assert(countQuadrants(mixed) == std::array<int, 5>({1, 1, 2, 1, 2}));

    // All points same quadrant
    std::vector<std::pair<int, int>> allQ1 = {{1, 2}, {3, 4}, {5, 6}};
    assert(countQuadrants(allQ1) == std::array<int, 5>({3, 0, 0, 0, 0}));

    // Negative coordinates only
    std::vector<std::pair<int, int>> negPoints = {{-1, -1}, {-2, -3}, {-4, -5}};
    assert(countQuadrants(negPoints) == std::array<int, 5>({0, 0, 3, 0, 0}));

    // Large coordinates
    std::vector<std::pair<int, int>> large = {{2147483647, 2147483647}, {-2147483647, -1}};
    assert(countQuadrants(large) == std::array<int, 5>({1, 0, 1, 0, 0}));

    // Mixed signs with zero
    std::vector<std::pair<int, int>> withZero = {{0, 0}, {1, -1}, {-1, 1}, {0, 3}};
    assert(countQuadrants(withZero) == std::array<int, 5>({0, 1, 0, 1, 2}));
}
