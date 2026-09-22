/*
Write a C++ function that simulates the elastic collision between two circular balls in a 2D plane. Given two balls represented by their center positions (as 2D vectors of doubles), radii, masses, and velocity vectors (as 2D vectors of doubles), the function must: (1) if the balls overlap (distance between centers < sum of radii), first separate them so they just touch without overlap by moving each ball along the line connecting their centers proportionally to their radii; (2) if the balls are coincident (zero distance), separate them by moving one ball up by its radius and the other down by its radius; (3) after separation, compute and apply the impulse-based velocity updates for a perfectly elastic collision (coefficient of restitution = 1) using the standard 1D elastic collision formulas along the line of impact, preserving total momentum and kinetic energy. The function should modify the positions and velocities of both balls in place. Handle the edge case where after separation the balls still have zero distance (e.g., both balls have zero radius) by doing no velocity change. Assume positive masses (mass = radius^2 or a provided value, but for simplicity, use the formula mass = radius * radius). No gravity or other forces are considered; only collision response is handled.
*/

#include <cmath>
#include <cstddef>
#include <vector>

// A simple 2D vector type with double components.
struct Vec2 {
    double x;
    double y;

    Vec2(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator*(double scalar) const { return Vec2(x * scalar, y * scalar); }
    Vec2 operator/(double scalar) const { return Vec2(x / scalar, y / scalar); }

    double dot(const Vec2& other) const { return x * other.x + y * other.y; }
    double length() const { return std::sqrt(x * x + y * y); }
    Vec2 unit() const {
        double len = length();
        if (len == 0.0) return Vec2(0.0, 0.0);
        return *this / len;
    }
};

// Represents a ball with position, velocity, and radius.
struct Ball {
    Vec2 pos;    // center position
    Vec2 vel;    // velocity
    double radius;

    Ball(const Vec2& p, const Vec2& v, double r) : pos(p), vel(v), radius(r) {}

