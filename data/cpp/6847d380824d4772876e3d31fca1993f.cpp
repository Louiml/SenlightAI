Write a C++ function named `reachesTarget` that takes two parameters: an integer `steps` (the number of moves) and a string `moves` (containing only the characters 'L', 'R', 'U', 'D' representing left, right, up, down movements by 1 unit each). The function should simulate a robot starting at position (0,0). After processing all moves in order, the robot returns `true` if it ever reaches the point (1,1) at any moment during the traversal (including intermediate positions after each individual move), and `false` otherwise. The input string length is guaranteed to equal `steps`. The function must handle cases where the robot visits (1,1) multiple times, never visits it, or visits it only at the very end. The function should be efficient and not assume any particular pattern in the moves.

The solution simulates the robot's path step by step. Maintain two integer variables `x` and `y` representing the current coordinate, starting at (0,0). For each character in the string, update `x` and `y` according to the move: 'L' decreases `x` by 1, 'R' increases `x` by 1, 'U' increases `y` by 1, and 'D' decreases `y` by 1. After each update, check if `x == 1 && y == 1`. If yes, return `true` immediately, because we only need to know if the point is ever reached. After processing all characters, if no check succeeded, return `false`. Edge cases include: an empty string (return `false`), moves that overshoot and come back, and the point being reached exactly on the last move. The algorithm runs in O(n) time where n is the number of moves, using O(1) extra space for the two coordinates. No additional data structures are needed.

#include <string>

// Simulates a robot starting at (0,0) following the given moves.
// Returns true if the robot ever reaches the point (1,1).
bool reachesTarget(int steps, const std::string& moves) {
    int x = 0;
    int y = 0;
    
    for (int i = 0; i < steps && i < static_cast<int>(moves.size()); ++i) {
        switch (moves[i]) {
            case 'L': --x; break;
            case 'R': ++x; break;
            case 'U': ++y; break;
            case 'D': --y; break;
            default: break; // ignore unexpected characters, though input is guaranteed valid
        }
        if (x == 1 && y == 1) {
            return true;
        }
    }
    return false;
}

#include <cassert>
#include <string>

// Declaration from the solution
bool reachesTarget(int steps, const std::string& moves);

int main() {
    // Basic moves reaching (1,1) in the middle
    assert(reachesTarget(4, "RULD") == true);
    // Moves that reach (1,1) at the very end
    assert(reachesTarget(2, "UR") == true);
    // Moves that never reach (1,1)
    assert(reachesTarget(3, "LLU") == false);
    // Empty string
    assert(reachesTarget(0, "") == false);
    // Starting point is not (1,1), and no moves
    assert(reachesTarget(1, "D") == false);
    // Reaching (1,1) multiple times
    assert(reachesTarget(6, "RULDRU") == true);
    // Moves that go past and return
    assert(reachesTarget(4, "RRLL") == false);
    // A single move that reaches (1,1)
    assert(reachesTarget(1, "R") == false); // (1,0) not (1,1)
    assert(reachesTarget(2, "RU") == true);
    // Long path that eventually reaches target
    assert(reachesTarget(8, "DDDDDDUR") == true);
    return 0;
}
