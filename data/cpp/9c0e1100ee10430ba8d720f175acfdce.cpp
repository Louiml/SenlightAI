// Write a C++ function `std::string decideWinner(const std::vector<std::pair<std::string, int>>& participants)` that takes a vector of name-score pairs and returns the name of the participant who wins according to the following rule: first, sort all participants lexicographically by name (ascending, case-sensitive using standard `std::string` comparison). Then compute the total sum of all scores. The winner is the participant at index `(total_sum % number_of_participants)` in the sorted vector. If the input is empty, return an empty string. The function must not modify the input vector; it should work with a const reference input.
#include <cassert>
#include <vector>
#include <string>
#include <utility>

// Function declaration (assumed from solution).
std::string decideWinner(const std::vector<std::pair<std::string, int>>& participants);

int main() {
    // Example: 3 participants, sum = 5 + 2 + 8 = 15, sorted: [Alice(2), Bob(8), Carol(5)], index 15%3=0 -> Alice
    std::vector<std::pair<std::string, int>> p1 = {{"Bob", 8}, {"Alice", 5}, {"Carol", 2}};
    assert(decideWinner(p1) == "Alice");

    // Same scores, different order; sorted: [Charlie(1), Dave(2), Eve(3)], sum=6, 6%3=0 -> Charlie
    std::vector<std::pair<std::string, int>> p2 = {{"Eve", 3}, {"Charlie", 1}, {"Dave", 2}};
    assert(decideWinner(p2) == "Charlie");

    // Only one participant: sum = 10, 10%1=0 -> winner is that participant.
    std::vector<std::pair<std::string, int>> p3 = {{"Solo", 10}};
    assert(decideWinner(p3) == "Solo");

    // Empty input returns empty string.
    std::vector<std::pair<std::string, int>> p4; 
    assert(decideWinner(p4) == "");

    // Duplicate names? Sorted order places them adjacently, but index is based on sorted position.
    std::vector<std::pair<std::string, int>> p5 = {{"Zoe", 2}, {"Amy", 3}, {"Amy", 1}};
    // sorted: [Amy(1), Amy(3), Zoe(2)], sum=6, 6%3=0 -> "Amy"
    assert(decideWinner(p5) == "Amy");

    // Negative scores allowed.
    std::vector<std::pair<std::string, int>> p6 = {{"Neg", -5}, {"Pos", 10}, {"Zero", 0}};
    // sorted: [Neg(-5), Pos(10), Zero(0)], sum=5, 5%3=2 -> "Zero"
    assert(decideWinner(p6) == "Zero");

    // Large number of participants, check no crash and correct modulo logic.
    std::vector<std::pair<std::string, int>> p7;
    for (int i = 0; i < 1000; ++i) {
        p7.push_back({"user" + std::to_string(i), i});
    }
    // sum = 0+1+...+999 = 499500, 499500 % 1000 = 500; sorted lexicographically: "user0", "user1", ... "user999" (since numeric order matches lexicographic for same length)
    assert(decideWinner(p7) == "user500");

    return 0;
}
#include <vector>
#include <string>
#include <algorithm>

// Returns the name of the winner based on the rule: sort by name,
// compute the sum of scores, and pick index (sum % count) after sorting.
// Returns an empty string if the input vector is empty.
std::string decideWinner(const std::vector<std::pair<std::string, int>>& participants) {
    if (participants.empty()) {
        return "";
    }

    // Make a mutable copy to sort without modifying the input.
    std::vector<std::pair<std::string, int>> sorted = participants;

    // Sort lexicographically by name (first element of each pair).
    std::sort(sorted.begin(), sorted.end());

    // Compute total sum of scores.
    int totalSum = 0;
    for (const auto& p : sorted) {
        totalSum += p.second;
    }

    // Determine winning index and return the winner's name.
    int winnerIndex = totalSum % static_cast<int>(sorted.size());
    return sorted[winnerIndex].first;
}
// The task requires three main steps: compute the total sum, sort the participants by name, and select the winner using modular arithmetic. Since the input must not be modified, copy the vector into a local mutable vector before sorting. For an empty vector, immediately return an empty string to avoid division by zero (though modulo by zero is undefined behavior; we guard against it explicitly). Summation uses a simple loop over `int` (which may overflow for large score values, but typical competitive constraints keep values within 32-bit range; if needed, you could use `long long`, but the problem statement implies `int`). Sorting uses `std::sort` on pairs, which sorts by the first element (name) by default; if two names are identical, the tie is broken by the second element, but since names are unique in typical problems, this behavior is fine. After sorting, compute `index = total_sum % n` where `n = participants.size()`, and return `.first` of that element. Time complexity is `O(n log n)` for sorting plus `O(n)` for summation, so overall `O(n log n)`. Space complexity is `O(n)` for the copy of the vector.
