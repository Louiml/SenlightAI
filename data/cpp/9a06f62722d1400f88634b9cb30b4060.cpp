Write a C++ function named `computeSpringTargetAcceleration` that models the behavior of a stiff spring connecting two particles. Given the current position and velocity of a particle, the position of the other endpoint of the spring (the anchor), the spring constant `k`, the damping coefficient `c`, the particle’s inverse mass, and a time step `deltaTime`, the function must return a 3D vector representing the acceleration to apply to the particle. The acceleration is derived from the target position formula for a damped harmonic oscillator:  
`target = (diff * cos(gamma*dt) + (diff*(c/(2*gamma)) + velocity*(1/gamma)) * sin(gamma*dt)) * exp(-0.5*dt*c)`  
where `diff = position - anchorPos`, and `gamma = sqrt(4*k - c*c) / 2`. The function should handle the case where `gamma` is zero (or imaginary, i.e., `4*k - c*c < 0`) by returning a zero vector, since the system is critically/overdamped and not valid for this formula. The returned acceleration should be `(target - position - velocity*dt) / (inverseMass * deltaTime)` — note that in the original snippet the denominator is mistakenly squared, but for a physically correct task we use the standard form. Use `std::array<double,3>` or a simple struct for the 3D vector. Do not use any external libraries; implement simple vector arithmetic manually.
The solution involves computing the difference vector between the particle and the anchor, then using the given damped harmonic oscillator formula to find the target position after `deltaTime`. Key steps: (1) compute `diff`; (2) check the discriminant `4*k - c*c`; if negative or exactly zero, return `{0,0,0}` because gamma would be imaginary or zero making the formula invalid. (3) Compute gamma as `sqrt(4*k - c*c) * 0.5`; (4) compute the `c` vector (the coefficient for the sine term): `diff * (damping/(2*gamma)) + velocity * (1/gamma)`; (5) compute the target position using trigonometric and exponential functions; (6) compute acceleration as `(target - position - velocity*deltaTime) / (inverseMass * deltaTime)`. Edge cases: ensure `inverseMass` and `deltaTime` are nonzero (if inverseMass is zero, the particle is fixed, so return zero; if deltaTime is zero, return zero to avoid division by zero). Time complexity is O(1) with constant space. All vector operations are element-wise.
#include <cmath>
#include <array>
#include <cstddef>

using Vec3 = std::array<double, 3>;

// Compute the acceleration for a stiff spring system.
// Returns zero vector if invalid parameters (gamma <= 0, deltaTime==0, or inverseMass==0).
Vec3 computeSpringTargetAcceleration(
    const Vec3& position,
    const Vec3& velocity,
    const Vec3& anchorPos,
    double springConstant,
    double damping,
    double inverseMass,
    double deltaTime)
{
    // Handle degenerate cases.
    if (deltaTime <= 0.0 || inverseMass <= 0.0) {
        return {0.0, 0.0, 0.0};
    }
    
    double discriminant = 4.0 * springConstant - damping * damping;
    if (discriminant <= 0.0) {
        return {0.0, 0.0, 0.0};
    }
    
    double gamma = std::sqrt(discriminant) * 0.5;
    
    // diff = position - anchorPos
    Vec3 diff = {
        position[0] - anchorPos[0],
        position[1] - anchorPos[1],
        position[2] - anchorPos[2]
    };
    
    // c vector for sine term: diff * (damping/(2*gamma)) + velocity * (1/gamma)
    double coeff1 = damping / (2.0 * gamma);
    double coeff2 = 1.0 / gamma;
    Vec3 cVec = {
        diff[0] * coeff1 + velocity[0] * coeff2,
        diff[1] * coeff1 + velocity[1] * coeff2,
        diff[2] * coeff1 + velocity[2] * coeff2
    };
    
    double angle = gamma * deltaTime;
    double cosA = std::cos(angle);
    double sinA = std::sin(angle);
    double decay = std::exp(-0.5 * deltaTime * damping);
    
    // target = (diff*cosA + cVec*sinA) * decay
    Vec3 target = {
        (diff[0] * cosA + cVec[0] * sinA) * decay,
        (diff[1] * cosA + cVec[1] * sinA) * decay,
        (diff[2] * cosA + cVec[2] * sinA) * decay
    };
    
    // acceleration = (target - position - velocity*dt) / (inverseMass * dt)
    double denom = inverseMass * deltaTime;
    Vec3 accel = {
        (target[0] - position[0] - velocity[0] * deltaTime) / denom,
        (target[1] - position[1] - velocity[1] * deltaTime) / denom,
        (target[2] - position[2] - velocity[2] * deltaTime) / denom
    };
    
    return accel;
}
#include <cassert>
#include <cmath>
#include <array>

