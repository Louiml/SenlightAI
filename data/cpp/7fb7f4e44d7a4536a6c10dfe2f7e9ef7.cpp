Write a C++ function `maxAssignments` that takes two vectors of positive integers, `players` and `trainers`, representing player skill levels and trainer capacities. A player can be assigned to a trainer only if the player’s skill is less than or equal to the trainer’s capacity. Each trainer can train at most one player, and each player can use at most one trainer. Return the maximum number of players that can be assigned to trainers. The function should handle empty vectors (returning 0) and duplicate values correctly.
The optimal strategy is to greedily match the smallest available player with the smallest trainer that can accommodate them, which maximizes the total number of matches. Sort both vectors in non-decreasing order. Use two indexes: one for players and one for trainers. For each player (in sorted order), advance the trainer index until you find a trainer whose capacity is at least the player’s skill. If such a trainer exists, increment the match count and move both indexes forward; otherwise, since the player cannot be matched and all remaining trainers are even smaller (or we've run out), skip the player by advancing only the player index. Edge cases include empty input, players with skills larger than all trainers (no matches), and trainers with capacities much larger than needed (wasted capacity is fine). Time complexity is \(O(p \log p + t \log t)\) for sorting, and \(O(p + t)\) for the two-pointer scan; space complexity is \(O(1)\) auxiliary beyond input storage.
#include <vector>
#include <algorithm>

// Returns the maximum number of players that can be matched to trainers.
// Each player has a skill; each trainer has a capacity.
// A player can be matched if skill <= capacity, and each trainer is used at most once.
int maxAssignments(std::vector<int> players, std::vector<int> trainers) {
    std::sort(players.begin(), players.end());
    std::sort(trainers.begin(), trainers.end());

    int playerIndex = 0;
    int trainerIndex = 0;
    int matchCount = 0;

    while (playerIndex < players.size() && trainerIndex < trainers.size()) {
        if (players[playerIndex] <= trainers[trainerIndex]) {
            // Match this player with the current trainer.
            ++matchCount;
            ++playerIndex;
            ++trainerIndex;
        } else {
            // This trainer is too small for the current player; skip it.
            ++trainerIndex;
        }
    }

    return matchCount;
}
#include <cassert>
#include <vector>

int maxAssignments(std::vector<int> players, std::vector<int> trainers); // declaration

int main() {
    // Basic matching
    assert(maxAssignments({1, 2, 3}, {1, 2, 3}) == 3);
    // Some players cannot be matched
    assert(maxAssignments({1, 5, 7}, {2, 6}) == 2);
    // Empty inputs
    assert(maxAssignments({}, {}) == 0);
    assert(maxAssignments({1, 2}, {}) == 0);
    assert(maxAssignments({}, {5, 6}) == 0);
    // Duplicate values
    assert(maxAssignments({2, 2, 2}, {2, 2, 2}) == 3);
    assert(maxAssignments({2, 2, 2}, {1, 1}) == 0);
    // All players too skilled
    assert(maxAssignments({10, 11}, {1, 2}) == 0);
    // Trainers with extra capacity
    assert(maxAssignments({3, 4}, {10, 10}) == 2);
    // Unsorted input
    assert(maxAssignments({4, 2, 3}, {3, 1, 2}) == 3);
    // Single player
    assert(maxAssignments({7}, {5}) == 0);
    assert(maxAssignments({7}, {7}) == 1);
    return 0;
}
