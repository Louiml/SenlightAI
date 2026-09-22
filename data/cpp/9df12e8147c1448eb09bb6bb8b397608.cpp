// Write a C++ function that takes a vector of positive integers and returns a string indicating the winner of a two-player game. The game starts with the given array of positive integers. On each turn, a player must choose a single element and decrease it by exactly 1 (so if the element is 1, it becomes 0 and is effectively removed from future consideration since it can't be decreased further). The game continues until no element can be decreased (i.e., all elements are 0). The player who makes the last valid move wins. If the total number of moves possible is even, the first player loses and the second player wins; if odd, the first player wins. The function should return `"maomao90"` if the first player wins, and `"errorgorn"` if the second player wins. The input vector will have at least 1 element and each element is between 1 and 10^9.

#include <cassert>
#include <vector>
#include <string>

// Forward declaration of the solution function.
std::string gameWinner(const std::vector<long long>& a);

int main() {
    // Single element, 3 moves -> odd -> first player wins
    assert(gameWinner({3}) == "maomao90");
    // Single element, 1 move -> odd -> first player wins
    assert(gameWinner({1}) == "errorgorn"); // 0 moves -> even
    // Two elements: (2,2) -> 1+1=2 moves -> even -> second player wins
    assert(gameWinner({2,2}) == "errorgorn");
    // Three elements: (3,1,2) -> 2+0+1=3 moves -> odd -> first player wins
    assert(gameWinner({3,1,2}) == "maomao90");
    // Large values: (1000000000, 1) -> 999999999+0 = 999999999 -> odd
    assert(gameWinner({1000000000LL, 1}) == "maomao90");
    // All ones: no moves -> even -> second player wins
    assert(gameWinner({1,1,1}) == "errorgorn");
    // Mixed parity: (5,4) -> 4+3=7 -> odd
    assert(gameWinner({5,4}) == "maomao90");
    // Multiple elements all 2: (2,2,2) -> 1+1+1=3 -> odd
    assert(gameWinner({2,2,2}) == "maomao90");
    // Empty test (should not happen per spec, but handles gracefully with no moves)
    std::vector<long long> empty;
    assert(gameWinner(empty) == "errorgorn");
    return 0;
}

#include <string>
#include <vector>

// Determines the winner of the described game.
// Returns "maomao90" if the first player wins, otherwise "errorgorn".
std::string gameWinner(const std::vector<long long>& a) {
    long long totalMoves = 0;
    for (long long value : a) {
        totalMoves += (value - 1);
    }
    // If total moves is odd, the first player makes the last move.
    return (totalMoves % 2 == 1) ? "maomao90" : "errorgorn";
}

// The total number of moves in the game is simply the sum of `(a[i] - 1)` over all elements, because each move reduces a selected element by exactly 1 until it reaches 0. Since players alternate and the game is deterministic (no choices affect the total move count), the winner is determined solely by the parity of this total sum: if the sum is odd, the first player makes the last move; if even, the second player does. No strategy is involved, so the solution is just to compute the sum of `(a[i] - 1)` modulo 2. Edge cases include large values and large `n` — use 64-bit integers to avoid overflow. Time complexity is O(n) and space complexity is O(1) beyond input storage.
