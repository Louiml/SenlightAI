Write a C++ function that takes four integers `a`, `b`, `c`, and `d` and returns a boolean value indicating whether it is possible to travel from point `a` to point `c` directly or indirectly. Direct travel is possible if the absolute difference between `a` and `c` is at most `d`. Indirect travel is possible if you can first travel from `a` to `b` (absolute difference ≤ `d`) and then from `b` to `c` (absolute difference ≤ `d`). The function should return `true` if either direct or indirect travel is possible, and `false` otherwise. All inputs are integers, and `d` is a non-negative integer representing the maximum allowed distance per jump.

#include <cassert>

int main() {
    // Direct reach works.
    assert(canReach(0, 10, 5, 10) == true);
    // Indirect via b works when direct fails.
    assert(canReach(0, 5, 10, 5) == true);
    // No path possible.
    assert(canReach(0, 3, 10, 5) == false);
    // d = 0: only exact equality works.
    assert(canReach(7, 7, 7, 0) == true);
    assert(canReach(7, 7, 8, 0) == false);
    // a == c always direct reachable, regardless of d.
    assert(canReach(-5, 100, -5, 0) == true);
    // Both direct and indirect are possible; returns true.
    assert(canReach(-3, 0, 3, 6) == true);
    // Indirect fails because first leg too long, direct also fails.
    assert(canReach(-3, 10, 3, 5) == false);
    // Edge case: b is exactly at distance d from both a and c.
    assert(canReach(0, 3, 6, 3) == true);
}

#include <cstdlib>

// Determine if point 'c' is reachable from point 'a' either directly or
// indirectly via point 'b', where each jump distance must be at most 'd'.
bool canReach(int a, int b, int c, int d) {
    if (std::abs(a - c) <= d) {
        return true;
    }
    return (std::abs(a - b) <= d) && (std::abs(b - c) <= d);
}

// The problem reduces to checking a simple reachability condition on a line. We need to determine whether point `c` is reachable from point `a` using at most two moves, where each move has a maximum allowed distance `d`. Two cases exist:  
// 1. Direct reach: `abs(a - c) <= d`.  
// 2. Indirect via `b`: require `abs(a - b) <= d` AND `abs(b - c) <= d`.  
// If either condition holds, return `true`; otherwise `false`. This is a straightforward constant-time decision. Edge cases include when `a == c` (always direct, since `abs` is 0 ≤ `d` for any non-negative `d`), when `d == 0` (only direct travel is possible if `a == c`, indirect only if `a == b == c`), and when `b` is not actually useful (e.g., `b` is outside both ranges). The time complexity is \(O(1)\) and space complexity is \(O(1)\).
