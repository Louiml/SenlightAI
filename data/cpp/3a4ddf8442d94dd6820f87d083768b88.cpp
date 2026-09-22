Write a C++ function that, given the coordinates of a query point (x, y) and a list of points on a 2D Cartesian plane, returns the index of the nearest valid point. A point is valid if it has the same x-coordinate or the same y-coordinate as the query point. The "distance" between two points is the Manhattan distance (|x1 - x2| + |y1 - y2|). If there are multiple valid points at the same minimum Manhattan distance, return the smallest index. If no valid point exists, return -1. The function should take the query coordinates as integers and the list of points as a vector of vectors of integers (each inner vector has exactly two integers), and return an integer index.
// The solution iterates over each point in the given list once, checking whether it is valid by comparing its x-coordinate to the query x or its y-coordinate to the query y. For each valid point, we compute the Manhattan distance using `abs(x - point[0]) + abs(y - point[1])`. We maintain a running minimum distance and the index corresponding to that minimum. Since the loop progresses from the smallest index to the largest, the first time we encounter a valid point with a distance strictly smaller than the current minimum, we update both the distance and index. Using `dis1 < dis` (strict less-than) ensures that if a later point has the same distance, we keep the earlier (smaller) index. Initialize the best index to -1 and the best distance to `INT_MAX` from `<climits>`. The main edge case is when no valid point exists; then we return -1. Also, handle empty input list gracefully (return -1). Time complexity is O(n) where n is the number of points, and space complexity is O(1) auxiliary, ignoring the input vector itself.
#include <vector>
#include <cstdlib>
#include <climits>

// Returns the index of the nearest valid point, or -1 if none.
// A point is valid if it shares x or y coordinate with (x, y).
// Distance is Manhattan distance. Ties prefer the smallest index.
int nearestValidPoint(int x, int y, const std::vector<std::vector<int>>& points) {
    int bestIndex = -1;
    int bestDistance = INT_MAX;
    
    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        const auto& p = points[i];
        // Check if point is valid (same x or same y)
        if (p[0] == x || p[1] == y) {
            int dist = std::abs(x - p[0]) + std::abs(y - p[1]);
            if (dist < bestDistance) {
                bestDistance = dist;
                bestIndex = i;
            }
        }
    }
    return bestIndex;
}
#include <cassert>
#include <vector>

int main() {
    // Basic test: valid points with different distances
    std::vector<std::vector<int>> points1 = {{1, 2}, {3, 2}, {2, 2}};
    assert(nearestValidPoint(1, 2, points1) == 0);
    
    // Tie in distance, choose smallest index
    std::vector<std::vector<int>> points2 = {{1, 1}, {1, 3}, {2, 2}};
    assert(nearestValidPoint(2, 1, points2) == 1); // both (1,1) and (1,3) valid, dist 1, index 1 smaller than 2? Actually index 1 is (1,3) dist=1, index 0 (1,1) dist=1, choose index 0
    
    // Correct tie test: query (2,2), points (2,1) index0, (3,2) index1, both dist1 => choose index0
    std::vector<std::vector<int>> points3 = {{2, 1}, {3, 2}};
    assert(nearestValidPoint(2, 2, points3) == 0);
    
    // No valid point
    std::vector<std::vector<int>> points4 = {{0, 0}, {5, 5}};
    assert(nearestValidPoint(1, 1, points4) == -1);
    
    // Empty list
    std::vector<std::vector<int>> points5;
    assert(nearestValidPoint(1, 1, points5) == -1);
    
    // Point exactly at query
    std::vector<std::vector<int>> points6 = {{2, 3}, {2, 5}};
    assert(nearestValidPoint(2, 3, points6) == 0);
    
    // All valid, but only one is nearest
    std::vector<std::vector<int>> points7 = {{0, 2}, {2, 0}, {2, 2}};
    assert(nearestValidPoint(2, 2, points7) == 2);
    
    return 0;
}
