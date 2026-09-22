// Write a C++ function `stablePartitionByGravity` that takes a `std::vector<Vec3f>` (where `Vec3f` is a simple 3D vector type with `x`, `y`, `z` public members and basic arithmetic operators) representing particle positions, a `Vec3f` representing the position of a gravitational well, and a `float` time step `dt`. The function should simulate one physics update step: for each particle, compute the gravitational acceleration toward the well using Newton's law of gravity with a force law \(a = \frac{G}{r^2} \cdot \hat{r}\), where \(G = 1.0\) and the distance \(r\) is clamped to a minimum of `0.1f` to prevent extreme velocities. Then update each particle's velocity and position using the semi-implicit Euler method (velocity first, then position). The particle velocities are stored in a parallel `std::vector<Vec3f>` passed by reference, and the function should modify both the positions and velocities in place. For particles whose updated position lies farther than a cutoff radius of `3.0` from the well, remove them from both vectors (i.e., stable partition the remaining particles to the front and resize, preserving relative order). The function must return the new size of the particles vector (and velocities vector). Assume `Vec3f` has member functions `mag()`, normalization via division by magnitude, and operator overloads for `+`, `-`, `*`, `/` with scalars, and also a default constructor that initializes to zero. The function should be `const`-correct where appropriate and work with any `float` `dt` including zero or negative values (treat negative `dt` as zero, but not remove particles in that case? Actually just apply the update with the given `dt`; no special handling for negative other than it will reverse motion, but still perform the removal check). Ensure the function does not leak memory and uses only standard library components.
// The core algorithm is straightforward: iterate over each particle index from 0 up to the current size, compute the displacement vector `r = well - pos[i]`. Compute its magnitude `dist = r.mag()`, clamp it to at least `0.1f`. Compute the acceleration `a = r / (dist * dist * dist)` (since `r` already points from particle to well, this yields acceleration toward the well). Then update velocity `vel[i] += a * dt`, then position `pos[i] += vel[i] * dt`. After the loop, we need to remove particles that are outside the cutoff distance `3.0`. The most efficient approach that preserves order is to use the remove-erase idiom: compute the new end iterator for particles (based on the condition `(well - pos).mag() <= 3.0f`), then erase those beyond that. Since we have two parallel vectors, we can use `std::remove_if` with an index-based lambda that accesses both vectors, but `std::remove_if` only works on one container; we need to manually swap or use a stable partition that handles both vectors simultaneously. A simple O(n) approach: maintain a write index `w`, iterate over read index `r` from 0 to current size, if the particle is within cutoff, copy (or swap) the element at `r` to index `w` and increment `w`. After the loop, resize both vectors to `w`. This is stable and O(n). Edge cases: if `dt` is zero, velocities and positions unchanged, but removal still happens. If `dt` is negative, the update moves backward, but still check cutoff. If the vectors are empty or have different sizes, handle gracefully: if sizes differ, resizing to the minimum? Actually, assume they are always of equal size per contract. The time complexity is O(n) for one update step, and space complexity O(1) auxiliary. The removal step is also O(n). We must use `std::max` from `<algorithm>` for clamping, and `std::vector` from `<vector>`. The function signature should be `size_t stablePartitionByGravity(std::vector<Vec3f>& particles, std::vector<Vec3f>& velocities, const Vec3f& well, float dt)`.
#include <vector>
#include <algorithm>
#include <cstddef>

