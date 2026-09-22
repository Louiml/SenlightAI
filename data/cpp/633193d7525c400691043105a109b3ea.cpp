Write a C++ function `std::vector<double> computeBranchCrossSections(int sides, double startRadius, double radiusContact, double radiusVariation, const std::vector<double>& anglesDegrees)` that, given the number of sides of a polygonal branch cross-section, an initial radius, a per-section radius contact factor (as a percentage), a radius variation factor (as a fraction), and a list of angles in degrees (one per section), computes the vertex positions of each cross-section. The function should model the growth of a branch: for each section, compute a circle of `sides` vertices (using standard cosine/sine tables with angle step `2π/(sides)`), and after generating a section, update the radius for the next section as `radius = radius * (radiusContact * 0.01) + (radiusVariation * 0.01 * pseudorandomFloat())`, where `pseudorandomFloat()` returns a deterministic pseudo-random float in [0,1) derived from a seeded LCG (e.g., multiplier 1588635695, modulus 4294967291U, quotient 2, remainder 1117695901, with an initial seed 93186752). The returned vector should contain all vertices flattened sequentially: for section index `i` (0-based), vertex `j` (0 to sides-1) has coordinates `(x_j, y_j)` where `x_j = cos(angle_j) * radius_i`, `y_j = sin(angle_j) * radius_i`, with `angle_j = j * (2π / sides)`. The number of entries in the output must be `sides * anglesDegrees.size()`. If `sides < 3` or `anglesDegrees.empty()`, return an empty vector. The function must be self-contained, deterministic, and must not use global mutable state (use local static seed or pass seed as parameter). Use `const` correctly and include necessary headers (`vector`, `cmath`, `cstdint`).

The problem requires simulating a branch with multiple cross-sections. The key algorithm is straightforward: initialize a radius from the given start radius. For each angle in the input list (each representing one section), generate `sides` vertices on a circle of the current radius. The circle vertices are computed using a precomputed table of cosines and sines for angles `0, 2π/sides, 4π/sides, ..., (sides-1)*2π/sides`. After each section, update the radius using the given formula with a deterministic pseudo-random float. The pseudo-random generator is a classic LCG with fixed constants; it must produce reproducible results across calls. Edge cases: if `sides < 3` (invalid polygon) or no angles provided, return empty. Also handle the possibility that `radiusVariation` and `radiusContact` might be any non-negative values; the formula is always applied. Time complexity is O(sides * number_of_sections) because each vertex is computed once. Space complexity is O(sides * number_of_sections) for the output vector, plus O(sides) for the sine/cosine tables. The sine/cosine tables can be computed once per call; no need for dynamic table reuse. The radius update uses the LCG; the seed is local static to keep the function pure but deterministic.

#include <vector>
#include <cmath>
#include <cstdint>

// Deterministic pseudo-random float in [0,1) using LCG.
static double branchRandomFloat(std::uint32_t& seed) {
    const std::uint32_t a = 1588635695;
    const std::uint32_t m = 4294967291U;
    const std::uint32_t q = 2;
    const std::uint32_t r = 1117695901;
    seed = a * (seed % q) - r * (seed / q);
    return static_cast<double>(seed) / static_cast<double>(m);
}

