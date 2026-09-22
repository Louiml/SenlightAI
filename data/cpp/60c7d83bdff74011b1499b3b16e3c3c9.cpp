/*
Create a C++ class `Quadratic` that represents a quadratic polynomial \( ax^2 + bx + c \) with three private `double` coefficients. Implement a default constructor (initializing all coefficients to 0), a parameterized constructor, and a method `double vertexDistance() const` that returns the Euclidean distance from the vertex of the parabola to the origin. The vertex’s x-coordinate is \( -b/(2a) \) and its y-coordinate is \( (b^2 - 4ac)/(4a) \). If \( a = 0 \), the polynomial is linear, so the method should return `-1.0` to indicate an invalid parabola (no unique vertex). Additionally, implement a free function `Quadratic addQuadratic(const Quadratic& lhs, const Quadratic& rhs)` that returns a new `Quadratic` whose coefficients are the sum of the corresponding coefficients of the inputs. The solution must be robust and fully const-correct, with no reliance on user input or output inside the class except for optional debug printing.
*/

#include <cmath>
#include <stdexcept>

class Quadratic {
public:
    // Default constructor: all coefficients zero.
    Quadratic() : a_(0.0), b_(0.0), c_(0.0) {}
    
    // Parameterized constructor.
    Quadratic(double a, double b, double c) : a_(a), b_(b), c_(c) {}
    
    // Accessors.
    double a() const { return a_; }
    double b() const { return b_; }
    double c() const { return c_; }
    
    // Distance from vertex to origin. Returns -1.0 if a == 0 (linear function).
    double vertexDistance() const {
        if (a_ == 0.0) {
            return -1.0;  // No quadratic vertex.
        }
        double x = -b_ / (2.0 * a_);
        double y = (b_ * b_ - 4.0 * a_ * c_) / (4.0 * a_);
        return std::hypot(x, y);  // More stable than sqrt(x*x + y*y).
    }
    
private:
    double a_, b_, c_;
};

// Free function to add two Quadratic objects.
Quadratic addQuadratic(const Quadratic& lhs, const Quadratic& rhs) {
    return Quadratic(lhs.a() + rhs.a(), lhs.b() + rhs.b(), lhs.c() + rhs.c());
}

#include <cassert>
#include <cmath>
#include "quadratic.h"  // Assume the solution is placed in this header.

int main() {
    // Test vertex distance for a simple parabola: f(x) = x^2 - 2x + 1.
    // Vertex: x = 1, y = (4 - 4)/4 = 0, distance = 1.
    Quadratic q1(1.0, -2.0, 1.0);
    assert(std::abs(q1.vertexDistance() - 1.0) < 1e-9);
    
    // Test a=0 case: linear function, expected -1.
    Quadratic q2(0.0, 3.0, -2.0);
    assert(q2.vertexDistance() == -1.0);
    
    // Test vertex at origin: f(x) = x^2, vertex (0,0), distance 0.
    Quadratic q3(1.0, 0.0, 0.0);
    assert(q3.vertexDistance() == 0.0);
    
    // Test negative 'a' and positive 'b': f(x) = -x^2 + 4x - 7.
    // Vertex: x = 2, y = (16 - 28)/(-4) = 3, distance = sqrt(13).
    Quadratic q4(-1.0, 4.0, -7.0);
    assert(std::abs(q4.vertexDistance() - std::sqrt(13.0)) < 1e-9);
    
    // Test addition.
    Quadratic p1(1.0, 2.0, 3.0);
    Quadratic p2(4.0, 5.0, 6.0);
    Quadratic sum = addQuadratic(p1, p2);
    assert(sum.a() == 5.0 && sum.b() == 7.0 && sum.c() == 9.0);
    
    // Test addition with negative coefficients.
    Quadratic p3(-1.0, -2.0, -3.0);
    Quadratic sum2 = addQuadratic(p3, p3);
    assert(sum2.a() == -2.0 && sum2.b() == -4.0 && sum2.c() == -6.0);
}

// The main algorithm is straightforward: for the distance, first check if `a` is zero. If so, return `-1.0` as a sentinel value for an invalid (non-quadratic) case. Otherwise, compute the vertex coordinates using the standard formulas. The distance is the square root of \( x^2 + y^2 \). Use `std::hypot` or `std::sqrt` (prefer `std::hypot` for numerical stability with large values). The addition is a simple element-wise sum. Edge cases: `a = 0` (must handle gracefully), negative coefficients (work fine), very large or small coefficients (use double precision and `std::hypot` to avoid overflow). Time complexity is \( O(1) \) for both operations; space complexity is \( O(1) \) excluding the returned object.