    double mass() const { return radius * radius; }
};

// Simulate an elastic collision between two balls, separating them if they overlap.
void elasticCollision(Ball& ballA, Ball& ballB) {
    Vec2 AB = ballB.pos - ballA.pos;
    double dist = AB.length();

    // Coincident centers: separate vertically and stop (no velocity change).
    if (dist == 0.0) {
        ballA.pos.y -= ballA.radius;
        ballB.pos.y += ballB.radius;
        return;
    }

    double sumRadii = ballA.radius + ballB.radius;
    if (dist < sumRadii) {
        // Separate balls so they just touch.
        double overlap = sumRadii - dist;
        Vec2 unitAB = AB.unit();
        Vec2 middle = ballA.pos + unitAB * (ballA.radius - overlap / 2.0);

        Vec2 dirA = ballA.pos - middle;
        Vec2 dirB = ballB.pos - middle;

        if (dirA.length() == 0.0 || dirB.length() == 0.0) {
            // Fallback if a radius is zero (cannot separate along AB).
            ballA.pos.y -= ballA.radius;
            ballB.pos.y += ballB.radius;
            return;
        }

        ballA.pos = middle + dirA.unit() * ballA.radius;
        ballB.pos = middle + dirB.unit() * ballB.radius;
    }

    // Compute velocity components along the line of impact.
    Vec2 dirAB = (ballB.pos - ballA.pos).unit(); // from A to B
    Vec2 dirBA = dirAB * -1.0;                  // from B to A

    double vA = ballA.vel.dot(dirAB); // A's component toward B
    double vB = ballB.vel.dot(dirBA); // B's component toward A

    double mA = ballA.mass();
    double mB = ballB.mass();

    // Elastic 1D collision formulas.
    double vA_after = ((mA - mB) * vA + 2.0 * mB * vB) / (mA + mB);
    double vB_after = ((mB - mA) * vB + 2.0 * mA * vA) / (mA + mB);

    // Apply changes as vectors.
    ballA.vel += dirAB * (vA_after - vA);
    ballB.vel += dirBA * (vB_after - vB);
}

#include <cassert>
#include <cmath>

int main() {
    // Helper to compare vectors with a small tolerance.
    auto close = [](const Vec2& a, const Vec2& b, double eps = 1e-9) {
        return std::fabs(a.x - b.x) < eps && std::fabs(a.y - b.y) < eps;
    };

    // Test 1: Two equal masses, head-on collision, velocities exchange and reverse.
    {
        Ball a({0.0, 0.0}, {1.0, 0.0}, 1.0);
        Ball b({3.0, 0.0}, {-1.0, 0.0}, 1.0);
        elasticCollision(a, b);
        assert(close(a.pos, {1.0, 0.0}));  // separated to just touching
        assert(close(b.pos, {2.0, 0.0}));
        assert(close(a.vel, {-1.0, 0.0}));
        assert(close(b.vel, {1.0, 0.0}));
    }

    // Test 2: One ball is stationary, other hits it (equal masses).
    {
        Ball a({0.0, 0.0}, {2.0, 0.0}, 1.0);
        Ball b({3.0, 0.0}, {0.0, 0.0}, 1.0);
        elasticCollision(a, b);
        assert(close(a.pos, {1.0, 0.0}));
        assert(close(b.pos, {2.0, 0.0}));
        assert(close(a.vel, {0.0, 0.0}));
        assert(close(b.vel, {2.0, 0.0}));
    }

    // Test 3: Different masses, stationary target (heavier target).
    {
        Ball a({0.0, 0.0}, {3.0, 0.0}, 1.0);  // mass = 1
        Ball b({3.0, 0.0}, {0.0, 0.0}, 2.0);  // mass = 4
        elasticCollision(a, b);
        // After separation: a at x=1, b at x=2.
        assert(close(a.pos, {1.0, 0.0}));
        assert(close(b.pos, {2.0, 0.0}));
        // vA' = ((1-4)*3 + 2*4*0)/(5) = -9/5 = -1.8
        // vB' = ((4-1)*0 + 2*1*3)/(5) = 6/5 = 1.2
        assert(close(a.vel, {-1.8, 0.0}));
        assert(close(b.vel, {1.2, 0.0}));
    }

    // Test 4: Overlapping balls are separated correctly.
    {
        Ball a({0.0, 0.0}, {0.0, 0.0}, 1.0);
        Ball b({1.5, 0.0}, {0.0, 0.0}, 1.0); // overlap: dist=1.5 < 2
        elasticCollision(a, b);
        assert(close(a.pos, {0.5, 0.0}));  // radius 1 away from middle at x=1
        assert(close(b.pos, {2.0, 0.0}));
        assert(close(a.vel, {0.0, 0.0}));
        assert(close(b.vel, {0.0, 0.0}));
    }

    // Test 5: Coincident centers (zero distance) separate vertically.
    {
        Ball a({0.0, 0.0}, {5.0, 0.0}, 1.0);
        Ball b({0.0, 0.0}, {0.0, 5.0}, 2.0);
        elasticCollision(a, b);
        assert(close(a.pos, {0.0, -1.0}));
        assert(close(b.pos, {0.0, 2.0}));
        // No velocity change due to coincident centers.
        assert(close(a.vel, {5.0, 0.0}));
        assert(close(b.vel, {0.0, 5.0}));
    }

    // Test 6: No overlap (distance >= sum of radii) – nothing changes.
    {
        Ball a({0.0, 0.0}, {1.0, 2.0}, 0.5);
        Ball b({5.0, 0.0}, {-3.0, 1.0}, 0.5);
        Vec2 posABefore = a.pos;
        Vec2 velABefore = a.vel;
        Vec2 posBBefore = b.pos;
        Vec2 velBBefore = b.vel;
        elasticCollision(a, b);
        assert(close(a.pos, posABefore) && close(a.vel, velABefore));
        assert(close(b.pos, posBBefore) && close(b.vel, velBBefore));
    }

    // Test 7: Zero-radius balls with coincident centers – no infinite loop.
    {
        Ball a({2.0, 3.0}, {1.0, 1.0}, 0.0);
        Ball b({2.0, 3.0}, {-1.0, -1.0}, 0.0);
        elasticCollision(a, b);
        assert(close(a.pos, {2.0, 3.0}));
        assert(close(b.pos, {2.0, 3.0}));
        assert(close(a.vel, {1.0, 1.0}));
        assert(close(b.vel, {-1.0, -1.0}));
    }

    return 0;
}

// The solution begins by computing the vector from ball A to ball B (`AB = B.pos - A.pos`). If the length of `AB` is zero (coincident centers), we separate them by moving A up by its radius and B down by its radius, then return early with no velocity change to avoid division by zero. Otherwise, we check if the balls overlap: `distance < A.radius + B.radius`. If they do, we compute the amount of overlap `tmp = (A.radius + B.radius) - distance`. The midpoint between the two centers when just touching is `middle = A.pos + (A.radius - tmp/2) * unit(AB)`. Then we move each ball so that its center is exactly its radius away from the midpoint along the direction from the midpoint to the original center. If after this separation either ball still has zero distance to the midpoint (which would happen if a radius is zero), we fall back to the vertical separation and return. After separation, we compute the component of each ball's velocity along the line of impact: `forceA = projection of A.vel onto direction from A to B`, and `forceB = projection of B.vel onto direction from B to A` (which is the negative of the direction from A to B). For an elastic collision, the velocity components along the impact line are exchanged according to the 1D formulas: `vA' = ((mA - mB)*vA + 2*mB*vB) / (mA+mB)` and `vB' = ((mB - mA)*vB + 2*mA*vA) / (mA+mB)`, where `vA` and `vB` are the scalar components along the line (positive toward the other ball). We then add the change (`vA' - vA`) to each ball's velocity as a vector in the direction of the impact line. The algorithm runs in O(1) time and O(1) space. Edge cases include zero-radius balls, coincident centers, and very small overlaps due to floating-point precision, which are handled by the separation step and early return for zero distance.
