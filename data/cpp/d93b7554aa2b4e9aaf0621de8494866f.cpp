// Write a C++ function `std::string ableToMove(const std::string& begin, const std::string& target)` that determines whether it is possible to transform the string `begin` into the string `target` under the following rules. Each string consists only of the characters `'L'`, `'R'`, and `'.'`, and has the same length `n` (1 ≤ n ≤ 50). An `'L'` piece can move only to the left (i.e., its index can only decrease) and an `'R'` piece can move only to the right (its index can only increase). A piece may move any number of positions, but cannot jump over another piece or occupy the same cell as another piece; empty cells are represented by `'.'`. The order of pieces by type (ignoring `'.'`) must be preserved, meaning that an `'L'` or `'R'` in `begin` corresponds to the first matching non-`'.'` character in `target` in left-to-right order. Return `"Possible"` if the transformation is achievable under these constraints, otherwise return `"Impossible"`. The function must not rely on global state and should be efficient for the given constraints.

// The key observation is that pieces never change their relative order: scanning both strings from left to right, the sequence of `'L'` and `'R'` characters (ignoring `'.'`) must be identical. Additionally, each piece's direction dictates movement constraints: an `'L'` can only move left, so its final index in `target` must be less than or equal to its initial index in `begin`; an `'R'` can only move right, so its final index must be greater than or equal to its initial index. The algorithm is a two-pointer scan: maintain a queue (or pointer) of the non-`'.'` characters from `begin` with their original indices. Then iterate through `target`, and whenever a non-`'.'` character is encountered, pop the next piece from the queue. Check that the character matches; if not, it's impossible. Then check the direction constraint: for `'L'`, the target index must be ≤ the original index; for `'R'`, the target index must be ≥ the original index. After processing all target characters, the queue must be empty (same number of non-`'.'` characters in both strings). If any check fails, return `"Impossible"`; otherwise, return `"Possible"`. This works in O(n) time and O(n) space for the queue, which is optimal given the need to store original positions. Edge cases include strings with no pieces (both all `'.'`), single-piece strings, and cases where pieces would need to cross each other (handled automatically because the order must match and direction constraints prevent invalid swaps).

#include <string>
#include <queue>
#include <utility>

// Determines if 'begin' can be transformed into 'target' by moving L/R pieces.
// L pieces move left only, R pieces move right only, and pieces cannot cross.
std::string ableToMove(const std::string& begin, const std::string& target) {
    // Queue of (character, original index) for all non-dot characters in begin.
    std::queue<std::pair<char, int>> pieces;
    for (int i = 0; i < static_cast<int>(begin.size()); ++i) {
        if (begin[i] != '.') {
            pieces.push({begin[i], i});
        }
    }

    // Process target from left to right, matching each non-dot to the next piece.
    for (int i = 0; i < static_cast<int>(target.size()); ++i) {
        if (target[i] != '.') {
            if (pieces.empty()) {
                return "Impossible";
            }
            auto piece = pieces.front();
            pieces.pop();
            char type = piece.first;
            int originalIndex = piece.second;

            // Characters must match in order.
            if (type != target[i]) {
                return "Impossible";
            }

            // Direction constraints: L can't move right, R can't move left.
            if (type == 'L' && i > originalIndex) {
                return "Impossible";
            }
            if (type == 'R' && i < originalIndex) {
                return "Impossible";
            }
        }
    }

    // All non-dot characters in begin must be accounted for.
    if (!pieces.empty()) {
        return "Impossible";
    }

    return "Possible";
}

#include <cassert>
#include <string>

// Forward declaration for the solution function.
std::string ableToMove(const std::string& begin, const std::string& target);

int main() {
    // Example from the prompt.
    assert(ableToMove("R...", "..R.") == "Possible");
    assert(ableToMove("..R.", "R...") == "Impossible");
    assert(ableToMove(".L.R.R.", "L...R.R") == "Possible");
    assert(ableToMove(".L.R.", ".R.L.") == "Impossible");
    assert(ableToMove("LRLLRLRLLRLLRLRLRL", "LRLLRLRLLRLLRLRLRL") == "Possible");
    assert(ableToMove("L", ".") == "Impossible");

    // Additional edge cases.
    assert(ableToMove("...", "...") == "Possible");
    assert(ableToMove("L.R", "L.R") == "Possible");
    assert(ableToMove("R.L", "R.L") == "Possible");
    assert(ableToMove("L.R", ".LR") == "Impossible"); // order changes
    assert(ableToMove("R.L", "RL.") == "Possible"); // R moves right, L stays
    assert(ableToMove("RL", "LR") == "Impossible"); // pieces swap order
    assert(ableToMove("R...L", "..RL.") == "Impossible"); // R can't move left, L can't move right
    assert(ableToMove("R...L", "...RL") == "Possible"); // R and L both move outward

    return 0;
}
