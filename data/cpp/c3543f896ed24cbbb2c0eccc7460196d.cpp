Write a C++ function named `findBestContestant` that takes three parameters: the number of contestants `N`, a boolean `preferAntal` (where `true` means prefer a higher number of apples `A` in ties, and `false` means prefer a higher number of pears `S` in ties), and a vector of pairs `(A, S)` representing the apples and pears for each contestant in order from 1 to N. The function must return the 1-based index of the contestant who has the maximum total fruits (`A + S`). If two or more contestants have the same total, the tie-breaker is the highest value of `A` if `preferAntal` is `true`, otherwise the highest value of `S`. If the tie persists (identical `A` and `S`), return the smallest index among them. The function should handle `N >= 1`, and values of `A` and `S` are unsigned long long (use `unsigned long long`). The function must not read from standard input or print; it only computes and returns the result.
#include <cassert>
#include <vector>
#include <utility>

unsigned long long findBestContestant(
    unsigned long long N,
    bool preferAntal,
    const std::vector<std::pair<unsigned long long, unsigned long long>>& contestants
);

int main() {
    // Test 1: Simple max total, no ties
    std::vector<std::pair<unsigned long long, unsigned long long>> v1 = {{3, 4}, {5, 1}, {2, 8}};
    assert(findBestContestant(3, true, v1) == 3); // total 10 vs 7 and 6

    // Test 2: Tie in total, prefer apples
    std::vector<std::pair<unsigned long long, unsigned long long>> v2 = {{10, 0}, {9, 1}, {8, 2}};
    assert(findBestContestant(3, true, v2) == 1); // all total 10, apples 10 > 9 > 8

    // Test 3: Tie in total, prefer pears
    std::vector<std::pair<unsigned long long, unsigned long long>> v3 = {{10, 0}, {9, 1}, {8, 2}};
    assert(findBestContestant(3, false, v3) == 3); // pears 2 > 1 > 0

    // Test 4: Full tie (A and S same) -> smallest index
    std::vector<std::pair<unsigned long long, unsigned long long>> v4 = {{5, 5}, {5, 5}, {5, 5}};
    assert(findBestContestant(3, true, v4) == 1);
    assert(findBestContestant(3, false, v4) == 1);

    // Test 5: Single contestant
    std::vector<std::pair<unsigned long long, unsigned long long>> v5 = {{123, 456}};
    assert(findBestContestant(1, true, v5) == 1);
    assert(findBestContestant(1, false, v5) == 1);

    // Test 6: Large values, no overflow
    std::vector<std::pair<unsigned long long, unsigned long long>> v6 = {{18446744073709551615ULL, 0}, {18446744073709551614ULL, 1}};
    assert(findBestContestant(2, true, v6) == 1); // total same, apples larger
    assert(findBestContestant(2, false, v6) == 2); // total same, pears larger

    // Test 7: Tie-break only applies when totals equal, not when higher total
    std::vector<std::pair<unsigned long long, unsigned long long>> v7 = {{100, 0}, {1, 1}};
    assert(findBestContestant(2, false, v7) == 1); // total 100 > 2, ignore pears

    // Test 8: Multiple contestants, check last one wins if better
    std::vector<std::pair<unsigned long long, unsigned long long>> v8 = {{1, 1}, {1, 2}, {2, 0}};
    assert(findBestContestant(3, true, v8) == 2); // total 3 vs 3 vs 2, apples 1 == 1, so smaller index 2
    assert(findBestContestant(3, false, v8) == 2); // pears 2 > 0, so index 2

    // Test 9: All zeros
    std::vector<std::pair<unsigned long long, unsigned long long>> v9 = {{0, 0}, {0, 0}, {0, 0}};
    assert(findBestContestant(3, true, v9) == 1);
    assert(findBestContestant(3, false, v9) == 1);

    return 0;
}
#include <vector>
#include <cstddef>

// Returns the 1-based index of the contestant with the highest total fruits.
// Tie-break: prefer larger apples if preferAntal is true, else prefer larger pears.
// On full tie (same A and S), keep the smallest index.
unsigned long long findBestContestant(
    unsigned long long N,
    bool preferAntal,
    const std::vector<std::pair<unsigned long long, unsigned long long>>& contestants
) {
    // Guard: if N is 0, return 0 (though problem guarantees N >= 1)
    if (N == 0) return 0;

    unsigned long long bestIndex = 1;
    unsigned long long bestTotal = contestants[0].first + contestants[0].second;
    unsigned long long bestA = contestants[0].first;
    unsigned long long bestS = contestants[0].second;

    // Loop from 1 because index 0 is already set as best
    for (unsigned long long i = 1; i < N; ++i) {
        unsigned long long A = contestants[i].first;
        unsigned long long S = contestants[i].second;
        unsigned long long total = A + S;

        bool better = false;
        if (total > bestTotal) {
            better = true;
        } else if (total == bestTotal) {
            if (preferAntal) {
                if (A > bestA) better = true;
                // if A == bestA, do not replace (keep smaller index)
            } else {
                if (S > bestS) better = true;
                // if S == bestS, do not replace
            }
        }

        if (better) {
            bestTotal = total;
            bestA = A;
            bestS = S;
            bestIndex = i + 1; // 1-based
        }
    }

    return bestIndex;
}
// The solution iterates through the list of contestants once, maintaining the best-so-far index, its total, its apples, and its pears. For each new contestant, compute the total `t = A + S`. Compare with the current best total. If the new total is greater, replace the best. If equal, apply the tie-breaker: if `preferAntal` is true, choose the one with larger `A`; if `preferAntal` is false, choose the one with larger `S`. If `A` and `S` are both equal (or the tie-breaker value is equal), do not replace, because we want the smallest index on full ties. The loop runs exactly `N` times, so time complexity is `O(N)`. Space complexity is `O(1)` besides the input vector (which is already given), as we only store a few scalar values. Edge cases: `N = 1` returns index 1; contestants may have identical totals, apples, and pears – the first encountered among them is kept; values are large unsigned, so use unsigned long long to avoid overflow when summing (max sum fits in unsigned long long for realistic inputs).
