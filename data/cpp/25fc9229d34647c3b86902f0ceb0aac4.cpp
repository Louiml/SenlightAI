/*
Given four points A, B, C, D in the plane (with coordinates specified as floating-point numbers), write a C++ function that determines whether they form a parallelogram (with A, B, C, D appearing in any cyclic order around the parallelogram, but not necessarily in that order as vertices) and returns the coordinates of the missing fourth vertex if three of the four points form three vertices of a parallelogram where the fourth point is one of the given points. More precisely, among the four given points, exactly three are vertices of a parallelogram, and the fourth given point is the missing vertex? Actually, the problem is: the four given points are the four vertices of a parallelogram, but one of them is missing? Let me re-read the snippet: The code reads four points, and if a==c or a==d or b==c or b==d, it computes the fourth vertex of a parallelogram using three points. The intended task: Given four points, determine if they are exactly three distinct vertices of a parallelogram (with one point duplicated? No, the code checks if any two points coincide, and if so, treats them as three vertices, finding the fourth). Actually the standard problem: Given three points that are three vertices of a parallelogram, find the fourth vertex. Here the input has four points, but two of them are identical (one vertex appears twice). So the function should take four points (where two are equal, representing the three distinct vertices) and return the coordinates of the fourth vertex. If no two points match, the input is invalid; in that case, you may return the point (0,0) or handle as specified. Write a function `Point findFourthVertex(const Point& a, const Point& b, const Point& c, const Point& d)` that returns the missing vertex. Use double precision, output with fixed 3 decimal places in the test? The output precision is handled in test, not in function. The function returns a `Point` struct with `double x, y`. The function must be const-correct and include necessary headers. The test should use `assert` with a tolerance for floating-point comparison, e.g., `assert(fabs(result.x - expected_x) < 1e-9)`. Assume input points are given as `std::pair<double,double>` or a custom struct. Provide the function only (no main). The main in test will call it.
*/
#include <cmath>
#include <cstddef>

struct Point {
    double x, y;

    Point operator+(const Point& other) const {
        return {x + other.x, y + other.y};
    }

    Point operator-(const Point& other) const {
        return {x - other.x, y - other.y};
    }

    bool operator==(const Point& other) const {
        const double eps = 1e-9;
        return std::fabs(x - other.x) < eps && std::fabs(y - other.y) < eps;
    }
};

// Given four points where exactly one pair is equal (representing three distinct vertices of a parallelogram),
// return the fourth vertex. If no pair matches, returns (0,0).
Point findFourthVertex(const Point& a, const Point& b, const Point& c, const Point& d) {
    if (a == c) {
        return d + b - a;  // a and c are the same, distinct are a,b,d
    }
    if (a == d) {
        return c + b - a;  // a and d are the same, distinct are a,b,c
    }
    if (b == c) {
        return d + a - b;  // b and c are the same, distinct are a,b,d
    }
    if (b == d) {
        return c + a - b;  // b and d are the same, distinct are a,b,c
    }
    return {0.0, 0.0};  // invalid input
}
#include <cassert>
#include <cmath>

int main() {
    using std::fabs;

    // Test 1: a and c are equal
    Point a1{0.0, 0.0}, b1{2.0, 0.0}, c1{0.0, 0.0}, d1{1.0, 1.0};
    Point res1 = findFourthVertex(a1, b1, c1, d1);
    assert(fabs(res1.x - 3.0) < 1e-9 && fabs(res1.y - 1.0) < 1e-9);

    // Test 2: a and d are equal
    Point a2{0.0, 0.0}, b2{2.0, 0.0}, c2{1.0, 1.0}, d2{0.0, 0.0};
    Point res2 = findFourthVertex(a2, b2, c2, d2);
    assert(fabs(res2.x - 3.0) < 1e-9 && fabs(res2.y - 1.0) < 1e-9);

    // Test 3: b and c are equal
    Point a3{0.0, 0.0}, b3{2.0, 0.0}, c3{2.0, 0.0}, d3{1.0, 1.0};
    Point res3 = findFourthVertex(a3, b3, c3, d3);
    assert(fabs(res3.x - -1.0) < 1e-9 && fabs(res3.y - 1.0) < 1e-9);

    // Test 4: b and d are equal
    Point a4{0.0, 0.0}, b4{2.0, 0.0}, c4{1.0, 1.0}, d4{2.0, 0.0};
    Point res4 = findFourthVertex(a4, b4, c4, d4);
    assert(fabs(res4.x - -1.0) < 1e-9 && fabs(res4.y - 1.0) < 1e-9);

    // Test 5: No pair equal (invalid input) returns (0,0)
    Point a5{0.0, 0.0}, b5{1.0, 0.0}, c5{2.0, 0.0}, d5{3.0, 0.0};
    Point res5 = findFourthVertex(a5, b5, c5, d5);
    assert(fabs(res5.x) < 1e-9 && fabs(res5.y) < 1e-9);

    // Test 6: Negative coordinates
    Point a6{-1.0, -1.0}, b6{1.0, -1.0}, c6{-1.0, -1.0}, d6{0.0, 2.0};
    Point res6 = findFourthVertex(a6, b6, c6, d6);
    assert(fabs(res6.x - 2.0) < 1e-9 && fabs(res6.y - 2.0) < 1e-9);

    return 0;
}
// The key observation is that in a parallelogram, the diagonals bisect each other. If we have three distinct vertices, say P, Q, R, the fourth vertex is either P+Q-R, P+R-Q, or Q+R-P, depending on which two are opposite. In the given code, the input contains four points where one point is repeated (e.g., a==c). That means a and c are the same vertex, so the three distinct vertices are a (or c), b, and d. To find the fourth vertex, we treat a as the common vertex (the one that appears twice) and use the formula: if a==c, then the three distinct are a, b, d. The fourth vertex is d + b - a. Similarly, if a==d, distinct are a, b, c, fourth is c + b - a. If b==c, distinct are a, b, d, fourth is d + a - b. If b==d, distinct are a, b, c, fourth is c + a - b. If no two points match, the input is invalid; the function can return the zero point. The algorithm simply checks pairwise equality and applies the appropriate vector addition. Time complexity O(1) and space O(1). Edge cases: floating-point equality is problematic; in practice, we compare with a small epsilon. The original snippet uses exact equality, but for a robust solution, we can use approximate equality with a tolerance (e.g., 1e-9). The function must return a point; we can include an epsilon-based comparison to determine which pair matches. If none match, return a point with NaN or (0,0) as specified.
