/*
Write a C++ free function named `applyTimeStep` that performs a single leapfrog time-step update of positions and velocities stored in a small physics simulation struct. The function takes a reference to a `ParticleSystem` struct containing arrays for positions (`posX`, `posY`), velocities (`velX`, `velY`), and a constant `dt` (time step size), plus a global force vector `forceX`, `forceY` applied to all particles. The update rule is: first update velocities by half a step using current forces, then update positions by a full step using the new velocities, then update velocities by another half step using forces recomputed (in a simplified way, keep the same force for this task). The number of particles is given by `count`. The function must modify the arrays in place and be `const`-correct: the `ParticleSystem` reference is non-const, but the function should not change `dt`, and should take `const double*` for force arrays. Ensure the function is efficient, handles zero or negative `count` gracefully (no-op), and uses only standard C++ headers.
*/

#include <cstddef>

// Particle system holding positions, velocities, and a fixed time step.
struct ParticleSystem {
    double* posX;
    double* posY;
    double* velX;
    double* velY;
    const double dt; // fixed time step size

    ParticleSystem(double* px, double* py, double* vx, double* vy, double dt_value)
        : posX(px), posY(py), velX(vx), velY(vy), dt(dt_value) {}
};

// Perform one leapfrog time step for all particles. Forces are constant for the step.
// Modifies pos arrays in place. No effect if count <= 0.
void applyTimeStep(ParticleSystem& particles, size_t count,
                   const double* forceX, const double* forceY) {
    const double half_dt = 0.5 * particles.dt;
    for (size_t i = 0; i < count; ++i) {
        // First half velocity update
        particles.velX[i] += half_dt * forceX[i];
        particles.velY[i] += half_dt * forceY[i];
        // Full position update
        particles.posX[i] += particles.dt * particles.velX[i];
        particles.posY[i] += particles.dt * particles.velY[i];
        // Second half velocity update (using same force for simplicity)
        particles.velX[i] += half_dt * forceX[i];
        particles.velY[i] += half_dt * forceY[i];
    }
}

#include <cassert>
#include <cmath>
#include <iostream>

// Include the solution code here (or a header containing it)
struct ParticleSystem {
    double* posX; double* posY; double* velX; double* velY; const double dt;
    ParticleSystem(double* px, double* py, double* vx, double* vy, double dt_value) : posX(px), posY(py), velX(vx), velY(vy), dt(dt_value) {}
};
void applyTimeStep(ParticleSystem& particles, size_t count, const double* forceX, const double* forceY) {
    const double half_dt = 0.5 * particles.dt;
    for (size_t i = 0; i < count; ++i) {
        particles.velX[i] += half_dt * forceX[i];
        particles.velY[i] += half_dt * forceY[i];
        particles.posX[i] += particles.dt * particles.velX[i];
        particles.posY[i] += particles.dt * particles.velY[i];
        particles.velX[i] += half_dt * forceX[i];
        particles.velY[i] += half_dt * forceY[i];
    }
}

int main() {
    // Manual test vectors
    double posX[3] = {0.0, 1.0, -2.0};
    double posY[3] = {0.0, 2.0, 3.0};
    double velX[3] = {1.0, 0.5, -1.0};
    double velY[3] = {0.5, -1.0, 2.0};
    double forceX[3] = {2.0, -1.0, 0.5};
    double forceY[3] = {-0.5, 1.5, 2.0};
    double dt = 0.5;

    // Compute expected manually using the rule:
    // v_half = v + 0.5*dt*f
    // pos_new = pos + dt * v_half
    // v_new = v_half + 0.5*dt*f = v + dt*f
    double expected_velX[3], expected_velY[3], expected_posX[3], expected_posY[3];
    for (int i = 0; i < 3; ++i) {
        double v_hx = velX[i] + 0.5 * dt * forceX[i];
        double v_hy = velY[i] + 0.5 * dt * forceY[i];
        expected_posX[i] = posX[i] + dt * v_hx;
        expected_posY[i] = posY[i] + dt * v_hy;
        expected_velX[i] = v_hx + 0.5 * dt * forceX[i]; // = velX[i] + dt*forceX[i]
        expected_velY[i] = v_hy + 0.5 * dt * forceY[i];
    }

    ParticleSystem ps(posX, posY, velX, velY, dt);
    applyTimeStep(ps, 3, forceX, forceY);

    for (int i = 0; i < 3; ++i) {
        assert(std::abs(posX[i] - expected_posX[i]) < 1e-12);
        assert(std::abs(posY[i] - expected_posY[i]) < 1e-12);
        assert(std::abs(velX[i] - expected_velX[i]) < 1e-12);
        assert(std::abs(velY[i] - expected_velY[i]) < 1e-12);
    }

    // Edge case: count zero should be no-op
    applyTimeStep(ps, 0, forceX, forceY);
    // Arrays unchanged (already verified, but just ensure no crash)

    // Edge case: dt = 0 should leave positions unchanged, velocities unchanged
    double posX2[2] = {3.0, -1.0};
    double posY2[2] = {0.5, 2.5};
    double velX2[2] = {1.0, -2.0};
    double velY2[2] = {-1.0, 0.25};
    ParticleSystem ps2(posX2, posY2, velX2, velY2, 0.0);
    applyTimeStep(ps2, 2, forceX, forceY);
    assert(posX2[0] == 3.0 && posY2[0] == 0.5 && velX2[0] == 1.0 && velY2[0] == -1.0);
    assert(posX2[1] == -1.0 && posY2[1] == 2.5 && velX2[1] == -2.0 && velY2[1] == 0.25);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

// The leapfrog (velocity Verlet) integration updates velocities in two half-steps separated by a full position update. The main algorithm is straightforward: for each particle `i` from 0 to `count-1`, compute `velX[i] += 0.5 * dt * forceX[i]` (and similarly for Y), then update `posX[i] += dt * velX[i]` (using the newly updated velocity), then update `velX[i] += 0.5 * dt * forceX[i]` again. This is a classic second-order symplectic integrator. Edge cases: if `count <= 0`, the loop simply does nothing; no special handling needed for `dt` values (including negative or zero, which are allowed but should work mathematically). We must ensure the function signature uses `const double*` for force arrays to respect const-correctness, and the `ParticleSystem` struct should have a `const double dt` member to be safe. Time complexity is O(count) for the single loop, space complexity O(1) auxiliary (only loop indices). The solution will define a `ParticleSystem` struct with fixed-size arrays (or pointers) and implement the function.
