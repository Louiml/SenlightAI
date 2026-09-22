// Write a standalone C++ function named `structureTensorParticleSimStep` that models the core iterative update of a particle chain under two forces: a "propagation force" that pushes each particle away from a structure-tensor-derived direction (simplified here as aligning with the z-axis) and a "spring force" that keeps neighboring particles roughly equidistant. The function should accept a `std::vector<cv::Vec3d>` representing the current chain positions, a constant `stepSize` (a positive double), a `springConstant` (a positive double), and a `propagationScale` (a positive double), and return the updated chain after **one** full Runge-Kutta 4th order integration step has been applied to the entire chain. The resting lengths between consecutive particles are computed once at the start from the initial positions (i.e., from the input chain) and remain fixed throughout that single step. The propagation force for each particle should be the normalized vector (0,0,1) projected onto the plane orthogonal to the local z-axis direction (i.e., the component of (0,0,1) perpendicular to itself is zero, so for simplicity, make the propagation force simply the normalized (0,0,1) vector, but scale it by `propagationScale`). The spring force for each particle is the sum of two contributions: from the left neighbor (if it exists) a force vector pointing from the left neighbor toward the current particle with magnitude `springConstant * (distance - restingLeft)`, and from the right neighbor (if exists) a force vector pointing from current particle to the right neighbor with magnitude `springConstant * (distance - restingRight)`. The total force at each particle is the sum of its propagation and spring forces. For the RK4 update, compute k1, k2, k3, k4 as the total force field evaluated at the current chain and at intermediate chains formed by adding fractions of the previous k's times `stepSize` (use the standard coefficients: k2 and k3 at half-step, k4 at full step). After computing all four force fields, update each particle's position by the weighted average: `position += stepSize * (k1 + 2*k2 + 2*k3 + k4) / 6`. The function should return the updated chain. Assume `cv::Vec3d` is a 3D vector with element access via `[0]`, `[1]`, `[2]`, and that `cv::norm(v)` returns the Euclidean length, and `cv::normalize(v, v)` scales a vector to unit length (if length is nonzero). Do not perform any bounds checking or early stopping. Edge cases: if the chain has fewer than 2 particles, return the input unchanged. The function must be self-contained and not rely on any external library except the provided OpenCV-like vector type (you may define a minimal `Vec3d` struct yourself if you prefer to avoid including OpenCV). The solution must be a free function with the signature `std::vector<Vec3d> structureTensorParticleSimStep(const std::vector<Vec3d>& chain, double stepSize, double springConstant, double propagationScale);` and must use `const` correctness appropriately.
The solution models the physical simulation of a chain of particles where each particle experiences two forces: a constant propagation force (pointing along the z-axis, normalized and scaled) and spring forces from its immediate neighbors that pull it toward the resting distances defined at the start. The RK4 method approximates the solution of the system of ODEs `dx/dt = F(x)` where `x` is the vector of all particle positions. The key steps are: 1) compute resting distances between consecutive particles from the initial chain; 2) define a helper function that, given a chain, computes the total force per particle (a `std::vector<Vec3d>`); 3) perform the four RK4 stages: k1 = F(chain), k2 = F(chain + 0.5*step*k1), k3 = F(chain + 0.5*step*k2), k4 = F(chain + step*k3); 4) update each particle position using the weighted sum. Important edge cases: a chain with 0 or 1 particle has no springs, and the propagation force is constant, so the update would move particles along z; but the problem states to return unchanged for fewer than 2 particles. To avoid division by zero when normalizing a zero vector, treat a zero-length vector as zero force (or as the zero vector). Time complexity is O(n) per force evaluation, and there are 4 evaluations per step, so O(n) overall for a single step. Space complexity is O(n) for the intermediate chains and force vectors.
#include <vector>
#include <cmath>
#include <cstddef>