using Vec3 = std::array<double, 3>;

// The function definition from the solution goes here (for completeness, omitted in this block).

bool approxEqual(const Vec3& a, const Vec3& b, double tol = 1e-6) {
    for (size_t i = 0; i < 3; ++i) {
        if (std::fabs(a[i] - b[i]) > tol) return false;
    }
    return true;
}

int main() {
    // Test 1: Simple case with known values.
    Vec3 pos = {0.0, 0.0, 0.0};
    Vec3 vel = {0.0, 0.0, 0.0};
    Vec3 anchor = {1.0, 0.0, 0.0};
    double k = 10.0, c = 1.0, invMass = 2.0, dt = 0.1;
    Vec3 acc = computeSpringTargetAcceleration(pos, vel, anchor, k, c, invMass, dt);
    // Since initial diff=1, gamma ~= 3.122, target ~= exp(-0.05)*cos(0.3122) ~= 0.902, so accel ~= (0.902-0)/0.2 = 4.51
    assert(std::fabs(acc[0] - 4.51) < 0.1);
    assert(acc[1] == 0.0 && acc[2] == 0.0);

    // Test 2: Zero deltaTime returns zero.
    acc = computeSpringTargetAcceleration(pos, vel, anchor, k, c, invMass, 0.0);
    assert(acc == Vec3{0.0, 0.0, 0.0});

    // Test 3: Zero inverseMass returns zero.
    acc = computeSpringTargetAcceleration(pos, vel, anchor, k, c, 0.0, dt);
    assert(acc == Vec3{0.0, 0.0, 0.0});

    // Test 4: Overdamped (critical) returns zero because discriminant <= 0.
    acc = computeSpringTargetAcceleration(pos, vel, anchor, 1.0, 2.0, 1.0, dt); // 4*1-4=0
    assert(acc == Vec3{0.0, 0.0, 0.0});

    // Test 5: Negative spring constant gives imaginary gamma -> zero.
    acc = computeSpringTargetAcceleration(pos, vel, anchor, -1.0, 1.0, 1.0, dt);
    assert(acc == Vec3{0.0, 0.0, 0.0});

    // Test 6: Symmetry: flipping anchor and position should produce negative acceleration for symmetric setup.
    Vec3 acc2 = computeSpringTargetAcceleration(anchor, vel, pos, k, c, invMass, dt);
    // Because diff flips sign, acceleration should approximately negate (velocity zero, other params same).
    assert(std::fabs(acc2[0] + acc[0]) < 0.1);
    assert(std::fabs(acc2[1]) < 1e-10 && std::fabs(acc2[2]) < 1e-10);

    // Test 7: All zero diff and velocity yields zero acceleration.
    acc = computeSpringTargetAcceleration(pos, pos, pos, k, c, invMass, dt);
    assert(approxEqual(acc, {0.0, 0.0, 0.0}));

    // Test 8: Large damping but still underdamped gives reasonable finite result.
    acc = computeSpringTargetAcceleration(pos, vel, anchor, 100.0, 5.0, 1.0, 0.01);
    // gamma = sqrt(400-25)/2 = sqrt(375)/2 ~= 9.682, coefficients finite.
    assert(std::isfinite(acc[0]) && std::isfinite(acc[1]) && std::isfinite(acc[2]));

    // Test 9: Nonzero velocity affects acceleration.
    Vec3 vel2 = {2.0, -1.0, 0.5};
    Vec3 acc_v = computeSpringTargetAcceleration(pos, vel2, anchor, k, c, invMass, dt);
    assert(!approxEqual(acc_v, acc)); // Should differ from zero-velocity case.

    // Test 10: Check that very large dt still produces finite (though possibly large) acceleration.
    acc = computeSpringTargetAcceleration(pos, vel, anchor, k, c, invMass, 100.0);
    assert(std::isfinite(acc[0]) && std::isfinite(acc[1]) && std::isfinite(acc[2]));

    return 0;
}
