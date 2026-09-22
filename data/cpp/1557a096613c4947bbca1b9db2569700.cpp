// Write a C++ function named `countOccurrences` that takes a vector of integers `cards` and a second vector of integers `queries`, and returns a vector of integers where each element corresponds to the number of times the respective query value appears in `cards`. The input vectors may contain duplicate values, negative numbers, and may be unsorted. The function must be efficient for large inputs (up to 10^5 elements) and handle cases where a query value does not appear (return 0). The function should not modify the input vectors.
#include <cassert>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Test 1: Basic duplicates and missing values
    std::vector<int> cards1 = {1, 2, 2, 3, 3, 3, 4, 5};
    std::vector<int> queries1 = {3, 2, 1, 5, 0, 6};
    std::vector<int> result1 = countOccurrences(cards1, queries1);
    assert((result1 == std::vector<int>{3, 2, 1, 1, 0, 0}));

    // Test 2: All identical cards
    std::vector<int> cards2 = {7, 7, 7, 7};
    std::vector<int> queries2 = {7, 8, 7};
    std::vector<int> result2 = countOccurrences(cards2, queries2);
    assert((result2 == std::vector<int>{4, 0, 4}));

    // Test 3: Negative numbers and unsorted input
    std::vector<int> cards3 = {-5, -1, -1, 0, 3, -5};
    std::vector<int> queries3 = {-5, -1, 0, 3, 1};
    std::vector<int> result3 = countOccurrences(cards3, queries3);
    assert((result3 == std::vector<int>{2, 2, 1, 1, 0}));

    // Test 4: Empty cards or empty queries
    std::vector<int> cards4 = {};
    std::vector<int> queries4 = {1, 2};
    assert((countOccurrences(cards4, queries4) == std::vector<int>{0, 0}));

    std::vector<int> cards5 = {1, 2, 3};
    std::vector<int> queries5 = {};
    assert((countOccurrences(cards5, queries5) == std::vector<int>{}));

    // Test 5: Single element cards
    std::vector<int> cards6 = {42};
    std::vector<int> queries6 = {42, 41, 43};
    assert((countOccurrences(cards6, queries6) == std::vector<int>{1, 0, 0}));

    return 0;
}
#include <vector>
#include <algorithm>

// Returns a vector where the i-th element is the count of queries[i] in cards.
std::vector<int> countOccurrences(const std::vector<int>& cards, const std::vector<int>& queries) {
    // Work on a sorted copy of cards
    std::vector<int> sortedCards = cards;
    std::sort(sortedCards.begin(), sortedCards.end());

    std::vector<int> result;
    result.reserve(queries.size());

    for (int query : queries) {
        auto lower = std::lower_bound(sortedCards.begin(), sortedCards.end(), query);
        auto upper = std::upper_bound(sortedCards.begin(), sortedCards.end(), query);
        result.push_back(static_cast<int>(upper - lower));
    }

    return result;
}
// The solution requires efficient frequency counting for multiple queries. The naive approach of scanning the entire `cards` vector for each query would be O(N*M), which is too slow. A better approach is to first sort `cards` in ascending order (O(N log N)), then for each query, use binary search to find the lower and upper bounds of the queried value. The difference between the upper and lower bound indices gives the count directly. Specifically, we can use `std::lower_bound` to find the first index where the value is not less than the query, and `std::upper_bound` to find the first index where the value is greater than the query. The count is `upper - lower`. This avoids manual boundary scanning and handles duplicates automatically. Edge cases: if the query value is not present, both bounds point to the same index (or one past the end), giving a count of 0. For each query, binary search costs O(log N), so total time is O(N log N + M log N). Space complexity is O(M) for the result vector, plus O(1) auxiliary for the binary searches (sorting is in-place). The function should be `const`-correct, taking vectors by `const&` and returning a new vector.
