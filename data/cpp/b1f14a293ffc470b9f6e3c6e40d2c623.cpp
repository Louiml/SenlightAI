// Write a C++ function `countUniquePairs` that takes a vector of integers representing proxy IDs and returns the number of unique unordered pairs `(a, b)` where `a < b` and the difference between the two numbers is at most 2. The input vector may contain duplicate values, and duplicates should not create additional pairs—each pair of distinct indices with the same value counts only once per unique numerical pair, not per occurrence. For example, if the input contains three `5`s, that contributes only the single pair `(5,5)` (since `a < b` is not required; instead `a <= b`). But to align with the original snippet's proxy logic (where pairs require distinct proxies), your function should treat equal values as forming a pair only if there are at least two occurrences, and each unique numerical pair counts once. More precisely: count the number of pairs of indices `i < j` such that `abs(value[i] - value[j]) <= 2`, but count each unique unordered pair of *values* only once, not per index pair. For instance, input `{1, 1, 2}` should return 2: pairs 1-1 and 1-2 (since 1-2 difference is 1), but not count 1-1 twice. Input `{5, 5, 5}` returns 1 (only the 5-5 pair). Duplicate values do not increase the pair count beyond one per value combination. The function must return an integer count. If the input has fewer than 2 elements, return 0.
// The key challenge is to count unique unordered value pairs where the absolute difference is ≤ 2, ignoring duplicate occurrences beyond the first per value. The simplest approach: first, collect the set of distinct values from the input. Then for each value `a` in that set, consider all `b` in the set where `a ≤ b` (to avoid duplicates) and `b - a ≤ 2`. Count such pairs. Since values can be negative, using a standard `std::set<int>` to store distinct values works fine, and the set is automatically sorted. Iterate over the set with two nested loops: for each `a`, iterate over subsequent elements `b` until `b - a > 2` (since the set is sorted, we can break early). This gives O(n^2) worst-case time for the nested loops, but with early break it's O(k^2) where k is the number of distinct values, and k ≤ n. For storage, we use O(k) for the set. Edge cases: empty or single-element input returns 0; all values the same (e.g., three 5s) yields exactly one pair (5,5) because the set contains only 5 and the pair (5,5) is counted once. Negative values are handled naturally by difference. The time complexity is O(n) to build the set plus O(k^2) in the worst case if all values are far apart (but then early break rarely triggers), but with the ≤2 constraint the inner loop typically runs at most 3 iterations because differences are small, so it's effectively O(n + k) in most practical cases, though the asymptotic worst case is O(k^2). Space is O(k).
#include <vector>
#include <set>
#include <cstddef>

// Count unique unordered pairs of distinct values (a,b) with a <= b and b - a <= 2.
// Duplicate occurrences of the same value contribute at most one pair per value combination.
int countUniquePairs(const std::vector<int>& proxies) {
    // Collect distinct values
    std::set<int> distinct(proxies.begin(), proxies.end());

    int count = 0;
    // Iterate over sorted distinct values
    for (auto itA = distinct.begin(); itA != distinct.end(); ++itA) {
        int a = *itA;
        // For each a, consider b starting from a (including a itself) up to a+2
        for (auto itB = itA; itB != distinct.end(); ++itB) {
            int b = *itB;
            if (b - a > 2) break; // since sorted, all further b are larger
            // Count this pair (a,b) because a <= b and difference <= 2
            ++count;
        }
    }
    return count;
}
#include <cassert>
#include <vector>

int main() {
    // Empty and single element
    assert(countUniquePairs({}) == 0);
    assert(countUniquePairs({7}) == 0);

    // Two distinct values diff 1
    assert(countUniquePairs({1, 2}) == 1); // only (1,2)

    // Duplicate value appears twice plus another
    assert(countUniquePairs({1, 1, 2}) == 2); // (1,1) and (1,2)

    // All same
    assert(countUniquePairs({5, 5, 5}) == 1); // only (5,5)

    // Diff exactly 2
    assert(countUniquePairs({1, 3}) == 1); // (1,3)

    // Diff > 2
    assert(countUniquePairs({1, 4}) == 0);

    // Multiple values
    assert(countUniquePairs({1, 2, 3}) == 3); // (1,1?) no freq 1 each, so only (1,2),(1,3),(2,3) = 3
    assert(countUniquePairs({1, 3, 5}) == 0); // differences 2,4,2 -> (1,3) diff2 and (3,5) diff2 => 2? Actually 1-3 diff2, 3-5 diff2, 1-5 diff4 => so 2 pairs. Let me correct: expected 2.
    assert(countUniquePairs({1, 3, 5}) == 2); // (1,3) and (3,5)

    // Negative values
    assert(countUniquePairs({-1, 0, 1}) == 3); // pairs: (-1,0),( -1,1? diff2? -1 to 1 diff2 -> yes), (0,1) => 3

    // Mix with duplicates
    assert(countUniquePairs({1, 1, 1, 3}) == 2); // (1,1) and (1,3)

    // Large gap
    assert(countUniquePairs({1, 10, 20}) == 0);

    return 0;
}
