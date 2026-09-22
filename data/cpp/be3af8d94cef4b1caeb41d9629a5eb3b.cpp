/*
Write a C++ function that takes two circles represented by their center coordinates (x, y) and radius, and determines whether they overlap. Two circles overlap if the distance between their centers is less than the sum of their radii. The function should return `true` if the circles intersect or one is fully inside the other (i.e., distance ≤ sum of radii), and `false` otherwise. Handle cases where circles are identical, tangent (distance equals sum of radii, which counts as overlap), or completely separated. Use floating-point arithmetic with double precision and compare with a small epsilon to avoid precision issues.
*/

#include <cmath>
#include <limits>

/**
 * Determines whether two circles overlap.
 * @param x1, y1: center of first circle
 * @param r1: radius of first circle (non-negative)
 * @param x2, y2: center of second circle
 * @param r2: radius of second circle (non-negative)
 * @return true if distance between centers <= r1 + r2 (with epsilon), false otherwise
 */
bool circlesOverlap(double x1, double y1, double r1, double x2, double y2, double r2) {
    // Use squared distance to avoid sqrt
    double dx = x1 - x2;
    double dy = y1 - y2;
    double distSq = dx * dx + dy * dy;
    
    double rSum = r1 + r2;
    double threshold = rSum * rSum;
    
    // Add a small epsilon to handle floating-point rounding near tangency
    double epsilon = std::numeric_limits<double>::epsilon() * 1e6;
    return distSq <= threshold + epsilon;
}

#include <cassert>
#include <cmath>

int main() {
    // Basic overlap
    assert(circlesOverlap(0.0, 0.0, 1.0, 3.0, 0.0, 1.0) == false); // distance 3 > 2
    assert(circlesOverlap(0.0, 0.0, 1.0, 1.0, 0.0, 1.0) == true);   // distance 1 <= 2
    // Tangent circles (distance == sum of radii) should overlap
    assert(circlesOverlap(0.0, 0.0, 1.0, 2.0, 0.0, 1.0) == true);   // exactly tangent
    // One circle inside another
    assert(circlesOverlap(0.0, 0.0, 5.0, 1.0, 0.0, 1.0) == true);   // inside
    // Identical circles
    assert(circlesOverlap(2.0, -1.0, 3.0, 2.0, -1.0, 3.0) == true);
    // Zero-radius circles at same point
    assert(circlesOverlap(0.0, 0.0, 0.0, 0.0, 0.0, 0.0) == true);   // points coincide
    // Zero-radius circles at different points
    assert(circlesOverlap(0.0, 0.0, 0.0, 5.0, 0.0, 0.0) == false);  // points apart
    // Negative coordinates
    assert(circlesOverlap(-2.0, -3.0, 4.0, 1.0, 2.0, 5.0) == true); // distance sqrt(25+25)=~7.07 <= 9
    // Nearly tangent with floating-point error (using exact expression)
    double dx = 1e-8;
    assert(circlesOverlap(0.0, 0.0, 1.0, 2.0 + 1e-8, 0.0, 1.0) == true); // dist ≈ 2 + 1e-8, sum=2, epsilon handles
    // Non-overlapping large distance
    assert(circlesOverlap(0.0, 0.0, 1.0, 100.0, 0.0, 1.0) == false);
    return 0;
}

// The solution computes the squared distance between the two centers to avoid an unnecessary square root for comparison. Let `dx = x1 - x2`, `dy = y1 - y2`, then `distSq = dx*dx + dy*dy`. The sum of radii `rSum = r1 + r2`. The condition for overlap is `distSq <= rSum * rSum` (using squared values to avoid sqrt). However, due to floating-point inaccuracies, add a small epsilon tolerance: `distSq <= rSum*rSum + epsilon`. Use `std::numeric_limits<double>::epsilon()` multiplied by a factor (e.g., 1e-9) to handle near-tangency cases. Edge cases: identical circles (distSq = 0, rSum > 0 → overlap), zero-radius circles (single points) – overlap if centers coincide, tangent circles where distSq ≈ rSum² → should return true. Time complexity: O(1), space complexity: O(1).
