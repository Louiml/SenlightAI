Write a standalone C++ function `double computeVoidRatio(int ntry, double regionVolume, int nlocal, const double* radii, const double* xCoords, const double* yCoords, const double* zCoords)` that estimates the void ratio of a granular system using Monte Carlo sampling. The function receives: the number of random points to sample (`ntry`), the total volume of the region containing the particles (`regionVolume`), the number of local particles (`nlocal`), arrays of particle radii and their (x, y, z) coordinates. For each random point uniformly distributed in the region (use a simple uniform distribution in a cube of side length `cbrt(regionVolume)`), determine whether it falls inside any sphere (distance from point to particle center ≤ radius). The estimated atom volume is `regionVolume * (pointsInside / ntry)`. Return `-2.0` if estimated atom volume is zero; otherwise return `voidVolume / atomVolume`, where `voidVolume = regionVolume - atomVolume`. Assume all inputs are valid: `ntry > 0`, `regionVolume > 0`, `nlocal ≥ 0`, and arrays are non-null when `nlocal > 0`. Use `std::mt19937` with fixed seed 42 for reproducibility.
#include <cassert>
#include <cmath>
#include <vector>

int main() {
    // Test with no particles: should return -2.0
    assert(computeVoidRatio(1000, 1000.0, 0, nullptr, nullptr, nullptr, nullptr) == -2.0);

    // Test with one particle in center of a cube region volume 1000 (side=10)
    // particle radius 1: volume ≈ 4.18879, void ratio ≈ (1000-4.18879)/4.18879 ≈ 237.9
    double radius = 1.0;
    double x = 5.0, y = 5.0, z = 5.0;
    // Use many samples for stability
    double ratio = computeVoidRatio(100000, 1000.0, 1, &radius, &x, &y, &z);
    assert(ratio > 0.0);
    assert(std::fabs(ratio - 237.9) < 5.0); // loose tolerance due to Monte Carlo noise

    // Test with large particle that fills region: radius 5 in cube side 10
    // Particle volume ~523.6, region 1000, void ratio ~(1000-523.6)/523.6 ≈ 0.91
    double radiusBig = 5.0;
    double xc = 5.0, yc = 5.0, zc = 5.0;
    double ratioBig = computeVoidRatio(100000, 1000.0, 1, &radiusBig, &xc, &yc, &zc);
    assert(ratioBig > 0.5 && ratioBig < 1.5);

    // Test with same point and particle: every sample inside, atomVolume ≈ regionVolume, ratio ≈ 0
    double radiusHuge = 10.0; // radius > side, but still sphere centered at (5,5,5)
    double ratioHuge = computeVoidRatio(1000, 1000.0, 1, &radiusHuge, &xc, &yc, &zc);
    assert(std::fabs(ratioHuge) < 0.01); // ratio near zero

    // Test with two non-overlapping particles (radii 1 at (2,2,2) and (8,8,8))
    std::vector<double> radii = {1.0, 1.0};
    std::vector<double> xs = {2.0, 8.0};
    std::vector<double> ys = {2.0, 8.0};
    std::vector<double> zs = {2.0, 8.0};
    double ratioTwo = computeVoidRatio(100000, 1000.0, 2, radii.data(), xs.data(), ys.data(), zs.data());
    assert(ratioTwo > 100.0); // still very large because particles are tiny

    // Test determinism: same inputs give same result
    double r1 = computeVoidRatio(5000, 1000.0, 1, &radius, &x, &y, &z);
    double r2 = computeVoidRatio(5000, 1000.0, 1, &radius, &x, &y, &z);
    assert(r1 == r2);
}
#include <cmath>
#include <random>

// Estimate void ratio using Monte Carlo sampling.
// Returns -2.0 if estimated atom volume is zero, otherwise (regionVolume - atomVolume)/atomVolume.
double computeVoidRatio(int ntry, double regionVolume, int nlocal,
                        const double* radii, const double* xCoords,
                        const double* yCoords, const double* zCoords) {
    if (ntry <= 0 || regionVolume <= 0.0) return -2.0;

    double side = std::cbrt(regionVolume);
    std::mt19937 rng(42); // fixed seed for reproducibility
    std::uniform_real_distribution<double> dist(0.0, side);

    unsigned long pointsInside = 0;

    for (int i = 0; i < ntry; ++i) {
        double randX = dist(rng);
        double randY = dist(rng);
        double randZ = dist(rng);

        for (int j = 0; j < nlocal; ++j) {
            double dx = randX - xCoords[j];
            double dy = randY - yCoords[j];
            double dz = randZ - zCoords[j];
            double r = radii[j];
            if (dx*dx + dy*dy + dz*dz <= r*r) {
                ++pointsInside;
                break;
            }
        }
    }

    double atomVolume = regionVolume * static_cast<double>(pointsInside) / static_cast<double>(ntry);

    if (atomVolume == 0.0) return -2.0;
    return (regionVolume - atomVolume) / atomVolume;
}
// The solution uses Monte Carlo integration to estimate the volume fraction occupied by spheres within a region. The main algorithm:
// 1. Initialize a random number generator with a fixed seed.
// 2. Loop `ntry` times, each time generating a random point uniformly in a cube of volume `regionVolume` (i.e., each coordinate uniform in `[0, cbrt(regionVolume))`).
// 3. For each random point, loop over all local particles and check if the squared distance from the point to the particle center is ≤ radius². If yes, increment `pointsInside` and break out of the particle loop to avoid double counting.
// 4. Compute `atomVolume = regionVolume * (pointsInside / ntry)`.
// 5. If `atomVolume == 0.0`, return `-2.0` as specified; otherwise return `(regionVolume - atomVolume) / atomVolume`.
//
// Edge cases:
// - `nlocal == 0`: No points will be inside any particle, so `pointsInside = 0`, `atomVolume = 0`, return `-2.0`.
// - `ntry` may be large, but the nested loop is `O(ntry * nlocal)`, which could be inefficient for large systems; for the task, this is acceptable.
// - Floating-point comparisons should use `<=` for the distance check; because radii and coordinates are given, no tolerance is required for the toy example.
// - To avoid overflow when computing squared distance, use `double` arithmetic; inputs are assumed to be reasonable magnitudes.
//
// Time complexity: `O(ntry * nlocal)` time, `O(1)` auxiliary space (beyond input arrays). The random number generation is done once per point.
