Write a C++ function `int assignPlayersToTrainers(std::vector<int>& players, std::vector<int>& trainers)` that takes two vectors of integers representing player skill levels and trainer capability levels. Each trainer can train exactly one player, but only if the trainer’s capability is **greater than or equal to** the player’s skill. The function must return the maximum number of players that can be matched (assigned to distinct trainers). Players and trainers can be matched in any order, and each player uses at most one trainer, and each trainer can handle at most one player. The input vectors may be unsorted, may contain duplicates, may be empty, and may have different sizes. The function should modify the input vectors (sorting them is allowed and expected for efficiency). If a player cannot be matched to any trainer, that player is skipped, and we still aim to maximize the total count. The function should not print anything and should return the integer count.

// The key insight is that to maximize the number of matches, we should sort both arrays in non-decreasing order. This allows us to use a two-pointer technique: one pointer `playerIdx` for the players array and one pointer `trainerIdx` for the trainers array. Because both arrays are sorted, if a trainer cannot handle the current player (i.e., trainer capability < player skill), then that trainer also cannot handle any later player (since later players have equal or higher skill). Therefore, we simply advance `trainerIdx` to skip that trainer. If the trainer can handle the current player, we match them (increment both pointers and the count). This greedy approach guarantees an optimal solution because sorting ensures we always pair the smallest possible trainer with each player, preserving larger trainers for future (harder) players. Edge cases include empty vectors (return 0), one vector empty (return 0), all trainers too weak (return 0), all players matched, and duplicate values (handled naturally by the comparison). Time complexity is O(m log m + n log n) for sorting, plus O(m + n) for the two-pointer scan, where m = players size, n = trainers size. Space complexity is O(log m + log n) for the sort (if using introspective sort) plus O(1) extra space for the pointers and count.

#include <vector>
#include <algorithm>

// Returns the maximum number of players that can be matched to distinct trainers.
// Each trainer can handle one player if trainer capability >= player skill.
// Sorts both vectors in place to enable an efficient two-pointer scan.
int assignPlayersToTrainers(std::vector<int>& players, std::vector<int>& trainers) {
    // Sort both arrays in non-decreasing order to use the greedy two-pointer approach.
    std::sort(players.begin(), players.end());
    std::sort(trainers.begin(), trainers.end());

    int playerIdx = 0;
    int trainerIdx = 0;
    int matchedCount = 0;

    // If a trainer can't handle the current player, it can't handle any later player either.
    // So we advance only the trainer pointer until we find a suitable match or run out.
    while (playerIdx < static_cast<int>(players.size()) && trainerIdx < static_cast<int>(trainers.size())) {
        if (trainers[trainerIdx] < players[playerIdx]) {
            // This trainer is too weak for current player; skip it.
            ++trainerIdx;
        } else {
            // Match this player with this trainer.
            ++matchedCount;
            ++playerIdx;
            ++trainerIdx;
        }
    }

    return matchedCount;
}

#include <cassert>
#include <vector>

int assignPlayersToTrainers(std::vector<int>& players, std::vector<int>& trainers);

int main() {
    // Basic case: all players match
    std::vector<int> p1 = {1, 2, 3};
    std::vector<int> t1 = {3, 2, 1};
    assert(assignPlayersToTrainers(p1, t1) == 3);

    // Some players cannot be matched
    std::vector<int> p2 = {1, 5, 2};
    std::vector<int> t2 = {3, 4};
    assert(assignPlayersToTrainers(p2, t2) == 2);

    // Empty players
    std::vector<int> p3 = {};
    std::vector<int> t3 = {1, 2, 3};
    assert(assignPlayersToTrainers(p3, t3) == 0);

    // Empty trainers
    std::vector<int> p4 = {1, 2};
    std::vector<int> t4 = {};
    assert(assignPlayersToTrainers(p4, t4) == 0);

    // All trainers too weak
    std::vector<int> p5 = {5, 6};
    std::vector<int> t5 = {1, 2, 3};
    assert(assignPlayersToTrainers(p5, t5) == 0);

    // Duplicates and unsorted input
    std::vector<int> p6 = {2, 2, 1, 1};
    std::vector<int> t6 = {2, 1, 3, 1};
    assert(assignPlayersToTrainers(p6, t6) == 4);

    // Only one match possible
    std::vector<int> p7 = {3, 4, 5};
    std::vector<int> t7 = {4, 1, 2};
    assert(assignPlayersToTrainers(p7, t7) == 1);

    // Large trainer pool, small player pool
    std::vector<int> p8 = {7, 2};
    std::vector<int> t8 = {5, 9, 1, 2, 8, 7};
    assert(assignPlayersToTrainers(p8, t8) == 2);

    // Players all stronger than any trainer
    std::vector<int> p9 = {10, 11, 12};
    std::vector<int> t9 = {9, 8, 7};
    assert(assignPlayersToTrainers(p9, t9) == 0);

    // Single player single trainer (equal capability)
    std::vector<int> p10 = {5};
    std::vector<int> t10 = {5};
    assert(assignPlayersToTrainers(p10, t10) == 1);

    return 0;
}
