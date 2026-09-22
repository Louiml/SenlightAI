// Write a C++ function `canBeMadeFromLegs` that takes two integers `X` and `Y`, where `X` is the total number of animals and `Y` is the total number of legs among those animals. Each animal is either a crane (2 legs) or a turtle (4 legs). The function must return `true` if there exists a non-negative integer combination of cranes and turtles that exactly matches both `X` and `Y`, and `false` otherwise. The function should handle all non-negative integer inputs (including zero for both). For example, with `X=3` and `Y=10`, it is possible (1 crane, 2 turtles), but with `X=3` and `Y=9`, it is impossible.

// The problem reduces to solving a system of two linear equations. Let `c` be the number of cranes and `t` the number of turtles. Then:
// - `c + t = X`
// - `2c + 4t = Y`
//
// From the first equation, `c = X - t`. Substitute into the second: `2(X - t) + 4t = Y` → `2X + 2t = Y` → `t = (Y - 2X) / 2`. For `t` to be an integer, `Y - 2X` must be even. Also `t` must satisfy `0 <= t <= X` because `c = X - t` must be non-negative. Similarly, we can derive an equivalent condition using `c = (4X - Y)/2` from solving directly; the code snippet uses `a = 4X - Y`, so `c = a/2`. Check `a >= 0`, `a` is even, and `a/2 <= X`. This ensures both `c` and `t` are non-negative integers. Edge cases: when `X=0`, only `Y=0` is possible; when `Y` is odd or less than `2X` (since minimum legs is `2X`), impossible. Time complexity is O(1) and space complexity is O(1).

#include <cstdint>

// Returns true if exactly X animals with total Y legs can be formed from cranes (2 legs) and turtles (4 legs).
bool canBeMadeFromLegs(int X, int Y) {
    // Let c = number of cranes, t = number of turtles.
    // c + t = X
    // 2c + 4t = Y  →  c = (4X - Y) / 2
    // Condition: (4X - Y) is even and non-negative, and c <= X.
    const int difference = 4 * X - Y;
    if (difference < 0) {
        return false;
    }
    if (difference % 2 != 0) {
        return false;
    }
    const int c = difference / 2;
    // Since c = X - t, need c >= 0 (already true) and t >= 0 → c <= X.
    return c <= X;
}

#include <cassert>

// Declaration from solution (placed above in actual compilation).
bool canBeMadeFromLegs(int, int);

int main() {
    // Basic known cases
    assert(canBeMadeFromLegs(3, 10) == true);   // 1 crane, 2 turtles
    assert(canBeMadeFromLegs(3, 9) == false);   // impossible
    assert(canBeMadeFromLegs(1, 2) == true);    // 1 crane
    assert(canBeMadeFromLegs(1, 4) == true);    // 1 turtle
    assert(canBeMadeFromLegs(0, 0) == true);    // no animals, no legs
    assert(canBeMadeFromLegs(0, 2) == false);   // no animals but legs exist
    assert(canBeMadeFromLegs(2, 6) == true);    // 1 crane, 1 turtle
    assert(canBeMadeFromLegs(2, 8) == true);    // 2 turtles
    assert(canBeMadeFromLegs(2, 5) == false);   // odd legs impossible
    assert(canBeMadeFromLegs(100, 400) == true);// all turtles
    assert(canBeMadeFromLegs(100, 200) == true);// all cranes
    assert(canBeMadeFromLegs(100, 201) == false);// odd
    assert(canBeMadeFromLegs(100, 199) == false);// less than minimum legs (2*X=200)
    return 0;
}
