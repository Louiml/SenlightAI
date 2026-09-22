/*
Write a C++ function named `optimalGameScore` that takes a vector of positive integers (the values on cards placed in a row) and returns a pair of integers representing the final scores of two players. The game proceeds as follows: the first player and the second player alternate turns, starting with the first player. On each turn, the current player must take the card from either the leftmost end or the rightmost end of the remaining sequence (choosing the end that has the larger value; if the two ends are equal, choose either end, which does not affect the final outcome given optimal play is forced). The taken value is added to that player's total score, and the card is removed. The game ends when no cards remain. The function must return `{score1, score2}` where `score1` is the first player's total and `score2` is the second player's total. Assume the input vector has at least one element. The function must be pure and not modify the input vector.
*/
#include <vector>
#include <utility>
#include <cstddef>

// Simulate the greedy card game. Returns {scorePlayer1, scorePlayer2}.
std::pair<int, int> optimalGameScore(const std::vector<int>& cards) {
    int left = 0;
    int right = static_cast<int>(cards.size()) - 1;
    int score1 = 0;
    int score2 = 0;
    bool playerOneTurn = true;

    while (left <= right) {
        int chosenValue;
        if (cards[left] > cards[right]) {
            chosenValue = cards[left];
            ++left;
        } else {
            // If equal, pick the right end; it does not affect the result.
            chosenValue = cards[right];
            --right;
        }

        if (playerOneTurn) {
            score1 += chosenValue;
        } else {
            score2 += chosenValue;
        }
        playerOneTurn = !playerOneTurn;
    }

    return {score1, score2};
}
#include <cassert>
#include <vector>
#include <utility>

// The function under test (declared here for the test).
std::pair<int, int> optimalGameScore(const std::vector<int>& cards);

int main() {
    // Single card
    assert(optimalGameScore({5}) == std::make_pair(5, 0));

    // Two cards: both players pick the larger end each time
    assert(optimalGameScore({3, 7}) == std::make_pair(7, 3));

    // Four cards with distinct ends
    assert(optimalGameScore({10, 20, 30, 40}) == std::make_pair(50, 50));

    // Five cards, alternating picks
    assert(optimalGameScore({1, 5, 9, 3, 7}) == std::make_pair(17, 8));

    // Equal ends case: 6 and 6
    assert(optimalGameScore({6, 2, 3, 6}) == std::make_pair(12, 5));

    // Larger input: verify sum is total
    std::vector<int> v = {4, 1, 2, 8, 5, 9, 3, 6};
    auto result = optimalGameScore(v);
    int total = 0;
    for (int x : v) total += x;
    assert(result.first + result.second == total);

    // All equal values
    assert(optimalGameScore({4, 4, 4, 4}) == std::make_pair(8, 8));

    // Descending order
    assert(optimalGameScore({9, 7, 5, 3, 1}) == std::make_pair(15, 10));

    // Edge case: two cards equal
    assert(optimalGameScore({2, 2}) == std::make_pair(2, 2));
    return 0;
}
// The solution simulates the game exactly as described. Since both players are greedy and must always pick the larger of the two ends, there is no strategic choice beyond comparing the two ends; thus the game's outcome is deterministic given the initial arrangement. We maintain two indices, `left` and `right`, representing the current valid range of cards. We loop until `left > right` (all cards taken). For each turn, we compare `cards[left]` and `cards[right]`. If `cards[left] > cards[right]`, we take the left card (add to current player's score, increment `left`); otherwise, we take the right card (add to current player's score, decrement `right`). In case of equality, taking either end yields the same immediate value, and because the rest of the sequence is symmetrical in terms of the forced greedy rule, it does not alter the final scores—so we may always take the right side on equality for simplicity. After each turn, we switch to the other player. The loop runs exactly `n` times where `n` is the number of cards. Time complexity is `O(n)`, and space complexity is `O(1)`, not counting the input storage.
