// Write a C++ function that detects whether a given polygon, represented by a sequence of 2D points with `double` coordinates, is convex. A polygon is convex if all its internal angles are less than 180 degrees, meaning no reflex vertices exist. The polygon vertices are provided in order (clockwise or counterclockwise) and form a simple polygon without self-intersections. Your function should return `true` if the polygon is strictly convex (no collinear consecutive edges), and `false` otherwise. Handle edge cases such as polygons with fewer than 3 vertices (return `false`) and duplicate consecutive vertices (treat as invalid, return `false`). You may assume the polygon is closed—the first and last points are not repeated in the input. The input is a `std::vector<std::pair<double, double>>` where each pair represents `(x, y)`. Use an orientation test based on the cross product to determine convexity, and account for floating-point precision by using a small epsilon (e.g., `1e-9`).

The problem is to determine if a simple polygon is strictly convex. The standard approach is to iterate over every vertex and compute the cross product of the vectors formed by the vertex and its two neighbors: `cross = (p2 - p1) × (p3 - p2) = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x)`. For a convex polygon, all these cross products must have the same sign (all positive for counterclockwise, all negative for clockwise). If any cross product is zero, the polygon has collinear points (not strictly convex). If any cross product has the opposite sign, the polygon is concave. Edge cases: fewer than 3 vertices → return `false`; duplicate consecutive vertices → the cross product with a zero-length edge is zero, causing ambiguity—treat as invalid by checking if any adjacent points are identical (within epsilon) and returning `false`. Use a small epsilon `1e-9` to compare cross products to zero to handle floating-point inaccuracies. Time complexity: O(n) where n is the number of vertices. Space complexity: O(1) auxiliary.

#include <vector>
#include <cmath>
#include <utility>

// Determine if a simple polygon (given in order) is strictly convex.
// Input: vector of (x, y) points representing polygon vertices in order.
// Returns true if the polygon is strictly convex, false otherwise.
bool isStrictlyConvex(const std::vector<std::pair<double, double>>& polygon) {
    const double EPS = 1e-9;
    int n = (int)polygon.size();
    
    // Need at least a triangle
    if (n < 3) return false;
    
    // Check for duplicate consecutive vertices
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        double dx = polygon[i].first - polygon[j].first;
        double dy = polygon[i].second - polygon[j].second;
        if (dx*dx + dy*dy < EPS*EPS) {
            return false;
        }
    }
    
    int sign = 0; // 0 = undetermined, 1 = positive, -1 = negative
    
    for (int i = 0; i < n; ++i) {
        // Three consecutive vertices: p1, p2, p3
        const auto& p1 = polygon[i];
        const auto& p2 = polygon[(i + 1) % n];
        const auto& p3 = polygon[(i + 2) % n];
        
        // Compute cross product of vectors (p2 - p1) and (p3 - p2)
        double cross = (p2.first - p1.first) * (p3.second - p2.second) -
                       (p2.second - p1.second) * (p3.first - p2.first);
        
        // If cross is nearly zero, points are collinear → not strictly convex
        if (std::fabs(cross) < EPS) {
            return false;
        }
        
        int currentSign = (cross > 0) ? 1 : -1;
        
        // If sign has not been set yet, set it
        if (sign == 0) {
            sign = currentSign;
        } else if (sign != currentSign) {
            // Sign changed → concave polygon
            return false;
        }
    }
    
    // All cross products have same sign and none are zero → convex
    return true;
}

#include <cassert>
#include <vector>
#include <utility>

// Forward declaration (included from solution)
bool isStrictlyConvex(const std::vector<std::pair<double, double>>& polygon);

int main() {
    // Simple square (counterclockwise) → convex
    std::vector<std::pair<double, double>> square = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    assert(isStrictlyConvex(square) == true);
    
    // Clockwise square → convex
    std::vector<std::pair<double, double>> square_cw = {{0, 0}, {0, 1}, {1, 1}, {1, 0}};
    assert(isStrictlyConvex(square_cw) == true);
    
    // Concave polygon (arrow shape) → not convex
    std::vector<std::pair<double, double>> concave = {{0, 0}, {2, 0}, {2, 2}, {1, 1}, {0, 2}};
    assert(isStrictlyConvex(concave) == false);
    
    // Triangle → convex
    std::vector<std::pair<double, double>> triangle = {{0, 0}, {1, 0}, {0.5, 1}};
    assert(isStrictlyConvex(triangle) == true);
    
    // Degenerate: only 2 points → not convex
    std::vector<std::pair<double, double>> two_points = {{0, 0}, {1, 0}};
    assert(isStrictlyConvex(two_points) == false);
    
    // Duplicate consecutive vertices → not convex
    std::vector<std::pair<double, double>> dup = {{0, 0}, {1, 0}, {1, 0}, {1, 1}};
    assert(isStrictlyConvex(dup) == false);
    
    // Collinear points (a flat line back and forth) → not strictly convex
    std::vector<std::pair<double, double>> collinear = {{0, 0}, {1, 0}, {2, 0}, {2, 0.5}, {1, 0.5}};
    // This is actually concave, but the main point is not strictly convex
    assert(isStrictlyConvex(collinear) == false);
    
    // Regular pentagon → convex
    std::vector<std::pair<double, double>> pentagon = {
        {0, 1}, {0.951, 0.309}, {0.588, -0.809}, {-0.588, -0.809}, {-0.951, 0.309}
    };
    assert(isStrictlyConvex(pentagon) == true);
    
    // Nearly collinear but convex (small angle) → should be true with epsilon handling
    std::vector<std::pair<double, double>> narrow = {{0, 0}, {1, 0}, {1, 0.0000001}, {0, 1}};
    assert(isStrictlyConvex(narrow) == true);
    
    return 0;
}
