Write a C++ function named `circleIntersectsCircle` that determines whether two circles in a 2D plane intersect (including touching) given their center coordinates and radii. The function should take two circle descriptors (each with `float` center x, center y, and radius) and return `bool`. Use the mathematical condition that two circles intersect if the squared distance between their centers is less than or equal to the square of the sum of their radii. The function must handle edge cases such as identical centers, one circle entirely inside another without touching, and negative radii (which should be treated as invalid, returning `false`). The function should be efficient and avoid floating-point precision issues by using squared distances. Provide a robust implementation with `const` references.
// The solution computes the squared Euclidean distance between the two circle centers: `dx = c1.x - c2.x`, `dy = c1.y - c2.y`, `distanceSquared = dx*dx + dy*dy`. Then, the sum of radii is `rSum = c1.radius + c2.radius`. Intersection occurs if `distanceSquared <= rSum * rSum`. This condition automatically handles all cases: separate circles (distance > sum), touching (distance == sum), overlapping (distance < sum), and one inside another (distance < sum even if one radius is much larger). However, if either radius is negative, we immediately return `false` because negative radii are invalid. Also, if both radii are zero, the circles are points; they intersect only if centers coincide, which the squared distance condition handles. Time complexity is O(1) and space complexity is O(1). Floating-point comparison with `<=` is safe because we use squared values, avoiding square roots and their associated precision errors.
#include <cmath>

// Descriptor for a circle in a 2D plane.
struct Circle {
    float x;
    float y;
    float radius;
};

// Determine if two circles intersect or touch.
// Returns false if either radius is negative (invalid) or if they are disjoint.
bool circleIntersectsCircle(const Circle& c1, const Circle& c2) {
    if (c1.radius < 0.0f || c2.radius < 0.0f) {
        return false;
    }

    float dx = c1.x - c2.x;
    float dy = c1.y - c2.y;
    float distanceSquared = dx * dx + dy * dy;

    float rSum = c1.radius + c2.radius;
    float rSumSquared = rSum * rSum;

    return distanceSquared <= rSumSquared;
}
#include <cassert>

int main() {
    Circle c1, c2;

    // Overlapping circles
    c1 = {0.0f, 0.0f, 3.0f};
    c2 = {4.0f, 0.0f, 2.0f};
    assert(circleIntersectsCircle(c1, c2) == true); // distance 4 <= 5

    // Touching externally
    c1 = {0.0f, 0.0f, 2.0f};
    c2 = {5.0f, 0.0f, 3.0f};
    assert(circleIntersectsCircle(c1, c2) == true); // distance 5 <= 5

    // Disjoint
    c1 = {0.0f, 0.0f, 1.0f};
    c2 = {10.0f, 0.0f, 2.0f};
    assert(circleIntersectsCircle(c1, c2) == false); // distance 10 > 3

    // One inside another (but not touching)
    c1 = {0.0f, 0.0f, 10.0f};
    c2 = {1.0f, 0.0f, 1.0f};
    assert(circleIntersectsCircle(c1, c2) == true); // distance 1 <= 11

    // Identical centers
    c1 = {3.0f, 3.0f, 5.0f};
    c2 = {3.0f, 3.0f, 5.0f};
    assert(circleIntersectsCircle(c1, c2) == true);

    // Zero-radius points coincident
    c1 = {0.0f, 0.0f, 0.0f};
    c2 = {0.0f, 0.0f, 0.0f};
    assert(circleIntersectsCircle(c1, c2) == true);

    // Zero-radius points separate
    c1 = {0.0f, 0.0f, 0.0f};
    c2 = {1.0f, 0.0f, 0.0f};
    assert(circleIntersectsCircle(c1, c2) == false);

    // Negative radius invalid
    c1 = {0.0f, 0.0f, -1.0f};
    c2 = {0.0f, 0.0f, 2.0f};
    assert(circleIntersectsCircle(c1, c2) == false);

    // Negative radius invalid even if overlapping otherwise
    c1 = {0.0f, 0.0f, 5.0f};
    c2 = {1.0f, 1.0f, -3.0f};
    assert(circleIntersectsCircle(c1, c2) == false);

    // Large float values without overflow (squared distance)
    c1 = {100000.0f, 0.0f, 1.0f};
    c2 = {-100000.0f, 0.0f, 1.0f};
    assert(circleIntersectsCircle(c1, c2) == false); // distance 200000 > 2

    return 0;
}
