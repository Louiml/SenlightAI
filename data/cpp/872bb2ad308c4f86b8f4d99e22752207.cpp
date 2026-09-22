// Write a C++ function `signedVolume` that, given three 3D points represented as `std::array<double, 3>` (or a simple struct with x, y, z), computes the signed volume of the tetrahedron formed by those three points and the origin (0,0,0). The signed volume is defined as one-sixth of the scalar triple product of the three vectors from the origin to each point: \( V = \frac{1}{6} \cdot (\vec{a} \times \vec{b}) \cdot \vec{c} \). The function must handle degenerate cases where points are collinear with the origin (volume = 0) and must work correctly for negative coordinates (returning negative volumes for certain orientations). The function should be `const`-correct and use no external libraries beyond the standard library.

#include <cassert>
#include <array>
#include <cmath>

int main() {
    // Basic positive volume: points along axes, volume = (1/6)*(1*1*1) = 1/6
    std::array<double, 3> a{1, 0, 0};
    std::array<double, 3> b{0, 1, 0};
    std::array<double, 3> c{0, 0, 1};
    assert(std::abs(signedVolume(a, b, c) - 1.0/6.0) < 1e-12);

    // Negative volume: swap two points
    assert(std::abs(signedVolume(b, a, c) + 1.0/6.0) < 1e-12);

    // Degenerate: points in same plane through origin (z=0 for all)
    std::array<double, 3> d{1, 1, 0};
    std::array<double, 3> e{2, 2, 0};
    std::array<double, 3> f{3, 3, 0};
    assert(std::abs(signedVolume(d, e, f)) < 1e-12);

    // Degenerate: one point is origin (zero vector)
    std::array<double, 3> origin{0, 0, 0};
    assert(std::abs(signedVolume(origin, a, b)) < 1e-12);

    // Points with negative coordinates: parity check
    std::array<double, 3> g{-1, 0, 0};
    std::array<double, 3> h{0, 2, 0};
    std::array<double, 3> i{0, 0, -3};
    // triple product = (-1)*(2)*(-3) = 6, /6 = 1
    assert(std::abs(signedVolume(g, h, i) - 1.0) < 1e-12);

    // Large values, check scaling
    std::array<double, 3> j{1e6, 0, 0};
    std::array<double, 3> k{0, 1e6, 0};
    std::array<double, 3> l{0, 0, 1e6};
    double expected = 1e18 / 6.0;
    assert(std::abs(signedVolume(j, k, l) - expected) < expected * 1e-12);

    // Collinear points with origin
    std::array<double, 3> m{1, 1, 1};
    std::array<double, 3> n{2, 2, 2};
    std::array<double, 3> o{3, 3, 3};
    assert(std::abs(signedVolume(m, n, o)) < 1e-12);
}

#include <array>
#include <cmath>

// Compute signed volume of tetrahedron formed by origin and three points.
// Volume = (1/6) * (a × b) · c, where a, b, c are vectors from origin to points.
double signedVolume(const std::array<double, 3>& a,
                    const std::array<double, 3>& b,
                    const std::array<double, 3>& c) {
    // Scalar triple product: dot(a, cross(b, c))
    double bx_cx = b[1] * c[2] - b[2] * c[1]; // (b × c)_x
    double by_cy = b[2] * c[0] - b[0] * c[2]; // (b × c)_y
    double bz_cz = b[0] * c[1] - b[1] * c[0]; // (b × c)_z
    double triple_product = a[0] * bx_cx + a[1] * by_cy + a[2] * bz_cz;
    return triple_product / 6.0;
}

// The scalar triple product \(\vec{a} \cdot (\vec{b} \times \vec{c})\) can be computed as the determinant of the 3×3 matrix whose rows (or columns) are the vectors \(\vec{a}, \vec{b}, \vec{c}\). The determinant formula is: \( a_x(b_y c_z - b_z c_y) - a_y(b_x c_z - b_z c_x) + a_z(b_x c_y - b_y c_x) \). Divide the absolute result (or signed result, as required) by 6. If any two vectors are parallel or any vector is zero, the triple product is zero, giving zero volume; this is handled naturally by the formula. The function takes three points by const reference and returns a double. The main edge case is numerical precision, but for typical double inputs, the determinant computation is stable. Time complexity is O(1) constant time, space complexity is O(1) as no extra storage is used.
