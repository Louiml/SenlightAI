// Given a vector of non-negative integers representing scores of athletes in a competition, write a C++ function `std::vector<std::string> findRelativeRanks(const std::vector<int>& score)` that returns a vector of strings where each string is the rank of the corresponding athlete. The top three scores receive "Gold Medal", "Silver Medal", and "Bronze Medal" respectively, and all other scores receive their numerical rank as a string (e.g., "4", "5", ...). The function must preserve the original order of the input vector in the output, meaning the string at index `i` corresponds to the athlete with score `score[i]`. The input will contain at least one score, and scores are distinct. The function should not modify the input vector.

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic example from problem
    std::vector<int> scores1 = {5, 4, 3, 2, 1};
    std::vector<std::string> expected1 = {"Gold Medal", "Silver Medal", "Bronze Medal", "4", "5"};
    assert(findRelativeRanks(scores1) == expected1);

    // Unsorted input
    std::vector<int> scores2 = {10, 3, 8, 9, 4};
    std::vector<std::string> expected2 = {"Gold Medal", "5", "Bronze Medal", "Silver Medal", "4"};
    assert(findRelativeRanks(scores2) == expected2);

    // Single element
    std::vector<int> scores3 = {42};
    std::vector<std::string> expected3 = {"Gold Medal"};
    assert(findRelativeRanks(scores3) == expected3);

    // Exactly three elements
    std::vector<int> scores4 = {0, -1, -2};  // Note: problem says non-negative, but test with negatives for robustness
    std::vector<std::string> expected4 = {"Gold Medal", "Silver Medal", "Bronze Medal"};
    assert(findRelativeRanks(scores4) == expected4);

    // All equal? Not allowed by problem, but if duplicate, top three distinct positions
    std::vector<int> scores5 = {100, 99, 98};
    std::vector<std::string> expected5 = {"Gold Medal", "Silver Medal", "Bronze Medal"};
    assert(findRelativeRanks(scores5) == expected5);

    // Large numbers
    std::vector<int> scores6 = {1000, 1, 500, 10000};
    std::vector<std::string> expected6 = {"Bronze Medal", "4", "Silver Medal", "Gold Medal"};
    assert(findRelativeRanks(scores6) == expected6);

    // Already sorted descending
    std::vector<int> scores7 = {9, 8, 7, 6, 5, 4};
    std::vector<std::string> expected7 = {"Gold Medal", "Silver Medal", "Bronze Medal", "4", "5", "6"};
    assert(findRelativeRanks(scores7) == expected7);
}

#include <vector>
#include <string>
#include <algorithm>
#include <utility>

// Returns relative ranks for each score, preserving original order.
std::vector<std::string> findRelativeRanks(const std::vector<int>& score) {
    const int n = static_cast<int>(score.size());
    std::vector<std::pair<int, int>> indexed_scores;
    indexed_scores.reserve(n);
    
    for (int i = 0; i < n; ++i) {
        indexed_scores.emplace_back(score[i], i);
    }
    
    // Sort descending by score (first element of pair).
    std::sort(indexed_scores.begin(), indexed_scores.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });
    
    std::vector<std::string> result(n);
    for (int rank = 0; rank < n; ++rank) {
        const int original_index = indexed_scores[rank].second;
        if (rank == 0) {
            result[original_index] = "Gold Medal";
        } else if (rank == 1) {
            result[original_index] = "Silver Medal";
        } else if (rank == 2) {
            result[original_index] = "Bronze Medal";
        } else {
            result[original_index] = std::to_string(rank + 1);
        }
    }
    return result;
}

// The problem requires computing relative ranks while preserving the original order. The most efficient approach is to create an auxiliary vector of pairs (or a 2D vector) that stores each original score along with its original index. Sort this auxiliary structure in descending order of score. After sorting, iterate through the sorted structure: for the first three positions (indices 0, 1, 2 in the sorted order), assign the medal strings; for all later positions, assign the numeric rank as a string. Since the auxiliary structure remembers the original index, we can place each rank string into the result vector at that original index. This approach works in `O(n log n)` time due to sorting, and uses `O(n)` extra space for the auxiliary structure and the result vector. Edge cases include a single score (which gets "Gold Medal") and exactly three scores (first three get medals, no numeric ranks). Since scores are distinct, there is no tie-breaking necessary. The comparator for sorting must be strict weak ordering; a simple lambda that compares the score part descending is sufficient.
