// Write a C++ function named `updateBodies` that performs one gravity-based physics simulation step on a collection of bodies in 2D space. Given a vector of body structs (each containing position `pos`, velocity `vel`, and mass `mass`), a real-world time delta `dtReal` (in seconds), a time scale factor `timeScale`, and a softening parameter `soften`, the function must update all velocities using pairwise gravitational acceleration (with gravitational constant `G = 6.67430e-11`), then update all positions using their new velocities. Use `double` for all numeric values. The function must be `const`-correct (i.e., it should not modify the vector's structure, only the body states). Assume the input vector is non-empty and all masses are positive. The update formulas are: for each pair (i, j), acceleration on i due to j is `G * mass_j * r / (|r|^2 + soften)^1.5`, where `r = pos_j - pos_i`. Then `vel_i += acc * dtSim` where `dtSim = dtReal * timeScale`, and finally `pos_i += vel_i * dtSim`. The function signature must be: `void updateBodies(std::vector<Body>& bodies, double dtReal, double timeScale, double soften)`.

#include <cassert>
#include <cmath>
#include <vector>

// Body struct and updateBodies as defined in solution above

int main() {
    // Test 1: Two bodies, symmetric attraction, small dt.
    {
        std::vector<Body> bodies = {
            {0.0, 0.0, 0.0, 0.0, 10.0},
            {1.0, 0.0, 0.0, 0.0, 10.0}
        };
        updateBodies(bodies, 1.0, 1.0, 0.1);
        // Both should move towards each other; acceleration magnitude equal.
        // We only check that positions changed and velocities are non-zero.
        assert(bodies[0].pos_x > 0.0);
        assert(bodies[1].pos_x < 1.0);
        assert(bodies[0].vel_x > 0.0);
        assert(bodies[1].vel_x < 0.0);
    }

    // Test 2: Single body should stay still (no self interaction).
    {
        std::vector<Body> bodies = {
            {5.0, -3.0, 1.0, 2.0, 100.0}
        };
        updateBodies(bodies, 0.5, 2.0, 0.01);
        assert(bodies[0].pos_x == 6.0);
        assert(bodies[0].pos_y == -1.0);
        assert(bodies[0].vel_x == 1.0);
        assert(bodies[0].vel_y == 2.0);
    }

    // Test 3: Time scale multiplies dtReal.
    {
        std::vector<Body> bodies = {
            {0.0, 0.0, 0.0, 0.0, 1.0},
            {2.0, 0.0, 0.0, 0.0, 1.0}
        };
        updateBodies(bodies, 1.0, 0.0, 0.1); // timeScale = 0 => no motion
        assert(bodies[0].pos_x == 0.0);
        assert(bodies[1].pos_x == 2.0);
        assert(bodies[0].vel_x == 0.0);
    }

    // Test 4: Softening prevents infinite acceleration at zero distance.
    {
        std::vector<Body> bodies = {
            {1.0, 1.0, 0.0, 0.0, 5.0},
            {1.0, 1.0, 0.0, 0.0, 5.0}
        };
        updateBodies(bodies, 0.1, 1.0, 0.5); // same position, softening > 0
        // Acceleration is finite, but velocities should be symmetric and opposite.
        assert(bodies[0].vel_x == -bodies[1].vel_x);
        assert(bodies[0].vel_y == -bodies[1].vel_y);
        assert(bodies[0].vel_x != 0.0 || bodies[0].vel_y != 0.0);
    }

    // Test 5: Three-body case, verify energy-like symmetry (velocities sum zero if total momentum zero initially).
    {
        std::vector<Body> bodies = {
            {0.0, 0.0, 0.0, 0.0, 2.0},
            {1.0, 0.0, 0.0, 0.0, 3.0},
            {0.5, 2.0, 0.0, 0.0, 4.0}
        };
        updateBodies(bodies, 0.01, 1.0, 0.001);
        double totalMomentumX = 0.0;
        double totalMomentumY = 0.0;
        for (const auto& b : bodies) {
            totalMomentumX += b.mass * b.vel_x;
            totalMomentumY += b.mass * b.vel_y;
        }
        // Momentum before was zero, so after should be (near) zero.
        assert(std::fabs(totalMomentumX) < 1e-10);
        assert(std::fabs(totalMomentumY) < 1e-10);
    }

    return 0;
}

#include <vector>
#include <cmath>

struct Body {
    double pos_x, pos_y;
    double vel_x, vel_y;
    double mass;
};

// Update velocities and positions for one gravity simulation step.
// All bodies are processed pairwise; softening avoids singularities.
void updateBodies(std::vector<Body>& bodies, double dtReal, double timeScale, double soften) {
    const double G = 6.67430e-11; // gravitational constant
    const double dtSim = dtReal * timeScale;
    const size_t n = bodies.size();

    // First pass: update velocities using current positions
    for (size_t i = 0; i < n; ++i) {
        double acc_x = 0.0;
        double acc_y = 0.0;

        for (size_t j = 0; j < n; ++j) {
            if (i == j) continue;

            double rx = bodies[j].pos_x - bodies[i].pos_x;
            double ry = bodies[j].pos_y - bodies[i].pos_y;

            double dist2 = rx * rx + ry * ry + soften;
            double invD = 1.0 / std::sqrt(dist2);
            double factor = G * bodies[j].mass * invD * invD * invD;

            acc_x += factor * rx;
            acc_y += factor * ry;
        }

        bodies[i].vel_x += acc_x * dtSim;
        bodies[i].vel_y += acc_y * dtSim;
    }

    // Second pass: update positions using new velocities
    for (auto& b : bodies) {
        b.pos_x += b.vel_x * dtSim;
        b.pos_y += b.vel_y * dtSim;
    }
}

// The algorithm is a direct O(n²) all-pairs simulation, standard for small n. For each body `i`, we accumulate an acceleration vector (initially zero) by iterating over all `j ≠ i`. We compute the displacement vector `r`, its squared length `dist2 = dot(r,r) + soften`, then the inverse of the square root: `invD = 1/sqrt(dist2)`. The acceleration contribution is `G * mass_j * invD * invD * invD * r` (since `r * invD` is unit direction and `invD^2` scales by `1/dist2`). After summing all contributions, we update velocity: `vel_i += acc * dtSim`. After all velocities are updated, we update every position: `pos += vel * dtSim`. Edge cases: `soften` must be positive or zero to avoid division by zero when bodies overlap (but with positive mass and zero distance, softening prevents singularity); if `soften` is zero and two bodies coincide, `dist2 = 0` causing division by zero—so we assume caller provides a small positive `soften` or guarantees non-overlap. Time complexity is O(n²) per call, space O(1) auxiliary (only the acceleration vector per iteration). The function modifies the input vector in-place, which matches the typical use.
