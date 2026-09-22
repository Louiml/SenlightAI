/*
Given a string of positive integers separated by whitespace, write a C++ function `std::string mergeInsertionSortDescriptive(const std::string& input, bool useDeque)` that returns a string containing the sorted integers in ascending order (space-separated). The function must implement the Ford–Johnson merge-insertion sort algorithm, operating on either a `std::deque<int>` or a `std::list<int>` depending on the flag. The input is guaranteed to contain only valid positive integers (no negatives, no non‑digit characters) separated by arbitrary whitespace, and may be empty (in which case return an empty string). The algorithm must handle duplicate values correctly, and must not use `std::sort` or any other ready‑made sorting function. The return value must be the sorted sequence as a single string with integers separated by single spaces, with no leading or trailing whitespace.
*/
#include <string>
#include <sstream>
#include <deque>
#include <list>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstddef>

// Helper to generate Jacobsthal numbers up to a given maximum.
static std::vector<int> generateJacobsthal(int max) {
    std::vector<int> jac;
    int a = 0, b = 1;
    while (b <= max) {
        jac.push_back(b);
        int tmp = b;
        b = b + 2 * a;
        a = tmp;
    }
    return jac;
}

// Create pairs from a vector of ints. Returns pairs (winner, loser) where winner <= loser.
// Also captures the unpaired element if the size is odd.
static std::vector<std::pair<int, int>> createPairs(const std::vector<int>& temp, int& unpaired, bool& hasUnpaired) {
    std::vector<std::pair<int, int>> pairs;
    for (std::size_t i = 0; i < temp.size(); i += 2) {
        if (i + 1 < temp.size()) {
            int a = temp[i], b = temp[i + 1];
            if (a > b) std::swap(a, b);
            pairs.push_back(std::make_pair(a, b));
        } else {
            unpaired = temp[i];
            hasUnpaired = true;
        }
    }
    return pairs;
}

// ---- Deque variants ----
static std::deque<int> extractWinnersDeque(const std::vector<std::pair<int, int>>& pairs) {
    std::deque<int> winners;
    for (const auto& p : pairs)
        winners.push_back(p.first);
    return winners;
}

static void insertUnpairedDeque(std::deque<int>& winners, int unpaired) {
    auto it = winners.begin();
    while (it != winners.end() && *it < unpaired)
        ++it;
    winners.insert(it, unpaired);
}

static void insertLosersDeque(std::deque<int>& winners,
                              const std::vector<std::pair<int, int>>& pairs,
                              const std::vector<int>& jac,
                              std::vector<bool>& inserted) {
    for (std::size_t j = 0; j < jac.size(); ++j) {
        int idx = jac[j] - 1;
        if (idx >= 0 && idx < static_cast<int>(pairs.size()) && !inserted[idx]) {
            int winner = pairs[idx].first;
            int loser = pairs[idx].second;
            auto it = std::find(winners.begin(), winners.end(), winner);
            if (it != winners.end()) ++it;
            while (it != winners.end() && *it < loser)
                ++it;
            winners.insert(it, loser);
            inserted[idx] = true;
        }
    }
}

static void insertRemainingLosersDeque(std::deque<int>& winners,
                                       const std::vector<std::pair<int, int>>& pairs,
                                       std::vector<bool>& inserted) {
    for (std::size_t i = 0; i < pairs.size(); ++i) {
        if (!inserted[i]) {
            int winner = pairs[i].first;
            int loser = pairs[i].second;
            auto it = std::find(winners.begin(), winners.end(), winner);
            if (it != winners.end()) ++it;
            while (it != winners.end() && *it < loser)
                ++it;
            winners.insert(it, loser);
        }
    }
}

static std::deque<int> fordJohnsonDeque(std::deque<int> input) {
    if (input.size() <= 1)
        return input;
    std::vector<int> temp(input.begin(), input.end());
    int unpaired = 0;
    bool hasUnpaired = false;
    auto pairs = createPairs(temp, unpaired, hasUnpaired);
    std::deque<int> winners = extractWinnersDeque(pairs);
    winners = fordJohnsonDeque(winners);
    if (hasUnpaired)
        insertUnpairedDeque(winners, unpaired);
    auto jac = generateJacobsthal(static_cast<int>(pairs.size()));
    std::vector<bool> inserted(pairs.size(), false);
    insertLosersDeque(winners, pairs, jac, inserted);
    insertRemainingLosersDeque(winners, pairs, inserted);
    return winners;
}

