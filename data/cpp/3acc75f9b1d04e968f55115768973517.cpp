Write a C++ function `canReachOnTime` that takes three integers: `distance` (in meters), `time` (in seconds), and `speed` (in meters per second). The function should return `true` if traveling at the given constant speed for the given time covers at least the required distance, otherwise `false`. The problem is inspired by a simple yes/no decision: if the product of time and speed is greater than or equal to the distance, the answer is yes. All inputs are non-negative integers and fit within the range of a 32-bit signed integer. The function must avoid integer overflow when computing the product of time and speed.
The straightforward check is whether `time * speed >= distance`. However, since `time` and `speed` can each be as large as about 2×10⁹, their product can overflow a 32-bit signed integer. To avoid overflow, we can compare without directly multiplying: we can check if `time` is non-zero and `distance / time <= speed` (but careful with integer division and zero), or better, use a 64-bit type for the multiplication. Since the problem guarantees non-negative inputs, the simplest robust solution is to cast to `long long` (or `int64_t`) before multiplying. The main algorithm is O(1) time and O(1) auxiliary space. Edge cases: distance=0 always true; time=0 or speed=0: if distance is 0, true; otherwise false (unless time*speed>=distance as integer). The cast solution handles all cases without overflow.
#include <cstdint>

// Return true if traveling at the given speed for the given time covers at least the required distance.
// Uses 64-bit arithmetic to avoid overflow in the product of time and speed.
bool canReachOnTime(int distance, int time, int speed) {
    // Cast to int64_t to safely compute the product without overflow.
    // All inputs are non-negative, so no sign issues.
    return static_cast<int64_t>(time) * speed >= distance;
}
#include <cassert>

int main() {
    // Basic positive case: 10 m at 2 m/s for 5 seconds = 10, yes.
    assert(canReachOnTime(10, 5, 2) == true);
    // Barely not enough: need 11, only 10.
    assert(canReachOnTime(11, 5, 2) == false);
    // Zero distance always reachable.
    assert(canReachOnTime(0, 0, 0) == true);
    // Time zero with positive distance -> false.
    assert(canReachOnTime(5, 0, 10) == false);
    // Speed zero with positive distance -> false.
    assert(canReachOnTime(5, 10, 0) == false);
    // Large values that would overflow 32-bit multiplication: 2e9 * 2e9 = 4e18.
    assert(canReachOnTime(2000000000, 2000000000, 2000000000) == true); // product is huge
    assert(canReachOnTime(4000000000LL, 2000000000, 2) == true); // 4e9 <= 4e9
    assert(canReachOnTime(4000000001LL, 2000000000, 2) == false); // 4e9+1 > 4e9
    // Edge where product equals distance exactly.
    assert(canReachOnTime(100, 10, 10) == true);
}
