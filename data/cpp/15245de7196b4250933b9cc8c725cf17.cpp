Write a C++ function named `seekerShouldFire` that simulates the firing decision logic for a seeker drone based on its current ammunition count, attack timer state, and a supplied current time value. The function takes four parameters: an integer `ammo` representing the remaining shots, a boolean `attackTimerDone` indicating whether the attack cooldown timer has expired, a positive integer `minDelayMs` and a positive integer `maxDelayMs` representing the inclusive range for the next cooldown delay (where `minDelayMs <= maxDelayMs`), and an integer `currentTimeMs` representing the current simulation time. The function returns a `bool` indicating whether the seeker should fire at this moment. The firing logic follows these rules: The seeker only fires if it has ammo remaining (`ammo > 0`). It only fires when the attack timer is done (`attackTimerDone == true`). When the seeker fires, it consumes one unit of ammo (decrementing the count) and sets a new attack timer delay. The delay is randomly chosen uniformly in the inclusive integer range `[minDelayMs, maxDelayMs]`, but it must be at least `minDelayMs` and at most `maxDelayMs`; for simplicity, outside the function, the timer will be set to `currentTimeMs + delay`. The function should return `true` if a shot is fired this call, `false` otherwise. Additionally, the function should handle edge cases: if `ammo` is `0`, it returns `false` regardless of timer state. If `attackTimerDone` is `false`, it returns `false`. If `minDelayMs` or `maxDelayMs` are non-positive, treat them as `1`. The random value must be reproducible for testing; therefore, use a fixed seed (e.g., `1`) for the random number generator inside the function, so that each call with the same inputs and same global RNG state produces the same output. The function must not modify its parameters (pass by value or `const` references), and it must be pure except for the internal random number generator state. Provide the function with a descriptive name and include necessary headers (`<random>`, `<algorithm>`, `<cstdlib>` if needed). The function does not need a `main`; only provide the free function and any supporting constants/types.
#include <cassert>

int main() {
    // Case: no ammo, timer done -> false.
    assert(seekerShouldFire(0, true, 100, 200, 0) == false);

    // Case: ammo but timer not done -> false.
    assert(seekerShouldFire(5, false, 100, 200, 0) == false);

    // Case: ammo and timer done -> true.
    assert(seekerShouldFire(1, true, 100, 100, 0) == true);

    // Case: ammo and timer done with larger range; should still fire.
    assert(seekerShouldFire(10, true, 250, 2500, 1000) == true);

    // Case: invalid delay bounds (negative) should be clamped to 1.
    // Since timer done and ammo positive, fires.
    assert(seekerShouldFire(2, true, -5, -1, 0) == true);

    // Case: min > max, swapped; fires.
    assert(seekerShouldFire(3, true, 500, 100, 0) == true);

    // Case: ammo exactly 1, timer done, fires.
    assert(seekerShouldFire(1, true, 10, 20, 0) == true);

    // Case: ammo positive but timer done false many times.
    for (int i = 0; i < 10; ++i) {
        assert(seekerShouldFire(5, false, 10, 20, i) == false);
    }

    // Case: ammo zero and timer false.
    assert(seekerShouldFire(0, false, 10, 20, 0) == false);

    // Case: ammo zero and timer true, but minDelay>0.
    assert(seekerShouldFire(0, true, 1, 1, 0) == false);
}
#include <random>
#include <algorithm>

// Determine if a seeker should fire based on ammo, timer state, and delay range.
// Returns true if firing occurs; ammo and timer are conceptually updated externally.
bool seekerShouldFire(int ammo, bool attackTimerDone, int minDelayMs, int maxDelayMs, int currentTimeMs) {
    // Validate delay bounds: ensure positive and min <= max.
    if (minDelayMs < 1) minDelayMs = 1;
    if (maxDelayMs < 1) maxDelayMs = 1;
    if (minDelayMs > maxDelayMs) std::swap(minDelayMs, maxDelayMs);

    // Can't fire without ammo or if timer is not done.
    if (ammo <= 0 || !attackTimerDone) {
        return false;
    }

    // Generate a random delay in the inclusive range.
    // Fixed seed for reproducibility (seed = 1).
    static std::mt19937 rng(1);
    std::uniform_int_distribution<int> dist(minDelayMs, maxDelayMs);
    int delay = dist(rng);
    // In a real system, timer would be set to currentTimeMs + delay.
    // Here we only return the firing decision.
    (void)currentTimeMs; // suppress unused parameter warning.
    (void)delay;         // delay is conceptually used to set the timer.

    // A shot is fired.
    return true;
}
// The core logic is straightforward: a seeker fires only if ammo is positive and the attack timer is done. If both conditions hold, we decrement the ammo (conceptually; since we pass by value, we can't modify the caller's variable, but the function must return a decision and optionally a new ammo count and delay; however, since the task only asks for a bool, we simplify to just the decision; the actual state update is external). The random delay is generated using a `std::mt19937` engine seeded with `1` and a `std::uniform_int_distribution` over the range `[max(1, minDelayMs), max(minDelayMs, maxDelayMs)]`. Edge cases: if `attackTimerDone` is false, immediately return `false`. If `ammo == 0`, return `false`. For delays, clamps ensure positivity and valid range. The function is deterministic given the fixed seed and call order, but note that each call advances the RNG state; if the caller calls multiple times, the sequence is reproducible. Time complexity is O(1) per call, space O(1). The function is pure in terms of inputs (no mutation of arguments) but has internal RNG state, which is acceptable.
