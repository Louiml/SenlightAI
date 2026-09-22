// Write a C++ function named `euclideanDistance` that takes four floating-point parameters representing two points in a 2D plane: `x1`, `y1` for the first point and `x2`, `y2` for the second point. The function must return the Euclidean distance between these two points as a `float`. The computation should follow the formula: sqrt((x2 - x1)² + (y2 - y1)²). The function must be robust for negative coordinates, zero distances (when both points are identical), and handle floating-point precision appropriately. Do not include a `main` function in your solution; provide only the function implementation with appropriate headers and `const` correctness.

// The solution directly applies the Euclidean distance formula. The main algorithm involves subtracting the x-coordinates, squaring the difference (using multiplication to avoid potential issues with `pow`), doing the same for y-coordinates, summing the squares, and taking the square root via `std::sqrt` from `<cmath>`. Important edge cases include: when both points are identical, the answer is `0.0f`; negative differences are handled naturally because squaring removes the sign; large coordinate values may cause overflow in single precision `float` if not careful, but for typical inputs this is acceptable. The time complexity is O(1) since only a fixed number of arithmetic operations are performed, and the space complexity is O(1) as only a few temporary variables are used. The function parameters should be passed by value (as they are primitive types) and marked `const` to prevent accidental modification.

#include <cmath>

// Compute the Euclidean distance between two points (x1, y1) and (x2, y2).
float euclideanDistance(const float x1, const float y1, const float x2, const float y2) {
    const float dx = x2 - x1;
    const float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

#include <cassert>
#include <cmath>

float euclideanDistance(const float x1, const float y1, const float x2, const float y2);

int main() {
    // Identical points -> distance zero
    assert(euclideanDistance(0.0f, 0.0f, 0.0f, 0.0f) == 0.0f);
    
    // Horizontal distance
    assert(euclideanDistance(1.0f, 2.0f, 4.0f, 2.0f) == 3.0f);
    
    // Vertical distance
    assert(euclideanDistance(3.0f, -1.0f, 3.0f, 5.0f) == 6.0f);
    
    // Classic 3-4-5 triangle
    assert(euclideanDistance(0.0f, 0.0f, 3.0f, 4.0f) == 5.0f);
    
    // Negative coordinates
    assert(euclideanDistance(-1.0f, -1.0f, -4.0f, -5.0f) == 5.0f);
    
    // Floating-point tolerance for non-perfect square results
    const float result = euclideanDistance(0.0f, 0.0f, 1.0f, 1.0f);
    assert(std::fabs(result - std::sqrt(2.0f)) < 1e-6f);
    
    // Large coordinate values (still within float range)
    assert(euclideanDistance(-1000.0f, 2000.0f, 1000.0f, 2000.0f) == 2000.0f);
    
    return 0;
}
