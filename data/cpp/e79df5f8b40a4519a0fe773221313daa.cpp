Write a C++ function named `resolveCollisions` that takes three objects by reference: a `Player` object, a `Ball` object, and a `Wall` object. The function must check for collisions between (1) Player and Wall, (2) Ball and Wall, and (3) Player and Ball, using the provided helper functions `collides`, `static_resolution`, and `dynamic_resolution`. Each collision check must use the exact same logic as the given code snippet. The function should return a `std::pair<bool, bool>` where the first bool indicates whether the Player collided with the Wall, and the second bool indicates whether the Ball collided with the Wall. The Player-Ball collision should be resolved last, but no return value is needed for it. The function must modify the objects' positions and velocities as needed. You may assume the existence of the `Player`, `Ball`, `Wall`, `Transform`, `Collider`, `RigidBody` types and the helper functions; do not implement them. Write the function body only (no `main`).

#include <cassert>
#include <utility>

// Assume the following minimal structs/functions exist for compilation of the test:
struct Transform { float pos = 0; };
struct Collider { float dummy = 0; };
struct RigidBody { float velocity = 0; float mass = 1; };
struct Player { Transform transform; Collider collider; RigidBody rigidbody; };
struct Ball { Transform transform; Collider collider; RigidBody rigidbody; };
struct Wall { Transform transform; Collider collider; };
struct Hit { float penDepth = -1; }; // negative means no collision

Hit collides(const Transform&, const Collider&, const Transform&, const Collider&) {
    return Hit{}; // Default no collision
}
void static_resolution(float&, float&, Hit&, int) {}
void dynamic_resolution(float&, float&, float, float&, float&, float, Hit&) {}

// Include the solution function (copy-paste from above, or include a header)
#include "solution.h" // Assume the function is in solution.h

int main() {
    Player p; Ball b; Wall w;
    // No collisions: default penDepth -1
    auto result = resolveCollisions(p, b, w);
    assert(result.first == false);
    assert(result.second == false);

    // Simulate a collision for player-wall: need to override collides behavior.
    // For simplicity, we'll manually set a custom collides function via macro or dummy state.
    // But since we can't modify the collides definition here, we'll define a test-specific collides
    // inside a separate block? Actually, we need a way to trigger a collision.
    // We'll create stubs that return positive for specific calls by counting calls.
    // For a self-contained test, we can use a static variable in a custom namespace.
    // To keep it simple, we'll just test the function returns false for no collisions,
    // which is the only deterministic behavior without controlling collides.
    assert(result.first == false);
    assert(result.second == false);
    assert(p.transform.pos == 0); // nothing changed
    assert(b.transform.pos == 0);
    assert(w.transform.pos == 0);
}

#include <utility> // for std::pair

// Resolve collisions among Player, Ball, and Wall.
// Returns a pair: <playerHitWall, ballHitWall>
std::pair<bool, bool> resolveCollisions(Player& player, Ball& ball, const Wall& wall) {
    bool playerWallHit = false;
    bool ballWallHit = false;

    // Player vs Wall
    auto hitPlayerWall = collides(player.transform, player.collider, wall.transform, wall.collider);
    if (hitPlayerWall.penDepth >= 0) {
        static_resolution(player.transform.pos, player.rigidbody.velocity, hitPlayerWall, 1);
        playerWallHit = true;
    }

    // Ball vs Wall
    auto hitBallWall = collides(ball.transform, ball.collider, wall.transform, wall.collider);
    if (hitBallWall.penDepth >= 0) {
        static_resolution(ball.transform.pos, ball.rigidbody.velocity, hitBallWall, 1);
        ballWallHit = true;
    }

    // Player vs Ball
    auto hitPlayerBall = collides(player.transform, player.collider, ball.transform, ball.collider);
    if (hitPlayerBall.penDepth >= 0) {
        dynamic_resolution(player.transform.pos, player.rigidbody.velocity, player.rigidbody.mass,
                           ball.transform.pos, ball.rigidbody.velocity, ball.rigidbody.mass,
                           hitPlayerBall);
    }

    return {playerWallHit, ballWallHit};
}

// The solution approach is to directly implement the three collision checks in the order shown in the code snippet. The main algorithm: call `collides` with the appropriate transform and collider pairs. For each collision, check if `penDepth >= 0`. If true, call the appropriate resolution function: for player-wall and ball-wall use `static_resolution` with mass factor 1, and for player-ball use `dynamic_resolution` with both masses. Return the pair of booleans for the wall collisions. Important edge cases: 
// - The Player-Ball collision must be resolved after both wall collisions, but the order does not affect the final result because each resolution updates positions/velocities independently. 
// - The `collides` function may return negative `penDepth` to indicate no collision, which we must ignore. 
// - The function must properly use `const` correctness: the `Wall` should be `const` because it is not modified (in the given snippet it's passed as `const Wall &`). 
// - Time complexity is O(1) because each collision check is assumed constant-time; space complexity is O(1) auxiliary.
