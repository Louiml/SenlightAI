// Given a 2D point represented as a struct with `double x` and `double y` members, and a weight factor `wx` for the x-coordinate and `wy` for the y-coordinate, write a C++ function `double weightedPointDistance(const Point& a, const Point& b, double wx, double wy)` that computes the Euclidean distance scaled by coordinate-specific weights: `sqrt(wx * (a.x - b.x)^2 + wy * (a.y - b.y)^2)`. The function must handle negative weights by treating them as absolute values, and must return `0.0` if the computed value is negative due to floating-point rounding (though mathematically it won't be, but guard for safety). This function will be used to grade split lengths in a character recognition system, so it must be robust and const-correct.

The solution is straightforward: compute the squared differences in x and y, multiply each by the corresponding weight (taking absolute value to ensure non-negative contribution), sum them, and take the square root. However, due to floating-point imprecision, the sum might be slightly negative if weights are negative and the differences are tiny, so clamp to zero before taking the square root. Edge cases include identical points (returns 0), zero weights (returns 0), and negative weights (use absolute). The time complexity is O(1) and space complexity is O(1). The function should be `const`-qualified because it doesn't modify inputs, and the parameters should be passed by const reference for efficiency.

#include <cmath>
#include <cstdlib>

// Simple 2D point struct
struct Point {
    double x;
    double y;
};

// Compute weighted Euclidean distance between two points.
// The weight factors wx and wy scale the x and y squared differences.
// Negative weights are treated as their absolute values.
double weightedPointDistance(const Point& a, const Point& b, double wx, double wy) {
    // Use absolute weights to ensure non-negative contribution
    double ax = std::abs(wx);
    double ay = std::abs(wy);
    
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    
    double weighted_sq = ax * dx * dx + ay * dy * dy;
    
    // Guard against tiny negative values from floating-point rounding
    if (weighted_sq < 0.0) {
        return 0.0;
    }
    
    return std::sqrt(weighted_sq);
}

#include <cassert>
#include <cmath>

// Point struct definition should be above this in real code, but for test completeness:
struct Point {
    double x;
    double y;
};

double weightedPointDistance(const Point& a, const Point& b, double wx, double wy);

int main() {
    Point p1{0.0, 0.0};
    Point p2{3.0, 4.0};
    
    // Standard weights: sqrt(1*9 + 1*16) = 5
    assert(std::abs(weightedPointDistance(p1, p2, 1.0, 1.0) - 5.0) < 1e-9);
    
    // Weighted x only: sqrt(4*9 + 0*16) = 6
    Point p3{0.0, 0.0};
    Point p4{3.0, 4.0};
    assert(std::abs(weightedPointDistance(p3, p4, 4.0, 0.0) - 6.0) < 1e-9);
    
    // Negative weights should be treated as absolute: sqrt(1*9 + 1*16) = 5
    assert(std::abs(weightedPointDistance(p1, p2, -1.0, -1.0) - 5.0) < 1e-9);
    
    // Identical points
    assert(weightedPointDistance(p1, p1, 100.0, -100.0) == 0.0);
    
    // Zero weights
    assert(weightedPointDistance(p1, p2, 0.0, 0.0) == 0.0);
    
    // Asymmetric weights: sqrt(2*9 + 3*16) = sqrt(18+48)=sqrt(66)
    double expected = std::sqrt(66.0);
    assert(std::abs(weightedPointDistance(p1, p2, 2.0, 3.0) - expected) < 1e-9);
    
    // Negative source coordinates with positive weights
    Point p5{-1.0, -2.0};
    Point p6{2.0, 2.0};
    // dx = -3, dy = -4 => same as standard: 5
    assert(std::abs(weightedPointDistance(p5, p6, 1.0, 1.0) - 5.0) < 1e-9);
    
    // Very small values to test rounding guard
    Point p7{0.0, 0.0};
    Point p8{1e-10, 1e-10};
    // Weighted sq = 1e-20 + 1e-20 = 2e-20, sqrt ~ 1.414e-10
    double val = weightedPointDistance(p7, p8, 1.0, 1.0);
    assert(val >= 0.0);
    assert(std::abs(val - std::sqrt(2e-20)) < 1e-15);
    
    return 0;
}
