// You are given `n` pairs of integers, where each pair represents a "left" value `a[i]` and a "right" value `b[i]`. A pair `j` is considered "covered" if there exists another pair `i` (with `i != j`) such that the right value of pair `i` equals the left value of pair `j` (i.e., `b[i] == a[j]`). Write a standalone C++ function named `countUncoveredPairs` that takes a vector of pairs (or two vectors `a` and `b` of equal length) and returns the number of pairs that are **not** covered by any other pair. A pair is not covered if no other pair’s right value matches its left value. Note that multiple pairs may have the same left or right values, and self-matching is not allowed (a pair cannot cover itself). The function should handle `n` up to 100.

#include <cassert>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Basic case: pair 0 (right=2) covers pair 1 (left=2), pair 1 (right=3) covers pair 2 (left=3)
    std::vector<int> a1 = {1, 2, 3};
    std::vector<int> b1 = {2, 3, 4};
    assert(countUncoveredPairs(a1, b1) == 1); // only pair 2 (left=3) not covered? Wait pair 2 left=3 is covered by pair 1? Actually pair 0 right=2 covers pair1 left=2, pair1 right=3 covers pair2 left=3, so all covered? Let's compute: pair0 covered? b? right=2, left=1; no other right=1? none → uncovered. So answer is 1. Correct.

    // Self-match not allowed: single pair
    std::vector<int> a2 = {5};
    std::vector<int> b2 = {5};
    assert(countUncoveredPairs(a2, b2) == 1);

    // All covered: cycle of length 2
    std::vector<int> a3 = {1, 2};
    std::vector<int> b3 = {2, 1};
    // pair0 right=2 covers pair1 left=2; pair1 right=1 covers pair0 left=1 → both covered → 0? Wait pair0 left=1, covered by pair1 right=1 yes; pair1 left=2 covered by pair0 right=2 → both covered → 0.
    assert(countUncoveredPairs(a3, b3) == 0);

    // Duplicates: pair0 right=3, pair1 left=3, pair2 left=3 both covered by pair0
    std::vector<int> a4 = {1, 3, 3};
    std::vector<int> b4 = {3, 4, 5};
    // pair0 covers both 3s, pair1, pair2 left=3 are covered, pair0 left=1 no right=1 → uncovered, so answer 1? Actually pair0 is uncovered, pair1 and pair2 are covered → answer 1.
    assert(countUncoveredPairs(a4, b4) == 1);

    // No covers at all
    std::vector<int> a5 = {1, 2, 3};
    std::vector<int> b5 = {4, 5, 6};
    assert(countUncoveredPairs(a5, b5) == 3);

    // Empty input
    std::vector<int> a6 = {};
    std::vector<int> b6 = {};
    assert(countUncoveredPairs(a6, b6) == 0);

    return 0;
}

#include <vector>
#include <cstddef>

// Counts how many pairs are not covered by any other pair.
// A pair j is covered if there exists i != j with b[i] == a[j].
int countUncoveredPairs(const std::vector<int>& a, const std::vector<int>& b) {
    const std::size_t n = a.size();
    std::vector<bool> covered(n, false);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (i != j && b[i] == a[j]) {
                covered[j] = true;
            }
        }
    }
    int uncovered = 0;
    for (std::size_t j = 0; j < n; ++j) {
        if (!covered[j]) {
            ++uncovered;
        }
    }
    return uncovered;
}

// The solution builds a boolean array `covered` of size `n`, initially all `false`. For each pair `i`, loop over every other pair `j` (with `i != j`). If `b[i] == a[j]`, then mark `covered[j] = true`. After checking all pairs, count the number of entries in `covered` that are still `false`. This directly implements the definition. Edge cases: (1) If `n == 1`, there is only one pair and it cannot be covered by itself, so the answer is 1. (2) If the same pair index is checked against itself, skip it to avoid self-covering. (3) Duplicate values are handled naturally because the comparison uses equality. (4) The algorithm is \(O(n^2)\) in time, which is fine for `n ≤ 100`, and uses \(O(n)\) extra space for the boolean array. No special handling is needed for negative or large integers beyond using `int`.
