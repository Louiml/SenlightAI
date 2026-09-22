/*
Write a C++ function named `boggsForward` that implements the forward Boggs Eumorphic projection for spheroidal (sphere) coordinates. The function must take a latitude `phi` (in radians) and a longitude `lam` (in radians) as inputs, and return a `std::pair<double, double>` representing the projected planar coordinates `(x, y)`. The projection formulas are:  
- If `|abs(phi) - π/2| < 1e-7`, then `x = 0`; otherwise solve the equation `theta + sin(theta) = sin(phi) * π` for `theta` using Newton's method with up to 20 iterations and a tolerance of `1e-7`, starting from `theta = phi`. Then let `theta = theta / 2`. Compute `x = 2.00276 * lam / (1 / cos(phi) + 1.11072 / cos(theta))`.  
- Compute `y = 0.49931 * (phi + √2 * sin(theta))`.  
Use the constants `NITER = 20`, `EPS = 1e-7`, `FXC = 2.00276`, `FXC2 = 1.11072`, `FYC = 0.49931`. The function must be self-contained, not rely on external projection libraries, and be robust for edge cases such as `phi = ±π/2` and `lam = 0`.
*/
#include <cmath>
#include <utility>

// Forward Boggs Eumorphic projection. Input: phi (latitude), lam (longitude) in radians.
// Returns (x, y) projected coordinates.
std::pair<double, double> boggsForward(double phi, double lam) {
    const double NITER = 20;
    const double EPS = 1e-7;
    const double FXC = 2.00276;
    const double FXC2 = 1.11072;
    const double FYC = 0.49931;
    const double M_PI = 3.14159265358979323846;
    const double M_SQRT2 = 1.41421356237309504880;

    double x, y;
    double theta = phi;
    double th1, c;
    int i;

    if (std::fabs(std::fabs(phi) - M_PI / 2.0) < EPS) {
        x = 0.0;
    } else {
        c = std::sin(theta) * M_PI;
        for (i = NITER; i; --i) {
            th1 = (theta + std::sin(theta) - c) / (1.0 + std::cos(theta));
            theta -= th1;
            if (std::fabs(th1) < EPS) break;
        }
        theta *= 0.5;
        x = FXC * lam / (1.0 / std::cos(phi) + FXC2 / std::cos(theta));
    }
    y = FYC * (phi + M_SQRT2 * std::sin(theta));
    return std::make_pair(x, y);
}
#include <cassert>
#include <cmath>
#include <utility>

// Function declaration from the solution
std::pair<double, double> boggsForward(double phi, double lam);

int main() {
    const double PI = 3.14159265358979323846;
    const double TOL = 1e-6;

    // Test 1: Equator, zero longitude -> (0,0)
    auto p1 = boggsForward(0.0, 0.0);
    assert(std::fabs(p1.first) < TOL && std::fabs(p1.second) < TOL);

    // Test 2: phi = 0, lam = 1 -> x ≈ 2.00276/(1+1.11072) ≈ 0.9486, y = 0
    auto p2 = boggsForward(0.0, 1.0);
    assert(std::fabs(p2.first - 2.00276 / (1.0 + 1.11072)) < TOL);
    assert(std::fabs(p2.second) < TOL);

    // Test 3: phi near pole -> x = 0
    auto p3 = boggsForward(PI/2 - 1e-8, 2.0);
    assert(std::fabs(p3.first) < TOL);

    // Test 4: At pole exactly -> x = 0, y = FYC * (PI/2 + sqrt(2)*sin(PI/4)) ≈ 0.49931 * (PI/2 + 1) ≈ 1.282
    auto p4 = boggsForward(PI/2, 0.0);
    assert(std::fabs(p4.first) < TOL);
    assert(std::fabs(p4.second - 0.49931 * (PI/2 + 1.0)) < 1e-6);

    // Test 5: phi = 0.5, lam = 0.5 (sanity check, compare with direct computation)
    double phi = 0.5;
    double lam = 0.5;
    double theta = phi;
    double c = std::sin(theta) * PI;
    for (int i = 20; i; --i) {
        double th1 = (theta + std::sin(theta) - c) / (1.0 + std::cos(theta));
        theta -= th1;
        if (std::fabs(th1) < 1e-7) break;
    }
    theta *= 0.5;
    double expected_x = 2.00276 * lam / (1.0 / std::cos(phi) + 1.11072 / std::cos(theta));
    double expected_y = 0.49931 * (phi + 1.4142135623730951 * std::sin(theta));
    auto p5 = boggsForward(phi, lam);
    assert(std::fabs(p5.first - expected_x) < 1e-6);
    assert(std::fabs(p5.second - expected_y) < 1e-6);

    // Test 6: Negative phi and lam
    auto p6 = boggsForward(-0.3, -1.2);
    assert(p6.first < 0.0 && p6.second < 0.0);

    // Test 7: Symmetry: f(-phi, -lam) = -f(phi, lam) approximately
    auto p7 = boggsForward(0.3, 1.2);
    assert(std::fabs(p6.first + p7.first) < 1e-6);
    assert(std::fabs(p6.second + p7.second) < 1e-6);

    return 0;
}
// The algorithm directly implements the given iterative formula. The key challenge is the Newton iteration for solving `theta + sin(theta) = c` where `c = sin(phi) * π`. Starting from `theta = phi`, we repeatedly compute `th1 = (theta + sin(theta) - c) / (1 + cos(theta))` and update `theta -= th1` until either the magnitude of `th1` is below `EPS` or we exhaust `NITER` iterations. Edge cases: when `phi` is near ±π/2, the `x` coordinate is set to 0 to avoid division by zero in the denominator `(1/cos(phi))`. When `phi = 0`, the iteration converges quickly (theta stays 0), and `x = 2.00276 * lam / (1/1 + 1.11072/1) = 2.00276 * lam / 2.11072` which is finite. Time complexity is O(NITER) constant, space complexity O(1). The function returns a `std::pair<double,double>`.
