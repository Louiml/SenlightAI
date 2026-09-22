Given a vector of non-negative integers representing the number of candies each kid has, and a non-negative integer `extraCandies`, write a C++ function that returns a vector of booleans where the element at index `i` is `true` if giving all `extraCandies` to the kid at index `i` would make that kid's total candies greater than or equal to the greatest number of candies among all kids (considering the original candies, not after any distribution), and `false` otherwise. The function must handle an empty input vector (return an empty vector), and must not modify the input vector. You may assume all values are within the range of a standard `int`.

The key insight is that the condition for each kid depends only on the maximum original candy count in the entire vector. So we first find the maximum value `m` among all `candies` using `std::max_element` (or a simple loop). Then, for each element `candies[i]`, we check if `candies[i] + extraCandies >= m`. If yes, push `true`; otherwise, push `false`. Edge cases: an empty `candies` vector should return an empty result (the loop naturally does this, but we should ensure `max_element` on an empty range is not called; we can handle by checking `if (candies.empty()) return {};`). Also, if all values are equal, then every kid already meets the maximum, so with `extraCandies >= 0` all entries become `true`. Time complexity is O(n) for the maximum scan and O(n) for the result construction, so overall O(n). Space complexity is O(n) for the output vector, plus O(1) auxiliary space (excluding output).

#include <vector>
#include <algorithm>

// Returns a boolean vector where at index i, true means the kid at i
// can have at least the maximum original candy count after adding extraCandies.
std::vector<bool> kidsWithGreatestCandies(const std::vector<int>& candies, int extraCandies) {
    if (candies.empty()) {
        return {};
    }
    int maxCandies = *std::max_element(candies.begin(), candies.end());
    std::vector<bool> result;
    result.reserve(candies.size());
    for (int candy : candies) {
        result.push_back(candy + extraCandies >= maxCandies);
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic example from typical prompts
    std::vector<int> candies1 = {2, 3, 5, 1, 3};
    std::vector<bool> expected1 = {true, true, true, false, true};
    assert(kidsWithGreatestCandies(candies1, 3) == expected1);

    // All kids already have max
    std::vector<int> candies2 = {4, 2, 1, 1, 2};
    std::vector<bool> expected2 = {true, false, false, false, false};
    assert(kidsWithGreatestCandies(candies2, 1) == expected2);

    // Only one kid
    std::vector<int> candies3 = {12};
    std::vector<bool> expected3 = {true};
    assert(kidsWithGreatestCandies(candies3, 10) == expected3);

    // Extra candies are zero
    std::vector<int> candies4 = {1, 1, 1, 1};
    std::vector<bool> expected4 = {true, true, true, true};
    assert(kidsWithGreatestCandies(candies4, 0) == expected4);

    // Empty input
    std::vector<int> candies5 = {};
    assert(kidsWithGreatestCandies(candies5, 5).empty());

    // Large extra candies make all true
    std::vector<int> candies6 = {1, 10, 3, 8};
    std::vector<bool> expected6 = {true, true, true, true};
    assert(kidsWithGreatestCandies(candies6, 100) == expected6);

    // Edge: extraCandies exactly enough for some
    std::vector<int> candies7 = {7, 5, 9, 6};
    std::vector<bool> expected7 = {false, false, true, false};
    assert(kidsWithGreatestCandies(candies7, 2) == expected7);

    // Duplicates and max at end
    std::vector<int> candies8 = {5, 1, 5, 2, 5};
    std::vector<bool> expected8 = {true, false, true, false, true};
    assert(kidsWithGreatestCandies(candies8, 0) == expected8);
}
