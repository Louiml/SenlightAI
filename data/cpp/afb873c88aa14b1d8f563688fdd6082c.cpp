/*
Write a C++ function that takes a vector of pairs of strings and integers (representing names and scores) and returns the name(s) of the player(s) who achieved the maximum total score. If multiple players have the same maximum total score, return all such names sorted in lexicographical order. The function should ignore zero or negative individual scores (treat them as not contributing) and should only consider players who have at least one positive score. Each player may appear multiple times with different scores; sum only positive scores per player. If no player has a positive score, return an empty vector.
*/
#include <vector>
#include <string>
#include <map>
#include <algorithm>

// Given a vector of (name, score) pairs, return names with maximum positive total score.
// Only positive scores are summed; ties broken lexicographically; empty if none.
std::vector<std::string> topPlayers(const std::vector<std::pair<std::string, int>>& records) {
    std::map<std::string, int> total; // name -> sum of positive scores

    // Accumulate positive scores for each player
    for (const auto& entry : records) {
        if (entry.second > 0) {
            total[entry.first] += entry.second;
        }
    }

    if (total.empty()) {
        return {};
    }

    // Find the maximum total score
    int maxScore = 0;
    for (const auto& [name, score] : total) {
        maxScore = std::max(maxScore, score);
    }

    // Collect all names with that max score (map is already sorted)
    std::vector<std::string> result;
    for (const auto& [name, score] : total) {
        if (score == maxScore) {
            result.push_back(name);
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <iostream>

int main() {
    // Basic case
    std::vector<std::pair<std::string, int>> data1 = {{"Alice", 10}, {"Bob", 5}, {"Alice", 20}};
    assert(topPlayers(data1) == std::vector<std::string>({"Alice"}));

    // Multiple players tied
    std::vector<std::pair<std::string, int>> data2 = {{"A", 5}, {"B", 5}, {"C", 3}};
    assert(topPlayers(data2) == std::vector<std::string>({"A", "B"})); // lexicographic order

    // Negative and zero scores ignored
    std::vector<std::pair<std::string, int>> data3 = {{"X", -5}, {"Y", 0}, {"Z", 2}};
    assert(topPlayers(data3) == std::vector<std::string>({"Z"}));

    // All non-positive scores -> empty result
    std::vector<std::pair<std::string, int>> data4 = {{"A", -1}, {"B", 0}};
    assert(topPlayers(data4).empty());

    // Empty input -> empty result
    std::vector<std::pair<std::string, int>> data5;
    assert(topPlayers(data5).empty());

    // Player with only negative scores is ignored even if others have positive
    std::vector<std::pair<std::string, int>> data6 = {{"A", -10}, {"B", 1}, {"C", 1}};
    assert(topPlayers(data6) == std::vector<std::string>({"B", "C"}));

    // Duplicate names but positive and negative mixed
    std::vector<std::pair<std::string, int>> data7 = {{"P", 3}, {"P", -2}, {"Q", 3}};
    assert(topPlayers(data7) == std::vector<std::string>({"P", "Q"})); // P sums to 3

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution uses a `std::map<std::string, int>` to accumulate total positive scores per player. Iterate through the input vector; for each entry, if the score is > 0, add it to the map entry corresponding to the name (map automatically handles duplicate names by updating the value). After accumulation, find the maximum value among all entries. Then iterate through the map (which stores keys in sorted order) and collect all keys whose value equals the maximum. This approach naturally yields the names in lexicographical order because `std::map` is ordered. Edge cases: empty input vector, all scores negative or zero, multiple players tied for maximum. Time complexity is \(O(N \log M)\) where \(N\) is the number of entries and \(M\) is the number of distinct players, due to map insertions; finding max and collecting results is \(O(M)\). Space complexity is \(O(M)\).
