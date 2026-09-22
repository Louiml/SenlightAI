Write a C++ function that takes a vector of non-negative integers representing attribute scores and an integer `iteration` (1-based) that simulates one round of "Evaporative Cooling": remove the `floor(0.25 * size)` attributes with the lowest scores, but never remove more than `iteration + 1` attributes total. Return a new vector containing the remaining attribute scores in their original relative order. The function must preserve the input vector unchanged (const correctness), handle empty input by returning an empty vector, and when the removal count is zero, return a copy of the original vector. The function must work for any size, including sizes smaller than 4, where the removal count is zero.

// The algorithm must compute the number of attributes to remove as `removeCount = min(floor(0.25 * n), iteration + 1)`, where `n` is the original vector size. Since the function should remove the lowest-scored attributes, we need to identify which indices hold the smallest values. A straightforward approach is to create a vector of index-value pairs, sort by value (with index as tiebreaker for deterministic behavior), then mark the first `removeCount` indices for removal. After marking, iterate through the original vector and collect elements whose indices are not marked for removal, preserving original order. Edge cases: empty input → return empty vector; `removeCount == 0` → return a copy of input (no removal); when `n < 4`, `floor(0.25 * n) == 0`, so only `iteration + 1` matters (but if `iteration >= 0`, the min still yields a valid small number). Time complexity is O(n log n) due to sorting the index-value pairs (or O(n) if using selection algorithms, but sorting is simpler and acceptable). Space complexity is O(n) for the copy, the pair vector, and the boolean markers.

#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>

// Simulate one iteration of Evaporative Cooling on a vector of attribute scores.
// Removes the lowest-scoring attributes, with removal count = min(floor(0.25*n), iteration+1).
// Preserves the original order of the remaining attributes.
std::vector<int> evaporativeCoolingStep(const std::vector<int>& scores, int iteration) {
    // Edge case: empty input
    if (scores.empty()) {
        return {};
    }

    // Compute how many attributes to remove
    int n = static_cast<int>(scores.size());
    int quarter = static_cast<int>(std::floor(0.25 * n));
    int removeCount = std::min(quarter, iteration + 1);

    // If nothing to remove, return a copy of the original
    if (removeCount == 0) {
        return scores;
    }

    // Create index-value pairs for sorting by value (and by index for deterministic tie-breaking)
    std::vector<std::pair<int, int>> indexedScores; // pair: (value, index)
    indexedScores.reserve(n);
    for (int i = 0; i < n; ++i) {
        indexedScores.emplace_back(scores[i], i);
    }

    // Stable sort by value only (std::pair's default comparison uses value then index)
    std::sort(indexedScores.begin(), indexedScores.end());

    // Mark the indices of the lowest 'removeCount' attributes
    std::vector<bool> toRemove(n, false);
    for (int i = 0; i < removeCount; ++i) {
        toRemove[indexedScores[i].second] = true;
    }

    // Build the result vector by keeping unmarked indices in original order
    std::vector<int> result;
    result.reserve(n - removeCount);
    for (int i = 0; i < n; ++i) {
        if (!toRemove[i]) {
            result.push_back(scores[i]);
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <iostream>

// The solution function is declared above in the same file (or included here via header).
// In a standalone test, we include the definition above; for clarity we use a forward declaration.

int main() {
    // Test 1: basic removal of lowest quartile
    std::vector<int> scores1 = {10, 20, 30, 40, 50, 60, 70, 80};
    std::vector<int> result1 = evaporativeCoolingStep(scores1, 0);
    // n=8, floor(0.25*8)=2, min(2, 0+1)=1 → remove lowest 1 (10)
    assert((result1 == std::vector<int>{20, 30, 40, 50, 60, 70, 80}));

    // Test 2: larger iteration increases removal count
    std::vector<int> scores2 = {5, 15, 25, 35, 45, 55, 65, 75, 85, 95, 105, 115};
    std::vector<int> result2 = evaporativeCoolingStep(scores2, 3);
    // n=12, floor(0.25*12)=3, min(3, 3+1)=3 → remove lowest 3 (5,15,25)
    assert((result2 == std::vector<int>{35, 45, 55, 65, 75, 85, 95, 105, 115}));

    // Test 3: iteration cap: even if iteration large, removal capped by quarter
    std::vector<int> scores3 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> result3 = evaporativeCoolingStep(scores3, 100);
    // n=10, floor(0.25*10)=2, min(2, 101)=2 → remove lowest 2 (1,2)
    assert((result3 == std::vector<int>{3, 4, 5, 6, 7, 8, 9, 10}));

    // Test 4: empty input
    std::vector<int> scores4;
    std::vector<int> result4 = evaporativeCoolingStep(scores4, 0);
    assert(result4.empty());

    // Test 5: small size (n<4) → quarter is 0, nothing removed even with iteration > 0
    std::vector<int> scores5 = {42, 43, 44};
    std::vector<int> result5 = evaporativeCoolingStep(scores5, 5);
    // floor(0.25*3)=0, min(0,6)=0 → no removal
    assert((result5 == std::vector<int>{42, 43, 44}));

    // Test 6: minimum removal when quarter is 1 and iteration is 0
    std::vector<int> scores6 = {7, 3, 9, 1, 5};
    std::vector<int> result6 = evaporativeCoolingStep(scores6, 0);
    // n=5, floor(0.25*5)=1, min(1,1)=1 → remove lowest (1) → keep {7,3,9,5}
    assert((result6 == std::vector<int>{7, 3, 9, 5}));

    // Test 7: duplicate values: removal is deterministic by index order when values tie
    std::vector<int> scores7 = {4, 4, 4, 4, 4, 4};
    std::vector<int> result7 = evaporativeCoolingStep(scores7, 0);
    // n=6, floor(0.25*6)=1, min(1,1)=1 → remove the first '4' by index (value tie, index 0)
    assert((result7 == std::vector<int>{4, 4, 4, 4, 4}));

    // Test 8: original input is not modified
    std::vector<int> scores8 = {2, 8, 4, 6, 1, 3};
    std::vector<int> original = scores8;
    std::vector<int> result8 = evaporativeCoolingStep(scores8, 1);
    assert(scores8 == original);
    // n=6, floor(0.25*6)=1, min(1,2)=1 → remove 1 → keep {2,8,4,6,3}
    assert((result8 == std::vector<int>{2, 8, 4, 6, 3}));

    // Test 9: all attributes removed if quarter covers everything (though quarter < n always for n≥1)
    // For n=1, quarter=0, nothing removed
    std::vector<int> scores9 = {99};
    std::vector<int> result9 = evaporativeCoolingStep(scores9, 0);
    assert((result9 == std::vector<int>{99}));

    // Test 10: exact quarter and iteration both large → full quarter removal
    std::vector<int> scores10 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1};
    // n=12, floor(0.25*12)=3, min(3, 2+1)=3 → remove lowest 3 (values: -1,0,1)
    std::vector<int> result10 = evaporativeCoolingStep(scores10, 2);
    assert((result10 == std::vector<int>{10, 9, 8, 7, 6, 5, 4, 3, 2}));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
