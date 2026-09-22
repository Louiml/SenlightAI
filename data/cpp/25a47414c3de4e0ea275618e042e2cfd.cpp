// Implement a C++ function that, given two 3D positions (`pointOnOne`, `pointOnTwo`), a collision normal, a penetration depth, and references to two rigid bodies (each with a position, velocity, and inverse mass), updates the manifold by adding a contact point with deduplication logic. The function must reject a contact point if another stored contact point on body one is within a squared distance of 0.04 (i.e., Euclidean distance < 0.2), and in that case replace the existing point if the new one has greater penetration, otherwise discard the new point. Store the contact point's relative positions (point position minus body position), the collision normal, penetration, and zero initial impulse accumulators. The manifold must maintain a capacity of at most 4 contact points, and the function should return a `bool` indicating whether the contact point was successfully added or replaced an old one. The solution must be a self-contained free function that operates on a simplified `Manifold` struct with an array of `ContactPoint` structs and a count, not relying on the original physics engine.
#include <cassert>

int main() {
    // Setup bodies
    RigidBody3D bodyA(Vector3(0,0,0), 1.0f);
    RigidBody3D bodyB(Vector3(10,0,0), 0.5f);
    Manifold manifold(&bodyA, &bodyB);

    // Test 1: Add first contact point - should succeed
    bool result = addContactPoint(manifold, Vector3(1,0,0), Vector3(11,0,0), Vector3(0,1,0), 0.5f);
    assert(result == true);
    assert(manifold.contactCount == 1);
    assert(manifold.contactPoints[0].relativePosOne.x == 1.0f);
    assert(manifold.contactPoints[0].relativePosTwo.x == 1.0f);
    assert(manifold.contactPoints[0].collisionPenetration == 0.5f);

    // Test 2: Add a point far away - should succeed
    result = addContactPoint(manifold, Vector3(2,0,0), Vector3(12,0,0), Vector3(0,1,0), 0.3f);
    assert(result == true);
    assert(manifold.contactCount == 2);

    // Test 3: Add a point very close to first (distance 0.1) with lower penetration - rejected
    result = addContactPoint(manifold, Vector3(1.1f,0,0), Vector3(11.1f,0,0), Vector3(0,1,0), 0.2f);
    assert(result == false);
    assert(manifold.contactCount == 2);

    // Test 4: Add a point very close to first with higher penetration - replaces old one
    result = addContactPoint(manifold, Vector3(1.05f,0,0), Vector3(11.05f,0,0), Vector3(0,1,0), 0.7f);
    assert(result == true);
    assert(manifold.contactCount == 2);
    // Find the point with penetration 0.7
    bool foundHighPenetration = false;
    for (int i = 0; i < manifold.contactCount; ++i) {
        if (manifold.contactPoints[i].collisionPenetration == 0.7f) {
            foundHighPenetration = true;
            assert(manifold.contactPoints[i].relativePosOne.x == 1.05f);
        }
    }
    assert(foundHighPenetration);

    // Test 5: Add 4 total points - max capacity
    result = addContactPoint(manifold, Vector3(3,0,0), Vector3(13,0,0), Vector3(0,1,0), 0.1f);
    assert(result == true);
    result = addContactPoint(manifold, Vector3(4,0,0), Vector3(14,0,0), Vector3(0,1,0), 0.2f);
    assert(result == true);
    assert(manifold.contactCount == 4);

    // Test 6: Exceed capacity with a new distant point - should fail
    result = addContactPoint(manifold, Vector3(5,0,0), Vector3(15,0,0), Vector3(0,1,0), 0.3f);
    assert(result == false);
    assert(manifold.contactCount == 4);

    // Test 7: Replace a point when at capacity - should succeed if it's close to an existing point with higher penetration
    // Current points have penetrations: 0.7 (at 1.05), 0.3 (at 2), 0.1 (at 3), 0.2 (at 4)
    // Add a new point near 2 with penetration 0.5 -> should replace the 0.3 one
    result = addContactPoint(manifold, Vector3(2.1f,0,0), Vector3(12.1f,0,0), Vector3(0,1,0), 0.5f);
    assert(result == true);
    assert(manifold.contactCount == 4);
    bool foundNewPoint = false;
    for (int i = 0; i < manifold.contactCount; ++i) {
        if (manifold.contactPoints[i].collisionPenetration == 0.5f) {
            foundNewPoint = true;
            assert(manifold.contactPoints[i].relativePosOne.x == 2.1f);
        }
    }
    assert(foundNewPoint);
    // Ensure no point with penetration 0.3 remains
    for (int i = 0; i < manifold.contactCount; ++i) {
        assert(manifold.contactPoints[i].collisionPenetration != 0.3f);
    }

    // Test 8: Normal and zero penetration handling
    RigidBody3D bodyC(Vector3(0,0,0), 0.0f); // static
    RigidBody3D bodyD(Vector3(5,0,0), 0.0f);
    Manifold manifold2(&bodyC, &bodyD);
    result = addContactPoint(manifold2, Vector3(0,1,0), Vector3(5,1,0), Vector3(0,0,1), 0.0f);
    assert(result == true);
    assert(manifold2.contactCount == 1);
    assert(manifold2.contactPoints[0].collisionNormal.z == 1.0f);
    assert(manifold2.contactPoints[0].collisionPenetration == 0.0f);
    assert(manifold2.contactPoints[0].totalImpulseFromContact == 0.0f);
    assert(manifold2.contactPoints[0].totalImpulseFromFriction == 0.0f);

    // Test 9: Distance exactly 0.2 should not be deduplicated
    RigidBody3D bodyE(Vector3(0,0,0), 1.0f);
    RigidBody3D bodyF(Vector3(5,0,0), 1.0f);
    Manifold manifold3(&bodyE, &bodyF);
    result = addContactPoint(manifold3, Vector3(0,0,0), Vector3(5,0,0), Vector3(0,1,0), 0.1f);
    assert(result == true);
    result = addContactPoint(manifold3, Vector3(0.2f,0,0), Vector3(5.2f,0,0), Vector3(0,1,0), 0.2f);
    assert(result == true);
    assert(manifold3.contactCount == 2);
    // Distance 0.199 should deduplicate
    Manifold manifold4(&bodyE, &bodyF);
    result = addContactPoint(manifold4, Vector3(0,0,0), Vector3(5,0,0), Vector3(0,1,0), 0.1f);
    result = addContactPoint(manifold4, Vector3(0.199f,0,0), Vector3(5.199f,0,0), Vector3(0,1,0), 0.2f);
    assert(result == true); // new higher penetration replaces
    assert(manifold4.contactCount == 1);
    assert(manifold4.contactPoints[0].collisionPenetration == 0.2f);

    // Test 10: Ensure non-zero impulse fields are set to zero for new points
    RigidBody3D bodyG(Vector3(1,2,3), 2.0f);
    RigidBody3D bodyH(Vector3(-1,-2,-3), 1.0f);
    Manifold manifold5(&bodyG, &bodyH);
    result = addContactPoint(manifold5, Vector3(2,2,3), Vector3(0,-2,-3), Vector3(1,0,0), 0.8f);
    assert(result == true);
    assert(manifold5.contactPoints[0].relativePosOne.x == 1.0f);
    assert(manifold5.contactPoints[0].relativePosOne.y == 0.0f);
    assert(manifold5.contactPoints[0].relativePosOne.z == 0.0f);
    assert(manifold5.contactPoints[0].relativePosTwo.x == 1.0f);
    assert(manifold5.contactPoints[0].relativePosTwo.y == 0.0f);
    assert(manifold5.contactPoints[0].relativePosTwo.z == 0.0f);
    assert(manifold5.contactPoints[0].totalImpulseFromContact == 0.0f);

    return 0;
}
#include <array>
#include <cmath>
#include <algorithm>

