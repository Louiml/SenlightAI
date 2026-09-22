/*
Write a C++ function `classifySideLengths(double a, double b, double c)` that takes three positive side lengths and returns a string. If the three lengths can form a valid triangle (the sum of any two sides is strictly greater than the third side), return the perimeter formatted as "Perimetro = X" where X is the sum of the three sides formatted to one decimal place. If they cannot form a triangle, return the area of a trapezoid with bases (a+b) and c, and height (a+b+c)/2? Actually the original code computes area as ((a+b)*c)/2, but that formula is not a standard trapezoid — it's a simple expression from the original code. Preserve that exact formula: if not a triangle, return "Area = Y" where Y = ((a+b)*c)/2 formatted to one decimal place. The function must return a `std::string` with fixed notation and exactly one digit after the decimal point (use `std::fixed` and `std::setprecision(1)` from `<iomanip>`). Assume inputs are positive finite doubles.
*/

#include <string>
#include <sstream>
#include <iomanip>

// Classify three side lengths as triangle or trapezoid-like area.
// Returns a string with fixed one-decimal formatting.
std::string classifySideLengths(double a, double b, double c) {
    // Check triangle inequality: all pair sums must be strictly greater
    // than the third side. If any is <=, then not a triangle.
    if ((a + b) <= c || (a + c) <= b || (b + c) <= a) {
        // Not a triangle -> compute area using the given original formula
        double area = ((a + b) * c) / 2.0;
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << "Area = " << area;
        return oss.str();
    } else {
        // Valid triangle -> compute perimeter
        double perimeter = a + b + c;
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << "Perimetro = " << perimeter;
        return oss.str();
    }
}

#include <cassert>
#include <string>

// The solution function is declared above; include it for testing.
// (For brevity, this test assumes the function is already defined.)

int main() {
    // Valid triangle: 3,4,5 -> perimeter 12.0
    assert(classifySideLengths(3.0, 4.0, 5.0) == "Perimetro = 12.0");
    // Invalid: 1,2,3 -> area ((1+2)*3)/2 = 4.5
    assert(classifySideLengths(1.0, 2.0, 3.0) == "Area = 4.5");
    // Invalid: 5,5,10 -> area ((5+5)*10)/2 = 50.0
    assert(classifySideLengths(5.0, 5.0, 10.0) == "Area = 50.0");
    // Valid: equilateral triangle side 2 -> perimeter 6.0
    assert(classifySideLengths(2.0, 2.0, 2.0) == "Perimetro = 6.0");
    // Valid: isosceles 2,3,2 -> perimeter 7.0
    assert(classifySideLengths(2.0, 3.0, 2.0) == "Perimetro = 7.0");
    // Invalid: 1.5, 1.5, 3.0 -> sum 1.5+1.5=3.0 equals third -> area ((1.5+1.5)*3)/2 = 4.5
    assert(classifySideLengths(1.5, 1.5, 3.0) == "Area = 4.5");
    // Valid: 0.5, 0.6, 0.8 -> perimeter 1.9
    assert(classifySideLengths(0.5, 0.6, 0.8) == "Perimetro = 1.9");
    // Invalid: 10, 20, 30 -> area ((10+20)*30)/2 = 450.0
    assert(classifySideLengths(10.0, 20.0, 30.0) == "Area = 450.0");
    // Valid: 7, 8, 10 -> perimeter 25.0
    assert(classifySideLengths(7.0, 8.0, 10.0) == "Perimetro = 25.0");
    // Invalid: 1, 1, 2.1 -> area ((1+1)*2.1)/2 = 2.1
    assert(classifySideLengths(1.0, 1.0, 2.1) == "Area = 2.1");
    return 0;
}

// The solution checks the triangle inequality: for a valid triangle, each pair sum must be strictly greater than the third side. If any pair sum is less than or equal to the third side, the condition fails. The original code uses `<=` for failure, so exactly that logic is preserved. If the condition fails, compute the area using the exact expression ((a+b)*c)/2 and format it with one decimal place. If the condition holds, compute perimeter a+b+c and format similarly. Edge cases: when the three sides are just touching (e.g., 1, 2, 3) the sum 1+2 equals 3, so it is not a triangle and the area formula is used. Also handle very small/large doubles without overflow by using double arithmetic (which is fine for typical test inputs). Time complexity is O(1) because only a few arithmetic operations and comparisons are performed. Space complexity is O(1) auxiliary, excluding the returned string.
