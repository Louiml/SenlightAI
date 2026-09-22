Write a C++ function `canReachDestination(ll distance, ll speed, ll time)` that takes three `long long` integers: the total distance `D` to travel, the average speed `S` (distance per unit time), and the available time `T`. The function must return a `bool` indicating whether the journey is possible, i.e., whether the maximum distance that can be covered (`S * T`) is at least `D`. Use straightforward arithmetic, handle large values within the 64-bit range, and ensure no overflow occurs. The function should be reusable in an independent program without a `main` function.
The problem is a simple feasibility check: the total distance covered in time `T` at speed `S` is the product `S * T`. If this product is greater than or equal to `D`, the journey is possible; otherwise, it is not. The only edge case to consider is overflow when multiplying two large `long long` values (up to ~1e15 each, which would exceed 64-bit range). Since the problem constraints are not given, a safe approach is to avoid direct multiplication for large numbers by using division: check if `S >= D / T` (integer division) when `T > 0`, but this can be problematic because `D / T` truncates. A more robust method is to check if `S > D / T` OR (`S == D / T` AND `D % T == 0`), ensuring the comparison is exact without overflow. Alternatively, since `D, S, T` are all non-negative and within typical 64-bit constraints (as per the original snippet using `ll` up to ~1e15), direct multiplication is safe if we assume each is ≤ 1e9, but to be general, the division-based check is safer. Time complexity is O(1), space complexity is O(1).
#include <cstdint>

using ll = long long;

// Check if distance D can be covered in time T at speed S without overflow.
bool canReachDestination(ll distance, ll speed, ll time) {
    // If time is zero, we can only cover zero distance.
    if (time == 0) {
        return distance == 0;
    }
    // Avoid overflow by using division: speed * time >= distance
    // is equivalent to speed >= ceil(distance / time).
    ll full_covers = distance / time;  // floor division
    if (speed > full_covers) {
        return true;
    }
    if (speed == full_covers && distance % time == 0) {
        return true;
    }
    return false;
}
#include <cassert>

int main() {
    // Must match the original snippet's behavior: S*T >= D
    assert(canReachDestination(100, 20, 5) == true);   // 20*5=100 >= 100
    assert(canReachDestination(101, 20, 5) == false);  // 20*5=100 < 101
    assert(canReachDestination(0, 0, 0) == true);      // 0*0=0 >= 0
    assert(canReachDestination(5, 0, 10) == false);    // speed zero, distance>0
    assert(canReachDestination(5, 1, 5) == true);      // 1*5=5 >= 5
    assert(canReachDestination(6, 1, 5) == false);     // 1*5=5 < 6
    assert(canReachDestination(1000000000000LL, 1000000LL, 1000000LL) == true); // product fits
    assert(canReachDestination(1000000000001LL, 1000000LL, 1000000LL) == false);
    assert(canReachDestination(10, 3, 4) == true);     // 3*4=12 >= 10
    assert(canReachDestination(13, 3, 4) == false);    // 3*4=12 < 13
    return 0;
}
