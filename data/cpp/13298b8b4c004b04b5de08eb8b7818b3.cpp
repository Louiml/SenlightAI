Write a standalone C++ function named `countMovesToCollide` that simulates the movement of one or two entities (like 3D printer axes) on a 1D line. The function should take a starting position `start` and a distance `distance_to_move` where `distance_to_move` can be positive (moving toward positive infinity) or negative (moving toward negative infinity). The entity moves step by step: each step increases the number of moves by 1, and the position changes by +1 or -1 depending on the sign of `distance_to_move`. However, simulation must stop if the entity reaches either boundary at `-limit` or `+limit` (inclusive). The function should return the total number of successful steps (moves) taken before stopping, or if the distance is fully traversed before hitting a boundary, return that number (which equals the absolute value of `distance_to_move`). If `distance_to_move` is 0, return 0. Additionally, if the start position is already outside the allowed range (i.e., `abs(start) > limit`), return -1 to indicate invalid input. The function must be `const`-correct and handle edge cases like starting exactly at a boundary (then it cannot move; returns 0), and moving exactly to a boundary counts as reaching it (so the move that lands on the boundary is counted). For this task, you may assume `limit >= 0`, and `start` is an integer, but `distance_to_move` may be any integer including negative values.
The algorithm is straightforward: first validate that `abs(start) <= limit`. If not, return -1. Then, if `distance_to_move == 0`, return 0. Determine the direction: if `distance_to_move > 0`, the entity moves right (position increases by 1 each step); otherwise moves left (position decreases by 1). The maximum number of possible steps in that direction before hitting the boundary is `limit - start` if moving right, or `start - (-limit) = start + limit` if moving left. The total moves is the minimum of the absolute distance to move and that maximum possible steps. This is because if the distance is larger than the available space, we stop at the boundary (and count the boundary-reaching step). If the distance is smaller, we complete the full move. Time complexity is O(1) since we just compute a formula; space is O(1). Edge cases: start at boundary (e.g., start == limit and moving right) yields max steps 0, so if distance positive, returns 0; if distance negative, returns min(abs(distance), start+limit) which is fine. Negative distance to a boundary also works. The formula handles all cases without any loop.
#include <cstdlib> // for std::abs

// Simulate a 1D movement of an entity that moves one unit per step.
// Returns the number of completed steps (moves) before either finishing the
// requested distance or hitting a boundary at -limit or +limit.
// Returns -1 if the start position is outside the allowed range.
// If distance_to_move is 0, returns 0.
// limit must be non-negative.
int countMovesToCollide(int start, int distance_to_move, int limit) {
    // Validate start position is within [-limit, limit]
    if (std::abs(start) > limit) {
        return -1;
    }

    if (distance_to_move == 0) {
        return 0;
    }

    // Determine the maximum number of steps we can take in the desired direction
    // before reaching a boundary (including the boundary step itself).
    int max_steps;
    if (distance_to_move > 0) {
        // Moving right, boundary is at +limit
        max_steps = limit - start;
    } else {
        // Moving left, boundary is at -limit
        max_steps = start - (-limit); // = start + limit
    }

    // The number of moves is the smaller of the requested distance and
    // the available space. Since distance_to_move may be negative, use abs.
    int requested = std::abs(distance_to_move);
    int moves = requested < max_steps ? requested : max_steps;

    return moves;
}
#include <cassert>

int main() {
    // Basic positive movement within bounds
    assert(countMovesToCollide(0, 5, 10) == 5);
    // Basic negative movement within bounds
    assert(countMovesToCollide(0, -5, 10) == 5);
    // Move that hits the positive boundary before completing distance
    assert(countMovesToCollide(8, 5, 10) == 2); // 8->9->10, hits boundary after 2 steps
    // Move that hits the negative boundary before completing distance
    assert(countMovesToCollide(-8, -5, 10) == 2); // -8->-9->-10
    // Starting exactly at boundary moving outward: cannot move
    assert(countMovesToCollide(10, 3, 10) == 0);
    // Starting exactly at negative boundary moving outward: cannot move
    assert(countMovesToCollide(-10, -3, 10) == 0);
    // Starting at boundary moving inward
    assert(countMovesToCollide(10, -3, 10) == 3);
    assert(countMovesToCollide(-10, 3, 10) == 3);
    // Zero distance
    assert(countMovesToCollide(0, 0, 10) == 0);
    // Invalid start outside range
    assert(countMovesToCollide(11, 0, 10) == -1);
    assert(countMovesToCollide(-11, 5, 10) == -1);
    // Distance exactly reaches boundary
    assert(countMovesToCollide(7, 3, 10) == 3); // 7->8->9->10, hits boundary on last step
    // Large distance but limited space
    assert(countMovesToCollide(-5, 100, 5) == 10); // from -5 to +5, 10 steps
    // Negative large distance
    assert(countMovesToCollide(5, -100, 5) == 10);
    // Limit zero: only position 0 allowed
    assert(countMovesToCollide(0, 1, 0) == 0);
    assert(countMovesToCollide(0, -1, 0) == 0);
    assert(countMovesToCollide(0, 0, 0) == 0);
}