struct Vector3 {
    float x, y, z;
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
};

struct RigidBody3D {
    Vector3 position;
    Vector3 velocity;
    float invMass;
    RigidBody3D(const Vector3& pos = Vector3(), float inv = 1.0f)
        : position(pos), velocity(), invMass(inv) {}
};

struct ContactPoint {
    Vector3 relativePosOne;
    Vector3 relativePosTwo;
    Vector3 collisionNormal;
    float collisionPenetration;
    float totalImpulseFromContact;
    float totalImpulseFromFriction;
};

struct Manifold {
    std::array<ContactPoint, 4> contactPoints;
    int contactCount;
    RigidBody3D* bodyOne;
    RigidBody3D* bodyTwo;
    Manifold(RigidBody3D* b1 = nullptr, RigidBody3D* b2 = nullptr)
        : contactCount(0), bodyOne(b1), bodyTwo(b2) {}
};

// Adds a contact point to the manifold with deduplication based on proximity.
// Returns true if the point was added (or replaced an existing one), false otherwise.
bool addContactPoint(Manifold& manifold,
                     const Vector3& pointOnOne,
                     const Vector3& pointOnTwo,
                     const Vector3& contactNormal,
                     float penetration) {
    Vector3 relativeOne = pointOnOne - manifold.bodyOne->position;
    Vector3 relativeTwo = pointOnTwo - manifold.bodyTwo->position;

    ContactPoint newPoint;
    newPoint.relativePosOne = relativeOne;
    newPoint.relativePosTwo = relativeTwo;
    newPoint.collisionNormal = contactNormal;
    newPoint.collisionPenetration = penetration;
    newPoint.totalImpulseFromContact = 0.0f;
    newPoint.totalImpulseFromFriction = 0.0f;

    const float minAllowedDistanceSquared = 0.04f;
    bool shouldAdd = true;

    for (int index = 0; index < manifold.contactCount; ++index) {
        Vector3 diff = manifold.contactPoints[index].relativePosOne - newPoint.relativePosOne;
        float distanceSquared = diff.dot(diff);
        if (distanceSquared < minAllowedDistanceSquared) {
            if (manifold.contactPoints[index].collisionPenetration > newPoint.collisionPenetration) {
                // Replace the existing weaker point with the last point, then remove last.
                std::swap(manifold.contactPoints[index], manifold.contactPoints[manifold.contactCount - 1]);
                --manifold.contactCount;
                --index; // Re-check the swapped-in point
            } else {
                shouldAdd = false;
            }
        }
    }

    if (shouldAdd && manifold.contactCount < 4) {
        manifold.contactPoints[manifold.contactCount] = newPoint;
        ++manifold.contactCount;
        return true;
    }
    return false;
}
// The core algorithm: compute the relative positions of the contact points with respect to each body's position. Iterate through all existing contact points in the manifold (up to the current count). For each, compute the squared distance between the candidate's relative position on body one and the existing point's relative position on body one. If this squared distance is less than 0.04 (meaning points are nearly identical), then compare penetration depths:
// - If the existing point has greater penetration than the candidate, swap the existing point with the last point in the array, decrement the count, and continue checking (this removes the weaker point and allows the candidate to be added later).
// - Otherwise (candidate has equal or greater penetration), set a flag `shouldAdd = false` to indicate the candidate should not be added (since the existing point is better or equal).
// After the loop, if `shouldAdd` is true and the count is less than 4, add the candidate to the array at index `count`, increment count, and return `true`. If the count is already 4, the function should not add (but this scenario is unlikely given the replacement logic; still handle gracefully). The function must also handle the edge case where both bodies have zero inverse mass (static objects) – in this case, still record the contact point but the physics solver would skip it later; for this task, just add normally. Time complexity is O(n) where n is the current contact count (max 4), so effectively O(1). Space complexity is O(1) beyond the input arrays.
