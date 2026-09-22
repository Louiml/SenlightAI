// Write a C++ function `std::string determineWinner(int n)` that, given a positive integer `n` representing the number of stones in a game where two players alternately remove stones, returns `"First"` if the player who goes first wins under optimal play, and `"Second"` otherwise. The rule is that in each turn, a player can remove exactly 1 or 2 stones (not 0 and not more than 2). The player who cannot move (i.e., takes the last stone) wins. The function must handle any `n >= 1` correctly, and the result should be entirely determined by the value of `n` modulo 3.

This is a classic impartial combinatorial game. The key is to find the losing positions (P-positions) and winning positions (N-positions). If `n = 1` or `n = 2`, the first player can remove all stones and win. If `n = 3`, whatever the first player does (removing 1 or 2) leaves 2 or 1 stones for the opponent, who then wins, so `n = 3` is a losing position for the first player. For `n = 4`, the first player can remove 1 stone, leaving 3 to the opponent (a losing position for the opponent), so 4 is winning. Similarly, 5 is winning (remove 2 to leave 3), and 6 is losing (any move leaves 4 or 5, both winning for the opponent). This pattern repeats every 3: positions where `n % 3 == 0` are losing for the first player (so second wins), and positions where `n % 3 == 1` or `2` are winning for the first player. Edge case: `n = 0` is not part of the problem since `n >= 1`, but if it were, it would be a P-position. Time complexity is O(1), space complexity is O(1).

#include <string>

// Determine the winner in a game where each move removes 1 or 2 stones.
// Returns "First" if the first player wins, "Second" otherwise.
// The game ends when a player takes the last stone; that player wins.
// n is the number of stones, n >= 1.
std::string determineWinner(int n) {
    // Positions where n % 3 == 0 are losing for the player to move.
    if (n % 3 == 0) {
        return "Second";
    }
    return "First";
}

#include <cassert>
#include <string>

// Free function declaration (assumed to be defined above)
std::string determineWinner(int n);

int main() {
    // Small cases
    assert(determineWinner(1) == "First");   // Take 1 stone
    assert(determineWinner(2) == "First");   // Take 2 stones
    assert(determineWinner(3) == "Second");  // Any move leaves winning position

    // Larger multiples of 3
    assert(determineWinner(6) == "Second");
    assert(determineWinner(9) == "Second");
    assert(determineWinner(300) == "Second");

    // Non-multiples of 3
    assert(determineWinner(4) == "First");
    assert(determineWinner(5) == "First");
    assert(determineWinner(7) == "First");
    assert(determineWinner(8) == "First");
    assert(determineWinner(100) == "First");

    // Edge case: large value
    assert(determineWinner(1000000000) == "First"); // 1e9 % 3 == 1

    // Verify pattern: all numbers with n%3==1 or 2 are First, 0 are Second
    for (int n = 1; n <= 100; ++n) {
        std::string expected = (n % 3 == 0) ? "Second" : "First";
        assert(determineWinner(n) == expected);
    }

    return 0;
}
