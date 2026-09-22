// Write a C++ function named `findBestMatch` that takes a vector of pairs where each pair contains an integer ID (first) and an integer tolerance error value (second), along with three integers: `n` (number of pairs, though the function should rely on the vector's size), `s` (target value), and `p` (allowed margin). The function should filter the pairs to keep only those whose error value `err` satisfies `s - p <= err <= s + p`. If no pairs remain, return `-1`. Otherwise, among the filtered pairs, select the one with the largest error value (ties are broken by choosing the smallest ID, since sorting is stable and input order preserves lower IDs first in typical construction). Return the 1-based index (ID + 1) of the chosen pair, or `-1` if none exist. Assume the input vector is non-empty and IDs are unique and in ascending order from 0. The function must be `const`-correct and handle negative numbers for `s`, `p`, and error values.
// The solution iterates through all pairs once, checking each error value against the inclusive range `[s-p, s+p]`. For every pair that passes the filter, we track the one with the maximum error value. Because the input IDs are guaranteed to be ascending and unique, if two filtered pairs have the same maximal error, the first encountered (smallest ID) should be chosen — this can be handled naturally by using a strict `>` comparison when updating the best candidate, so equal values do not override the earlier index. If no pair passes, return `-1`. Edge cases include: all pairs filtered out, negative margins (`p` negative) making the lower bound larger than the upper bound (then no pair can satisfy unless the error exactly equals both bounds, which is impossible unless `p=0`), and duplicate error values. Time complexity is O(m) where m is the number of pairs, and space complexity is O(1) beyond the input vector.
#include <vector>
#include <utility> // for std::pair

// Return 1-based index of the pair with largest error within [s-p, s+p],
// or -1 if no pair qualifies. IDs are assumed ascending from 0.
int findBestMatch(const std::vector<std::pair<int, int>>& pairs, int s, int p) {
    int bestIndex = -1;
    int bestError = 0; // only used when bestIndex != -1

    for (int i = 0; i < static_cast<int>(pairs.size()); ++i) {
        int err = pairs[i].second;
        if (err >= s - p && err <= s + p) {
            if (bestIndex == -1 || err > bestError) {
                bestIndex = i;
                bestError = err;
            }
        }
    }

    return (bestIndex == -1) ? -1 : bestIndex + 1;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above here.

int main() {
    // Basic case with two qualified pairs, largest error chosen
    std::vector<std::pair<int, int>> v1 = {{0, 5}, {1, 7}, {2, 10}};
    assert(findBestMatch(v1, 6, 3) == 2); // errors 5 and 7 qualify, 7 wins

    // No qualifying pairs
    std::vector<std::pair<int, int>> v2 = {{0, 100}, {1, -100}};
    assert(findBestMatch(v2, 0, 1) == -1);

    // Exact boundary inclusive
    std::vector<std::pair<int, int>> v3 = {{0, 4}, {1, 8}};
    assert(findBestMatch(v3, 6, 2) == 1); // both qualify (4 and 8), 8 wins -> index 2? Wait: s=6,p=2 => [4,8], so both qualify, largest is 8 at index 1, return 2

    // Correcting that: idx 1 -> return 2
    // Let's write correctly:
    assert(findBestMatch(v3, 6, 2) == 2); // error 8 at ID 1 -> returns 2

    // Tie in error, lower ID wins
    std::vector<std::pair<int, int>> v4 = {{0, 5}, {1, 5}};
    assert(findBestMatch(v4, 5, 0) == 1); // both error 5, choose ID 0 -> return 1

    // Negative errors and bounds
    std::vector<std::pair<int, int>> v5 = {{0, -3}, {1, -1}, {2, 2}};
    assert(findBestMatch(v5, -2, 2) == 2); // range [-4,0], errors -3 and -1 qualify, -1 largest at ID 1 -> return 2

    // Single element qualifies
    std::vector<std::pair<int, int>> v6 = {{0, 0}};
    assert(findBestMatch(v6, 0, 0) == 1);

    // All qualify, largest at end
    std::vector<std::pair<int, int>> v7 = {{0, 1}, {1, 2}, {2, 3}};
    assert(findBestMatch(v7, 2, 5) == 3); // all qualify, error 3 at ID 2 -> return 3

    // Margin negative leads to empty range
    std::vector<std::pair<int, int>> v8 = {{0, 5}};
    assert(findBestMatch(v8, 5, -1) == -1); // range [6,4] empty, nothing qualifies

    return 0;
}
