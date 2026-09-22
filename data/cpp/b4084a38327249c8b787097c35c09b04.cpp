Write a C++ function named `isReturnToOrigin` that takes a single string `moves` consisting only of the characters `'U'`, `'D'`, `'L'`, and `'R'` (representing up, down, left, and right moves on a 2D grid). The function should return `true` if, after processing all moves sequentially starting from the origin `(0,0)`, the final position is again `(0,0)`; otherwise return `false`. The input string may be empty (which should return `true`), may have unbalanced counts (e.g., more `'U'` than `'D'`), and may contain any combination of the four letters. You may assume the input contains only these four characters.
// The problem reduces to checking whether the total number of up moves equals the total number of down moves and the total number of left moves equals the total number of right moves. This is because each `'U'` increases the y-coordinate by 1, each `'D'` decreases it by 1, each `'R'` increases the x-coordinate by 1, and each `'L'` decreases it by 1. The starting and ending positions coincide exactly when the net changes in both x and y are zero. Thus we can simply count the occurrences of each character in a single pass and compare the corresponding pairs. The empty string trivially satisfies the condition. There are no special edge cases beyond the fact that the counts must be compared as integers; no overflow risk exists because the string length is bounded by practical input sizes. Time complexity is O(n) where n is the length of the string (single pass), and space complexity is O(1) (four integer counters).
#include <string>

// Returns true if a sequence of moves returns to the origin (0,0).
// Each 'U' increases y, 'D' decreases y, 'R' increases x, 'L' decreases x.
bool isReturnToOrigin(const std::string& moves) {
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    for (char move : moves) {
        if (move == 'U') {
            ++up;
        } else if (move == 'D') {
            ++down;
        } else if (move == 'L') {
            ++left;
        } else { // move == 'R' (by problem constraint)
            ++right;
        }
    }

    return (up == down) && (left == right);
}
int main() {
    assert(isReturnToOrigin("") == true);
    assert(isReturnToOrigin("UD") == true);
    assert(isReturnToOrigin("LR") == true);
    assert(isReturnToOrigin("ULDR") == true);
    assert(isReturnToOrigin("U") == false);
    assert(isReturnToOrigin("RLUD") == true);
    assert(isReturnToOrigin("URDL") == true);
    assert(isReturnToOrigin("UUDDLLRR") == true);
    assert(isReturnToOrigin("UUDDLLR") == false);
    assert(isReturnToOrigin("LLLLRRRR") == true);
    return 0;
}
