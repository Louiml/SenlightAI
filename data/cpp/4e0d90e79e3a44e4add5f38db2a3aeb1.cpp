// Write a C++ function `bool cardsCanBePlayed(int s1, int s2, int s3, int s4)` that simulates a two-player card game. Each player has two cards (player 1: `s1, s2`, player 2: `s3, s4`). In each round, both players reveal their larger card, and the player with the higher larger card wins that round. Then both players reveal their smaller card, and the player with the higher smaller card wins that round. The game is considered playable if it is possible for the same player to win **both** rounds (i.e., one player has both the largest and the smallest card among the four values, but only if that player's two cards are not both larger than the opponent's two cards in a way that makes one round a tie). More precisely, return `true` if and only if the two players' card sets are **not** strictly separated: it must not be the case that both cards of one player are strictly greater than both cards of the other player, nor that both cards of one player are strictly less than both cards of the other. Equivalently, the intervals [min(s1,s2), max(s1,s2)] and [min(s3,s4), max(s3,s4)] must overlap (including endpoints). The function should handle all integer inputs, including negatives and zeros, and return `true` if the intervals overlap, `false` otherwise.
The problem reduces to checking whether two intervals on the integer line overlap. Let player 1's cards form interval `[a1, b1]` with `a1 = min(s1,s2)` and `b1 = max(s1,s2)`. Similarly, player 2's interval is `[a2, b2]` with `a2 = min(s3,s4)` and `b2 = max(s3,s4)`. The game is "playable" (meaning neither player dominates both rounds outright) iff these intervals overlap at least at one point. Two intervals `[a1,b1]` and `[a2,b2]` do **not** overlap if one is entirely to the left of the other, i.e., `b1 < a2` or `b2 < a1`. Therefore, the intervals overlap iff `b1 >= a2` and `b2 >= a1`. That is the exact condition to implement. Edge cases: equal values on all cards, identical intervals, and values that touch at a single point (e.g., `[1,2]` and `[2,3]`) all count as overlap and should return `true`. Time complexity is O(1) and space complexity O(1), as only a constant number of arithmetic and comparison operations are performed.
#include <algorithm>

// Checks whether the two card intervals overlap.
// player1 cards: s1, s2 ; player2 cards: s3, s4
// Returns true if [min(s1,s2), max(s1,s2)] and [min(s3,s4), max(s3,s4)] intersect.
bool cardsCanBePlayed(int s1, int s2, int s3, int s4) {
    const int lo1 = std::min(s1, s2);
    const int hi1 = std::max(s1, s2);
    const int lo2 = std::min(s3, s4);
    const int hi2 = std::max(s3, s4);

    // Intervals overlap if neither is completely to the left of the other.
    return !(hi1 < lo2 || hi2 < lo1);
}
#include <cassert>

// Forward declaration of the solution function (assumes it is defined above).
bool cardsCanBePlayed(int s1, int s2, int s3, int s4);

int main() {
    // Basic overlapping intervals
    assert(cardsCanBePlayed(1, 2, 3, 4) == false);  // [1,2] and [3,4] no overlap
    assert(cardsCanBePlayed(3, 4, 1, 2) == false);  // [3,4] and [1,2] no overlap
    assert(cardsCanBePlayed(1, 3, 2, 4) == true);   // [1,3] and [2,4] overlap
    // Touch at a single point
    assert(cardsCanBePlayed(1, 2, 2, 3) == true);   // [1,2] and [2,3] touch at 2
    // Identical values
    assert(cardsCanBePlayed(5, 5, 5, 5) == true);   // [5,5] and [5,5] overlap
    // Negative and zero values
    assert(cardsCanBePlayed(-5, -1, -3, 0) == true); // [-5,-1] and [-3,0] overlap
    assert(cardsCanBePlayed(-5, -5, -4, -4) == false); // [-5,-5] and [-4,-4] no overlap
    // One player's cards both smaller and both larger in different orders
    assert(cardsCanBePlayed(10, 20, 5, 15) == true); // [10,20] and [5,15] overlap
    assert(cardsCanBePlayed(10, 20, 30, 40) == false); // [10,20] and [30,40] no overlap
    return 0;
}
