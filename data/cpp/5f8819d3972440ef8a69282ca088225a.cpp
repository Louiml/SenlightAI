// Write a C++ function that takes a vector of 2D points (each with integer x and y coordinates) representing a sequence of points along a path, and returns a new vector containing only the unique points in their original relative order. A point is considered a duplicate of an earlier point if both its x and y coordinates are within 0.001 (i.e., absolute differences are less than 0.001). The function should remove all such duplicate points and return the cleaned list. The input vector may be empty, and the input coordinates can be any integers (including negatives). The output must preserve the first occurrence order of each unique point.
#include <cassert>
#include <vector>

// Point struct and pruneDuplicatePoints function are defined above.

int main() {
    // Empty input returns empty.
    std::vector<Point> empty;
    assert(pruneDuplicatePoints(empty).empty());

    // No duplicates.
    std::vector<Point> unique = {{1, 2}, {3, 4}, {5, 6}};
    auto result1 = pruneDuplicatePoints(unique);
    assert(result1.size() == 3);
    assert(result1[0].x == 1 && result1[0].y == 2);
    assert(result1[1].x == 3 && result1[1].y == 4);
    assert(result1[2].x == 5 && result1[2].y == 6);

    // Duplicate points adjacent and non-adjacent.
    std::vector<Point> withDup = {{10, 20}, {10, 20}, {11, 20}, {10, 20}, {12, 22}};
    auto result2 = pruneDuplicatePoints(withDup);
    assert(result2.size() == 3);
    assert(result2[0].x == 10 && result2[0].y == 20);
    assert(result2[1].x == 11 && result2[1].y == 20);
    assert(result2[2].x == 12 && result2[2].y == 22);

    // Points within 0.001 difference (e.g., (0,0) and (0,0) same; (0,0) and (0,1) not).
    std::vector<Point> nearDup = {{0, 0}, {0, 0}, {0, 1}};
    auto result3 = pruneDuplicatePoints(nearDup);
    assert(result3.size() == 2);
    assert(result3[0].x == 0 && result3[0].y == 0);
    assert(result3[1].x == 0 && result3[1].y == 1);

    // Negative coordinates.
    std::vector<Point> neg = {{-5, -3}, {-5, -3}, {-4, -3}};
    auto result4 = pruneDuplicatePoints(neg);
    assert(result4.size() == 2);
    assert(result4[0].x == -5 && result4[0].y == -3);
    assert(result4[1].x == -4 && result4[1].y == -3);

    // All duplicates.
    std::vector<Point> allDup = {{7, 9}, {7, 9}, {7, 9}};
    auto result5 = pruneDuplicatePoints(allDup);
    assert(result5.size() == 1);
    assert(result5[0].x == 7 && result5[0].y == 9);

    return 0;
}
#include <vector>
#include <cmath>

// A simple 2D point structure with integer coordinates.
struct Point {
    int x;
    int y;
};

/**
 * Removes duplicate points from a path, keeping the first occurrence order.
 * Two points are considered duplicates if their x and y coordinates differ by less than 0.001.
 * 
 * @param points The input vector of points (may be empty).
 * @return A new vector containing only unique points in their original order.
 */
std::vector<Point> pruneDuplicatePoints(const std::vector<Point>& points) {
    std::vector<Point> uniquePoints;
    
    for (const auto& p : points) {
        bool isDuplicate = false;
        for (const auto& existing : uniquePoints) {
            if (std::abs(existing.x - p.x) < 0.001 && std::abs(existing.y - p.y) < 0.001) {
                isDuplicate = true;
                break;
            }
        }
        if (!isDuplicate) {
            uniquePoints.push_back(p);
        }
    }
    
    return uniquePoints;
}
// The solution iterates through the input vector of points, maintaining a list of already-accepted unique points. For each new point, we check it against all previously accepted points using the duplicate criterion: if both `abs(new.x - accepted.x) < 0.001` and `abs(new.y - accepted.y) < 0.001`, then it's considered duplicate and skipped. Otherwise, it's appended to the result. This is a simple O(n²) approach in the worst case (when all points are unique), and O(n) time when all points are identical, which is acceptable for typical path sizes. Space complexity is O(n) for the output vector. Edge cases: empty input returns an empty vector; points with very large coordinate values still work, though the tolerance is absolute and fixed; duplicates that are not adjacent are still removed because we compare against all previously kept points.
