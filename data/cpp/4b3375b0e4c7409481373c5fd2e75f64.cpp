// Write a C++ function `int shortestClockwiseDistance(int a, int b)` that takes two integers representing positions on a clock face numbered 0 through 99 (like a 100-minute clock), and returns the shortest distance along the clock face between them. The distance can be measured in either direction (clockwise or counterclockwise), and you must choose the minimum possible absolute difference, considering that if you go past 99 you wrap around to 0. For example, the distance between 95 and 5 is 10 (not 90), because 95 -> 96 -> ... -> 99 -> 0 -> ... -> 5 is 10 steps. The function should handle any integers, though in practice they will be within 0–99.

The problem reduces to computing the circular distance on a cycle of length 100. First, compute the absolute difference `d = abs(a - b)`. The direct distance is `d`. However, because the clock wraps around, the alternative distance is `100 - d` (going the other way around the circle). The answer is the minimum of these two values: `min(d, 100 - d)`. This works for all inputs, including when `a == b` (then `d = 0`, and `min(0, 100) = 0`), and when the two numbers are opposite (e.g., 0 and 50, then `d = 50`, `min(50, 50) = 50`). No special handling for negative numbers is needed because `abs` gives a non‑negative result, and `100 - d` is always ≥ 0 for `d ≤ 100` (which holds since `|a-b|` ≤ 99 if inputs are within 0–99, but even if not, the formula still works as long as `100 - d` is non‑negative, which it is for `d ≤ 100`). Time complexity is O(1), space O(1).

#include <algorithm>
#include <cstdlib>

// Returns the shortest distance on a 100-position clock face between a and b.
int shortestClockwiseDistance(int a, int b) {
    int direct = std::abs(a - b);
    return std::min(direct, 100 - direct);
}

#include <cassert>

int main() {
    // Basic cases
    assert(shortestClockwiseDistance(10, 20) == 10);
    assert(shortestClockwiseDistance(20, 10) == 10);
    // Wrapping case
    assert(shortestClockwiseDistance(95, 5) == 10);
    assert(shortestClockwiseDistance(5, 95) == 10);
    // Same point
    assert(shortestClockwiseDistance(42, 42) == 0);
    // Opposite points
    assert(shortestClockwiseDistance(0, 50) == 50);
    assert(shortestClockwiseDistance(99, 49) == 50);
    // Edge at boundaries
    assert(shortestClockwiseDistance(0, 99) == 1);
    assert(shortestClockwiseDistance(99, 0) == 1);
    // Values outside 0-99 (still works if within 100 range)
    assert(shortestClockwiseDistance(150, 160) == 10);
}
