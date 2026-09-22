/*
Write a C++ function `findWinner` that takes an array of strings representing candidate names cast in an election (each string is one vote) and its size `n`, and returns a `std::vector<std::string>` with exactly two elements: the name of the candidate with the most votes, and the number of votes they received as a string. If multiple candidates tie for the highest number of votes, select the candidate whose name is lexicographically smallest (standard string comparison). Votes are case-sensitive and may include spaces in names (treat each complete array element as a single candidate name). The function should be standalone, use `const` where appropriate, and handle an empty or single-element array gracefully (if `n == 0`, return an empty vector; if `n == 1`, return the only name and "1").
*/
#include <string>
#include <vector>
#include <map>

// Return a vector with the winning candidate's name and vote count (as string).
// If n == 0, return an empty vector. Ties are broken by lexicographic order.
std::vector<std::string> findWinner(const std::string arr[], int n) {
    if (n <= 0) {
        return {};
    }

    std::map<std::string, int> voteCount;
    for (int i = 0; i < n; ++i) {
        ++voteCount[arr[i]];
    }

    int maxVotes = -1;
    std::string winnerName;

    // map iterates in lexicographic order, so the first time we see a count greater
    // than the current max, that name is the smallest lexicographic among those with that count.
    for (const auto& entry : voteCount) {
        if (entry.second > maxVotes) {
            maxVotes = entry.second;
            winnerName = entry.first;
        }
    }

    return {winnerName, std::to_string(maxVotes)};
}
#include <cassert>
#include <string>
#include <vector>

// Include the solution function here or ensure it's visible.

int main() {
    // Test 1: Basic case with clear winner
    std::string votes1[] = {"Alice", "Bob", "Alice", "Charlie", "Bob", "Alice"};
    std::vector<std::string> result1 = findWinner(votes1, 6);
    assert(result1.size() == 2);
    assert(result1[0] == "Alice" && result1[1] == "3");

    // Test 2: Tie, lexicographically smaller name wins
    std::string votes2[] = {"Bob", "Alice", "Bob", "Alice"};
    std::vector<std::string> result2 = findWinner(votes2, 4);
    assert(result2[0] == "Alice" && result2[1] == "2");

    // Test 3: Single vote
    std::string votes3[] = {"Solo"};
    std::vector<std::string> result3 = findWinner(votes3, 1);
    assert(result3[0] == "Solo" && result3[1] == "1");

    // Test 4: All same candidate
    std::string votes4[] = {"X", "X", "X"};
    std::vector<std::string> result4 = findWinner(votes4, 3);
    assert(result4[0] == "X" && result4[1] == "3");

    // Test 5: Empty array returns empty vector
    std::string votes5[] = {};
    assert(findWinner(votes5, 0).empty());

    // Test 6: Names with spaces and case sensitivity
    std::string votes6[] = {"John Doe", "jane", "John Doe", "JOHN DOE", "jane"};
    std::vector<std::string> result6 = findWinner(votes6, 5);
    assert(result6[0] == "John Doe" && result6[1] == "2");

    // Test 7: All distinct, lexicographically smallest has one vote
    std::string votes7[] = {"zeta", "alpha", "mid"};
    std::vector<std::string> result7 = findWinner(votes7, 3);
    assert(result7[0] == "alpha" && result7[1] == "1");

    return 0;
}
// The solution uses a `std::map<std::string, int>` to count the frequency of each candidate name. A map is chosen because it automatically stores keys in sorted (lexicographic) order, which simplifies the tie-breaking rule. First, iterate over the input array and increment each candidate’s count in the map. Then, iterate through the map from beginning to end; since the map is ordered lexicographically, the first time we encounter a count greater than the current maximum, we update both the maximum count and the associated name. Because we only update on strictly greater counts, if a later (lexicographically greater) name has the same maximum count, we do not replace the previously stored candidate, preserving the smallest lexicographic name among ties. The result vector is built with the winning name and the count converted to a string via `std::to_string`. The algorithm runs in O(n log k) time where n is the number of votes and k is the number of distinct candidates (due to map insertions), and uses O(k) auxiliary space for the map. The edge case of an empty array returns an empty vector; a single element returns `{"name", "1"}`. All counts are positive integers.
