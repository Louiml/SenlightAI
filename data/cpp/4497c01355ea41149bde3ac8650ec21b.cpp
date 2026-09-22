// Write a C++ function `pickRandomDestination` that simulates the selection of a valid random destination for a creature roaming within a circular area around its home position. The function takes as parameters the creature's home coordinates `(homeX, homeY, homeZ)`, a maximum wander distance `wanderDistance`, a boolean `canFly` indicating whether the creature can move in 3D, a height accessor callback `getHeight(x, y, z)` that returns the terrain height at the given horizontal position (or `homeZ` if no valid height is found), and a random number generator callback `randUnit()` returning a uniformly distributed double in `[0,1)`. The function must return a `std::tuple<float, float, float>` containing the chosen destination `(destX, destY, destZ)`, or `std::nullopt` (using `std::optional`) if no valid destination could be found after a fixed number of attempts (e.g., 10 attempts). For 2D movement (when `canFly` is false), the destination Z must be the terrain height at that X/Y, and the horizontal distance from home must not exceed `wanderDistance`. The vertical difference from homeZ must not exceed the horizontal distance traveled (i.e., `abs(destZ - homeZ) <= horizontalDistance`). For 3D movement (when `canFly` is true), the destination Z is the homeZ plus a random vertical offset limited to half the horizontal distance, and the destination Z must be strictly above the terrain height plus a small clearance (e.g., 2.0f) at that point. If a computed candidate fails the checks, retry with new random values, but stop after 10 attempts and return `std::nullopt`. The function should be pure and not depend on any global state, using only the provided callbacks. The function signature is: `std::optional<std::tuple<float,float,float>> pickRandomDestination(float homeX, float homeY, float homeZ, float wanderDistance, bool canFly, const std::function<float(float,float,float)>& getHeight, const std::function<double()>& randUnit);`
// The algorithm generates random polar coordinates: a random angle uniformly in `[0, 2π)` and a random radius uniformly in `[0, wanderDistance]`. From these, compute the horizontal offset in X and Y. The destination X and Y are simply homeX + distanceX and homeY + distanceY. For 2D mode: compute `horizontalDistSq = distanceX² + distanceY²`, and `horizontalDist = sqrt(horizontalDistSq)`. The candidate Z is obtained by calling `getHeight(destX, destY, homeZ - 2.0f)` (a two-step fallback is not needed for simplicity, but the callback is expected to return a reasonable terrain height). The valid condition is `abs(destZ - homeZ) <= horizontalDist`. If horizontalDist is 0, then the creature stays at home. For 3D mode: compute a random vertical offset `distanceZ = randUnit() * horizontalDist / 2.0f`, and candidate `destZ = homeZ + distanceZ`. Then get the terrain height via `getHeight(destX, destY, destZ - 2.0f)`, and require `terrainHeight + 2.0f < destZ` (strictly above with clearance). If any condition fails, retry with a new random angle and radius. After 10 attempts, return `std::nullopt`. The main edge cases: when wanderDistance is zero or negative, we should immediately return `std::nullopt` because no movement is possible; when horizontalDist is zero, in 2D mode the vertical difference must be zero (since `abs(destZ - homeZ) <= 0` forces equality), so we need to check the height accessor returns homeZ exactly, otherwise invalid. In 3D mode, if horizontalDist is zero, distanceZ is zero, so destZ = homeZ, and we still need `terrainHeight + 2.0f < homeZ`, which may fail, and we retry. The time complexity is O(1) per attempt with up to 10 attempts, so O(1) worst-case. Space complexity is O(1). The `std::optional` and `std::tuple` add negligible overhead. The pure function uses only callbacks and no global state.
#include <optional>
#include <tuple>
#include <cmath>
#include <functional>

