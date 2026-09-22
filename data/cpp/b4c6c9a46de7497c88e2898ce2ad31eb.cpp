Given a 2D scene with a particle system where particles move, collide elastically (swapping velocities), and fade out over time, write a standalone C++ function that simulates one frame of the particle system and returns the number of particles still alive after the frame. The function should take a vector of particles (each with `x`, `y`, `vx`, `vy`, `alpha`, `size`, and `isAlive` fields) and a boolean flag indicating whether collision checking should be performed this frame. For each particle, update its position by adding velocity, decrease its alpha by a constant fade rate (e.g., 0.01), set `isAlive` to false if alpha drops to zero or below, and bounce off bottom and left walls (reversing the respective velocity component, with the particle clamped inside the bounds). If the collision flag is true, perform pairwise elastic collisions (swap velocities) between all alive particles whose centers are closer than the sum of their radii. Return the count of alive particles after all updates. The function must be self-contained (no external graphics libraries), use only standard C++ headers, and operate on a provided `Particle` struct definition that you also define.
// The core algorithm is straightforward: iterate over all particles, apply motion and fading, handle wall collisions by checking if `x` is less than the particle radius (or 0) and if `y` is greater than the window height minus the radius (or simply above some bottom bound). To simplify the problem, assume a fixed world of size 800x600 (or define constants). For each particle, if it is past the left wall, set its position to the wall and reverse `vx`; similarly for the bottom wall, set its y to the bottom bound and reverse `vy`. For the fade, subtract a constant fade rate from alpha; if alpha <= 0, mark as not alive. After updating all particles, if collision checking is requested, use a nested loop over pairs of alive particles, compute the Euclidean distance, and if it is less than the sum of half-sizes, swap their velocity vectors (elastic collision with equal masses). The key edge case is not checking collisions against dead particles, and not updating dead particles beyond marking them dead (though updating them is harmless if we ignore them). Time complexity is O(N) for the update plus O(N²) for collisions when enabled, where N is the number of particles. Space complexity is O(1) auxiliary.
#include <vector>
#include <cmath>
#include <algorithm>

// Constants for the simulation world
const double WORLD_WIDTH = 800.0;
const double WORLD_HEIGHT = 600.0;
const double FADE_RATE = 0.01;

// Particle structure
struct Particle {
    double x, y, vx, vy;
    double alpha;
    double size;
    bool isAlive;

    Particle(double x, double y, double vx, double vy, double size)
        : x(x), y(y), vx(vx), vy(vy), size(size), alpha(1.0), isAlive(true) {}
};

// Simulate one frame. If checkCollisions is true, perform pairwise elastic collisions.
// Returns the number of alive particles after the frame.
int simulateParticleFrame(std::vector<Particle>& particles, bool checkCollisions) {
    // Update positions, fade, and handle wall bounces
    for (auto& p : particles) {
        if (!p.isAlive) continue;

        // Move
        p.x += p.vx;
        p.y += p.vy;

        // Fade
        p.alpha -= FADE_RATE;
        if (p.alpha <= 0.0) {
            p.isAlive = false;
            continue;
        }

        // Left wall bounce (assuming particle stays within left boundary)
        double radius = p.size / 2.0;
        if (p.x < radius) {
            p.x = radius;
            p.vx = std::abs(p.vx);  // Ensure positive vx moving right
        }

        // Bottom wall bounce (assuming y increases downward, bottom is at WORLD_HEIGHT)
        if (p.y > WORLD_HEIGHT - radius) {
            p.y = WORLD_HEIGHT - radius;
            p.vy = -std::abs(p.vy); // Ensure negative vy moving up
        }
    }

    // Collision detection and resolution if requested
    if (checkCollisions) {
        size_t n = particles.size();
        for (size_t i = 0; i < n; ++i) {
            if (!particles[i].isAlive) continue;
            for (size_t j = i + 1; j < n; ++j) {
                if (!particles[j].isAlive) continue;

                double dx = particles[i].x - particles[j].x;
                double dy = particles[i].y - particles[j].y;
                double dist = std::sqrt(dx * dx + dy * dy);
                double minDist = (particles[i].size + particles[j].size) / 2.0;

                if (dist < minDist) {
                    // Swap velocities (elastic collision with equal masses)
                    std::swap(particles[i].vx, particles[j].vx);
                    std::swap(particles[i].vy, particles[j].vy);
                }
            }
        }
    }

    // Count alive particles
    int aliveCount = 0;
    for (const auto& p : particles) {
        if (p.isAlive) ++aliveCount;
    }
    return aliveCount;
}
#include <cassert>
#include <vector>
#include <cmath>

// Include the solution above (or copy the Particle struct and function)

