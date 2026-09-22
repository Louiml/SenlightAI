// Given the ultimate moment capacity calculation for a reinforced concrete beam section, write a standalone C++ function named `calculateBeamMomentAndSteelRatio` that takes five floating-point parameters representing the concrete compressive strength `fck` (in MPa), steel yield strength `fy` (in MPa), effective depth `d` (in mm), neutral axis depth `xumax` (in mm), and area of tensile steel `ast` (in mm²). The function should compute and return a `std::pair<double, double>` containing the ultimate moment `mu` (in N·mm) as the first value and the percentage of steel `pt` (as a percentage, unitless) as the second value, using the formulas: `mu = 0.87 * fy * ast * (d - 0.416 * xumax)` and `pt = (0.4 * fck * xumax) / (fy * d)`. The function must handle positive inputs and typical engineering values; it should not perform input validation but should follow the given formulas exactly. This task requires creating a reusable, const-correct function that can be tested independently without a main function in the solution section.
#include <cassert>
#include <cmath>
#include <utility>

// Global main function for testing
int main() {
    // Test case 1: Basic values
    auto result1 = calculateBeamMomentAndSteelRatio(20.0, 415.0, 500.0, 200.0, 1500.0);
    double expectedMu1 = 0.87 * 415.0 * 1500.0 * (500.0 - 0.416 * 200.0);
    double expectedPt1 = (0.4 * 20.0 * 200.0) / (415.0 * 500.0);
    assert(std::abs(result1.first - expectedMu1) < 1e-6);
    assert(std::abs(result1.second - expectedPt1) < 1e-12);

    // Test case 2: Zero ast gives zero moment and zero pt
    auto result2 = calculateBeamMomentAndSteelRatio(25.0, 500.0, 600.0, 100.0, 0.0);
    assert(std::abs(result2.first - 0.0) < 1e-6);
    assert(std::abs(result2.second - 0.0) < 1e-12);

    // Test case 3: Typical values with different input
    auto result3 = calculateBeamMomentAndSteelRatio(30.0, 550.0, 450.0, 150.0, 2000.0);
    double expectedMu3 = 0.87 * 550.0 * 2000.0 * (450.0 - 0.416 * 150.0);
    double expectedPt3 = (0.4 * 30.0 * 150.0) / (550.0 * 450.0);
    assert(std::abs(result3.first - expectedMu3) < 1e-6);
    assert(std::abs(result3.second - expectedPt3) < 1e-12);

    // Test case 4: Edge case with large values
    auto result4 = calculateBeamMomentAndSteelRatio(50.0, 600.0, 1000.0, 400.0, 5000.0);
    double expectedMu4 = 0.87 * 600.0 * 5000.0 * (1000.0 - 0.416 * 400.0);
    double expectedPt4 = (0.4 * 50.0 * 400.0) / (600.0 * 1000.0);
    assert(std::abs(result4.first - expectedMu4) < 1e-6);
    assert(std::abs(result4.second - expectedPt4) < 1e-12);

    // Test case 5: Verify consistency with formula manually for one case
    auto result5 = calculateBeamMomentAndSteelRatio(25.0, 415.0, 300.0, 75.0, 1000.0);
    assert(std::abs(result5.first - 0.87 * 415.0 * 1000.0 * (300.0 - 0.416 * 75.0)) < 1e-6);
    assert(std::abs(result5.second - (0.4 * 25.0 * 75.0) / (415.0 * 300.0)) < 1e-12);

    return 0;
}
#include <utility> // for std::pair

// Calculate ultimate moment (N·mm) and steel percentage for a reinforced concrete beam.
// Parameters: fck (MPa), fy (MPa), d (mm), xumax (mm), ast (mm²)
// Returns: pair where first = mu (N·mm), second = pt (% steel)
std::pair<double, double> calculateBeamMomentAndSteelRatio(
    double fck, double fy, double d, double xumax, double ast) {
    
    const double mu = 0.87 * fy * ast * (d - 0.416 * xumax);
    const double pt = (0.4 * fck * xumax) / (fy * d);
    return {mu, pt};
}
// The solution is straightforward: compute two arithmetic expressions from the inputs. The first formula calculates the ultimate moment of resistance of the beam section, which depends on the steel area, yield strength, effective depth, and neutral axis depth. The second formula determines the reinforcement percentage based on the concrete strength, neutral axis depth, steel yield strength, and effective depth. The main algorithm involves evaluating these two expressions with the given parameters and returning them as a pair. Edge cases include zero or negative inputs—while the function does not validate, the formulas will produce valid or zero results for zero values, but division by zero occurs if `fy` or `d` is zero, which should be avoided in test cases. Time complexity is O(1) as it uses constant-time arithmetic; space complexity is O(1) auxiliary, as only a pair is returned. The function should be marked `const`? Actually, since it's a free function, we use `const` correctness by taking parameters by value (or const references) and not modifying them—the function itself is implicitly const. We'll use `double` for all parameters and the return pair, since engineering calculations involve decimals.
