/*
Write a C++ function `splitGroups` that takes a vector of integers representing the "sociability scores" of a group of people and returns a `struct` containing three integers: the number of outgoing people, the number of introverted people, and the maximum possible difference between the total scores of the outgoing group and the introverted group. The total number of people `n` is at least 1. The outgoing group must be at least as large as the introverted group (i.e., outgoing count ≥ introverted count), and the difference in sizes must be minimized (so for odd `n`, outgoing has one more; for even, equal size). Among all valid splits, choose the one that maximizes the sum of outgoing scores minus the sum of introverted scores. The function should be pure (no I/O) and handle duplicates and negative numbers correctly. Return the result as a `SplitResult` struct with fields `outgoingCount`, `introvertedCount`, and `maxDiff`.
*/

#include <vector>
#include <algorithm>

struct SplitResult {
    int outgoingCount;
    int introvertedCount;
    int maxDiff;
};

// Given a vector of sociability scores, return the split with minimal size
// difference (outgoing >= introverted) and maximum outgoing-minus-introverted sum.
SplitResult splitGroups(std::vector<int> scores) {
    std::sort(scores.begin(), scores.end());
    const int n = static_cast<int>(scores.size());
    const int introvertedCount = n / 2;
    const int outgoingCount = n - introvertedCount;

    int introvertedSum = 0;
    int outgoingSum = 0;
    for (int i = 0; i < introvertedCount; ++i) {
        introvertedSum += scores[i];
    }
    for (int i = introvertedCount; i < n; ++i) {
        outgoingSum += scores[i];
    }

    return {outgoingCount, introvertedCount, outgoingSum - introvertedSum};
}

#include <cassert>
#include <vector>

// The solution function is declared above; tests below.

int main() {
    // Even count
    SplitResult r1 = splitGroups({1, 2, 3, 4});
    assert(r1.outgoingCount == 2 && r1.introvertedCount == 2 && r1.maxDiff == 4);

    // Odd count
    SplitResult r2 = splitGroups({1, 2, 3, 4, 5});
    assert(r2.outgoingCount == 3 && r2.introvertedCount == 2 && r2.maxDiff == 9);

    // Single element
    SplitResult r3 = splitGroups({7});
    assert(r3.outgoingCount == 1 && r3.introvertedCount == 0 && r3.maxDiff == 7);

    // Negative numbers and duplicates
    SplitResult r4 = splitGroups({-5, -1, -1, 10});
    assert(r4.outgoingCount == 2 && r4.introvertedCount == 2 && r4.maxDiff == 17);

    // All same values
    SplitResult r5 = splitGroups({3, 3, 3});
    assert(r5.outgoingCount == 2 && r5.introvertedCount == 1 && r5.maxDiff == 3);

    // Large mix
    SplitResult r6 = splitGroups({10, -20, 5, 0, 15});
    assert(r6.outgoingCount == 3 && r6.introvertedCount == 2 && r6.maxDiff == 50);
}

// The optimal strategy is to sort the scores in ascending order and assign the smallest `floor(n/2)` values to the introverted group and the largest `ceil(n/2)` values to the outgoing group. This minimizes the introverted sum and maximizes the outgoing sum, thereby maximizing the difference. For even `n`, both groups have size `n/2`; for odd `n`, the outgoing group gets `n - n/2` (one more) while introverted gets `n/2`. This satisfies the size constraint (outgoing ≥ introverted) and minimizes size imbalance. Edge cases: `n=1` gives introverted count 0, outgoing count 1, diff = the only score. Negative scores work because sorting still places the smallest (most negative) in introverted, which maximizes difference. Duplicates are fine. Time complexity is O(n log n) due to sorting, space complexity is O(1) auxiliary (ignoring input storage). No special handling beyond sorting and summing.
