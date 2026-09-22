// Write a C++ function `int minRearrangementMoves(const std::vector<int>& permutation)` that takes a permutation of the numbers `1` through `n` (where `n` is the length of the vector) and returns the minimum number of operations required to sort it into increasing order. An operation consists of selecting **any** contiguous segment of the permutation and reversing that segment. You may perform this operation zero, one, or two times. The function must return `0`, `1`, or `2` — the minimum number of reversals needed to obtain the identity permutation `[1, 2, ..., n]`. It is guaranteed that any permutation can be sorted in at most 2 reversals. Assume the input always contains a valid permutation of `1..n` (no duplicates, all numbers present).
#include <cassert>
#include <vector>

// (Include the solution function here or via header)
int minRearrangementMoves(const std::vector<int>& permutation);

int main() {
    // Already sorted: answer 0
    assert(minRearrangementMoves({1, 2, 3, 4}) == 0);
    assert(minRearrangementMoves({1}) == 0);

    // Single reversal needed
    assert(minRearrangementMoves({2, 1}) == 1);
    assert(minRearrangementMoves({3, 2, 1}) == 1);
    assert(minRearrangementMoves({1, 4, 3, 2}) == 1);   // reverse middle [4,3,2]
    assert(minRearrangementMoves({2, 1, 3, 4}) == 1);   // reverse first two

    // Two reversals needed
    assert(minRearrangementMoves({2, 3, 1}) == 2);
    assert(minRearrangementMoves({3, 1, 2}) == 2);
    assert(minRearrangementMoves({2, 1, 4, 3}) == 2);   // two separate swaps

    // Larger example requiring two reversals
    assert(minRearrangementMoves({1, 3, 2, 5, 4}) == 2);
    assert(minRearrangementMoves({4, 3, 2, 1, 5}) == 1); // reverse first four

    return 0;
}
#include <vector>

// Returns the minimum number of reversals (0, 1, or 2) needed to sort
// the given permutation of 1..n into increasing order.
int minRearrangementMoves(const std::vector<int>& permutation) {
    const int n = static_cast<int>(permutation.size());

    // Count how many elements are already in their correct positions.
    int inPosition = 0;
    for (int i = 0; i < n; ++i) {
        if (permutation[i] == i + 1) {
            ++inPosition;
        }
    }

    // If all are in position, no moves are needed.
    if (inPosition == n) {
        return 0;
    }

    // Length of the longest prefix where elements are already correct.
    int prefixLength = 0;
    while (prefixLength < n && permutation[prefixLength] == prefixLength + 1) {
        ++prefixLength;
    }

    // Length of the longest suffix where elements are already correct.
    int suffixLength = 0;
    while (suffixLength < n - prefixLength &&
           permutation[n - 1 - suffixLength] == n - suffixLength) {
        ++suffixLength;
    }

    // One reversal works only if the only in-position elements are
    // exactly the prefix and suffix blocks (so the middle is a contiguous
    // misplaced segment).
    if (inPosition == prefixLength + suffixLength) {
        return 1;
    }

    // Otherwise, two reversals always suffice.
    return 2;
}
// The key insight is that sorting a permutation with at most two reversals has a known characterization. First, check if the array is already sorted — that requires `0` moves. Otherwise, we need to determine whether one reversal suffices. A single reversal can fix a permutation if and only if all elements that are already in their correct positions form a contiguous block that starts at the beginning and ends at the end of the array, with the "misplaced" middle segment being exactly the part to reverse. More formally, let `prefix` be the length of the longest prefix where `arr[i] == i+1` (using 0-based indexing), and let `suffix` be the length of the longest suffix where `arr[n-1-suffix] == n-suffix`. All other elements are "misplaced" except possibly some "in-position" elements in the middle. If the total number of elements already in their correct positions equals `prefix + suffix`, then a single reversal of the middle segment will sort the array, so the answer is `1`. Otherwise, the answer is `2`. This works because if there are extra in-position elements scattered away from the boundaries, they cannot be kept fixed by a single reversal and must be moved, requiring at least two reversals. The algorithm runs in O(n) time and O(1) extra space, aside from the input vector.