int main() {
    // Test 1: Basic movement and fade
    {
        std::vector<Particle> particles;
        particles.emplace_back(100, 100, 1.0, 0.0, 10.0);
        int alive = simulateParticleFrame(particles, false);
        assert(alive == 1);
        assert(std::abs(particles[0].x - 101.0) < 1e-6);
        assert(std::abs(particles[0].y - 100.0) < 1e-6);
        assert(std::abs(particles[0].alpha - 0.99) < 1e-6);
    }

    // Test 2: Particle fades out completely
    {
        std::vector<Particle> particles;
        particles.emplace_back(50, 50, 0, 0, 10.0);
        particles[0].alpha = 0.005;
        int alive = simulateParticleFrame(particles, false);
        assert(alive == 0);
        assert(!particles[0].isAlive);
    }

    // Test 3: Left wall bounce (velocity reversed to positive)
    {
        std::vector<Particle> particles;
        particles.emplace_back(2.0, 100.0, -5.0, 0.0, 10.0); // radius=5, so should clamp to x=5 and vx=5
        int alive = simulateParticleFrame(particles, false);
        assert(alive == 1);
        assert(std::abs(particles[0].x - 5.0) < 1e-6);
        assert(particles[0].vx > 0);
    }

    // Test 4: Bottom wall bounce (velocity reversed to negative)
    {
        std::vector<Particle> particles;
        particles.emplace_back(100.0, 597.0, 0.0, 5.0, 10.0); // radius=5, bottom at 600, so y should clamp to 595
        int alive = simulateParticleFrame(particles, false);
        assert(alive == 1);
        assert(std::abs(particles[0].y - 595.0) < 1e-6);
        assert(particles[0].vy < 0);
    }

    // Test 5: Collision swaps velocities (identical size particles)
    {
        std::vector<Particle> particles;
        particles.emplace_back(10, 10, 2.0, 0.0, 10.0);
        particles.emplace_back(14, 10, -1.0, 0.0, 10.0); // distance=4, sum radii=10, so they collide
        int alive = simulateParticleFrame(particles, true);
        assert(alive == 2);
        // After collision, first particle should have vx = -1, second vx = 2
        assert(std::abs(particles[0].vx - (-1.0)) < 1e-6);
        assert(std::abs(particles[1].vx - 2.0) < 1e-6);
    }

    // Test 6: Dead particles are not updated or collided
    {
        std::vector<Particle> particles;
        particles.emplace_back(10, 10, 1.0, 0.0, 10.0);
        particles.emplace_back(14, 10, -1.0, 0.0, 10.0);
        particles[0].isAlive = false; // mark first dead
        int alive = simulateParticleFrame(particles, true);
        assert(alive == 1);
        // Dead particle should not move
        assert(particles[0].x == 10.0);
        // Alive particle should still move and not have velocity swapped
        assert(std::abs(particles[1].x - 13.0) < 1e-6);
        assert(std::abs(particles[1].vx - (-1.0)) < 1e-6);
    }

    // Test 7: No collision when distance exactly equals sum of radii
    {
        std::vector<Particle> particles;
        particles.emplace_back(10, 10, 2.0, 0.0, 10.0);
        particles.emplace_back(20, 10, -1.0, 0.0, 10.0); // distance=10, sum radii=10 -> no collision
        int alive = simulateParticleFrame(particles, true);
        assert(alive == 2);
        assert(std::abs(particles[0].vx - 2.0) < 1e-6);
        assert(std::abs(particles[1].vx - (-1.0)) < 1e-6);
    }

    // Test 8: Multiple particles and counting
    {
        std::vector<Particle> particles;
        particles.emplace_back(100, 100, 0, 0, 5.0);
        particles.emplace_back(200, 200, 0, 0, 5.0);
        particles[1].alpha = 0.005; // will die this frame
        int alive = simulateParticleFrame(particles, false);
        assert(alive == 1);
    }

    // Test 9: Empty vector
    {
        std::vector<Particle> particles;
        int alive = simulateParticleFrame(particles, true);
        assert(alive == 0);
    }

    // Test 10: Collision with multiple particles in a cluster
    {
        std::vector<Particle> particles;
        particles.emplace_back(100, 100, 1.0, 0.0, 10.0);
        particles.emplace_back(104, 100, -1.0, 0.0, 10.0);
        particles.emplace_back(108, 100, 0.0, 0.0, 10.0);
        int alive = simulateParticleFrame(particles, true);
        assert(alive == 3);
        // After frame, velocities should have been swapped among all colliding pairs
        // This is a simple sanity check that all are alive and no crash
        assert(particles[0].isAlive && particles[1].isAlive && particles[2].isAlive);
    }

    return 0;
}
