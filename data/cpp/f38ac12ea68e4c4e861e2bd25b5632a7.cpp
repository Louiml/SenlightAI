Write a standalone C++ function that generates a grid of 3D points representing a mathematically defined egg-like surface using the parametric equations from the snippet: `x(u,v) = (-90u^5 + 225u^4 - 270u^3 + 180u^2 - 45u) * cos(π v)`, `y(u) = 160u^4 - 320u^3 + 160u^2`, and `z(u,v) = (-90u^5 + 225u^4 - 270u^3 + 180u^2 - 45u) * sin(π v)`, for `u` and `v` each sampled evenly from 0 to 1 inclusive with a given integer resolution `n` (number of intervals, so `n+1` points per dimension). The function must allocate and fill a 2D dynamic array of `std::array<double,3>` (or a suitable struct) representing the grid, return it (e.g., as a `std::vector<std::vector<std::array<double,3>>>`), and also support an optional output parameter to store the offset needed to center the shape vertically (the snippet translates the object downward by half the maximum y value, which is `160*0.5^4 - 320*0.5^3 + 160*0.5^2 = 10` for 0.5). The function must be `const`-correct where appropriate, handle edge cases like `n = 0` (should produce a single point at u=0, v=0), and be self-contained with no reliance on OpenGL or external graphics libraries.

The main algorithm iterates `i` from 0 to `n` for the `u` parameter, computing `u = i * (1.0/n)` (with special care to avoid division by zero when `n=0`, in which case `u=0`). For each `u`, it precomputes powers (u², u³, u⁴, u⁵) to speed up the polynomial evaluation. Then for each `j` from 0 to `n`, it computes `v = j * (1.0/n)` and evaluates `cos(M_PI*v)` and `sin(M_PI*v)`. The resulting point is stored in a 2D vector of `std::array<double,3>`. The offset for vertical centering is the y-coordinate at `u=0.5`, which is `160*0.5^4 - 320*0.5^3 + 160*0.5^2 = 10.0`. Edge cases: `n=0` must produce a single point; the function should return an empty vector if `n` is negative (invalid input). Time complexity is O(n²) because there are `(n+1)²` points. Space complexity is also O(n²) for the output grid. The function uses `std::numbers::pi` (C++20) or defines `M_PI` fallback for portability, and uses `std::vector` for automatic memory management.

#include <vector>
#include <array>
#include <cmath>
#include <cstddef>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using Point3 = std::array<double, 3>;

// Generate a grid of (n+1)x(n+1) points on an egg-like surface.
// 'n' is the number of intervals per dimension (must be >= 0).
// Returns an empty vector if n < 0.
// Optionally outputs the vertical offset (y at u=0.5) via 'outOffset' if not null.
std::vector<std::vector<Point3>> generateEggSurface(int n, double* outOffset = nullptr) {
    if (n < 0) {
        return {};
    }

    if (outOffset != nullptr) {
        *outOffset = 160.0 * std::pow(0.5, 4) - 320.0 * std::pow(0.5, 3) + 160.0 * std::pow(0.5, 2);
    }

    std::vector<std::vector<Point3>> grid(n + 1, std::vector<Point3>(n + 1));

    for (int i = 0; i <= n; ++i) {
        double u = (n == 0) ? 0.0 : static_cast<double>(i) / n;
        double u2 = u * u;
        double u3 = u2 * u;
        double u4 = u3 * u;
        double u5 = u4 * u;

        // Precompute the polynomial factor for x and z
        double xzFactor = -90.0 * u5 + 225.0 * u4 - 270.0 * u3 + 180.0 * u2 - 45.0 * u;
        double y = 160.0 * u4 - 320.0 * u3 + 160.0 * u2;

        for (int j = 0; j <= n; ++j) {
            double v = (n == 0) ? 0.0 : static_cast<double>(j) / n;
            double angle = M_PI * v;
            double cosA = std::cos(angle);
            double sinA = std::sin(angle);

            grid[i][j][0] = xzFactor * cosA;
            grid[i][j][1] = y;
            grid[i][j][2] = xzFactor * sinA;
        }
    }

    return grid;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <array>

// Include the solution here or assume it's defined above.
// (The function generateEggSurface is assumed to be included.)

int main() {
    // Test 1: n=0 produces a single point at u=0, v=0.
    {
        auto grid = generateEggSurface(0);
        assert(grid.size() == 1);
        assert(grid[0].size() == 1);
        // At u=0: xzFactor = 0, y = 0.
        assert(std::fabs(grid[0][0][0]) < 1e-9);
        assert(std::fabs(grid[0][0][1]) < 1e-9);
        assert(std::fabs(grid[0][0][2]) < 1e-9);
    }

    // Test 2: n=1 gives 2x2 grid; check u=0.5, v=0.5 point.
    {
        auto grid = generateEggSurface(1);
        assert(grid.size() == 2);
        assert(grid[0].size() == 2);
        // i=1, j=1: u=1, v=1 → xzFactor = -90+225-270+180-45 = 0, y = 160-320+160 = 0.
        assert(std::fabs(grid[1][1][0]) < 1e-9);
        assert(std::fabs(grid[1][1][1]) < 1e-9);
        assert(std::fabs(grid[1][1][2]) < 1e-9);
    }

    // Test 3: Verify a known point at u=0.5, v=0.0 for n=2 (i=1, j=0).
    {
        auto grid = generateEggSurface(2);
        // u=0.5, v=0.0: xzFactor = -90*(0.03125)+225*(0.0625)-270*(0.125)+180*(0.25)-45*(0.5)
        // = -2.8125 + 14.0625 - 33.75 + 45 - 22.5 = 0.0? Actually compute precisely:
        // Let's use the constant: for u=0.5, xzFactor should be 0? Let's trust math: 
        // -90*0.03125 = -2.8125; 225*0.0625 = 14.0625; -270*0.125 = -33.75; 180*0.25 = 45; -45*0.5 = -22.5
        // Sum = -2.8125 + 14.0625 = 11.25; 11.25 - 33.75 = -22.5; -22.5 + 45 = 22.5; 22.5 - 22.5 = 0.
        // x = 0 * cos(0) = 0, y = 160*0.0625 - 320*0.125 + 160*0.25 = 10 - 40 + 40 = 10, z = 0*sin(0)=0.
        assert(std::fabs(grid[1][0][0]) < 1e-9);
        assert(std::fabs(grid[1][0][1] - 10.0) < 1e-9);
        assert(std::fabs(grid[1][0][2]) < 1e-9);

        // Also check offset output.
        double offset = -1.0;
        generateEggSurface(2, &offset);
        assert(std::fabs(offset - 10.0) < 1e-9);
    }

    // Test 4: Negative n returns empty.
    {
        auto grid = generateEggSurface(-5);
        assert(grid.empty());
    }

    // Test 5: n=3, check bottom and top points.
    {
        auto grid = generateEggSurface(3);
        // Bottom at u=0 (i=0): y=0.
        assert(std::fabs(grid[0][0][1]) < 1e-9);
        // Top at u=1 (i=3): y=0.
        assert(std::fabs(grid[3][0][1]) < 1e-9);
        // Middle at u=0.5 (i=1.5 not integer, but i=1 is u=1/3, i=2 is 2/3). For u=1/3:
        double u = 1.0/3.0;
        double expectedY = 160*std::pow(u,4) - 320*std::pow(u,3) + 160*std::pow(u,2);
        assert(std::fabs(grid[1][0][1] - expectedY) < 1e-6);
    }

    return 0;
}
