// You are given three integers `n`, `z`, and `w`, followed by a list of `n` non-negative integers `a[0], a[1], ..., a[n-1]`. Two players play a game on this sequence: Player 1 starts with the number `w` and Player 2 starts with the number `z`. The players alternate turns, and on each turn, a player must choose one card from the remaining deck (the sequence `a`). The chosen card's value becomes that player's new number, and the card is removed from the deck. The game ends when all cards have been drawn. The final score is the absolute difference between Player 1's final number and Player 2's final number. Player 1 wants to maximize the final score, and Player 2 wants to minimize it. Both players play optimally. Write a C++ function `int optimalScore(int n, int z, int w, const std::vector<int>& a)` that returns the final score under optimal play. Note that the order of drawing is fixed: Player 1 draws first, then Player 2, then Player 1, etc., but players can choose any remaining card on their turn. The function should handle `n >= 1`.

#include <cassert>
#include <vector>
#include <cstdlib>

// Assuming optimalScore is defined as above
int main() {
    // n=1, simple
    assert(optimalScore(1, 10, 5, {3}) == std::abs(3-5)); // 2
    assert(optimalScore(1, 0, 0, {0}) == 0);

    // n=2, Player 1 can take last or second last
    assert(optimalScore(2, 0, 0, {1, 10}) == std::max(std::abs(10-0), std::abs(1-10))); // max(10,9)=10
    assert(optimalScore(2, 0, 5, {10, 1}) == std::max(std::abs(1-5), std::abs(10-1))); // max(4,9)=9

    // n=3, example from snippet
    std::vector<int> a1 = {1, 2, 3};
    assert(optimalScore(3, 0, 0, a1) == std::max(std::abs(3-0), std::abs(2-3))); // max(3,1)=3

    // n=4 with larger numbers
    std::vector<int> a2 = {7, 2, 9, 4};
    assert(optimalScore(4, 100, 1, a2) == std::max(std::abs(4-1), std::abs(9-4))); // max(3,5)=5

    // n=5, all equal
    std::vector<int> a3 = {5, 5, 5, 5, 5};
    assert(optimalScore(5, 0, 0, a3) == std::max(std::abs(5-0), std::abs(5-5))); // max(5,0)=5

    // n=2 with negative? Problem states non-negative, but function works anyway
    // We'll test with non-negative per spec.

    return 0;
}

#include <vector>
#include <cstdlib>
#include <algorithm>

// Computes the optimal final score in the card game described.
// n: number of cards, z: Player 2's initial number (unused in final formula for n>=2),
// w: Player 1's initial number, a: the card values in order.
int optimalScore(int n, int z, int w, const std::vector<int>& a) {
    // The parameter z is intentionally unused because the first move by Player 1
    // effectively replaces w, and Player 2's initial z only matters if n == 1? Actually
    // if n==1, Player 1 draws a[0], score = abs(a[0]-w). For n>=2, the optimal is as derived.
    (void)z; // suppress unused parameter warning
    if (n == 1) {
        return std::abs(a[0] - w);
    }
    // Player 1 can either take the last card immediately (score abs(a.back()-w))
    // or leave the last two cards to be decided between a[n-2] and a[n-1].
    return std::max(std::abs(a.back() - w), std::abs(a[n-2] - a[n-1]));
}

// This is a classic "optimal play" game that can be solved by observing that after the first move by Player 1, the game reduces to a deterministic outcome based only on the last two cards. Because both players are rational and the deck is finite, the optimal result is actually always either `abs(a.back() - w)` (if Player 1 takes the last card immediately) or `abs(a[n-2] - a[n-1])` (if Player 1 passes to Player 2 who is forced to take the last card). More specifically, on Player 1's first turn, he can either take the last card `a[n-1]`, ending the game with score `abs(a[n-1] - w)` (since Player 2 still has `z`), or he can take some other card, leaving Player 2 to take the last card. If Player 1 takes a non-last card, Player 2 will optimally take the last card to minimize the score, resulting in `abs(a[n-2] - a[n-1])` (the score between the last two cards, because Player 1's final number becomes the card he took, which is at least as good for him as taking `a[n-2]`? Actually, the key is that after the first move, the game is forced: the remaining cards are drawn in pairs, but the final score depends only on the last two cards because the players can always choose to leave the last two as the decider. The optimal strategy is to take either the last card immediately, or take the second-to-last card so that Player 2 is forced to take the last card. All other choices are suboptimal because they allow the opponent to control the last two cards. Thus the answer is `max(abs(a.back() - w), abs(a[n-2] - a[n-1]))` for `n >= 2`. For `n == 1`, Player 1 must take the only card, resulting in `abs(a[0] - w)`. The time complexity is O(1) (or O(n) if we need to read the vector but the function itself is O(1) after having the vector), and space complexity is O(1) auxiliary.
