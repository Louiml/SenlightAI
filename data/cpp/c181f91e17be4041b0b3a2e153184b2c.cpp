Write a standalone C++ function that manages a simple particle system similar to the one in the provided code snippet. Specifically, implement a class `ParticleSystem` that stores particles, each with position (x, y, z), velocity (vx, vy, vz), a stellar mass (distinct from a gravitating mass), and a radius. The class must support adding particles, retrieving/setting individual properties via index-based accessors (position, velocity, mass, gravitating mass, radius), computing the total mass, the maximum radius from the origin, and the center-of-mass position and velocity (weighted by gravitating mass). All index-based accessors must validate the index and return `false` on failure, while returning `true` on success. The class should store particles in a `std::vector` and use `struct` or `class` for each particle. The function must be self-contained without requiring any external libraries beyond the C++ standard library, and must not include a `main` function in the solution section.

#include <cassert>
#include <cmath>

int main() {
    ParticleSystem ps;

    // Initially empty.
    assert(ps.getNumber() == 0);
    assert(ps.getTotalMass() == 0.0);
    assert(ps.getTotalRadius() == 0.0);

    // Add first particle at origin, stationary, mass 5.
    int idx0 = ps.addParticle(5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    assert(idx0 == 0);
    assert(ps.getNumber() == 1);
    assert(ps.getTotalMass() == 5.0);
    assert(ps.getTotalRadius() == 0.0);

    // Add second particle at (3,4,0), moving, mass 10.
    int idx1 = ps.addParticle(10.0, 3.0, 4.0, 0.0, 1.0, -2.0, 3.0, 2.0);
    assert(idx1 == 1);
    assert(ps.getNumber() == 2);
    assert(std::abs(ps.getTotalMass() - 15.0) < 1e-12);
    assert(std::abs(ps.getTotalRadius() - 5.0) < 1e-12);

    // Get and set position.
    double x, y, z;
    assert(ps.getPosition(idx1, x, y, z));
    assert(x == 3.0 && y == 4.0 && z == 0.0);
    assert(ps.setPosition(idx1, 1.0, 1.0, 1.0));
    assert(ps.getPosition(idx1, x, y, z));
    assert(x == 1.0 && y == 1.0 && z == 1.0);
    // Now total radius should be sqrt(3) from idx1.
    assert(std::abs(ps.getTotalRadius() - std::sqrt(3.0)) < 1e-12);

    // Get and set velocity.
    double vx, vy, vz;
    assert(ps.getVelocity(idx1, vx, vy, vz));
    assert(vx == 1.0 && vy == -2.0 && vz == 3.0);
    assert(ps.setVelocity(idx1, 0.0, 0.0, 0.0));
    assert(ps.getVelocity(idx1, vx, vy, vz));
    assert(vx == 0.0 && vy == 0.0 && vz == 0.0);

    // Mass scaling: set stellar mass of idx0 from 5 to 20 with gravitating mass 5.
    assert(ps.setMass(idx0, 20.0));
    double gm;
    assert(ps.getGravitatingMass(idx0, gm));
    assert(std::abs(gm - 20.0) < 1e-12); // 5 * (20/5) = 20
    double sm;
    assert(ps.getMass(idx0, sm));
    assert(sm == 20.0);

    // Set gravitating mass directly.
    assert(ps.setGravitatingMass(idx0, 100.0));
    assert(ps.getGravitatingMass(idx0, gm));
    assert(gm == 100.0);
    assert(ps.getMass(idx0, sm));
    assert(sm == 20.0); // stellar mass unchanged

    // Radius accessors.
    double r;
    assert(ps.getRadius(idx1, r));
    assert(r == 2.0);
    assert(ps.setRadius(idx1, 7.5));
    assert(ps.getRadius(idx1, r));
    assert(r == 7.5);

    // Invalid indices.
    assert(!ps.getPosition(2, x, y, z));
    assert(!ps.setPosition(-1, 0, 0, 0));
    assert(!ps.getMass(99, sm));
    assert(!ps.setMass(2, 1.0));

    // Center of mass position: idx0 at (0,0,0) mass 20, idx1 at (1,1,1) mass 10.
    double cmx, cmy, cmz;
    assert(ps.getCenterOfMassPosition(cmx, cmy, cmz));
    // (20*0 + 10*1)/30 = 1/3
    assert(std::abs(cmx - 1.0/3.0) < 1e-12);
    assert(std::abs(cmy - 1.0/3.0) < 1e-12);
    assert(std::abs(cmz - 1.0/3.0) < 1e-12);

    // Center of mass velocity: idx0 vel=0, idx1 vel=0 after set.
    double cmvx, cmvy, cmvz;
    assert(ps.getCenterOfMassVelocity(cmvx, cmvy, cmvz));
    assert(cmvx == 0.0 && cmvy == 0.0 && cmvz == 0.0);

    // Test center of mass with non-zero velocity: set idx1 velocity back.
    assert(ps.setVelocity(idx1, 2.0, 0.0, 0.0));
    assert(ps.getCenterOfMassVelocity(cmvx, cmvy, cmvz));
    // (0*20 + 2*10)/30 = 2/3
    assert(std::abs(cmvx - 2.0/3.0) < 1e-12);
    assert(std::abs(cmvy) < 1e-12);
    assert(std::abs(cmvz) < 1e-12);

    // Empty system center of mass should fail.
    ParticleSystem empty;
    assert(!empty.getCenterOfMassPosition(cmx, cmy, cmz));
    assert(!empty.getCenterOfMassVelocity(cmvx, cmvy, cmvz));

    return 0;
}

#include <vector>
#include <cmath>

class ParticleSystem {
public:
    // Add a particle with given properties, returns its index.
    int addParticle(double mass, double x, double y, double z,
                    double vx, double vy, double vz, double radius) {
        Particle p;
        p.x = x; p.y = y; p.z = z;
        p.vx = vx; p.vy = vy; p.vz = vz;
        p.stellarMass = mass;
        p.gravitatingMass = mass;
        p.radius = radius;
        particles.push_back(p);
        return static_cast<int>(particles.size()) - 1;
    }

    // Accessors return true on success, false on invalid index.
    bool getPosition(int index, double& x, double& y, double& z) const {
        if (!validIndex(index)) return false;
        const Particle& p = particles[index];
        x = p.x; y = p.y; z = p.z;
        return true;
    }

    bool setPosition(int index, double x, double y, double z) {
        if (!validIndex(index)) return false;
        Particle& p = particles[index];
        p.x = x; p.y = y; p.z = z;
        return true;
    }

    bool getVelocity(int index, double& vx, double& vy, double& vz) const {
        if (!validIndex(index)) return false;
        const Particle& p = particles[index];
        vx = p.vx; vy = p.vy; vz = p.vz;
        return true;
    }

    bool setVelocity(int index, double vx, double vy, double vz) {
        if (!validIndex(index)) return false;
        Particle& p = particles[index];
        p.vx = vx; p.vy = vy; p.vz = vz;
        return true;
    }

    bool getMass(int index, double& mass) const {
        if (!validIndex(index)) return false;
        mass = particles[index].stellarMass;
        return true;
    }

    bool setMass(int index, double mass) {
        if (!validIndex(index)) return false;
        Particle& p = particles[index];
        if (p.stellarMass != 0.0) {
            // Scale gravitating mass proportionally.
            p.gravitatingMass *= mass / p.stellarMass;
        } else {
            p.gravitatingMass = mass;
        }
        p.stellarMass = mass;
        return true;
    }

    bool getGravitatingMass(int index, double& mass) const {
        if (!validIndex(index)) return false;
        mass = particles[index].gravitatingMass;
        return true;
    }

    bool setGravitatingMass(int index, double mass) {
        if (!validIndex(index)) return false;
        particles[index].gravitatingMass = mass;
        return true;
    }

    bool getRadius(int index, double& radius) const {
        if (!validIndex(index)) return false;
        radius = particles[index].radius;
        return true;
    }

    bool setRadius(int index, double radius) {
        if (!validIndex(index)) return false;
        particles[index].radius = radius;
        return true;
    }

    // Aggregate queries.
    int getNumber() const { return static_cast<int>(particles.size()); }

    double getTotalMass() const {
        double total = 0.0;
        for (const auto& p : particles) total += p.gravitatingMass;
        return total;
    }

    // Maximum distance from origin; returns 0 if no particles.
    double getTotalRadius() const {
        double maxR = 0.0;
        for (const auto& p : particles) {
            double r = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
            if (r > maxR) maxR = r;
        }
        return maxR;
    }

    // Center of mass position; returns false if no particles (or total mass zero).
    bool getCenterOfMassPosition(double& x, double& y, double& z) const {
        if (particles.empty()) return false;
        double sumX = 0, sumY = 0, sumZ = 0, sumM = 0;
        for (const auto& p : particles) {
            sumX += p.gravitatingMass * p.x;
            sumY += p.gravitatingMass * p.y;
            sumZ += p.gravitatingMass * p.z;
            sumM += p.gravitatingMass;
        }
        if (sumM == 0.0) return false;
        x = sumX / sumM;
        y = sumY / sumM;
        z = sumZ / sumM;
        return true;
    }

    // Center of mass velocity; returns false if no particles (or total mass zero).
    bool getCenterOfMassVelocity(double& vx, double& vy, double& vz) const {
        if (particles.empty()) return false;
        double sumVx = 0, sumVy = 0, sumVz = 0, sumM = 0;
        for (const auto& p : particles) {
            sumVx += p.gravitatingMass * p.vx;
            sumVy += p.gravitatingMass * p.vy;
            sumVz += p.gravitatingMass * p.vz;
            sumM += p.gravitatingMass;
        }
        if (sumM == 0.0) return false;
        vx = sumVx / sumM;
        vy = sumVy / sumM;
        vz = sumVz / sumM;
        return true;
    }

private:
    struct Particle {
        double x, y, z;
        double vx, vy, vz;
        double stellarMass;
        double gravitatingMass;
        double radius;
    };

    std::vector<Particle> particles;

    bool validIndex(int index) const {
        return index >= 0 && index < static_cast<int>(particles.size());
    }
};

// The solution involves creating a `ParticleSystem` class with an internal `std::vector<Particle>` where `Particle` is a simple struct containing `x, y, z, vx, vy, vz, stellarMass, gravitatingMass, radius`. The key operations are:
// - `addParticle(mass, x, y, z, vx, vy, vz, radius)`: appends a new particle, setting both stellar and gravitating mass to the given mass (since in the original code, when adding a particle, both masses are set to the same value). Returns the index of the new particle.
// - Accessors like `getPosition(index, &x, &y, &z)`, `setPosition(index, x, y, z)`, etc., each first check that `index >= 0 && index < particles.size()`, returning `false` otherwise. For setters, modify the corresponding field and return `true`.
// - For `setMass`, to match the original behavior, scaling the gravitating mass proportionally: `gravitatingMass *= newMass / stellarMass`, then set `stellarMass = newMass`. If `stellarMass` is zero, division by zero occurs; handle by setting both to the new mass.
// - `getTotalMass()`: sum of gravitating masses.
// - `getTotalRadius()`: maximum over all particles of `sqrt(x*x + y*y + z*z)`. If no particles, return 0.
// - `getCenterOfMassPosition` and `getCenterOfMassVelocity`: weighted averages by gravitating mass. If no particles, return `false` or set outputs to zero; for robustness, return `true` with zeros if no particles, but better to return `false` if no particles to avoid division by zero. The problem statement does not specify, but for edge cases, return `false` if no particles.
//
// Time complexity is O(1) for individual accessors, O(n) for aggregate functions. Space complexity is O(n) for storage.
//
// Edge cases: empty particle system (accessors return false, aggregate functions return 0 or false), index out of bounds, mass set to zero (handle scaling carefully), and division by zero in center-of-mass when total mass is zero.
