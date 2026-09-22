/*
Write a C++ function named `winnerOfGame` that takes a string `colors` consisting only of the characters 'A' and 'B', representing a sequence of colored pieces on a line. Two players play a game: Alice can only remove a piece 'A' if it has both left and right neighbors that are also 'A' (i.e., it is part of three consecutive 'A's). Similarly, Bob can only remove a piece 'B' if it has both left and right neighbors that are also 'B'. Each move removes exactly one such eligible piece. Alice and Bob alternate turns, with Alice moving first. A player who cannot make a move on their turn loses. The function should return `true` if Alice will win assuming both play optimally, and `false` otherwise. Note that the string length is at least 1, and the game continues until no legal moves remain for the current player. For example, for `"AAABABB"` the function returns `true`, while for `"ABBBBBAAA"` it returns `false`. Provide a robust implementation with proper `const` correctness.
*/
#include <string>

// Determine if Alice wins the game given the initial color sequence.
// Alice moves first, can remove 'A' only if surrounded by 'A's.
// Bob can remove 'B' only if surrounded by 'B's.
// Returns true if Alice wins, false otherwise.
bool winnerOfGame(const std::string& colors) {
    int aliceMoves = 0;
    int bobMoves = 0;
    const int n = static_cast<int>(colors.size());

    // Count maximum possible moves for each player based on initial triples.
    for (int i = 1; i < n - 1; ++i) {
        if (colors[i - 1] == colors[i] && colors[i] == colors[i + 1]) {
            if (colors[i] == 'A') {
                ++aliceMoves;
            } else { // colors[i] == 'B'
                ++bobMoves;
            }
        }
    }

    // Alice wins if she has strictly more moves than Bob.
    return aliceMoves > bobMoves;
}
#include <cassert>
#include <string>

// Declaration of the function under test.
bool winnerOfGame(const std::string& colors);

int main() {
    // Example cases
    assert(winnerOfGame("AAABABB") == true);   // Alice has 1 triple, Bob has 0 -> Alice wins
    assert(winnerOfGame("ABBBBBAAA") == false); // Bob has 2 triples, Alice has 1 -> Bob wins
    assert(winnerOfGame("AAA") == true);        // Alice has 1 triple, Bob 0 -> Alice wins
    assert(winnerOfGame("BBB") == false);       // Alice 0, Bob 1 -> Bob wins
    assert(winnerOfGame("AB") == false);        // No triples, Alice cannot move -> Bob wins
    assert(winnerOfGame("A") == false);         // Single character, no moves -> Bob wins
    assert(winnerOfGame("AAAA") == true);       // Two triples for Alice, Bob 0 -> Alice wins
    assert(winnerOfGame("BBBBBB") == false);    // Bob has 4 triples, Alice 0 -> Bob wins
    assert(winnerOfGame("AABBAA") == false);    // Alice 0, Bob 0 -> Bob wins
    assert(winnerOfGame("AAABBB") == true);     // Alice 1, Bob 1 -> Alice wins (moves first)
    return 0;
}
// The key insight is that each move by a player only removes a piece and does not affect the ability of other same-colored pieces to be removed, because the removal condition only depends on the immediate neighbors at the moment of removal, and after removal the neighbors change but the count of potential moves for each color can be precomputed as the number of occurrences of three consecutive same-colored pieces in the initial string. Why? Because any three consecutive same-colored pieces in the initial string will remain three consecutive same-colored pieces until one of them is removed, but removal of a middle piece breaks that triple; however, the total number of moves each player can make is exactly the initial count of such triples for their color. This is because each move consumes exactly one triple, and after removing a piece from a triple, the remaining two pieces cannot form a new triple without a third, and no new triples are created by removals (only broken). Thus, the game reduces to comparing the number of legal moves available to each player: Alice’s total moves = count of triples "AAA", Bob’s total moves = count of triples "BBB". Since Alice moves first, if Alice’s count is greater than Bob’s count, then Alice will make the last move and win; otherwise Bob wins (or if equal, Bob wins because after Alice runs out, Bob moves next and then Alice loses). Edge cases: strings of length less than 3 yield zero triples for both, so Bob wins (false). Empty string is not expected, but the function should handle length 0 safely. Time complexity: O(n) single pass. Space complexity: O(1).
