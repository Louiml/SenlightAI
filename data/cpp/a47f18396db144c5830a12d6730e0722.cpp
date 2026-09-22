/*
Given a simulation framework where particles are stored in a `std::vector<Eigen::Vector3d>` of positions and a corresponding vector of velocities, write a standalone C++ function named `computeAverageVelocity` that takes a constant reference to a vector of 3D velocity vectors and returns the average velocity as an `Eigen::Vector3d`. The function must handle an empty input vector by returning a zero vector, and it must correctly sum all components (x, y, z) across all particles. The function should be usable independently of any particle system class, with only Eigen headers included, and must be `const`-correct (i.e., the input is read-only). The average is computed as the arithmetic mean of each component separately.
*/
#include <Eigen/Core>
#include <vector>

// Compute the average velocity across all particles.
// Returns a zero vector if the input is empty.
Eigen::Vector3d computeAverageVelocity(const std::vector<Eigen::Vector3d>& velocities) {
    Eigen::Vector3d sum = Eigen::Vector3d::Zero();
    
    for (const auto& v : velocities) {
        sum += v;
    }
    
    if (velocities.empty()) {
        return Eigen::Vector3d::Zero();
    }
    
    double n = static_cast<double>(velocities.size());
    return sum / n;
}
#include <cassert>
#include <Eigen/Core>
#include <vector>

int main() {
    // Empty input
    std::vector<Eigen::Vector3d> empty;
    assert(computeAverageVelocity(empty) == Eigen::Vector3d::Zero());

    // Single particle
    std::vector<Eigen::Vector3d> one = {Eigen::Vector3d(1.0, 2.0, 3.0)};
    assert(computeAverageVelocity(one) == Eigen::Vector3d(1.0, 2.0, 3.0));

    // Multiple particles, exact average
    std::vector<Eigen::Vector3d> multi = {
        Eigen::Vector3d(0.0, 0.0, 0.0),
        Eigen::Vector3d(2.0, 4.0, 6.0),
        Eigen::Vector3d(4.0, 8.0, 12.0)
    };
    assert(computeAverageVelocity(multi) == Eigen::Vector3d(2.0, 4.0, 6.0));

    // Negative and positive mixture
    std::vector<Eigen::Vector3d> mixed = {
        Eigen::Vector3d(-1.0, -2.0, -3.0),
        Eigen::Vector3d(3.0, 4.0, 5.0)
    };
    assert(computeAverageVelocity(mixed) == Eigen::Vector3d(1.0, 1.0, 1.0));

    // Fractional average
    std::vector<Eigen::Vector3d> frac = {
        Eigen::Vector3d(1.0, 1.0, 1.0),
        Eigen::Vector3d(2.0, 2.0, 2.0)
    };
    Eigen::Vector3d result = computeAverageVelocity(frac);
    assert(result.x() == 1.5 && result.y() == 1.5 && result.z() == 1.5);

    return 0;
}
// The solution iterates over all velocity vectors using a range-based for loop, accumulating the x, y, and z components into a single `Eigen::Vector3d` sum (initialized to zero). After looping, if the input vector is empty, return a zero vector (avoid division by zero). Otherwise, divide each component of the sum by `static_cast<double>(velocities.size())`. Edge cases: empty input returns `Vector3d::Zero()`; single particle returns that particle's velocity unchanged; large counts are handled because `size_t` is used for the size, and the sum uses `double` precision. Time complexity is O(n), where n is the number of particles; space complexity is O(1) auxiliary, excluding the input storage.