// Minimal 3D vector type to avoid external dependencies.
struct Vec3d {
    double x, y, z;
    Vec3d() : x(0), y(0), z(0) {}
    Vec3d(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {}

    Vec3d operator+(const Vec3d& other) const {
        return Vec3d(x + other.x, y + other.y, z + other.z);
    }
    Vec3d operator-(const Vec3d& other) const {
        return Vec3d(x - other.x, y - other.y, z - other.z);
    }
    Vec3d operator*(double scalar) const {
        return Vec3d(x * scalar, y * scalar, z * scalar);
    }
    Vec3d& operator+=(const Vec3d& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }
};

// Euclidean norm of a 3D vector.
double norm(const Vec3d& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// In-place normalization to unit length; if length is zero, leaves vector unchanged.
void normalize(Vec3d& v) {
    double len = norm(v);
    if (len > 1e-12) {
        v.x /= len; v.y /= len; v.z /= len;
    }
}

// Compute the total force per particle for a given chain.
std::vector<Vec3d> computeForces(const std::vector<Vec3d>& chain,
                                 const std::vector<double>& restingLeft,
                                 const std::vector<double>& restingRight,
                                 double springConstant,
                                 double propagationScale) {
    std::vector<Vec3d> forces(chain.size());
    for (std::size_t i = 0; i < chain.size(); ++i) {
        // Propagation force: normalized z-axis scaled.
        Vec3d propagation(0.0, 0.0, propagationScale);  // (0,0,1) normalized is itself.

        // Spring forces.
        Vec3d spring(0.0, 0.0, 0.0);
        if (i > 0) {
            Vec3d vec = chain[i] - chain[i-1];  // from left to current
            double dist = norm(vec);
            double mag = springConstant * (dist - restingLeft[i]);
            if (dist > 1e-12) {
                spring += vec * (mag / dist);  // unit direction scaled by magnitude
            }
        }
        if (i + 1 < chain.size()) {
            Vec3d vec = chain[i+1] - chain[i];  // from current to right
            double dist = norm(vec);
            double mag = springConstant * (dist - restingRight[i]);
            if (dist > 1e-12) {
                spring += vec * (mag / dist);
            }
        }

        Vec3d total = propagation + spring;
        normalize(total);  // normalize total force (if near zero, leaves as is)
        forces[i] = total;
    }
    return forces;
}

// Perform one Runge-Kutta 4th order integration step for the particle chain.
std::vector<Vec3d> structureTensorParticleSimStep(const std::vector<Vec3d>& chain,
                                                  double stepSize,
                                                  double springConstant,
                                                  double propagationScale) {
    if (chain.size() < 2) {
        return chain;  // no springs possible, propagate unchanged per spec
    }

    // Compute resting distances from input chain.
    std::vector<double> restingLeft(chain.size(), 0.0);
    std::vector<double> restingRight(chain.size(), 0.0);
    for (std::size_t i = 0; i < chain.size(); ++i) {
        if (i > 0) {
            restingLeft[i] = norm(chain[i] - chain[i-1]);
        }
        if (i + 1 < chain.size()) {
            restingRight[i] = norm(chain[i+1] - chain[i]);
        }
    }

    // Helper lambda to evaluate forces on a given chain.
    auto forceFunc = [&](const std::vector<Vec3d>& c) {
        return computeForces(c, restingLeft, restingRight, springConstant, propagationScale);
    };

    std::vector<Vec3d> current = chain;
    std::size_t n = chain.size();

    // K1
    std::vector<Vec3d> k1 = forceFunc(current);

    // K2: chain + 0.5*step*k1
    std::vector<Vec3d> mid1(n);
    for (std::size_t i = 0; i < n; ++i) {
        mid1[i] = current[i] + k1[i] * (0.5 * stepSize);
    }
    std::vector<Vec3d> k2 = forceFunc(mid1);

    // K3: chain + 0.5*step*k2
    std::vector<Vec3d> mid2(n);
    for (std::size_t i = 0; i < n; ++i) {
        mid2[i] = current[i] + k2[i] * (0.5 * stepSize);
    }
    std::vector<Vec3d> k3 = forceFunc(mid2);

    // K4: chain + step*k3
    std::vector<Vec3d> mid3(n);
    for (std::size_t i = 0; i < n; ++i) {
        mid3[i] = current[i] + k3[i] * stepSize;
    }
    std::vector<Vec3d> k4 = forceFunc(mid3);

    // Update positions.
    std::vector<Vec3d> result(n);
    for (std::size_t i = 0; i < n; ++i) {
        Vec3d sum = k1[i] + k2[i] * 2.0 + k3[i] * 2.0 + k4[i];
        result[i] = current[i] + sum * (stepSize / 6.0);
    }
    return result;
}
#include <cassert>
#include <cmath>
#include <vector>

// The solution function prototype is already included above.

int main() {
    // Test 1: Single particle chain should remain unchanged (returns copy).
    std::vector<Vec3d> single = {Vec3d(1,2,3)};
    auto res1 = structureTensorParticleSimStep(single, 0.1, 1.0, 1.0);
    assert(res1.size() == 1);
    assert(std::fabs(res1[0].x - 1.0) < 1e-9);
    assert(std::fabs(res1[0].y - 2.0) < 1e-9);
    assert(std::fabs(res1[0].z - 3.0) < 1e-9);

    // Test 2: Two particles with initial resting distance 1, step size 0, should not move.
    std::vector<Vec3d> two = {Vec3d(0,0,0), Vec3d(1,0,0)};
    auto res2 = structureTensorParticleSimStep(two, 0.0, 1.0, 0.0);
    assert(res2.size() == 2);
    assert(std::fabs(res2[0].x - 0.0) < 1e-9);
    assert(std::fabs(res2[0].z - 0.0) < 1e-9);
    assert(std::fabs(res2[1].x - 1.0) < 1e-9);

    // Test 3: Two particles, small step, propagation only (springs disabled by large constant not used? Actually spring forces are present).
    // Use springs with zero constant to isolate propagation.
    std::vector<Vec3d> chain = {Vec3d(0,0,0), Vec3d(1,0,0)};
    auto res3 = structureTensorParticleSimStep(chain, 0.1, 0.0, 1.0);
    // Propagation force is (0,0,1) normalized, so z increases for both.
    assert(res3[0].z > 0.0);
    assert(res3[1].z > 0.0);
    // Spring forces zero, so x positions should stay roughly constant.
    assert(std::fabs(res3[0].x - 0.0) < 1e-6);
    assert(std::fabs(res3[1].x - 1.0) < 1e-6);

    // Test 4: Spring force pulls too-close particles apart. Chain of two at (0,0,0) and (0.5,0,0), resting distance 1 (as defined from initial), spring constant 1, propagation 0.
    // At t=0, left spring force on particle 0 is zero (no left neighbor). Right spring force on particle0: vec from p0 to p1 = (0.5,0,0), dist=0.5, mag=1*(0.5-1)=-0.5, so force direction would be opposite to vec (pushing p0 left), but force is toward right? Actually definition: vec = chain[i+1] - chain[i]; mag = K*(dist-resting); so for dist < resting, mag negative, force = vec*(mag/dist) = (0.5,0,0)*(-0.5/0.5)=(-0.5,0,0). That pushes p0 left. For particle1, left spring: vec = chain[1]-chain[0]=(0.5,0,0); mag=-0.5; force = (0.5,0,0)*(-0.5/0.5)=(-0.5,0,0), pushing p1 left? That's odd but according to spec that's correct. Anyway we just test that some movement occurs.
    std::vector<Vec3d> close = {Vec3d(0,0,0), Vec3d(0.5,0,0)};
    auto res4 = structureTensorParticleSimStep(close, 0.1, 1.0, 0.0);
    // Assert that positions changed from initial.
    assert(!(std::fabs(res4[0].x - 0.0) < 1e-9 && std::fabs(res4[0].y - 0.0) < 1e-9 && std::fabs(res4[0].z - 0.0) < 1e-9));

    // Test 5: Chain of three aligned particles, all forces zero (propagation 0, resting distances match exactly, spring constant any). Should remain unchanged.
    std::vector<Vec3d> aligned = {Vec3d(0,0,0), Vec3d(1,0,0), Vec3d(2,0,0)};
    auto res5 = structureTensorParticleSimStep(aligned, 0.5, 1.0, 0.0);
    assert(res5.size() == 3);
    for (std::size_t i = 0; i < 3; ++i) {
        assert(std::fabs(res5[i].x - aligned[i].x) < 1e-9);
        assert(std::fabs(res5[i].y - aligned[i].y) < 1e-9);
        assert(std::fabs(res5[i].z - aligned[i].z) < 1e-9);
    }

    // Test 6: Ensure propagation moves all particles up in z with zero springs.
    std::vector<Vec3d> three = {Vec3d(0,0,0), Vec3d(1,0,0), Vec3d(2,0,0)};
    auto res6 = structureTensorParticleSimStep(three, 0.2, 0.0, 1.0);
    for (const auto& p : res6) {
        assert(p.z > 0.0);
    }

    // Test 7: Large step with propagation only, all z increase monotonically.
    auto res7 = structureTensorParticleSimStep(three, 1.0, 0.0, 1.0);
    for (std::size_t i = 0; i < res7.size(); ++i) {
        assert(res7[i].z > res6[i].z);  // larger step leads to larger z change
    }

    // Test 8: Empty chain returns empty.
    std::vector<Vec3d> empty;
    auto res8 = structureTensorParticleSimStep(empty, 0.1, 1.0, 1.0);
    assert(res8.empty());

    return 0;
}
