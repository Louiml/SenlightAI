Write a C++ function named `computeInterfaceCompressionSpeed` that simulates the core compression-flux calculation from the given OpenFOAM snippet. The function takes three parameters: a vector of face-area magnitudes (`std::vector<double> magSf`), a vector of face volume fluxes (`std::vector<double> phi`), and a scalar isotropic compression coefficient `icAlpha`. It must also read a constant coefficient `cAlpha` provided via a global constant (e.g., `constexpr double cAlphaGlobal = 1.0;`). The function should return a `std::vector<double>` containing the face compression speed values, computed as: `phic = cAlpha * |phi| / magSf`, then if `icAlpha > 0`, modified as `phic *= (1.0 - icAlpha); phic += (cAlpha * icAlpha) * someInterpolatedMagnitude`, where for simplicity the interpolated magnitude of velocity is approximated as the average of the absolute flux magnitudes divided by the corresponding face-area magnitude (i.e., `avgMagU = (|phi| / magSf)`). Also, for any face where `magSf` is zero, set `phic` to 0.0 to avoid division by zero. The function must handle empty input vectors by returning an empty vector.
#include <cassert>
#include <cmath>

// Global constant must match the one in the solution
constexpr double cAlphaGlobal = 1.0;

// Declaration of the function to test (assumed defined in solution)
std::vector<double> computeInterfaceCompressionSpeed(
    const std::vector<double>& magSf,
    const std::vector<double>& phi,
    double icAlpha
);

int main() {
    // Test 1: Simple uniform face areas and fluxes
    {
        std::vector<double> magSf = {2.0, 2.0, 2.0};
        std::vector<double> phi   = {4.0, -6.0, 0.0};
        std::vector<double> result = computeInterfaceCompressionSpeed(magSf, phi, 0.0);
        assert(result.size() == 3);
        // Face 0: |4|/2 = 2.0
        assert(std::fabs(result[0] - 2.0) < 1e-9);
        // Face 1: |−6|/2 = 3.0
        assert(std::fabs(result[1] - 3.0) < 1e-9);
        // Face 2: phi=0 -> 0
        assert(std::fabs(result[2]) < 1e-9);
    }

    // Test 2: With isotropic compression (icAlpha = 0.5)
    {
        std::vector<double> magSf = {1.0, 2.0};
        std::vector<double> phi   = {3.0, 4.0};
        std::vector<double> result = computeInterfaceCompressionSpeed(magSf, phi, 0.5);
        // Face 0: base=|3|/1=3; modified = 3*0.5 + 1*0.5*3 = 1.5+1.5 = 3.0
        assert(std::fabs(result[0] - 3.0) < 1e-9);
        // Face 1: base=|4|/2=2; modified = 2*0.5 + 1*0.5*2 = 1+1 = 2.0
        assert(std::fabs(result[1] - 2.0) < 1e-9);
    }

    // Test 3: Zero area magnitude and negative area (invalid) -> result 0
    {
        std::vector<double> magSf = {0.0, -1.0, 5.0};
        std::vector<double> phi   = {10.0, 20.0, 15.0};
        std::vector<double> result = computeInterfaceCompressionSpeed(magSf, phi, 0.0);
        assert(result.size() == 3);
        assert(std::fabs(result[0]) < 1e-9); // zero area
        assert(std::fabs(result[1]) < 1e-9); // negative area
        assert(std::fabs(result[2] - 3.0) < 1e-9); // 15/5 = 3
    }

    // Test 4: Empty input
    {
        std::vector<double> empty;
        auto result = computeInterfaceCompressionSpeed(empty, empty, 0.0);
        assert(result.empty());
    }

    // Test 5: Mismatched sizes -> empty output
    {
        std::vector<double> magSf = {1.0, 2.0};
        std::vector<double> phi   = {1.0};
        auto result = computeInterfaceCompressionSpeed(magSf, phi, 0.0);
        assert(result.empty());
    }

    return 0;
}
#include <vector>
#include <cmath>
#include <cstddef>

// Global constant to mimic the "cAlpha" coefficient from the dictionary
constexpr double cAlphaGlobal = 1.0;

// Compute interface compression speed for each face.
// magSf: per-face area magnitudes, phi: per-face volume fluxes, icAlpha: isotropic coefficient.
// Returns a vector of compression speeds; zero for empty input or invalid faces.
std::vector<double> computeInterfaceCompressionSpeed(
    const std::vector<double>& magSf,
    const std::vector<double>& phi,
    double icAlpha
) {
    // If inputs have different sizes or are empty, return empty vector.
    if (magSf.size() != phi.size() || magSf.empty()) {
        return {};
    }

    std::vector<double> phic(magSf.size(), 0.0);

    for (std::size_t i = 0; i < magSf.size(); ++i) {
        // Skip faces with zero or negative area magnitude, or zero flux
        if (magSf[i] <= 0.0 || phi[i] == 0.0) {
            phic[i] = 0.0;
            continue;
        }

        // Base compression: cAlpha * |phi| / magSf
        double base = cAlphaGlobal * std::abs(phi[i]) / magSf[i];

        if (icAlpha > 0.0) {
            // Approximate interpolated magnitude of velocity as |phi|/magSf
            double avgMagU = std::abs(phi[i]) / magSf[i];
            // Isotropic compression modification
            phic[i] = base * (1.0 - icAlpha) + (cAlphaGlobal * icAlpha) * avgMagU;
        } else {
            phic[i] = base;
        }
    }

    return phic;
}
// The solution iterates over all faces simultaneously, processing each face independently. For each face index `i`, we first check if `magSf[i]` is zero or if `phi[i]` is zero; in either case the base compression speed is 0.0. Otherwise, we compute `base = cAlpha * std::abs(phi[i]) / magSf[i]`. This represents the standard face-flux compression. Then, if `icAlpha > 0`, we apply the isotropic correction: first scale the base compression by `(1.0 - icAlpha)`, then add the term `cAlpha * icAlpha * averageMagU`, where `averageMagU` is approximated as `std::abs(phi[i]) / magSf[i]` (the same as the flux magnitude per unit area). This simplification captures the spirit of `fvc::interpolate(mag(U))` without requiring a full field interpolation. Edge cases include zero or negative `magSf` (we use `<= 0.0` to handle potential negative magnitudes defensively), and empty input vectors for which we return an empty result. The time complexity is O(n) where n is the number of faces, and the space complexity is O(n) for the output vector, with O(1) auxiliary space per iteration.
