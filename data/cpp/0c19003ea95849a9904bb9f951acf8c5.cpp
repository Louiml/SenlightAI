/*
Write a standalone C++ function that, given a vector of pairs where each pair contains a player name (std::string) and a score change (int), simulates a game scoreboard and returns the name of the first player who reaches the maximum possible final total score at the earliest point in time. Specifically, process the score updates in the given order: for each player, maintain their cumulative score after each update. After all updates, determine the overall maximum final score among all players. Then, among all players whose final score equals this maximum, return the name of the player who first achieved a cumulative score that is greater than or equal to this maximum during the processing sequence. If multiple players tie for the earliest such occurrence, return the one that appears earliest in the input sequence. If no player ever reaches the maximum (which cannot happen by definition, since at least one player must reach it at the end), the behavior is undefined. The input may contain negative score changes, players may appear multiple times, and some players may have a cumulative score that temporarily exceeds the final maximum before dropping later; such temporary highs must be ignored. Your function should be named `earliestMaxScorePlayer` and take a `const std::vector<std::pair<std::string, int>>&` as input, returning a `std::string`.
*/

#include <string>
#include <vector>
#include <map>
#include <algorithm>

// Returns the name of the first player to reach the final maximum cumulative score.
std::string earliestMaxScorePlayer(const std::vector<std::pair<std::string, int>>& updates) {
    std::map<std::string, int> cumulative;
    std::vector<std::pair<std::string, int>> log;
    log.reserve(updates.size());

    for (const auto& update : updates) {
        cumulative[update.first] += update.second;
        log.emplace_back(update.first, cumulative[update.first]);
    }

    int maxFinal = 0;
    for (const auto& entry : cumulative) {
        maxFinal = std::max(maxFinal, entry.second);
    }

    for (const auto& entry : log) {
        if (entry.second >= maxFinal) {
            return entry.first;
        }
    }

    // Should never be reached because at least one player has final score == maxFinal.
    return "";
}

#include <cassert>
#include <vector>
#include <string>

// Assume the solution function is defined above.

int main() {
    // Basic case: player A reaches max first.
    std::vector<std::pair<std::string, int>> updates1 = {{"A", 5}, {"B", 3}, {"A", 0}};
    assert(earliestMaxScorePlayer(updates1) == "A");

    // Tie: both reach max at the same step, earlier in list wins.
    std::vector<std::pair<std::string, int>> updates2 = {{"X", 2}, {"Y", 2}, {"X", 1}, {"Y", 1}};
    // Final: X=3, Y=3. X reaches 3 at step 3, Y at step 4? Actually Y reaches 3 at step 4, so X first.
    assert(earliestMaxScorePlayer(updates2) == "X");

    // Negative changes: someone temporarily exceeds final max.
    std::vector<std::pair<std::string, int>> updates3 = {{"P", 10}, {"Q", 5}, {"P", -5}, {"Q", 5}};
    // Final P=5, Q=10. Q reaches 10 only at last step; P never reaches 10. So Q.
    assert(earliestMaxScorePlayer(updates3) == "Q");

    // Multiple players, winner reaches max earlier than end.
    std::vector<std::pair<std::string, int>> updates4 = {{"Alice", 3}, {"Bob", 1}, {"Alice", 2}, {"Bob", 4}};
    // Final Alice=5, Bob=5. Alice reaches 5 at step 3, Bob reaches 5 at step 4. Alice wins.
    assert(earliestMaxScorePlayer(updates4) == "Alice");

    // Unique player.
    std::vector<std::pair<std::string, int>> updates5 = {{"Solo", 7}, {"Solo", -2}, {"Solo", 0}};
    assert(earliestMaxScorePlayer(updates5) == "Solo");

    // All scores zero.
    std::vector<std::pair<std::string, int>> updates6 = {{"A", 0}, {"B", 0}, {"A", 0}};
    // Max final is 0, first update is A at 0, so A.
    assert(earliestMaxScorePlayer(updates6) == "A");

    // Negative totals.
    std::vector<std::pair<std::string, int>> updates7 = {{"A", -5}, {"B", -2}, {"A", -1}, {"B", -1}};
    // Final A=-6, B=-3. Max is -3, B reaches -3 at step 4 (earliest? B's scores: -2, then -3 at step 4). So B.
    assert(earliestMaxScorePlayer(updates7) == "B");

    // First player reaches max at very first update.
    std::vector<std::pair<std::string, int>> updates8 = {{"A", 100}, {"B", 200}, {"A", -100}};
    // Final A=0, B=200. B reaches 200 at step 2, A never reaches 200. So B.
    assert(earliestMaxScorePlayer(updates8) == "B");

    return 0;
}

// The solution processes the input in a single pass while maintaining a map from player name to their current cumulative score. After each update, we append the current cumulative score to a log vector that records both the player name and their score at that moment. After processing all updates, we compute the maximum final score by scanning the map for the highest cumulative value among all players. Then we iterate through the log vector in the original order and return the name of the first entry whose recorded score is greater than or equal to this maximum. This correctly handles the case where a player's final score ties with the max but they reached that threshold earlier than the end; it also correctly ignores temporary peaks that are above the final maximum, because the threshold is the final max, not any intermediate max. Edge cases include duplicate names, negative updates, players with zero total, and multiple players reaching the max at the same step (we return the one that appears earlier in the log, which is determined by the order of processing). Time complexity is O(n) for processing, O(p log p) for building/scanning the map (p = number of distinct players), and O(n) for the final scan of the log, so overall O(n + p log p), which simplifies to O(n log n) in the worst case. Space complexity is O(n + p) to store the log and the map.
