// Write a C++ function `canFormPairs` that takes three positive integers `n`, `m`, `k` (with `k` guaranteed even), and two vectors `v1` of size `n` and `v2` of size `m`, where all elements in both vectors are positive integers. The function must determine whether it is possible to select `k/2` distinct integers from `v1` and `k/2` distinct integers from `v2` such that the union of the selected sets contains every integer from `1` to `k` exactly once. In other words, decide if the combined sets of unique numbers from `v1` and `v2` collectively cover all numbers `1..k`, and each of the two selected subsets can contribute exactly `k/2` of those numbers (without overlap in coverage). Return `true` if such a selection is possible, `false` otherwise. Assume `k` is even and at least 2. The vectors may contain arbitrary positive integers, including duplicates and numbers outside `1..k`. The result should ignore numbers outside `1..k` entirely.

The goal is to check two conditions: (1) For every integer `i` from `1` to `k`, at least one of the two input sets (derived from `v1` and `v2` via sets to remove duplicates) contains `i`. (2) It is possible to assign each `i` to exactly one of the two sets such that each set gets exactly `k/2` numbers. For each number `i` in `1..k`, there are three possibilities: it appears only in set1, only in set2, or in both. Numbers appearing in only one set are forced to be assigned to that set, so if the forced count for either set exceeds `k/2`, it's impossible. If not, the remaining slots can be filled from numbers appearing in both sets, and since these can be arbitrarily split, it suffices to check that the forced counts do not exceed `k/2`. Also, if any number is missing from both sets, fail. The algorithm uses two `std::set<int>` for deduplication, then iterates `i` from `1` to `k` counting forced assignments. Time complexity is `O(n + m + k log(min(n,m)))` due to set insertions and lookups, and space is `O(n + m)` for the sets. Edge cases: duplicates and numbers outside `1..k` are ignored; if `k` is even but both sets are empty, fail when `k>0`.

#include <vector>
#include <set>

// Determine if it's possible to select k/2 distinct numbers from v1 and k/2 from v2
// so that their union covers all integers 1..k exactly once.
// v1 and v2 may contain duplicates; only unique values matter.
bool canFormPairs(const std::vector<int>& v1, const std::vector<int>& v2, int k) {
    // Build sets of unique values from both vectors.
    std::set<int> set1(v1.begin(), v1.end());
    std::set<int> set2(v2.begin(), v2.end());

    const int half = k / 2;

    // Check if every number 1..k is present in at least one set.
    for (int i = 1; i <= k; ++i) {
        if (set1.count(i) == 0 && set2.count(i) == 0) {
            return false;
        }
    }

    // Count forced assignments: numbers that appear in only one set.
    int onlySet1 = 0;
    int onlySet2 = 0;

    for (int i = 1; i <= k; ++i) {
        const bool in1 = set1.count(i) > 0;
        const bool in2 = set2.count(i) > 0;

        if (in1 && !in2) {
            ++onlySet1;
        } else if (!in1 && in2) {
            ++onlySet2;
        }
        // Numbers in both sets can be assigned flexibly later.
    }

    // Each set can take at most half numbers; forced counts must not exceed that.
    return (onlySet1 <= half && onlySet2 <= half);
}

#include <cassert>
#include <vector>

// Declare the function (or include the header if separate).
bool canFormPairs(const std::vector<int>& v1, const std::vector<int>& v2, int k);

int main() {
    // Test 1: Basic coverage, balanced split possible.
    assert(canFormPairs({1, 2}, {3, 4}, 4) == true); // 2 from each, union covers 1..4

    // Test 2: Missing number 3 entirely.
    assert(canFormPairs({1, 2}, {4}, 4) == false); // 3 missing

    // Test 3: Too many forced from set1 (needs 3 but k/2=2).
    assert(canFormPairs({1, 2, 3}, {4}, 4) == false); // 1,2,3 only in set1 -> 3 > 2

    // Test 4: Forced counts exactly at half, shared number fills the rest.
    assert(canFormPairs({1, 2, 5}, {3, 4}, 6) == true); // half=3, forced set1=2 (1,2), set2=2 (3,4), number 5 in both fills.

    // Test 5: k=2, all numbers in both sets.
    assert(canFormPairs({1, 2}, {2, 1}, 2) == true);

    // Test 6: k=2, one number only in set1, other only in set2.
    assert(canFormPairs({1}, {2}, 2) == true);

    // Test 7: k=2, both numbers only in set1 -> fail.
    assert(canFormPairs({1, 2}, {}, 2) == false);

    // Test 8: Duplicates and extras outside 1..k should be ignored.
    assert(canFormPairs({1, 1, 99}, {2, 100}, 2) == true);

    // Test 9: k=4, all four numbers are in both sets, but forced none, possible.
    assert(canFormPairs({1,2,3,4}, {1,2,3,4}, 4) == true);

    // Test 10: k=6, set1 has 1..4 and set2 has 3..6 -> both forced counts are 2 (set1 forced 1,2; set2 forced 5,6), shared 3,4 fill to 3 each.
    assert(canFormPairs({1,2,3,4}, {3,4,5,6}, 6) == true);
}