// Compute cross-section vertices for a branch growth.
std::vector<double> computeBranchCrossSections(
    int sides,
    double startRadius,
    double radiusContact,      // percentage, e.g., 90 means 90%
    double radiusVariation,    // fraction, e.g., 0.1 means 10%
    const std::vector<double>& anglesDegrees) 
{
    if (sides < 3 || anglesDegrees.empty()) {
        return {};
    }

    // Precompute sine/cosine tables.
    const double pi = 3.14159265358979323846;
    const double angleStep = (2.0 * pi) / static_cast<double>(sides);
    std::vector<double> cosTable(sides);
    std::vector<double> sinTable(sides);
    for (int i = 0; i < sides; ++i) {
        double angle = static_cast<double>(i) * angleStep;
        cosTable[i] = std::cos(angle);
        sinTable[i] = std::sin(angle);
    }

    double radius = startRadius;
    std::uint32_t seed = 93186752;  // deterministic seed
    std::vector<double> result;
    result.reserve(static_cast<size_t>(sides) * anglesDegrees.size());

    for (size_t s = 0; s < anglesDegrees.size(); ++s) {
        // Generate circle of current radius (anglesDegrees is used only for count).
        for (int j = 0; j < sides; ++j) {
            result.push_back(cosTable[j] * radius);
            result.push_back(sinTable[j] * radius);
        }
        // Update radius for next section.
        double variation = branchRandomFloat(seed);
        radius = radius * (radiusContact * 0.01) + (radiusVariation * 0.01 * variation);
    }

    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

// Declaration of the function to test.
std::vector<double> computeBranchCrossSections(
    int sides,
    double startRadius,
    double radiusContact,
    double radiusVariation,
    const std::vector<double>& anglesDegrees);

int main() {
    // Test 1: sides < 3 returns empty
    assert(computeBranchCrossSections(2, 1.0, 90.0, 0.1, {0.0}).empty());

    // Test 2: no angles returns empty
    assert(computeBranchCrossSections(4, 1.0, 90.0, 0.1, {}).empty());

    // Test 3: simple single section with zero variation -> circle radius 1
    std::vector<double> out1 = computeBranchCrossSections(4, 1.0, 0.0, 0.0, {0.0});
    assert(out1.size() == 8);
    // For sides=4, angles: 0, 90, 180, 270 degrees
    // cos/sin gives (1,0), (0,1), (-1,0), (0,-1) times radius 1
    assert(std::fabs(out1[0] - 1.0) < 1e-9);
    assert(std::fabs(out1[1] - 0.0) < 1e-9);
    assert(std::fabs(out1[2] - 0.0) < 1e-9);
    assert(std::fabs(out1[3] - 1.0) < 1e-9);
    assert(std::fabs(out1[4] + 1.0) < 1e-9);
    assert(std::fabs(out1[5] - 0.0) < 1e-9);
    assert(std::fabs(out1[6] - 0.0) < 1e-9);
    assert(std::fabs(out1[7] + 1.0) < 1e-9);

    // Test 4: two sections with no variation, contact=50% -> radius halves each step
    std::vector<double> out2 = computeBranchCrossSections(3, 2.0, 50.0, 0.0, {0.0, 0.0});
    // First section radius 2, second radius = 2*0.5 = 1
    // The first three vertices (x,y) have length 2, next three length 1
    assert(out2.size() == 12);
    for (int i = 0; i < 3; ++i) {
        double x = out2[i*2];
        double y = out2[i*2+1];
        assert(std::fabs(std::sqrt(x*x + y*y) - 2.0) < 1e-9);
    }
    for (int i = 3; i < 6; ++i) {
        double x = out2[i*2];
        double y = out2[i*2+1];
        assert(std::fabs(std::sqrt(x*x + y*y) - 1.0) < 1e-9);
    }

    // Test 5: deterministic behavior with variation - same input gives same output
    std::vector<double> a = computeBranchCrossSections(5, 1.0, 90.0, 0.2, {10.0, 20.0, 30.0});
    std::vector<double> b = computeBranchCrossSections(5, 1.0, 90.0, 0.2, {10.0, 20.0, 30.0});
    assert(a == b);

    // Test 6: size check for 6 sides and 4 angles
    auto out3 = computeBranchCrossSections(6, 0.5, 80.0, 0.05, {0.0, 1.0, 2.0, 3.0});
    assert(out3.size() == 6 * 4 * 2); // sides * sections * 2 components

    // Test 7: radius never negative (even with huge variation)
    auto out4 = computeBranchCrossSections(8, 1.0, 0.0, 1000.0, {0.0, 0.0, 0.0});
    for (size_t i = 0; i < out4.size(); i += 2) {
        double x = out4[i];
        double y = out4[i+1];
        double len = std::sqrt(x*x + y*y);
        assert(len >= 0.0);
    }

    return 0;
}
