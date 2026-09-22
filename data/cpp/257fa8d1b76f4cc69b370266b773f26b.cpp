/*
Write a C++ function named `isRobotBounded` that takes a non-empty string `instructions` consisting only of the characters `'G'` (move forward one unit), `'L'` (turn left 90 degrees without moving), and `'R'` (turn right 90 degrees without moving). The robot starts at position `(0,0)` facing north (positive y-direction). The robot executes the given instruction sequence exactly once, but then repeats the entire sequence indefinitely. Return `true` if the robot's path is bounded (i.e., it never moves infinitely far from the origin over an infinite number of repetitions), and `false` otherwise. The function must handle arbitrary-length strings and correctly account for direction changes and periodic behavior.
*/

#include <string>

// Determine whether a robot executing the given instruction string repeatedly stays bounded.
bool isRobotBounded(const std::string& instructions) {
    // Direction vectors: north, east, south, west.
    const int dx[4] = {0, 1, 0, -1};
    const int dy[4] = {1, 0, -1, 0};
    
    int x = 0, y = 0;      // Current position.
    int d = 0;             // Facing direction index (0=north, 1=east, 2=south, 3=west).
    
    for (char command : instructions) {
        if (command == 'L') {
            d = (d + 3) % 4;  // Equivalent to d -= 1 modulo 4.
        } else if (command == 'R') {
            d = (d + 1) % 4;
        } else { // 'G'
            x += dx[d];
            y += dy[d];
        }
    }
    
    // Bounded if back at origin or facing any direction other than north.
    return (x == 0 && y == 0) || (d != 0);
}

#include <cassert>

int main() {
    // Single move: after repeating, moves infinitely north.
    assert(isRobotBounded("G") == false);
    // Turn left four times: returns to original position and orientation.
    assert(isRobotBounded("LLLL") == true);
    // Simple right turn and move: forms a square after four repetitions.
    assert(isRobotBounded("GR") == true);
    // Move two steps, turn left: after four repetitions returns.
    assert(isRobotBounded("GGL") == true);
    // No movement at all: always at origin.
    assert(isRobotBounded("L") == true);
    assert(isRobotBounded("R") == true);
    // Mixed sequence that eventually returns to origin in one pass.
    assert(isRobotBounded("GLGLGLGL") == true);
    // Sequence that drifts away: moves north-east forever.
    assert(isRobotBounded("GRG") == false);
    // Longer string with rotation: ends facing south, bounded.
    assert(isRobotBounded("GGLLRR") == true);
}

// The key insight is that after executing the instruction string exactly once, the robot's displacement and orientation determine its long-term behavior. If the robot returns to `(0,0)` after one full sequence, it is trivially bounded. If after one sequence the robot is not at the origin but its facing direction is still north (the original direction), then each repetition adds the same displacement, causing unbounded drift away from the origin. However, if after one sequence the robot's facing direction is not north (i.e., it is facing east, south, or west), then repeating the sequence will trace a closed path: after four repetitions the robot returns to its starting position and orientation because turns are multiples of 90 degrees and any net rotation that is not 0 modulo 360 will cause a cyclic return. Therefore, the algorithm simulates one pass of the instruction string, tracking `x`, `y`, and direction index `d` in `{0,1,2,3}` corresponding to north, east, south, west. Use direction vectors `dx = {0,1,0,-1}` and `dy = {1,0,-1,0}`. For each character: if `'L'`, decrement `d` (modulo 4); if `'R'`, increment `d` (modulo 4); if `'G'`, update `x += dx[d]`, `y += dy[d]`. After the pass, return `(x == 0 && y == 0) || (d != 0)`. Edge cases include a sequence with no `'G'` (always bounded), and a sequence that rotates but also moves in a repeating triangular or square path. Time complexity is `O(n)` where `n` is the length of the string, space complexity is `O(1)`.
