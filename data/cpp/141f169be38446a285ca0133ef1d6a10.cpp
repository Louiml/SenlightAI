// Write a standalone C++ function that computes a simplified hydrogen-bond adjacency matrix weight for a single acceptor-donor-hydrogen triple. Given three 3D points (acceptor, donor, hydrogen) and three switching function thresholds (maximum O-O distance, maximum O-H distance, and maximum O-O-H angle in degrees), the function must return 1.0 if all three geometric criteria are satisfied (i.e., the O-O distance is below its threshold, the O-H distance is below its threshold, and the angle between the O-O vector and O-H vector is below the angle threshold), and 0.0 otherwise. The function must accept the points and thresholds as parameters, use standard library math functions only (no PLUMED dependencies), and be usable as a standalone helper for a larger matrix-building program. The function should compute the angle as the smaller angle (0 to 180 degrees) between the two vectors, using `acos` on the normalized dot product, and must clamp any floating-point rounding errors in the dot-product argument to the valid `[-1, 1]` interval to avoid `NaN` results.
The problem reduces to evaluating three independent geometric conditions. For the O-O distance, compute the Euclidean norm of `(acceptor - donor)`. For the O-H distance, compute the norm of `(acceptor - hydrogen)`. For the angle, compute the normalized vectors `ood = (acceptor - donor)/|acceptor - donor|` and `ohd = (acceptor - hydrogen)/|acceptor - hydrogen|`, then take `acos(dot(ood, ohd))` to get radians, convert to degrees, and compare against the provided angle threshold in degrees. Key edge cases: (1) if either distance is zero, the normalized vector would be undefined—since the distance condition would already fail (a zero distance is below any positive threshold but could cause division by zero), we must explicitly handle zero-length vectors by returning 0.0 (no bond) if either distance is zero, because the geometry is degenerate. (2) Floating-point errors in the dot product can exceed 1 or fall below -1; clamp the value before `acos`. (3) The function returns a simple boolean-like double (1.0 or 0.0), and uses a strict `<` comparison for all thresholds (matching the original code's use of switching functions that evaluate to 1.0 only when the value is strictly below the cutoff; if equal or above, the switching function returns 0.0). Time complexity is O(1) as only constant arithmetic is performed. Space complexity is O(1).
#include <cmath>
#include <array>

/**
 * @brief Computes the hydrogen-bond indicator for a single acceptor-donor-hydrogen triple.
 * @param acceptor 3D coordinates of the acceptor atom.
 * @param donor    3D coordinates of the donor atom.
 * @param hydrogen 3D coordinates of the hydrogen atom.
 * @param maxOOD   Maximum allowed O-O distance (must be positive).
 * @param maxOHD   Maximum allowed O-H distance (must be positive).
 * @param maxAngleDegrees Maximum allowed O-O-H angle in degrees (0–180).
 * @return 1.0 if all three geometric criteria are satisfied, 0.0 otherwise.
 */
double hydrogenBondWeight(const std::array<double,3>& acceptor,
                          const std::array<double,3>& donor,
                          const std::array<double,3>& hydrogen,
                          double maxOOD,
                          double maxOHD,
                          double maxAngleDegrees) {
    // Compute O-O vector (acceptor - donor) and its length
    double ooX = acceptor[0] - donor[0];
    double ooY = acceptor[1] - donor[1];
    double ooZ = acceptor[2] - donor[2];
    double ooDist = std::sqrt(ooX*ooX + ooY*ooY + ooZ*ooZ);
    
    // Compute O-H vector (acceptor - hydrogen) and its length
    double ohX = acceptor[0] - hydrogen[0];
    double ohY = acceptor[1] - hydrogen[1];
    double ohZ = acceptor[2] - hydrogen[2];
    double ohDist = std::sqrt(ohX*ohX + ohY*ohY + ohZ*ohZ);
    
    // If either distance is zero, the geometry is degenerate → no bond.
    if (ooDist == 0.0 || ohDist == 0.0) {
        return 0.0;
    }
    
    // Check distance criteria first (short-circuit if either fails)
    if (ooDist >= maxOOD || ohDist >= maxOHD) {
        return 0.0;
    }
    
    // Compute normalized vectors and the angle between them
    double nx1 = ooX / ooDist;
    double ny1 = ooY / ooDist;
    double nz1 = ooZ / ooDist;
    double nx2 = ohX / ohDist;
    double ny2 = ohY / ohDist;
    double nz2 = ohZ / ohDist;
    
    double dot = nx1*nx2 + ny1*ny2 + nz1*nz2;
    // Clamp to valid acos domain to avoid NaN from floating-point drift
    if (dot > 1.0) dot = 1.0;
    if (dot < -1.0) dot = -1.0;
    
    double angleRad = std::acos(dot);
    double angleDeg = angleRad * 180.0 / M_PI;
    
    // Check angle criterion (strictly less than threshold)
    if (angleDeg >= maxAngleDegrees) {
        return 0.0;
    }
    
    return 1.0;
}
#include <cassert>
#include <cmath>
#include <array>

// Function declaration (or include the solution here)
double hydrogenBondWeight(const std::array<double,3>& acceptor,
                          const std::array<double,3>& donor,
                          const std::array<double,3>& hydrogen,
                          double maxOOD,
                          double maxOHD,
                          double maxAngleDegrees);

int main() {
    // Basic satisfied case: O at (0,0,0), donor at (2,0,0), H at (1.5,0,0)
    // O-O distance = 2, O-H distance = 1.5, angle = 0°
    std::array<double,3> acc1 = {0.0, 0.0, 0.0};
    std::array<double,3> don1 = {2.0, 0.0, 0.0};
    std::array<double,3> hyd1 = {1.5, 0.0, 0.0};
    assert(hydrogenBondWeight(acc1, don1, hyd1, 3.0, 2.0, 30.0) == 1.0);

    // O-O distance too large
    std::array<double,3> acc2 = {0.0, 0.0, 0.0};
    std::array<double,3> don2 = {4.0, 0.0, 0.0};
    std::array<double,3> hyd2 = {3.5, 0.0, 0.0};
    assert(hydrogenBondWeight(acc2, don2, hyd2, 3.0, 2.0, 30.0) == 0.0);

    // O-H distance too large
    std::array<double,3> acc3 = {0.0, 0.0, 0.0};
    std::array<double,3> don3 = {2.0, 0.0, 0.0};
    std::array<double,3> hyd3 = {4.0, 0.0, 0.0}; // O-H distance = 4
    assert(hydrogenBondWeight(acc3, don3, hyd3, 3.0, 2.0, 30.0) == 0.0);

    // Angle too large: O at origin, donor at (2,0,0), H at (0,2,0) → angle = 90°
    std::array<double,3> acc4 = {0.0, 0.0, 0.0};
    std::array<double,3> don4 = {2.0, 0.0, 0.0};
    std::array<double,3> hyd4 = {0.0, 2.0, 0.0};
    assert(hydrogenBondWeight(acc4, don4, hyd4, 3.0, 3.0, 60.0) == 0.0);

    // Angle exactly at threshold: acceptor at (0,0,0), donor at (1,0,0), H at (cos60°, sin60°, 0) → angle = 60°
    std::array<double,3> acc5 = {0.0, 0.0, 0.0};
    std::array<double,3> don5 = {1.0, 0.0, 0.0};
    std::array<double,3> hyd5 = {0.5, std::sqrt(3.0)/2.0, 0.0};
    assert(hydrogenBondWeight(acc5, don5, hyd5, 2.0, 2.0, 60.0) == 0.0); // strictly less required

    // Zero distance degenerate case
    std::array<double,3> acc6 = {0.0, 0.0, 0.0};
    std::array<double,3> don6 = {0.0, 0.0, 0.0};
    std::array<double,3> hyd6 = {0.0, 0.0, 0.0};
    assert(hydrogenBondWeight(acc6, don6, hyd6, 3.0, 3.0, 90.0) == 0.0);

    // Just below the angle threshold should be 1.0
    std::array<double,3> acc7 = {0.0, 0.0, 0.0};
    std::array<double,3> don7 = {1.0, 0.0, 0.0};
    std::array<double,3> hyd7 = {0.5, 0.01, 0.0}; // small angle ~1.15°
    assert(hydrogenBondWeight(acc7, don7, hyd7, 2.0, 2.0, 30.0) == 1.0);

    // Floating-point drift: nearly collinear opposite directions → dot ≈ -1
    std::array<double,3> acc8 = {0.0, 0.0, 0.0};
    std::array<double,3> don8 = {1.0, 0.0, 0.0};
    std::array<double,3> hyd8 = {-1.0, 1e-15, 0.0};
    // angle ~179.99°, should be 0.0 with a threshold of 30°
    assert(hydrogenBondWeight(acc8, don8, hyd8, 2.0, 2.0, 30.0) == 0.0);

    // Test that clamping works: dot product might be slightly >1 due to rounding
    // Construct points where dot is mathematically 1 but floating point may give >1
    std::array<double,3> acc9 = {0.0, 0.0, 0.0};
    std::array<double,3> don9 = {1e-12, 0.0, 0.0}; // tiny distance
    std::array<double,3> hyd9 = {1e-12, 0.0, 0.0};
    // ooDist and ohDist are tiny but nonzero; normalized vectors likely produce dot≈1
    // This should not crash and should return 1.0 if distances are below thresholds
    assert(hydrogenBondWeight(acc9, don9, hyd9, 1e-9, 1e-9, 10.0) == 1.0);

    return 0;
}
