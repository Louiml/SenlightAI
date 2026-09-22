/*
Write a C++ function that determines the winner of a two-player dice game based on the rolls of two pairs of dice. Each player rolls two dice with values between 0 and 6 inclusive (where 0 represents a special case, but for this task values are non-negative integers). The function receives four integers: `a1`, `a2` (Player 1's dice) and `b1`, `b2` (Player 2's dice). The game follows these rules to decide the winner:
1. If both of a player's dice sum to 3 (i.e., one die shows 1 and the other shows 2, in any order), that player has a "spinner"; if both players have spinners it's a tie, otherwise the spinner player wins.
2. If a player's two dice are equal (a double), that player has a "double"; if only one player has a double, that player wins. If both players have doubles, the higher double value wins, or tie if equal.
3. Otherwise, compare the numbers formed by each player's two dice ordered descending (e.g., dice 3 and 5 form the number 53). The player with the higher number wins, or tie if equal.
Return the string `"Player 1 wins."`, `"Player 2 wins."`, or `"Tie."` accordingly. The function should handle any non-negative integer inputs, and the rules for spinners take precedence over doubles, and doubles take precedence over the numeric comparison.
*/

#include <string>
#include <algorithm>

// Determines the winner of the dice game given Player 1's dice (a1, a2)
// and Player 2's dice (b1, b2). Returns "Player 1 wins.", "Player 2 wins.",
// or "Tie.".
std::string determineDiceWinner(int a1, int a2, int b1, int b2) {
    const bool p1_spinner = (a1 + a2 == 3);
    const bool p2_spinner = (b1 + b2 == 3);

    if (p1_spinner || p2_spinner) {
        if (p1_spinner && p2_spinner) return "Tie.";
        if (p1_spinner) return "Player 1 wins.";
        return "Player 2 wins.";
    }

    const bool p1_double = (a1 == a2);
    const bool p2_double = (b1 == b2);

    if (p1_double || p2_double) {
        if (p1_double && p2_double) {
            if (a1 > b1) return "Player 1 wins.";
            if (a1 < b1) return "Player 2 wins.";
            return "Tie.";
        }
        if (p1_double) return "Player 1 wins.";
        return "Player 2 wins.";
    }

    const int p1_number = std::max(a1, a2) * 10 + std::min(a1, a2);
    const int p2_number = std::max(b1, b2) * 10 + std::min(b1, b2);

    if (p1_number > p2_number) return "Player 1 wins.";
    if (p1_number < p2_number) return "Player 2 wins.";
    return "Tie.";
}

#include <cassert>
#include <string>

// The function is declared above; including its definition here for completeness.
// (In a real test, you'd include the solution file.)
std::string determineDiceWinner(int a1, int a2, int b1, int b2);

int main() {
    // Spinner (1+2) cases
    assert(determineDiceWinner(1, 2, 3, 3) == "Player 1 wins.");
    assert(determineDiceWinner(3, 4, 2, 1) == "Player 2 wins.");
    assert(determineDiceWinner(2, 1, 1, 2) == "Tie.");

    // Double cases
    assert(determineDiceWinner(3, 3, 4, 5) == "Player 1 wins.");
    assert(determineDiceWinner(4, 5, 6, 6) == "Player 2 wins.");
    assert(determineDiceWinner(5, 5, 4, 4) == "Player 1 wins.");
    assert(determineDiceWinner(4, 4, 4, 4) == "Tie.");

    // Numeric comparison
    assert(determineDiceWinner(3, 5, 2, 3) == "Player 1 wins.");  // 53 vs 32
    assert(determineDiceWinner(2, 3, 5, 3) == "Player 2 wins.");  // 32 vs 53
    assert(determineDiceWinner(2, 4, 4, 2) == "Tie.");            // 42 vs 42

    // Spinner beats double
    assert(determineDiceWinner(1, 2, 6, 6) == "Player 1 wins.");
    assert(determineDiceWinner(3, 3, 2, 1) == "Player 2 wins.");

    // Double beats numeric
    assert(determineDiceWinner(2, 2, 6, 5) == "Player 1 wins.");
    assert(determineDiceWinner(1, 6, 3, 3) == "Player 2 wins.");

    return 0;
}

// The solution must evaluate the rules in the exact order given: first check if either player has a spinner (sum of dice equals 3). A spinner occurs only when one die is 1 and the other is 2 (since dice are non-negative, a sum of 3 can also be 0+3, but the problem intends standard dice 1-6; however, since inputs can be any non-negative integer, we must interpret "sum equals 3" literally as the code does, so 0+3 also counts as a spinner). If both have spinners, it's a tie; if exactly one, that player wins. Next, check for doubles (both dice equal). If exactly one player has a double, that player wins; if both have doubles, compare the double value directly (higher wins, equal ties). Otherwise, both players have non-double, non-spinner rolls; form a two-digit number from each pair by placing the larger die as the tens digit and the smaller as the units digit, then compare these numbers numerically. The order of precedence is critical: spinners beat doubles and any numeric roll, and doubles beat numeric rolls. The algorithm runs in constant time \(O(1)\) since it only does a fixed number of comparisons and arithmetic operations, and uses \(O(1)\) space.
