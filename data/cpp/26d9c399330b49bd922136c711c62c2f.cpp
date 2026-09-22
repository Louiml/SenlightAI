/*
Design a standalone C++ function that simulates the scoring logic for a single trick in a simplified card game (Hearts-like) without relying on any external classes. The function takes four integers representing the “hearts values” of the four cards played in order (player 0, 1, 2, 3), and a leading card suit indicator (0 = clubs, 1 = diamonds, 2 = hearts, 3 = spades) as well as the suit of each played card (also 0–3). The rules: the first player (index 0) always leads; the winner is the player who played the highest card (higher integer value) of the leading suit, provided they followed suit; if a player did not follow suit (their suit differs from the leading suit), they cannot win even with a higher value. All players who follow suit are compared only by their card value (an integer, e.g., 2–14 for Ace-high). The function returns an integer 0–3 indicating the winner’s index. If no player follows suit (only the leader does), the leader wins. The function must be pure, deterministic, and work for arbitrary inputs. Edge case: all four cards may have the same suit and value duplicates; the first occurrence among followers in order (starting from index 0) wins ties.
*/

#include <vector>

// Determine the winner of a single trick in a simplified card game.
// Parameters:
//   values - four integers representing card ranks (e.g., 2-14, higher is better)
//   suits  - four integers representing suits (0-3), player 0 leads.
// Returns index (0-3) of the winner.
int trickWinner(const std::vector<int>& values, const std::vector<int>& suits) {
    int leadingSuit = suits[0];
    int winner = 0;
    int winningValue = values[0];

    for (int i = 1; i < 4; ++i) {
        if (suits[i] == leadingSuit && values[i] > winningValue) {
            winner = i;
            winningValue = values[i];
        }
    }

    return winner;
}

#include <cassert>
#include <vector>

int main() {
    // Leader wins because no one follows suit.
    assert(trickWinner({5, 10, 12, 14}, {0, 1, 2, 3}) == 0);

    // Player 2 follows suit and has highest value.
    assert(trickWinner({5, 10, 12, 14}, {0, 0, 0, 0}) == 3);

    // Player 1 wins with higher value; player 3 has higher value but off-suit.
    assert(trickWinner({5, 10, 7, 14}, {0, 0, 1, 2}) == 1);

    // Tie on value, earliest follower (index 0) wins.
    assert(trickWinner({10, 10, 5, 2}, {0, 0, 0, 0}) == 0);

    // Player 2 wins with higher value; player 3 same value but later loses.
    assert(trickWinner({3, 4, 9, 9}, {2, 2, 2, 2}) == 2);

    // Leader has maximum value among followers, so wins.
    assert(trickWinner({14, 3, 5, 7}, {1, 1, 1, 1}) == 0);

    // All off-suit except leader.
    assert(trickWinner({2, 13, 12, 11}, {3, 0, 1, 2}) == 0);

    // Player 3 wins despite low value because all others off-suit.
    assert(trickWinner({2, 3, 4, 5}, {1, 2, 3, 0}) == 3);

    // Mixed: player 1 follows and wins.
    assert(trickWinner({6, 8, 7, 9}, {0, 0, 1, 2}) == 1);

    // Duplicate value among followers: earliest index wins.
    assert(trickWinner({5, 11, 11, 2}, {0, 0, 0, 0}) == 1);

    return 0;
}

// The solution iterates over the four players in order, but the leading suit is fixed by the first player’s suit (since player 0 leads). The winner starts as player 0 (the leader). For each subsequent player (indices 1, 2, 3), if that player’s suit matches the leading suit, then compare their card value to the current winner’s value. If the current winner’s suit does not match the leading suit (which can only happen if the current winner was a previous follower who failed suit? Actually no, only followers who match suit are considered; the initial leader always matches because their suit defines the leading suit). So the algorithm: let leadingSuit = suit[0], winner = 0, winningValue = value[0]. For i from 1 to 3: if suit[i] == leadingSuit && value[i] > winningValue (strictly greater, so ties go to earlier player), update winner = i, winningValue = value[i]. This is correct because any player not following suit cannot win, and the leader is always following their own suit. Time complexity O(4)=O(1), space O(1). Edge case: if the leader’s card has a low value but no other player follows suit, leader wins. If another player follows suit with the same value as leader, leader wins (since strict less-than comparison). If a later follower has equal value but earlier follower, the earliest index among equal values wins because we only update on strict greater.