// Simulate random movement destination selection for a creature.
// Returns nullopt if no valid destination found after 10 attempts.
std::optional<std::tuple<float, float, float>> pickRandomDestination(
    float homeX, float homeY, float homeZ,
    float wanderDistance, bool canFly,
    const std::function<float(float, float, float)>& getHeight,
    const std::function<double()>& randUnit) {
    
    if (wanderDistance <= 0.0f) {
        return std::nullopt;
    }
    
    const float pi = std::acos(-1.0f);
    
    for (int attempt = 0; attempt < 10; ++attempt) {
        const float angle = static_cast<float>(randUnit()) * 2.0f * pi;
        const float range = static_cast<float>(randUnit()) * wanderDistance;
        const float distanceX = range * std::cos(angle);
        const float distanceY = range * std::sin(angle);
        
        const float destX = homeX + distanceX;
        const float destY = homeY + distanceY;
        const float horizontalDistSq = distanceX * distanceX + distanceY * distanceY;
        const float horizontalDist = std::sqrt(horizontalDistSq);
        
        float destZ;
        if (canFly) {
            const float distanceZ = static_cast<float>(randUnit()) * horizontalDist / 2.0f;
            destZ = homeZ + distanceZ;
            const float terrainHeight = getHeight(destX, destY, destZ - 2.0f);
            if (terrainHeight + 2.0f < destZ) {
                return std::make_tuple(destX, destY, destZ);
            }
        } else {
            // 2D movement: get terrain height at destination with a small probe offset.
            destZ = getHeight(destX, destY, homeZ - 2.0f);
            if (horizontalDist == 0.0f) {
                // Must stay exactly at homeZ.
                if (std::abs(destZ - homeZ) <= 1e-5f) {
                    return std::make_tuple(destX, destY, destZ);
                }
            } else if (std::abs(destZ - homeZ) <= horizontalDist) {
                return std::make_tuple(destX, destY, destZ);
            }
        }
    }
    
    return std::nullopt;
}
#include <cassert>
#include <cmath>
#include <optional>
#include <tuple>
#include <functional>

// Declare the solution function (or include the header with it).
std::optional<std::tuple<float, float, float>> pickRandomDestination(
    float homeX, float homeY, float homeZ,
    float wanderDistance, bool canFly,
    const std::function<float(float, float, float)>& getHeight,
    const std::function<double()>& randUnit);

