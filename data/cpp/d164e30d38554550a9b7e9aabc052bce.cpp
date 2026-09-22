// Write a C++ function named `turnUpSwitchToLeft` that simulates a segment of the Centipede game’s movement logic. The function receives a `std::array<int, 4>` representing the current iterator state (0 to 3), a `std::array<int, 2>` representing the current row and column of the head, and a `bool` indicating whether the cell ahead (to the left) is clear, blocked, or poison (encoded as an `int` obstacle code: 0 = clear, 1 = blocked, 2 = poison). The function must return a `std::string` describing the next state and updated position, following exactly the logic of the provided snippet: if the iterator is not at the cap (3), increment it and return `"TurnUpSwitchToLeft"` with the same row/col; if at the cap, reset iterator to 0, update the head’s position (move to next row/col per the turn-up-left movement), then inspect the cell one column to the left of the new position. If clear, return `"StateMoveLeftAndUpwards"` with the updated row and column (column decremented by 1). If blocked, return `"StateTurnUpSwitchToRight"` or `"StateTurnDownSwitchToRight"` depending on whether the row is not equal to the top player row (assume `TopPlayerRow = 0`). If poison, return `"StatePoisonTurnDownSwitchToRight"`. For body states, the function should simply increment the iterator (no cap reset) and return `"TurnUpSwitchToLeft"` without position changes, but since the task is head-only, simplify the API to one function that handles the head logic described. The function must be `const`-correct, meaning it does not modify its inputs (pass by const reference or by value), and returns a string representing the next state name plus `:` plus the new row and column (space-separated) when the iterator reaches the cap; otherwise, return just the state name with the existing row and column appended. Ensure the function works for all valid iterator values (0–3) and handles edge cases like the top player row boundary.
#include <cassert>
#include <string>

// The solution function is declared above; here we test it.
// (In a real file, include the function definition above this main.)
int main() {
    // Iterator not at cap: state unchanged, position unchanged.
    assert(turnUpSwitchToLeft(0, 5, 3, 0) == "TurnUpSwitchToLeft:5 3");
    assert(turnUpSwitchToLeft(2, 5, 3, 1) == "TurnUpSwitchToLeft:5 3");

    // Iterator at cap (3), clear obstacle (0): moves up and then left.
    assert(turnUpSwitchToLeft(3, 5, 3, 0) == "StateMoveLeftAndUpwards:4 2");

    // Iterator at cap, blocked obstacle, not top row: turn up-right.
    assert(turnUpSwitchToLeft(3, 5, 3, 1) == "StateTurnUpSwitchToRight:4 3");

    // Iterator at cap, blocked obstacle, top row (row becomes 0): turn down-right.
    assert(turnUpSwitchToLeft(3, 1, 3, 1) == "StateTurnDownSwitchToRight:0 3");

    // Iterator at cap, poison obstacle (2): poison turn down-right.
    assert(turnUpSwitchToLeft(3, 5, 3, 2) == "StatePoisonTurnDownSwitchToRight:4 3");

    // Edge case: row 0 at cap, clear obstacle (row becomes -1, but logic is only for example).
    // Here we assume the function does not guard overflow for simplicity; test with row=1 to avoid negative.
    assert(turnUpSwitchToLeft(3, 1, 0, 0) == "StateMoveLeftAndUpwards:0 -1");

    // Multiple calls with same iterator but different obstacle codes.
    assert(turnUpSwitchToLeft(3, 4, 5, 0) == "StateMoveLeftAndUpwards:3 4");
    assert(turnUpSwitchToLeft(3, 4, 5, 1) == "StateTurnUpSwitchToRight:3 5");

    // Iterator exactly 3 with top boundary (row=1 -> newRow=0) and blocked -> down-right.
    assert(turnUpSwitchToLeft(3, 1, 7, 1) == "StateTurnDownSwitchToRight:0 7");

    return 0;
}
#include <string>
#include <array>

// Simulates the TurnUpSwitchToLeft head state transition.
// Parameters:
//   i        - current iterator index (0..3)
//   row, col - current head position
//   obstacle - 0=clear, 1=blocked, 2=poison
// Returns a string formatted as "<state>:<row> <col>" 
// (the row/col reflect the head's new position after the turn step,
//  and optionally the look‑ahead column when clear).
std::string turnUpSwitchToLeft(const int i, const int row, const int col, const int obstacle) {
    constexpr int ITERATOR_CAP = 3;
    constexpr int TOP_PLAYER_ROW = 0;

    if (i < ITERATOR_CAP) {
        // Not at cap: iterate and stay in the same state, position unchanged.
        return "TurnUpSwitchToLeft:" + std::to_string(row) + " " + std::to_string(col);
    }

    // At cap: reset iterator, update position (turn step: move up one row).
    int newRow = row - 1;  // moving up
    int newCol = col;      // column unchanged for the turn step

    // Look ahead one column to the left.
    int lookAheadCol = newCol - 1;

    // Determine next state based on obstacle.
    if (obstacle == 0) { // Clear
        return "StateMoveLeftAndUpwards:" + std::to_string(newRow) + " " + std::to_string(lookAheadCol);
    } else if (obstacle == 1) { // Blocked
        if (newRow != TOP_PLAYER_ROW) {
            return "StateTurnUpSwitchToRight:" + std::to_string(newRow) + " " + std::to_string(newCol);
        } else {
            return "StateTurnDownSwitchToRight:" + std::to_string(newRow) + " " + std::to_string(newCol);
        }
    } else { // Poison (obstacle == 2)
        return "StatePoisonTurnDownSwitchToRight:" + std::to_string(newRow) + " " + std::to_string(newCol);
    }
}
// The solution must replicate the state‑machine transition logic from the snippet. The main algorithm: check the iterator index. If `i < 3`, increment it (since we pass the iterator by value, we construct the new iterator as `i+1`) and return the state name `"TurnUpSwitchToLeft"` with the unchanged row and column. If `i == 3`, we simulate the movement—reset iterator to 0, then update row/col based on the turn‑up‑left direction. In the original code, the movement offsets for `TurnUpSwitchToLeft` represent a diagonal step: the head moves one cell up and one cell left (or possibly up then left). Since the snippet calls `UpdateRowAndCol()` after rotation, and then inspects `(row, col-1)`, the head’s new position is effectively `row-1` (up) and `col` (unchanged horizontally for the turn step), and then it looks ahead to `col-1` for the next state. So we set the new row to `row-1` (assuming up decreases row) and keep the same column for the turn step. Then we inspect the cell at `(newRow, newCol-1)`. Based on obstacle type: clear → return `"StateMoveLeftAndUpwards:"` + `newRow` + `" "` + `(newCol-1)`; blocked → if `newRow != 0` (top player row), return `"StateTurnUpSwitchToRight:"` + `newRow` + `" "` + `newCol`; else `"StateTurnDownSwitchToRight:"` + `newRow` + `" "` + `newCol`. Poison → return `"StatePoisonTurnDownSwitchToRight:"` + `newRow` + `" "` + `newCol`. For iterator values, we must handle that `i` is valid (0–3). Edge cases: row may be 0; ensure we handle bounds logically. Time complexity is O(1) since we do constant work. Space complexity O(1) for the returned string.