// ---- List variants ----
static std::list<int> extractWinnersList(const std::vector<std::pair<int, int>>& pairs) {
    std::list<int> winners;
    for (const auto& p : pairs)
        winners.push_back(p.first);
    return winners;
}

static void insertUnpairedList(std::list<int>& winners, int unpaired) {
    auto it = winners.begin();
    while (it != winners.end() && *it < unpaired)
        ++it;
    winners.insert(it, unpaired);
}

static void insertLosersList(std::list<int>& winners,
                             const std::vector<std::pair<int, int>>& pairs,
                             const std::vector<int>& jac,
                             std::vector<bool>& inserted) {
    for (std::size_t j = 0; j < jac.size(); ++j) {
        int idx = jac[j] - 1;
        if (idx >= 0 && idx < static_cast<int>(pairs.size()) && !inserted[idx]) {
            int winner = pairs[idx].first;
            int loser = pairs[idx].second;
            auto it = std::find(winners.begin(), winners.end(), winner);
            if (it != winners.end()) ++it;
            while (it != winners.end() && *it < loser)
                ++it;
            winners.insert(it, loser);
            inserted[idx] = true;
        }
    }
}

static void insertRemainingLosersList(std::list<int>& winners,
                                      const std::vector<std::pair<int, int>>& pairs,
                                      std::vector<bool>& inserted) {
    for (std::size_t i = 0; i < pairs.size(); ++i) {
        if (!inserted[i]) {
            int winner = pairs[i].first;
            int loser = pairs[i].second;
            auto it = std::find(winners.begin(), winners.end(), winner);
            if (it != winners.end()) ++it;
            while (it != winners.end() && *it < loser)
                ++it;
            winners.insert(it, loser);
        }
    }
}

static std::list<int> fordJohnsonList(std::list<int> input) {
    if (input.size() <= 1)
        return input;
    std::vector<int> temp(input.begin(), input.end());
    int unpaired = 0;
    bool hasUnpaired = false;
    auto pairs = createPairs(temp, unpaired, hasUnpaired);
    std::list<int> winners = extractWinnersList(pairs);
    winners = fordJohnsonList(winners);
    if (hasUnpaired)
        insertUnpairedList(winners, unpaired);
    auto jac = generateJacobsthal(static_cast<int>(pairs.size()));
    std::vector<bool> inserted(pairs.size(), false);
    insertLosersList(winners, pairs, jac, inserted);
    insertRemainingLosersList(winners, pairs, inserted);
    return winners;
}

// Main solution function.
// Parses the input string, sorts using Ford–Johnson on either deque or list,
// and returns the sorted space-separated string.
std::string mergeInsertionSortDescriptive(const std::string& input, bool useDeque) {
    std::istringstream iss(input);
    if (useDeque) {
        std::deque<int> d;
        int value;
        while (iss >> value)
            d.push_back(value);
        d = fordJohnsonDeque(d);
        if (d.empty())
            return "";
        std::ostringstream oss;
        for (auto it = d.begin(); it != d.end(); ++it) {
            if (it != d.begin())
                oss << ' ';
            oss << *it;
        }
        return oss.str();
    } else {
        std::list<int> l;
        int value;
        while (iss >> value)
            l.push_back(value);
        l = fordJohnsonList(l);
        if (l.empty())
            return "";
        std::ostringstream oss;
        for (auto it = l.begin(); it != l.end(); ++it) {
            if (it != l.begin())
                oss << ' ';
            oss << *it;
        }
        return oss.str();
    }
}
#include <cassert>
#include <string>

// The solution function is declared above; for the test we just need the declaration.
std::string mergeInsertionSortDescriptive(const std::string& input, bool useDeque);

