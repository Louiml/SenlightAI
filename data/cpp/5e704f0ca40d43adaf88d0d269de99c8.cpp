Write a C++ function `bool hasNeighborInSet(const std::vector<int>& firstList, const std::vector<int>& secondList)` that returns `true` if there exists any integer `x` in `secondList` such that either `x - 1` or `x + 1` appears in `firstList`, and `false` otherwise. The function must handle empty lists gracefully (returning `false`), must work with duplicate values in either list, and must not modify the input vectors. The lists contain no more than 10^5 integers each, and each integer is within the range [-10^9, 10^9].
The core idea is to store all elements from `firstList` in an `unordered_set` for O(1) average lookup. Then iterate through each element `x` in `secondList`, and check if either `x - 1` or `x + 1` exists in that set. If any check succeeds, return `true` immediately. If no match is found after scanning the entire second list, return `false`. Edge cases include empty first or second list (no matches possible), duplicate values (set handles deduplication naturally), and extreme integer values near the bounds of the range (subtraction/addition may overflow if not careful, but within the given range [-10^9, 10^9], `x - 1` and `x + 1` remain within the 32-bit `int` range, so no overflow occurs). Time complexity is O(n + m) where n and m are the sizes of the first and second lists, respectively, because building the set takes O(n) and each of the m lookups is O(1) average. Space complexity is O(n) for the set.
#include <vector>
#include <unordered_set>

// Returns true if any value in secondList has a neighbor (value-1 or value+1)
// present in firstList. Handles empty lists and duplicates correctly.
bool hasNeighborInSet(const std::vector<int>& firstList, const std::vector<int>& secondList) {
    // Build a set of all values from firstList for O(1) lookups.
    std::unordered_set<int> firstSet(firstList.begin(), firstList.end());

    // Check each element in secondList for a neighboring value.
    for (const int value : secondList) {
        if (firstSet.count(value - 1) || firstSet.count(value + 1)) {
            return true;
        }
    }

    return false;
}
#include <cassert>
#include <vector>

// The solution function is declared above; include it in the same translation unit.

int main() {
    // Basic positive case: neighbor exists.
    assert(hasNeighborInSet({1, 5, 10}, {2, 4, 11}) == true);
    assert(hasNeighborInSet({1, 5, 10}, {2}) == true);
    assert(hasNeighborInSet({1, 5, 10}, {11}) == true);

    // Negative case: no neighbor in first list.
    assert(hasNeighborInSet({1, 5, 10}, {3, 7, 20}) == false);

    // Empty second list.
    assert(hasNeighborInSet({1, 2, 3}, {}) == false);

    // Empty first list.
    assert(hasNeighborInSet({}, {5, 6}) == false);
    assert(hasNeighborInSet({}, {}) == false);

    // Duplicates in both lists (should still work).
    assert(hasNeighborInSet({5, 5, 5}, {4, 6}) == true);
    assert(hasNeighborInSet({5, 5, 5}, {5}) == false); // 5 is not a neighbor of itself.

    // Negative numbers.
    assert(hasNeighborInSet({-10, 0, 10}, {-9, 1}) == true);
    assert(hasNeighborInSet({-10, 0, 10}, {-11, 9}) == true);
    assert(hasNeighborInSet({-10, 0, 10}, {-8, 2}) == false);

    // Boundary values near the range limits.
    assert(hasNeighborInSet({-1000000000}, {-999999999}) == true);
    assert(hasNeighborInSet({1000000000}, {999999999}) == true);
    assert(hasNeighborInSet({-1000000000}, {-1000000000}) == false); // no neighbor
    
    return 0;
}
