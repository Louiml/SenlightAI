Write a C++ function that, given an integer coordinate `(x, y)` and a list of points (each represented as a vector of two integers `[a, b]`), returns the index of the nearest valid point that shares either the same x-coordinate or the same y-coordinate as the given point. A valid point is one where `a == x` or `b == y`. The distance is measured as the Manhattan distance `abs(a - x) + abs(b - y)`. If there are multiple valid points with the same minimum distance, return the smallest index. If no valid point exists, return `-1`. The input list is non-empty and contains at least one point, but none of the points may be valid. You can assume the input coordinates are within the range `[-10^4, 10^4]`.

The solution iterates through the list of points once, checking for validity by comparing the first coordinate with `x` or the second coordinate with `y`. For each valid point, compute the Manhattan distance using absolute differences. Maintain two variables: the current minimum distance (initialized to a large sentinel like `1e6` or `INT_MAX`) and the answer index (initialized to `-1`). When a valid point has a distance strictly less than the current minimum, update both the minimum and the answer index. Because we iterate in increasing index order and only update on strictly smaller distances, the first occurrence of the minimum distance will be retained, satisfying the tie-breaking rule. Edge cases include: no valid point (returns `-1`), exactly one valid point, and multiple valid points with the same distance where the smallest index must be selected. Time complexity is O(n) where n is the number of points, and space complexity is O(1) additional memory, apart from input storage.

#include <vector>
#include <cstdlib>  // for std::abs
#include <climits>  // for INT_MAX

// Returns the index of the nearest valid point sharing x or y with (x, y),
// or -1 if none exists. Ties are broken by smallest index.
int nearestValidPoint(int x, int y, const std::vector<std::vector<int>>& points) {
    int answer = -1;
    int min_distance = INT_MAX;

    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        const int a = points[i][0];
        const int b = points[i][1];
        if (a == x || b == y) {
            const int distance = std::abs(a - x) + std::abs(b - y);
            if (distance < min_distance) {
                min_distance = distance;
                answer = i;
            }
        }
    }
    return answer;
}

#include <cassert>
#include <vector>

int main() {
    // Example 1: valid points at indices 2 and 3, distance 1 and 2 respectively.
    {
        std::vector<std::vector<int>> points = {{1, 2}, {3, 1}, {2, 2}, {2, 1}};
        assert(nearestValidPoint(2, 2, points) == 2);
    }

    // No valid points: all points differ in both x and y.
    {
        std::vector<std::vector<int>> points = {{0, 0}, {5, 5}};
        assert(nearestValidPoint(1, 1, points) == -1);
    }

    // Single valid point.
    {
        std::vector<std::vector<int>> points = {{3, 4}};
        assert(nearestValidPoint(3, 9, points) == 0);
    }

    // Tie distance: indices 0 and 1 both have distance 2, return smallest.
    {
        std::vector<std::vector<int>> points = {{0, 0}, {2, 0}, {0, 3}};
        assert(nearestValidPoint(2, 2, points) == 0);
    }

    // All points valid, some with equal distance.
    {
        std::vector<std::vector<int>> points = {{0, 0}, {0, 5}, {5, 0}};
        assert(nearestValidPoint(0, 0, points) == 0);
    }

    // Only one valid point among many.
    {
        std::vector<std::vector<int>> points = {{1, 1}, {2, 2}, {3, 3}};
        assert(nearestValidPoint(4, 3, points) == 2);
    }

    // Negative coordinates.
    {
        std::vector<std::vector<int>> points = {{-1, -1}, {-2, -2}};
        assert(nearestValidPoint(-1, -5, points) == 0);
    }

    // Points with same coordinate as target in both axes, distance 0.
    {
        std::vector<std::vector<int>> points = {{5, 5}, {5, 6}, {6, 5}};
        assert(nearestValidPoint(5, 5, points) == 0);
    }

    // Large input size but no valid points.
    {
        std::vector<std::vector<int>> points;
        for (int i = 0; i < 1000; ++i) {
            points.push_back({i, i + 1});
        }
        assert(nearestValidPoint(0, 100000, points) == -1);
    }

    // Edge: target point itself exists in the list, distance 0.
    {
        std::vector<std::vector<int>> points = {{1, 1}, {2, 2}, {1, 2}};
        assert(nearestValidPoint(1, 1, points) == 0);
    }

    return 0;
}