int main() {
    // Flat terrain with constant height at z=10.0f.
    auto flatHeight = [](float, float, float) -> float { return 10.0f; };
    
    // Test 1: 2D movement on flat terrain, home at z=10, wander distance 5.
    // Since all terrain is flat, destZ should always be 10, but horizontal distance <=5.
    // Use a deterministic random generator that gives 0.5 for both angle and range -> angle=pi, range=2.5.
    double counter = 0.0;
    auto deterministicRand = [&counter]() -> double {
        counter += 0.5;
        return counter; // returns 0.5, 1.0, 1.5, ... but we need [0,1); clamp.
        // To avoid out-of-range, just return 0.5 always for simplicity.
    };
    // Simpler: always return 0.5.
    auto alwaysHalf = []() -> double { return 0.5; };
    
    auto res1 = pickRandomDestination(0.0f, 0.0f, 10.0f, 5.0f, false, flatHeight, alwaysHalf);
    assert(res1.has_value());
    auto [x1, y1, z1] = *res1;
    // angle=pi => cos(pi)=-1, sin(pi)=0. range=2.5 => distanceX=-2.5, distanceY=0 => destX=-2.5, destY=0.
    assert(std::abs(x1 - (-2.5f)) < 1e-3f);
    assert(std::abs(y1 - 0.0f) < 1e-3f);
    assert(std::abs(z1 - 10.0f) < 1e-3f);
    
    // Test 2: 2D movement, terrain returns far away height (e.g., 100) when queried at x,y.
    // This should fail all attempts because abs(destZ - homeZ) = 90 > horizontalDist <=5.
    auto badHeight = [](float, float, float) -> float { return 100.0f; };
    auto res2 = pickRandomDestination(0.0f, 0.0f, 10.0f, 5.0f, false, badHeight, alwaysHalf);
    assert(!res2.has_value());
    
    // Test 3: 3D movement with flat terrain at z=10, home z=10, wander distance 5.
    // randUnit returns 0.5 for angle, range, and distanceZ => angle=pi, range=2.5, distanceZ = 0.5*2.5/2=0.625.
    // So destZ = 10.625, which is above terrain (10+2=12? No, 10.625 > 12 is false), so this attempt fails.
    // After 10 attempts with same random, fails, returns nullopt. But we need to test a successful 3D case.
    // Let's craft a custom random generator that yields values ensuring success.
    // For 3D successful: terrain at some low height, e.g., mountain valley. Home z=50, terrain everywhere = 0.
    auto lowTerrain = [](float, float, float) -> float { return 0.0f; };
    auto alwaysQuarter = []() -> double { return 0.25; }; // angle=0.25*2pi=pi/2, range=0.25*5=1.25, distanceZ=0.25*1.25/2=0.15625
    auto res3 = pickRandomDestination(0.0f, 0.0f, 50.0f, 5.0f, true, lowTerrain, alwaysQuarter);
    assert(res3.has_value());
    auto [x3, y3, z3] = *res3;
    // angle=pi/2 => cos=0, sin=1 => destX=0, destY=1.25. destZ = 50 + 0.15625 = 50.15625.
    assert(std::abs(x3 - 0.0f) < 1e-3f);
    assert(std::abs(y3 - 1.25f) < 1e-3f);
    assert(std::abs(z3 - 50.15625f) < 1e-3f);
    
    // Test 4: 3D movement with terrain too high relative to home, should fail.
    auto highTerrain = [](float, float, float) -> float { return 60.0f; };
    auto res4 = pickRandomDestination(0.0f, 0.0f, 50.0f, 5.0f, true, highTerrain, alwaysQuarter);
    // destZ starts at 50+0.15625=50.15625, terrain+2=62, not above, fails all attempts.
    assert(!res4.has_value());
    
    // Test 5: Zero wander distance returns nullopt.
    auto res5 = pickRandomDestination(0.0f, 0.0f, 0.0f, 0.0f, false, flatHeight, alwaysHalf);
    assert(!res5.has_value());
    
    // Test 6: 2D with horizontalDist zero and terrain height matches homeZ exactly.
    // Use a random that returns 0 for both angle and range => angle=0, range=0 => destX=homeX, destY=homeY.
    auto alwaysZero = []() -> double { return 0.0; };
    auto res6 = pickRandomDestination(3.0f, 4.0f, 7.0f, 2.0f, false, flatHeight, alwaysZero);
    assert(res6.has_value());
    auto [x6, y6, z6] = *res6;
    assert(std::abs(x6 - 3.0f) < 1e-3f);
    assert(std::abs(y6 - 4.0f) < 1e-3f);
    assert(std::abs(z6 - 7.0f) < 1e-3f);
    
    // Test 7: 2D with horizontalDist zero but terrain does not match homeZ => should retry and eventually fail (all attempts same).
    auto wrongHeight = [](float, float, float) -> float { return 8.0f; };
    auto res7 = pickRandomDestination(3.0f, 4.0f, 7.0f, 2.0f, false, wrongHeight, alwaysZero);
    assert(!res7.has_value());
    
    // Test 8: 3D with horizontalDist zero and terrain below homeZ, allowed.
    // destZ = homeZ because distanceZ = 0 * something /2 = 0. Terrain+2 < homeZ => success.
    auto res8 = pickRandomDestination(1.0f, 1.0f, 10.0f, 2.0f, true, lowTerrain, alwaysZero);
    assert(res8.has_value());
    auto [x8, y8, z8] = *res8;
    assert(std::abs(x8 - 1.0f) < 1e-3f);
    assert(std::abs(y8 - 1.0f) < 1e-3f);
    assert(std::abs(z8 - 10.0f) < 1e-3f);
    
    // Test 9: 3D with horizontalDist zero and terrain exactly at homeZ+2 (borderline) => strict <, fails.
    auto borderTerrain = [](float, float, float) -> float { return 8.0f; };
    auto res9 = pickRandomDestination(1.0f, 1.0f, 10.0f, 2.0f, true, borderTerrain, alwaysZero);
    // require terrain+2 < destZ => 10 < 10 false, so fails.
    assert(!res9.has_value());
    
    return 0;
}
