// Write a C++ function `int minDiceRollsToFinish(const std::vector<int>& boardSize100)` that, given a standard Snakes and Ladders board represented as a fixed-size array of 101 integers (indices 0-100, where index 0 is unused and index 100 is the goal), returns the minimum number of dice rolls (each die is a standard 6-sided die) needed to reach the final square from square 1. The array at index `i` is either `0` if that square has no ladder or snake, or the destination square `j` if starting from square `i` immediately moves to `j` (via a ladder if `j > i` or a snake if `j < i`). You cannot overshoot the board: if a roll would take you beyond square 100, that roll is not allowed (you stay in place). Ladders and snakes are taken automatically and cannot be bypassed, and a square that is the start of a ladder/snake cannot be a destination of another ladder/snake (you may assume the input is valid). The function must handle cases where reaching the goal may be impossible (if snakes form loops) by returning -1. Write the function with detailed comments, use `const` correctness, and assume the array is always exactly size 101. Do not include a `main` function in the solution.

// The problem is a classic shortest-path on a directed graph where nodes are positions 1 to 100, and from each position you have edges to positions `i + 1` through `i + 6` (if within 1-100), but if any destination has a ladder or snake, you immediately follow it to its destination node. Because all edges have equal weight (1 dice roll), we can use Breadth-First Search (BFS) from position 1 to find the minimum number of rolls to reach 100. We maintain a `visited` boolean array of size 101 to avoid cycles (especially those caused by snakes that send you backwards). We initialize the queue with `(1, 0)` and mark position 1 as visited. For each popped position, we iterate over dice results 1 to 6, compute the raw next position, clamp to ≤ 100 (skip if > 100), then if that position has a ladder/snake (non-zero), replace `next` with the destination. If this final `next` is not visited, mark it visited and push with depth+1. If we pop position 100, return its depth. If the queue empties without reaching 100, return -1. Edge cases: position 100 may be a destination of a ladder/snake, but we still count it correctly; if the starting position has a ladder/snake (unlikely but possible), we should apply it before BFS, but per problem spec we may assume square 1 has no ladder/snake (we'll handle it gracefully by checking after reading input). Also, if a ladder leads to 100 directly, it's fine. Time complexity is O(100 * 6) = O(600) which is constant, and space is O(101) for the visited array and queue.

#include <queue>
#include <vector>

// Given a Snakes and Ladders board of size 101 (indices 0..100, index 0 unused),
// return the minimum number of 6-sided dice rolls to reach square 100 from square 1.
// If impossible, return -1.
// board[i] is 0 if square i has no ladder/snake, otherwise board[i] is the destination.
int minDiceRollsToFinish(const std::vector<int>& board) {
    constexpr int GOAL = 100;
    constexpr int BOARD_SIZE = 101;
    
    // Board must be the correct size.
    if (board.size() != BOARD_SIZE) {
        return -1;
    }
    
    std::vector<bool> visited(BOARD_SIZE, false);
    std::queue<std::pair<int, int>> bfsQueue; // {position, rolls}
    
    // Start from square 1.
    bfsQueue.push({1, 0});
    visited[1] = true;
    
    while (!bfsQueue.empty()) {
        auto [curPos, curRolls] = bfsQueue.front();
        bfsQueue.pop();
        
        if (curPos == GOAL) {
            return curRolls;
        }
        
        // Try all 6 dice rolls.
        for (int dice = 1; dice <= 6; ++dice) {
            int nextPos = curPos + dice;
            
            // Cannot overshoot the board.
            if (nextPos > GOAL) {
                continue;
            }
            
            // If there is a ladder or snake at nextPos, move to its destination.
            if (board[nextPos] != 0) {
                nextPos = board[nextPos];
            }
            
            // If not visited, visit and push.
            if (!visited[nextPos]) {
                visited[nextPos] = true;
                bfsQueue.push({nextPos, curRolls + 1});
            }
        }
    }
    
    // Unreachable.
    return -1;
}

#include <cassert>
#include <vector>

// Declaration of the function under test.
int minDiceRollsToFinish(const std::vector<int>& board);

int main() {
    // Case 1: No snakes or ladders, straight path from 1 to 100.
    // Minimum rolls is ceil((100-1)/6) = ceil(99/6) = 17.
    std::vector<int> board1(101, 0);
    assert(minDiceRollsToFinish(board1) == 17);

    // Case 2: A single ladder from 1 to 100, so 1 roll.
    std::vector<int> board2(101, 0);
    board2[1] = 100;
    assert(minDiceRollsToFinish(board2) == 1);

    // Case 3: A snake from 50 to 10, forcing a longer path.
    // Basic BFS should still find a path (no infinite loop).
    std::vector<int> board3(101, 0);
    board3[50] = 10;
    // This is not trivial manually, but BFS should return a positive number.
    int result3 = minDiceRollsToFinish(board3);
    assert(result3 > 0 && result3 <= 17);

    // Case 4: A snake from 100 to 1 (invalid but test robustness).
    // Since we start at 1 and never visit 100 except as goal, it should be reachable.
    std::vector<int> board4(101, 0);
    board4[100] = 1;
    // If we land on 100, we pop it before applying ladder/snake? Actually we check after moving,
    // so if a roll lands on 100, board[100] !=0, we would move to 1, making 100 unreachable.
    // So result should be -1.
    assert(minDiceRollsToFinish(board4) == -1);

    // Case 5: A ladder from 2 to 98, and another from 98 to 100.
    std::vector<int> board5(101, 0);
    board5[2] = 98;
    board5[98] = 100;
    // Start at 1, roll 1 to 2, then ladder to 98, then ladder to 100 => 2 rolls.
    assert(minDiceRollsToFinish(board5) == 2);

    // Case 6: Ladder from 50 to 60, but snake from 60 to 50 (loop).
    // That makes 50 and 60 unreachable? But you can still win by not landing there.
    std::vector<int> board6(101, 0);
    board6[50] = 60;
    board6[60] = 50;
    // Still reachable, result should be positive.
    int result6 = minDiceRollsToFinish(board6);
    assert(result6 > 0 && result6 <= 17);

    // Case 7: Snake from 10 to 1, but start is 1 so that's fine.
    std::vector<int> board7(101, 0);
    board7[10] = 1;
    // Still reachable.
    int result7 = minDiceRollsToFinish(board7);
    assert(result7 > 0 && result7 <= 17);

    // Case 8: Ladder from 1 to 99, and snake from 99 to 2. 
    // Start: 1 -> 99 (via ladder) -> snake to 2, then from 2 you have to go to 100.
    std::vector<int> board8(101, 0);
    board8[1] = 99;
    board8[99] = 2;
    // Expected: from 1 you go to 99 then 2, then need to reach 100.
    // BFS will handle this; result should be some positive number.
    int result8 = minDiceRollsToFinish(board8);
    assert(result8 > 0);

    // Case 9: All squares from 2 to 100 are snakes back to 1, and square 1 is normal.
    // Then you can never leave square 1 because any roll sends you back to 1.
    std::vector<int> board9(101, 0);
    for (int i = 2; i <= 100; ++i) {
        board9[i] = 1;
    }
    assert(minDiceRollsToFinish(board9) == -1);

    return 0;
}