// A minimal 3D vector type matching the task requirements.
struct Vec3f {
    float x, y, z;
    Vec3f(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    
    Vec3f operator+(const Vec3f& o) const { return Vec3f(x + o.x, y + o.y, z + o.z); }
    Vec3f operator-(const Vec3f& o) const { return Vec3f(x - o.x, y - o.y, z - o.z); }
    Vec3f operator*(float s) const { return Vec3f(x * s, y * s, z * s); }
    Vec3f operator/(float s) const { return Vec3f(x / s, y / s, z / s); }
    float mag() const { return std::sqrt(x*x + y*y + z*z); }
};

// Simulate one gravity step and remove particles beyond cutoff radius.
// Returns the new number of particles (and velocities, same size).
std::size_t stablePartitionByGravity(std::vector<Vec3f>& particles,
                                     std::vector<Vec3f>& velocities,
                                     const Vec3f& well,
                                     float dt) {
    // Ensure both vectors have same size (contract).
    if (particles.size() != velocities.size()) {
        // Handle gracefully by taking the minimum size.
        std::size_t minSize = std::min(particles.size(), velocities.size());
        particles.resize(minSize);
        velocities.resize(minSize);
    }

    // Update positions and velocities using semi-implicit Euler.
    for (std::size_t i = 0; i < particles.size(); ++i) {
        Vec3f r = well - particles[i];          // displacement vector from particle to well
        float dist = r.mag();
        dist = std::max(dist, 0.1f);            // clamp to avoid extreme force
        Vec3f acc = r / (dist * dist * dist);   // acceleration = G * (r / r^3), G=1
        
        velocities[i] = velocities[i] + acc * dt;
        particles[i] = particles[i] + velocities[i] * dt;
    }

    // Stable partition: keep particles within cutoff radius of 3.0.
    const float cutoff = 3.0f;
    std::size_t writeIdx = 0;
    for (std::size_t readIdx = 0; readIdx < particles.size(); ++readIdx) {
        Vec3f r = well - particles[readIdx];
        if (r.mag() <= cutoff) {
            if (writeIdx != readIdx) {
                particles[writeIdx] = particles[readIdx];
                velocities[writeIdx] = velocities[readIdx];
            }
            ++writeIdx;
        }
    }
    particles.resize(writeIdx);
    velocities.resize(writeIdx);
    return writeIdx;
}
#include <cassert>
#include <cmath>

// The Vec3f definition is assumed to be provided above; for testing, include it here or use a header.
// For brevity, assume Vec3f is defined as in the solution.

int main() {
    // Test 1: Simple single particle moving toward well (well at origin, particle at (1,0,0))
    {
        std::vector<Vec3f> particles = {Vec3f(1,0,0)};
        std::vector<Vec3f> velocities = {Vec3f(0,0,0)};
        Vec3f well(0,0,0);
        float dt = 1.0f;
        std::size_t n = stablePartitionByGravity(particles, velocities, well, dt);
        assert(n == 1);
        // Acceleration magnitude = 1 / (1^2) = 1, so velocity after dt = 1
        assert(std::abs(velocities[0].x - (-1.0f)) < 1e-5); // acceleration toward origin negative x
        assert(std::abs(velocities[0].y) < 1e-5);
        assert(std::abs(velocities[0].z) < 1e-5);
        // Position = 1 + (-1)*1 = 0
        assert(std::abs(particles[0].x) < 1e-5);
    }

    // Test 2: Removal of far particle, preserving order
    {
        std::vector<Vec3f> particles = {Vec3f(0,0,0), Vec3f(5,0,0), Vec3f(1,0,0), Vec3f(10,0,0)};
        std::vector<Vec3f> velocities = {Vec3f(0,0,0), Vec3f(0,0,0), Vec3f(0,0,0), Vec3f(0,0,0)};
        Vec3f well(0,0,0);
        float dt = 0.0f; // no motion, just removal
        std::size_t n = stablePartitionByGravity(particles, velocities, well, dt);
        assert(n == 2);
        assert(particles[0].x == 0.0f);
        assert(particles[1].x == 1.0f);
        assert(velocities.size() == 2);
    }

    // Test 3: Clamping prevents high velocity when very close
    {
        std::vector<Vec3f> particles = {Vec3f(0.01f,0,0)};
        std::vector<Vec3f> velocities = {Vec3f(0,0,0)};
        Vec3f well(0,0,0);
        float dt = 1.0f;
        std::size_t n = stablePartitionByGravity(particles, velocities, well, dt);
        assert(n == 1);
        // Distance clamped to 0.1, acceleration magnitude = 1/(0.1^2) = 100
        // Velocity should not exceed -100 in magnitude
        assert(std::abs(velocities[0].x) <= 100.0f);
        // Position moved by 100 units toward origin, but still within? Actually origin at 0, particle at 0.01, distance = 0.01, clamp to 0.1, acc = r/(0.1^3) = 0.01/0.001=10? Wait r/(r^3)=1/r^2, with r clamped to 0.1, so 1/0.01=100, direction negative, so vel=-100, pos=0.01-100=-99.99, which is beyond cutoff 3, so it should be removed
        assert(n == 0);
        assert(particles.empty());
    }

    // Test 4: Negative dt (optional, but check no crash and removal still works)
    {
        std::vector<Vec3f> particles = {Vec3f(0,0,0)};
        std::vector<Vec3f> velocities = {Vec3f(0,0,0)};
        Vec3f well(0,0,0);
        float dt = -1.0f;
        std::size_t n = stablePartitionByGravity(particles, velocities, well, dt);
        assert(n == 1); // zero distance, force infinite? Actually distance 0 -> clamped to 0.1, acceleration zero because r=0 -> 0/(something)=0, so stays
    }

    // Test 5: Empty vectors
    {
        std::vector<Vec3f> p, v;
        std::size_t n = stablePartitionByGravity(p, v, Vec3f(0,0,0), 1.0f);
        assert(n == 0);
    }

    return 0;
}