int main() {
    // Basic cases
    assert(mergeInsertionSortDescriptive("5 4 3 2 1", true) == "1 2 3 4 5");
    assert(mergeInsertionSortDescriptive("5 4 3 2 1", false) == "1 2 3 4 5");

    // Single element
    assert(mergeInsertionSortDescriptive("42", true) == "42");
    assert(mergeInsertionSortDescriptive("42", false) == "42");

    // Empty string
    assert(mergeInsertionSortDescriptive("", true) == "");
    assert(mergeInsertionSortDescriptive("", false) == "");

    // Duplicate values
    assert(mergeInsertionSortDescriptive("3 1 2 3", true) == "1 2 3 3");
    assert(mergeInsertionSortDescriptive("3 1 2 3", false) == "1 2 3 3");

    // Already sorted
    assert(mergeInsertionSortDescriptive("1 2 3 4 5", true) == "1 2 3 4 5");
    assert(mergeInsertionSortDescriptive("1 2 3 4 5", false) == "1 2 3 4 5");

    // Odd count (tests unpaired handling)
    assert(mergeInsertionSortDescriptive("9 8 7 6 5 4 3", true) == "3 4 5 6 7 8 9");
    assert(mergeInsertionSortDescriptive("9 8 7 6 5 4 3", false) == "3 4 5 6 7 8 9");

    // Multiple whitespace and leading/trailing whitespace
    assert(mergeInsertionSortDescriptive("  10 2  8  4  ", true) == "2 4 8 10");
    assert(mergeInsertionSortDescriptive("  10 2  8  4  ", false) == "2 4 8 10");

    // Larger set to stress recursion
    assert(mergeInsertionSortDescriptive("12 11 13 5 6 7 1 2 3 4 5 6 7", true)
           == "1 2 3 4 5 5 6 6 7 7 11 12 13");
    assert(mergeInsertionSortDescriptive("12 11 13 5 6 7 1 2 3 4 5 6 7", false)
           == "1 2 3 4 5 5 6 6 7 7 11 12 13");

    return 0;
}
// The Ford–Johnson algorithm (also known as merge-insertion sort) is a comparison sort that minimizes the number of comparisons in the worst case. The approach:
// 1. **Pairing**: Convert the input container (deque or list) into a `std::vector<int>` for pairing. Pair adjacent elements: if `a > b`, swap so that the smaller of each pair becomes the "winner" (stored as the pair's first element) and the larger is the "loser" (second element). If there's an odd count, the last element remains unpaired.
// 2. **Recursive sorting of winners**: Extract all winners into a new container of the same type (deque/list). Recursively apply the same algorithm to the winners until the base case (size ≤ 1).
// 3. **Inserting the unpaired element** (if any): Since the unpaired element has no "partner", it is inserted into the sorted winners sequence using simple linear insertion.
// 4. **Inserting losers using Jacobsthal numbers**: To achieve near-optimal comparisons, the losers are inserted in the order given by the Jacobsthal sequence. Generate Jacobsthal numbers up to the number of pairs. For each Jacobsthal index (1‑based), insert the corresponding loser into the winners sequence at the correct position by linear search starting right after its winner. Mark that loser as inserted. Finally, insert any remaining losers in the original order.
// 5. **Time complexity**: The Ford–Johnson algorithm has a worst-case number of comparisons of \(n \log n - 1.415n\), but the actual running time (including element moves) is \(O(n \log n)\) for both comparisons and moves. Space complexity is \(O(n)\) due to recursion stack and auxiliary vectors.
// 6. **Edge cases**: Empty input → empty string. Single element → just return that element. Duplicates are handled naturally by linear insertion (`while (*it < loser)` stops at the first element ≥ loser). The algorithm works correctly for any size, though the given snippet restricts to 3000 elements for performance; the task does not require that restriction, but the reference implementation can include it or not as a comment.
// 7. **Implementation detail**: Both `std::deque` and `std::list` support the needed operations (`push_back`, `begin`, `end`, `insert`). The linear insertion for each loser is in the worst case \(O(n)\), yielding overall \(O(n^2)\) in the worst case for insertion moves, but in practice with Jacobsthal order, it’s efficient. For clarity, the reference solution uses the same helper functions as the provided snippet, parameterized by container type.
