/*
Write a C++ function `std::string triangleType(int a, int b, int c)` that accepts three integer side lengths and returns a string describing the triangle: `"khong phai tam giac"` if the sides cannot form a valid triangle (any side ≤ 0 or the sum of the two smaller sides is ≤ the largest side), `"tam giac deu"` for equilateral, `"tam giac can"` for isosceles (exactly two equal sides), `"tam giac vuong"` for a right triangle, and `"tam giac thuong"` otherwise (scalene acute/obtuse). The function must treat side order arbitrarily (e.g., `(3,4,5)` and `(5,3,4)` both yield `"tam giac vuong"`). The function should not read or write from standard input/output; it only computes based on the parameters.
*/

#include <string>
#include <algorithm>

// Classify a triangle given three side lengths.
// Returns one of: "khong phai tam giac", "tam giac deu",
// "tam giac can", "tam giac vuong", "tam giac thuong".
std::string triangleType(int a, int b, int c) {
    // Sort sides so that a <= b <= c
    int Min = std::min({a, b, c});
    int Max = std::max({a, b, c});
    int Mid = a + b + c - Min - Max;
    a = Min;
    b = Mid;
    c = Max;

    // Invalid triangle: non-positive side or degenerate (sum of two <= third)
    if (a <= 0 || b <= 0 || c <= 0 || a + b <= c) {
        return "khong phai tam giac";
    }

    // Equilateral
    if (a == b && b == c) {
        return "tam giac deu";
    }

    // Isosceles (exactly two equal sides)
    if (a == b || b == c) {
        return "tam giac can";
    }

    // Right triangle (use long long to avoid overflow)
    long long A = static_cast<long long>(a);
    long long B = static_cast<long long>(b);
    long long C = static_cast<long long>(c);
    if (A * A + B * B == C * C) {
        return "tam giac vuong";
    }

    // Ordinary scalene triangle
    return "tam giac thuong";
}

#include <cassert>
#include <string>

// Forward declaration of the solution function
std::string triangleType(int a, int b, int c);

int main() {
    // Valid triangles
    assert(triangleType(3, 4, 5) == "tam giac vuong");
    assert(triangleType(5, 3, 4) == "tam giac vuong");  // order irrelevant
    assert(triangleType(7, 7, 7) == "tam giac deu");
    assert(triangleType(5, 5, 3) == "tam giac can");
    assert(triangleType(2, 3, 4) == "tam giac thuong");
    assert(triangleType(10, 6, 8) == "tam giac vuong"); // largest not last

    // Invalid triangles
    assert(triangleType(1, 2, 3) == "khong phai tam giac"); // degenerate
    assert(triangleType(0, 5, 5) == "khong phai tam giac"); // zero side
    assert(triangleType(-1, 2, 3) == "khong phai tam giac"); // negative side
    assert(triangleType(1, 1, 0) == "khong phai tam giac"); // zero side, out of order

    return 0;
}

// The algorithm first sorts the three side lengths into ascending order: find the minimum, maximum, and the middle value by subtracting the min and max from the total sum. This gives `a ≤ b ≤ c`. A valid triangle requires all sides positive and the sum of the two smaller sides strictly greater than the largest side (`a + b > c`). If invalid, return the "not a triangle" string. Otherwise, classify: if all three equal → equilateral; else if exactly two equal (`a == b` or `b == c` — note `a==c` would imply all equal, already handled) → isosceles; else if `a*a + b*b == c*c` (using integer arithmetic to avoid floating‑point errors) → right; else → ordinary. Edge cases include very large integers (use `long long` internally for squares to prevent overflow), zero or negative inputs, and degenerate cases where `a+b == c` (invalid). Time complexity is O(1) (constant number of comparisons and arithmetic operations); space complexity is O(1) auxiliary.
