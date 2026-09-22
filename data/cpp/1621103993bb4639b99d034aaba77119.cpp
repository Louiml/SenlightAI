/*
Write a C++ function that takes a non-negative integer radius (passed by pointer as in the original snippet, but converted to a cleaner interface) and returns the area of a circle, using the exact same logic as the given code: if the radius is divisible by 7, compute the area using the fraction `22/7` (with integer arithmetic for the numerator and denominator), otherwise use `3.14` with floating-point multiplication. The function must accept a pointer to the radius, modify the pointed value if needed (to match the original's out-parameter style), and return the computed area as a `float`. The function should be named `circleAreaFromPointer` and must handle the edge case of radius 0 (area 0). For clarity, avoid using `conio.h` or `getch()` in your solution; only include necessary standard headers.
*/

#include <cassert>

// Compute the area of a circle given a pointer to its radius (non-negative).
// Uses the rule: if radius % 7 == 0, area = 22 * r^2 / 7 (integer division), else area = 3.14 * r^2.
float circleAreaFromPointer(const int* radiusPtr) {
    assert(radiusPtr != nullptr);
    int r = *radiusPtr;
    if (r % 7 == 0) {
        return static_cast<float>(22 * r * r / 7);
    } else {
        return 3.14f * r * r;
    }
}

int main() {
    int r1 = 7;
    assert(circleAreaFromPointer(&r1) == 154.0f); // 22*49/7=154
    int r2 = 3;
    assert(circleAreaFromPointer(&r2) == 28.26f); // 3.14*9=28.26
    int r3 = 0;
    assert(circleAreaFromPointer(&r3) == 0.0f);
    int r4 = 14;
    assert(circleAreaFromPointer(&r4) == 616.0f); // 22*196/7=616
    int r5 = 1;
    assert(circleAreaFromPointer(&r5) == 3.14f);
    int r6 = 21;
    assert(circleAreaFromPointer(&r6) == 1386.0f); // 22*441/7=1386
    int r7 = 2;
    assert(circleAreaFromPointer(&r7) == 12.56f);
    return 0;
}

// The main algorithm is straightforward: dereference the pointer to obtain the radius, then check if the radius modulo 7 equals 0. If yes, compute the area as `22 * (radius * radius) / 7` using integer arithmetic, but store the result in a `float` to allow possible fractional parts (though for multiples of 7 the division yields an exact integer when radius² is a multiple of 7, but using integer division could truncate if radius² is not divisible by 7; however, the original code uses integer division, so we must preserve that behavior precisely). If not divisible by 7, compute `3.14 * radius * radius` with floating-point multiplication. Edge cases: radius = 0 gives result 0 (both branches yield 0). Very large radii may overflow `int` during `radius * radius`; but since the original used `int`, we maintain the same behavior. The pointer should be non-null; assume valid input. Time complexity is O(1), space complexity is O(1). The function directly returns the computed area, and the pointer parameter serves as an input (not mutated), but for compatibility with the original pattern, we still pass by pointer.
