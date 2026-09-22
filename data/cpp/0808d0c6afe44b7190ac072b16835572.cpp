// Write a standalone C++ function `int countBeeWins(int turns, int beeAttackPower)` that simulates the core combat logic of the Ants-vs-bees game on a simplified 1D board of length 10. The board has a queen at position 0 and is initially empty except for the queen. At the start of each turn, a new bee with 1 health and attack power `beeAttackPower` spawns at position 9 (the far end). Then, all bees on the board move one step toward the queen (position decreases by 1) if position 0 is not already occupied by a bee at the start of the movement phase. If a bee reaches position 0 (i.e., moves into it or is already there), the game ends immediately with a bee win. The function should simulate exactly `turns` turns (spawning and moving a bee each turn) and return the total number of bee wins that occur during the simulation. Note: After a bee win, the game would normally stop, but for this simulation, treat it as a complete restart: after a win, the board resets to empty (queen only), and subsequent turns continue from the beginning, each with a fresh empty board. The bees never attack ants, and ants are not placed; only bees move. The function should count how many times a bee reaches position 0 over the entire sequence of `turns` turns.

The approach is to maintain a board state as a small array (or vector) of bee counts per position, but since only one bee spawns per turn and they all move one step left per turn (if the queen isn't already being attacked), we can model this with a queue or simple array. The key insight: On each turn, we spawn a bee at index 9. Then we check if the queen position (index 0) already has a bee after spawning; if so, that's a win. Otherwise, we move all bees one step left. However, careful: The rule says "if position 0 is not already occupied by a bee at the start of the movement phase" — meaning we check before moving. So the algorithm:
- Initialize an array `beeCounts[10]` all zeros.
- For each turn from 1 to `turns`:
  - Spawn a bee at index 9: `beeCounts[9]++`.
  - If `beeCounts[0] > 0`, then a bee is already at the queen (this could happen from previous turns? Actually, after each turn if we move bees left, a bee could reach index 0 and we should count a win and reset. So at the start of the turn, before spawning, if `beeCounts[0] > 0`, that would only happen if last turn ended with a win but we reset, so it's always 0 after reset. So we can simply check after spawning if `beeCounts[0] > 0`? But the condition says "at the start of the movement phase" — that means after spawning, before moving. So we check if position 0 is occupied before moving. If so, we count a win, and reset board to zeros, and continue to next turn. If not, we move all bees left: for i from 0 to 8: `beeCounts[i] = beeCounts[i+1]`, then set `beeCounts[9] = 0`. But note: moving left means each bee at index i+1 moves to i. So we iterate from left to right. After moving, if a bee lands on index 0? That won't happen because if index 1 had a bee, it moves to index 0, but we already checked before moving that index 0 was empty. However, it could be that index 1 has bees and index 0 is empty, so after moving a bee from 1 to 0, that is also a win? The problem statement says "If a bee reaches position 0 (i.e., moves into it or is already there), the game ends immediately with a bee win." That suggests we should also check after moving, not just before. But the later rule says "the movement phase" has a condition: bees only move if position 0 is not already occupied at the start of the movement. But if a bee moves into position 0 during the movement, that's a win as well. So we must check both before moving (if already occupied after spawn) and after moving (if any bee moved into 0). To simplify: After spawning, if `beeCounts[0] > 0` (should be 0 unless from previous turn, but we reset on win, so always 0), but logically, if a bee is already at queen, it's a win. Then move all bees left. After moving, if `beeCounts[0] > 0`, that means a bee reached the queen, count a win and reset. Otherwise, continue. Also note: if position 0 is occupied at start of movement, we abort movement and count a win. But since we reset after a win, that can't happen unless there are multiple bees at index 0 from a previous turn's leftover? Actually, if a bee is at index 0, we would have already counted a win and reset, so it shouldn't persist. The problem says "After a bee win, the game would normally stop, but for this simulation, treat it as a complete restart: after a win, the board resets to empty (queen only), and subsequent turns continue from the beginning." So yes, after counting a win, we zero the board. Therefore, we can implement: for each turn:
- Spawn at index 9.
- If queen position (index 0) has a bee (should be 0, but just in case), count win, reset, continue.
- Else, move all bees left one step (from index 1 to 0, etc.). This is a shift.
- After shift, if index 0 has a bee, count win and reset board to zeros.

This will correctly simulate. Time complexity: O(turns * 10) which is O(turns). Space: O(1) with fixed array size 10. Edge cases: `turns` could be 0, return 0. `beeAttackPower` is unused in this simulation because we ignore ant attacks, but we include it as a parameter to match the original design; it has no effect. The function must be standalone with only the described logic.

#include <array>
#include <cstddef>

int countBeeWins(int turns, int /*beeAttackPower*/) {
    std::array<int, 10> board{};
    int wins = 0;

    for (int t = 0; t < turns; ++t) {
        // Spawn a new bee at the far end (position 9)
        board[9]++;

        // If a bee is already at the queen, that's a win (should not normally happen)
        if (board[0] > 0) {
            ++wins;
            board.fill(0);
            continue;
        }

        // Move all bees one cell left (toward the queen)
        for (int i = 0; i < 9; ++i) {
            board[i] = board[i + 1];
        }
        board[9] = 0;

        // Check if any bee reached the queen after moving
        if (board[0] > 0) {
            ++wins;
            board.fill(0);
        }
    }

    return wins;
}

#include <cassert>

int main() {
    // No turns -> no wins
    assert(countBeeWins(0, 1) == 0);

    // One turn: spawn at 9, move to 8, no queen -> 0 wins
    assert(countBeeWins(1, 1) == 0);

    // Two turns: Turn1 spawn@9 move to8, Turn2 spawn (two bees at 8 and9?), 
    // after spawn, move all left: bee from 8 to7, from9 to8, still no queen
    assert(countBeeWins(2, 1) == 0);

    // Ten turns: after 9 moves, first bee reaches position1, after 10th spawn it's at position0? Let's simulate:
    // Turn 1: spawn at9, move to8
    // ... after 9 turns, a bee is at position1. Turn 10: spawn at9, move all left: bee at1 ->0, bee at9->8. 
    // So win occurs on turn 10. After win, reset.
    assert(countBeeWins(10, 1) == 1);

    // Now for 20 turns, second run also wins at turn 10, total 2
    assert(countBeeWins(20, 1) == 2);

    // 35 turns -> wins at 10,20,30, then after reset, turn 31-35 no win yet, total 3
    assert(countBeeWins(35, 1) == 3);

    // The beeAttackPower parameter doesn't affect the result
    assert(countBeeWins(10, 100) == 1);
    assert(countBeeWins(20, 0) == 2);

    // Smaller board? Not applicable; fixed size 10.

    // Ensure repeated wins work correctly: 
    // After first win at turn 10, board resets, second win at turn 20, etc.
    assert(countBeeWins(100, 5) == 10); // wins at 10,20,...,100

    return 0;
}
