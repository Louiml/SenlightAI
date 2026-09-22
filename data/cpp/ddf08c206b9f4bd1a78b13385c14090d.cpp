/*
Write a C++ function that takes two three-dimensional vectors represented as `Eigen::Vector3d` objects and returns a `std::string` containing their dot product, cross product vector (as three components separated by commas), and the scalar result of the inner product via matrix multiplication (`v.adjoint() * w`), each on a separate line. The output format must be: first line the dot product as an integer if it is whole, otherwise with one decimal place; second line the cross product components in the form `(x, y, z)` with one decimal place each; third line the inner product as computed by `v.adjoint() * w`, formatted identically to the dot product. Assume vectors are non-empty and of size exactly 3. Handle negative values and zeros correctly—no division is involved, but ensure formatting adjusts to whole numbers.
*/
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <Eigen/Dense>

// Format a double to a string: whole numbers without decimal, otherwise one decimal place.
std::string formatNumber(double value) {
    std::ostringstream oss;
    if (std::floor(value) == value) {
        oss << std::fixed << std::setprecision(0) << value;
    } else {
        oss << std::fixed << std::setprecision(1) << value;
    }
    return oss.str();
}

// Compute dot, cross, and inner product for two 3D vectors and return a formatted string.
std::string vectorProducts(const Eigen::Vector3d& v, const Eigen::Vector3d& w) {
    const double dot = v.dot(w);
    const Eigen::Vector3d cross = v.cross(w);
    const double inner = (v.adjoint() * w).coeff(0, 0);

    std::ostringstream oss;
    oss << formatNumber(dot) << "\n";
    oss << "(" << formatNumber(cross.x()) << ", "
                << formatNumber(cross.y()) << ", "
                << formatNumber(cross.z()) << ")\n";
    oss << formatNumber(inner);
    return oss.str();
}
#include <cassert>
#include <string>
#include <Eigen/Dense>

// Declaration of the function under test (must be included or defined above).
std::string vectorProducts(const Eigen::Vector3d& v, const Eigen::Vector3d& w);

int main() {
    // Example from the snippet: v=(1,2,3), w=(0,1,2)
    Eigen::Vector3d v(1, 2, 3);
    Eigen::Vector3d w(0, 1, 2);
    assert(vectorProducts(v, w) == "8\n(-1, -2, 1)\n8");

    // Perpendicular vectors: dot=0, cross non-zero
    Eigen::Vector3d a(1, 0, 0);
    Eigen::Vector3d b(0, 1, 0);
    assert(vectorProducts(a, b) == "0\n(0, 0, 1)\n0");

    // Parallel vectors: cross is zero vector
    Eigen::Vector3d c(2, 4, 6);
    Eigen::Vector3d d(1, 2, 3);
    assert(vectorProducts(c, d) == "28\n(0, 0, 0)\n28");

    // Negative and zero components
    Eigen::Vector3d e(-1.5, 0, 2);
    Eigen::Vector3d f(0.5, -3, 0);
    // dot = -1.5*0.5 + 0*(-3) + 2*0 = -0.75, cross = (0*0 - 2*(-3), 2*0.5 - (-1.5)*0, (-1.5)*(-3) - 0*0.5) = (6, 1, 4.5)
    // inner = same as dot
    assert(vectorProducts(e, f) == "-0.8\n(6.0, 1.0, 4.5)\n-0.8");

    // All zeros
    Eigen::Vector3d g(0, 0, 0);
    Eigen::Vector3d h(1, 2, 3);
    assert(vectorProducts(g, h) == "0\n(0, 0, 0)\n0");
    
    return 0;
}
// The solution involves computing three quantities using Eigen’s built-in operations: `v.dot(w)` for the dot product, `v.cross(w)` for the cross product, and `v.adjoint() * w` for the inner product via matrix multiplication. Since `v.adjoint() * w` yields a 1x1 matrix, we extract its scalar value using `operator()` or `coeff(0,0)`. For formatting, use `std::ostringstream` to build output line by line. To handle whole numbers vs. decimals, check if the value is an integer by comparing `value == std::floor(value)`; if so, print with `std::fixed` and `std::setprecision(0)` to avoid a trailing ".0", otherwise print with `std::fixed` and `std::setprecision(1)` to one decimal place. Edge cases include zero components, negative numbers, and when the dot product or inner product is exactly whole (e.g., 4.0 should print as "4"). Time complexity is O(1) because vectors are fixed-size; space complexity is O(1) aside from the returned string.
