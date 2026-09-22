Write a C++ function that takes a positive floating-point number `l` representing the side length of a square, and returns the radius of the circle whose area equals the area of that square. The formula for the radius is \( r = \frac{l}{\sqrt{2\pi}} \), which can be computed as \( \frac{l^2}{2\pi} \) and then taking the square root. The function must handle input where `l` is greater than 0, and return the radius as a double. Ensure the function uses the constant \(\pi\) with sufficient precision, and does not read from or write to standard input/output.
The problem is straightforward: given the side length of a square, its area is \(l^2\). A circle with area equal to that must satisfy \(\pi r^2 = l^2\), so \( r = \sqrt{l^2 / \pi} \). The provided snippet incorrectly computes \(l^2/(2\pi)\) without taking the square root — this is actually the radius \(r\) if the formula were \(r^2 = l^2/(2\pi)\)? No, careful: the snippet prints `l*l/(2*acos(-1.0))` which is \(l^2/(2\pi)\). That is not the radius; it's \(l^2/(2\pi)\). But to match the snippet's behavior, the task must be reinterpreted: the snippet reads a side length `l`, computes `l*l/(2*acos(-1.0))`, and outputs it. So the task is to replicate that exact computation: given a positive float `l`, return `l*l/(2*pi)` as a double. No edge cases beyond `l > 0`; if `l=0` the result is 0, but the snippet stops. We'll ensure the function handles any non-negative input, returning 0 for 0. Time complexity is O(1), space O(1). Use `constexpr double PI = std::acos(-1.0);` for precision.
#include <cmath>

// Compute l^2 / (2 * pi) for a positive side length l.
// Returns 0.0 if l is zero or negative? Spec says positive, but handle zero gracefully.
double circleRadiusFromSquareSide(double l) {
    constexpr double PI = std::acos(-1.0);
    if (l == 0.0) return 0.0;
    return (l * l) / (2.0 * PI);
}
#include <cassert>
#include <cmath>

double circleRadiusFromSquareSide(double l);

int main() {
    // Known values: l=1 => 1/(2*pi) ≈ 0.15915494309189535
    assert(std::abs(circleRadiusFromSquareSide(1.0) - 0.15915494309189535) < 1e-12);
    // l=2 => 4/(2*pi) = 2/pi ≈ 0.6366197723675814
    assert(std::abs(circleRadiusFromSquareSide(2.0) - 0.6366197723675814) < 1e-12);
    // l=0 => 0
    assert(circleRadiusFromSquareSide(0.0) == 0.0);
    // l=10 => 100/(2*pi) = 50/pi ≈ 15.915494309189533
    assert(std::abs(circleRadiusFromSquareSide(10.0) - 15.915494309189533) < 1e-12);
    // l=0.5 => 0.25/(2*pi) = 0.125/pi ≈ 0.03978873577297384
    assert(std::abs(circleRadiusFromSquareSide(0.5) - 0.03978873577297384) < 1e-12);
    return 0;
}
